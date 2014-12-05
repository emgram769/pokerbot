
# -*- coding: utf-8 -*-
from Tkinter import *
import random
import copy
import pokerbot
# --------------------------------- M O D E L --------------------------------
def initGameVars():
    #misc
    g.diagUp= False
    g.potOutlineWidth=5
    g.playCounter = 0

    #colors
    g.bgColor = 'dark green'
    g.spaceColor = 'black'
    g.activeSpace = 'yellow'
    g.cardColor = 'white'
    g.stackColor='white'
    g.blindsFill = ['purple','yellow']#little blind first
    g.diagColor = 'gray'
    g.inputBoxColor = 'white'

    #card vars
    g.SUITS = ['♠','♣','♥','♦']
    g.SUIT_VAL = range(4)
    g.RANKS = range(0,14)
    g.FACE = ['A']+range(2,11) + ['J','Q','K','A']
    g.firstRoundCards = [(9,2),(12,1),(4,3)]
    g.round = 1
    g.tableCards = [] #no cards placed on table at start


    #player vars
    g.numPlayers = 4
    class Struct: pass
    g.player=[]
    for i in xrange(g.numPlayers):
        g.player.append(Struct())
        g.player[i].stackSize = 100
        g.player[i].amountDue = 0
        g.player[i].index = i
        g.player[i].card =[(False,False),(False,False)]
        g.player[i].isBot = False
    g.player[2].isBot = True #set player 2 as bot

    #GAME VARS
    g.potSize = 0
    g.blinds = 4 #make even number
    g.littleBlind= 0 #Player 1 starts as little blind
    g.bigBlind=g.littleBlind+1
    g.activePlayer = g.player[(g.bigBlind+1)%g.numPlayers]
    g.blindsText = ['Little Blind','Big Blind'] #little blind first
    g.highestBet = 0

    #viewVars
    g.actionBarHeight=70
    g.actionLabels = ['Call','Check','Raise','Fold']
    g.diagCaption = 'Enter Raise Amount:'
    g.diagCaptionFont = 'Arial 22'
    g.raiseAmount = ''

def initGraphicsVars():
    spaceHeight = int((g.screenHeight-g.actionBarHeight)/4)
    spaceWidth = int(g.screenWidth/3)
    g.actionBar=(0,g.screenHeight-g.actionBarHeight,g.screenWidth,g.screenHeight)
    #(x,y) top left corner of  player space
    x = int((g.screenWidth-spaceWidth)/2)
    y = g.screenHeight-g.actionBarHeight-spaceHeight

    g.player[0].space=(x,y,x+spaceWidth,y+spaceHeight)
    y = int(g.screenHeight-g.actionBarHeight-spaceWidth)/2
    g.player[1].space =(0,y,spaceHeight,y+spaceWidth)
    g.player[2].space =(x,0,x+spaceWidth,spaceHeight)
    g.player[3].space =(g.screenWidth-spaceHeight,y,g.screenWidth,y+spaceWidth)


    #ACTION BAR BUTTONS
    buttonWidth = int(g.screenWidth/4)
    g.buttonSpace=[]
    x=0
    y=g.screenHeight-g.actionBarHeight

    for i in xrange(4):
        g.buttonSpace.append((x,y,x+buttonWidth,y+g.actionBarHeight))
        x+=buttonWidth

    #card and blind spaces
    cardWidth = int(spaceWidth/4) #do not change
    cardHeight = int(spaceHeight/1.5)

    for i in xrange(g.numPlayers):
        g.player[i].cardSpace = [(0,0,0,0),(0,0,0,0)] #initialize card spaces


    y = g.player[0].space[3]-cardHeight
    for i in [0,2]: #players on top or bottom
        space = g.player[i].space
        #blind space
        x = space[0]
        blindBounds = x,y,x+cardWidth,y+cardHeight
        g.player[i].blindCenter= center(blindBounds)
        g.player[i].blindRadius =getMin(blindBounds)

        #card spaces
        for j in xrange(2):
            x +=cardWidth
            g.player[i].cardSpace[j]=(x,y,x+cardWidth,y+cardHeight)
        y = 0

    x = g.player[1].space[0]
    for i in [1,3]: #players on sides
        (width, height) = (cardHeight, cardWidth)#swap width and height

        y = g.player[i].space[1]
        blindBounds=(x,y,x+width,y+height)
        g.player[i].blindCenter = center(blindBounds)
        g.player[i].blindRadius = getMin(blindBounds)

        #card spaces
        for j in xrange(2):
            y +=height
            g.player[i].cardSpace[j]=(x,y,x+width,y+height)
        x=g.screenWidth-cardHeight

    # POT SPACES
    for i in xrange(4):
        g.player[i].stackSpace= list(copy.copy(g.player[i].space))
    g.player[0].stackSpace[3]-=cardHeight
    g.player[1].stackSpace[0]+=cardHeight
    g.player[2].stackSpace[1]+=cardHeight
    g.player[3].stackSpace[2]-=cardHeight

    #TABLE CARD SPACES
    margin = 50
    tableCardSpaceX=g.screenWidth- (2*spaceHeight)
    spacing = int((tableCardSpaceX-(5*cardWidth))/2)
    x = g.player[1].space[2]+spacing
    y =g.player[0].space[1]-cardHeight-margin
    g.tableCardSpace=[]

    for i in xrange(5):
        g.tableCardSpace.append((x,y,x+cardWidth,y+cardHeight))
        x+=cardWidth

    #MAIN POT SPACE
    x1 = g.tableCardSpace[1][0]
    x2 = g.tableCardSpace[3][2]
    y1 = g.player[2].space[3]+margin
    y2 = g.tableCardSpace[0][1] -g.potOutlineWidth
    g.potSpace=(x1,y1,x2,y2)
    nextRound()

    #RAISE DIAG SPACE
    diagWidth = g.screenWidth/4
    diagHeight= g.screenHeight/4
    heightPadding = diagHeight/10
    widthPadding = diagWidth /10
    inputBoxHeight = diagHeight /5

    g.diagX1=(g.screenWidth-diagWidth)/2
    g.diagX2=g.diagX1+diagWidth
    g.diagY1=(g.screenHeight-diagHeight)/2
    g.diagY2 = g.diagY1 + diagHeight
    g.inputBoxX1 = g.diagX1 + widthPadding
    g.inputBoxX2 = g.diagX2 - widthPadding
    g.inputBoxY2 = g.diagY2 - heightPadding
    g.inputBoxY1 = g.inputBoxY2 - inputBoxHeight

def init():
    initGameVars()
    initGraphicsVars()

#--------------------------------------CONTROLLER------------------------------

def mousePressed(event):
    (x,y)=(event.x,event.y)
    if(pressedIn(x,y,g.buttonSpace[0])):
        doCall()
    elif(pressedIn(x,y,g.buttonSpace[1])):
        doCheck()
    elif(pressedIn(x,y,g.buttonSpace[2])):
        doRaise()
    elif(pressedIn(x,y,g.buttonSpace[3])):
        doFold()
    redrawAll()

def keyPressed(event):
    if(g.diagUp == True):
        if (event.char.isdigit()):
            g.raiseAmount+=event.char
        elif(event.keysym == 'BackSpace' and len(g.raiseAmount)>0):
            g.raiseAmount = g.raiseAmount[0:len(g.raiseAmount)-1]
        if(event.keysym == 'Return'):
            if(int(g.raiseAmount) > g.activePlayer.stackSize or
               int(g.raiseAmount) < (g.activePlayer.amountDue + g.blinds)):
                pass
            else:
                g.diagUp = False
                makeRaise(int(g.raiseAmount))
                g.raiseAmount = ''
                redrawAll()
        else:
            drawDiag()


def doCall():
    if(g.activePlayer.amountDue!=0):

        #if player cannot meet amount due, make split pot
        if(g.activePlayer.stackSize < g.activePlayer.amountDue):
            bet(g.activePlayer,g.activePlayer.stackSize)
            g.activePlayer.stackSize = 0
            g.activePlayer.allIn = True
            g.activePlayer.amountDue = 0
            g.allInPlayers.append(g.activePlayer)
        else:
            bet(g.activePlayer,g.activePlayer.amountDue)
        switchActivePlayer()
        if(g.notRaised == False and g.activePlayer.amountDue == 0):
            nextRound()

def doCheck():
    #can only check if a bet hasnt been made

    if(g.notRaised == True and g.activePlayer.amountDue==0):
        if(g.activePlayer==g.lastToCheck):
            nextRound()
        else:
            switchActivePlayer()

def doRaise():
    g.diagUp=True

def makeRaise(amount):
    g.notRaised = False
    bet(g.activePlayer,amount)
    switchActivePlayer()

def doFold():
    g.activePlayer.folded=True
    g.numActivePlayers-=1
    if(g.numActivePlayers == 1):
        endRoundBeforeShow()
    switchActivePlayer()
    if(g.notRaised == False and g.activePlayer.amountDue == 0):
        nextRound()

def nextRound():
    g.playCounter+=1
    if(g.playCounter==1):
        print 'dealing'
        deal()
        preFlop()
    elif(g.playCounter==2):
        #FLOP
        print 'flop'
        for i in xrange(3):
            g.tableCards.append(getCard())
        if(g.round == 1):
            g.tableCards=g.firstRoundCards[:3]
        nextBetRound()
    elif(g.playCounter==3 or g.playCounter == 4):
        #turn and river
        print 'turn/river'
        if(g.round==1):
            if(len(g.tableCards)==3):
                g.tableCards.append((10,3))
            else:
               g.tableCards.append((2,1))
        nextBetRound()
    elif(g.playCounter ==5):
        showdown()


def getActivePlayers():
    activePlayers = []
    for i in xrange(g.numPlayers):
        if(g.player[i].folded == False):
            activePlayers.append(i)
    return activePlayers

def makeIntCard(rank, suit):
    card = (14 + 1) * g.SUIT_VAL[suit] #magic number but who cares
    card+= rank  + 1
    return card

def showdown():
    print 'called'
    activePlayers = getActivePlayers()
    g.tableIntCards = []
    assert(len(activePlayers) > 1)
    for i in xrange(len(g.tableCards)):
        (rank,suit) = (g.tableCards[i][0],g.tableCards[i][1])
        g.tableIntCards.append(makeIntCard(rank,suit))

    for i in activePlayers:
        print i
        g.player[i].intCards=[]
        for j in xrange(2): #magic number again
            print j,g.player[i].intCards
            (rank,suit) = (g.player[i].card[j][0],g.player[i].card[j][1])
            g.player[i].intCards.append(makeIntCard(rank,suit))

    winner = compareTwoPlayers(g.player[activePlayers[0]],
                                g.player[activePlayers[1]])
    if(len(activePlayers)==2):
        giveWinner(winner)
    else:
        winner2 = compareTwoPlayers(g.player[activePlayers[2]],
                                     g.player[winner])
        if(len(activePlayers)==3):
            giveWinner(winner2)
        else:
            giveWinner(compareTwoPlayers(g.player[activePlayers[3]],
                                         g.player[winner2]))
    g.playCounter = 0
    g.round+=1


def giveWinner(i):
    g.player[i].stackSize+= g.potSize

def compareTwoPlayers(p1,p2):
    (card1,card2,card3,card4,card5)=g.tableIntCards
    winner = pokerbot.bestHand(p1.intCards[0],p1.intCards[1],
                               p2.intCards[0],p2.intCards[1],
                        card1,card2,card3,card4,card5)


    if(winner == 2):
        return p1.index
    else:
        return p2.index


def deal():
    #no players have folded yet
    g.numActivePlayers = g.numPlayers
    for i in xrange(4):
        g.player[i].folded = False
        g.player[i].allIn = False
        g.allInPlayers =[]
    g.deck = []
    g.littleBlind = (g.littleBlind+1)%g.numPlayers
    g.bigBlind = (g.bigBlind+1)%g.numPlayers

    #generate deck
    for i in xrange(len(g.SUITS)):
        for j in xrange(1,len(g.RANKS)):
            g.deck.append((g.RANKS[j],g.SUIT_VAL[i]))


        #deal 2 random cards to each player
        for j in xrange(2):
            #deal card and remove from deck
            g.player[i].card[j]=getCard()
    if(g.round==1):
        firstRoundPlayerCards()
    elif(g.round==2):
        secondRoundPlayerCards()
    redrawAll()

def firstRoundPlayerCards():
    g.player[0].card=[(12,2),(10,0)]
    g.player[1].card=[(6,3),(8,2)]
    g.player[2].card=[(8,1),(11,2)]
    g.player[3].card=[(2,3),(7,1)]

def secondRoundPlayerCards():
    g.player[0].card=[(12,2),(10,0)]
    g.player[1].card=[(8,3),(6,2)]
    g.player[2].card=[(9,2),(4,3)]
    g.player[3].card=[(12,3),(12,2)]
    
def preFlop():
    g.potSize = 0
    g.tableCards = []
    g.tableIntCards = []

    nextBetRound()
    #put up blinds
    bet(g.player[g.littleBlind],int(g.blinds/2))
    bet(g.player[g.bigBlind],g.blinds)

    #person after big blind starts, only for pre-flop
    g.activePlayer = g.player[(g.bigBlind+1)% g.numPlayers]
    g.lastToCheck= g.player[g.bigBlind]
    redrawAll()

def nextBetRound():
    #no player has bet as yet
    for i in xrange(g.numPlayers):
        g.player[i].bet=0
    #set active player
    g.notRaised = True
    g.highestBet=0
    g.activePlayer = prevPlayer(g.player[g.littleBlind])
    switchActivePlayer()
    g.lastToCheck = prevNonFoldedPlayer(
                    g.player[(g.littleBlind-1)%g.numPlayers])
    
def bet(player, amount):
    player.stackSize -= amount
    g.potSize+=amount
    player.bet+=amount
    if(player.bet > g.highestBet): g.highestBet = player.bet
    for i in xrange(g.numPlayers):
        g.player[i].amountDue = g.highestBet - g.player[i].bet
        print g.highestBet,g.player[i].bet,g.player[i].amountDue
        assert(g.player[i].amountDue>=0)

    for i in xrange(g.numPlayers):
        print 'Player ',i,' amount due: ',g.player[i].amountDue

def getCard():
    assert(len(g.deck)!=0) #deck must have cards in it
    card=(random.choice(xrange(len(g.deck))))
    return g.deck.pop(card)


def getMin(bounds):
    (x1,y1,x2,y2)=bounds
    x = int((abs(x1-x2)/2))
    y= int(abs(y1-y2)/2)
    if x>y: return y
    else: return x

def text(pos,caption,fill = 'black'):
    (x,y) = center(pos)
    canvas.create_text(x,y,text=caption,anchor=CENTER, font = 'Arial 32',
                       fill=fill)

def center(bounds):
    (x1,y1,x2,y2) = bounds
    x = int(x1+x2)/2
    y = int(y1+y2)/2
    return (x,y)


#Situation where all but one player has folded.
#Players do not need to match the best hands to decide the winner.
def endRoundBeforeShow():
    assert(g.numActivePlayers == 1) #only end before show if 1 player remains
    for i in xrange(g.numPlayers):
        if(g.player[i].folded==False):
            winner = i
    giveWinner(winner)

def switchActivePlayer():
    print 'aybaybay'
    #moves to next player, then checks for the next nonfolded player
    g.activePlayer=nextNonFoldedPlayer(nextPlayer(g.activePlayer))
    if(g.activePlayer.isBot):
       botAction()

def botAction():
    print 'Rise up di bot action!'
    print g.notRaised
    if(g.notRaised == False and g.activePlayer.amountDue == 0):
        nextRound()
    else:
        print 'ya trick'
        player=g.activePlayer
        chance = 2.0
        c1 = makeIntCard(player.card[0][0], player.card[0][1])
        c2 = makeIntCard(player.card[1][0],player.card[1][1])
        if(len(g.tableCards)>=3):
            c3 = makeIntCard(g.tableCards[0][0],g.tableCards[0][1])
            c4 = makeIntCard(g.tableCards[1][0],g.tableCards[1][1])
            c5 = makeIntCard(g.tableCards[2][0],g.tableCards[2][1])
            chance = pokerbot.botStrength(c1,c2,c3,c4,c5,0,0)
            if(len(g.tableCards)==3):
                chance = pokerbot.botStrength(c1,c2,c3,c4,c5,0,0)
            elif(len(g.tableCards)==4):
                c6=makeIntCard(g.tableCards[3][0],g.tableCards[3][1])
                chance = pokerbot.botStrength(c1,c2,c3,c4,c5,c6,0)
            else:
                c6=makeIntCard(g.tableCards[3][0],g.tableCards[3][1])
                c7=makeIntCard(g.tableCards[4][0],g.tableCards[4][1])
                chance = pokerbot.botStrength(c1,c2,c3,c4,c5,c6,c7)
        else:
            chance = pokerbot.botStrength(c1,c2,0,0,0,0,0)
        botBet = int(g.potSize * (chance ** g.numActivePlayers))
        print 'I have a ',chance**g.numActivePlayers, ' chance to win!'
        print 'I want to bet ',botBet
        if(botBet >= g.activePlayer.amountDue + g.blinds):
            print 'raised'
            makeRaise(botBet)
        elif(botBet<g.activePlayer.amountDue):
            print 'folded'
            doFold()
        elif(g.activePlayer.amountDue==0):
            print 'checked'
            doCheck()
        else:
            print 'called'
            doCall()

def prevNonFoldedPlayer(player):
    while(player.folded==True):
        player=prevPlayer(player)
    return player

def nextNonFoldedPlayer(player): #also skips all in players
    while(player.folded==True or player.allIn == True):
        player=nextPlayer(player)
    return player


def prevPlayer(player):
    return g.player[(player.index-1)%g.numPlayers]

def nextPlayer(player):
    return g.player[(player.index+1)%g.numPlayers]

def pressedIn(x,y,bounds):
    (x1,y1,x2,y2)=bounds
    return (x>=x1 and x<=x2 and y>=y1 and y<=y2)


#CURRENTLY NOT USING TIMERFIRED
def timerFired():
    redrawAll()
    delay = 250 # milliseconds
    canvas.after(delay, timerFired) # pause, then call timerFired again


#----------------------------------------VIEW-------------------------------
def drawDiag():
    pos = (g.diagX1,g.diagY1,g.diagX2,g.diagY2)
    (x,y) = center(pos)
    canvas.create_rectangle(g.diagX1,g.diagY1,g.diagX2,g.diagY2,
                            fill = g.diagColor)
    canvas.create_rectangle(g.inputBoxX1,g.inputBoxY1,g.inputBoxX2,g.inputBoxY2,
                            fill = g.inputBoxColor)
    canvas.create_text(x,y, text = g.diagCaption, anchor = CENTER,
                       font = g.diagCaptionFont)
    pos = (g.inputBoxX1,g.inputBoxY1,g.inputBoxX2,g.inputBoxY2)
    (x,y) = center(pos)
    canvas.create_text(x,y,text = g.raiseAmount, anchor = CENTER,
                       font = g.diagCaptionFont)
def drawBlinds():
    blindIndex = 0
    for i in [g.littleBlind,g.bigBlind]:
        (cx,cy) = g.player[i].blindCenter
        drawCircle(cx,cy,g.player[i].blindRadius,fill=g.blindsFill[blindIndex])
        canvas.create_text(cx,cy,text=g.blindsText[blindIndex],
                           anchor=CENTER, font = 'Arial 16')
        blindIndex+=1

def drawCircle(cx,cy,r,fill='black'):
    canvas.create_oval(cx-r,cy-r,cx+r,cy+r,fill=fill)

def drawCard(pos,card):
    x=int(pos[0] + (0.8* (abs(pos[2]-pos[0]))))
    y=int(pos[1]+ (0.25*abs(pos[1]-pos[3])))
    suit=g.SUITS[card[1]]
    caption = g.FACE[card[0]-2]
    if suit == '♦' or suit == '♥':
        color = 'red'
    else:
        color = 'black'
    canvas.create_rectangle(pos,fill=g.cardColor)
    canvas.create_text(x,y,text=caption,anchor= CENTER,
                       font ='Arial 32', fill = color)
    (x,y) = center(pos)
    canvas.create_text(x,y,text=suit,anchor= CENTER,
                       fill = color, font ='Arial 32')
def redrawAll():
    #canvas.delete(ALL)
    canvas.create_rectangle(0,0,g.screenWidth,g.screenHeight,fill=g.bgColor)
    canvas.create_rectangle(g.actionBar,fill='green')

    #DRAW FOR ALL PLAYERS
    for i in xrange(4):
        if(g.activePlayer==g.player[i]):
            outline='yellow'
        else:
            outline= None
        canvas.create_rectangle(g.player[i].space, fill = g.spaceColor,
                                outline = outline,width=5)

        canvas.create_rectangle(g.buttonSpace[i],width=5,
                                fill='yellow',activefill='light yellow',
                                disabledfill='grey')
        text(g.player[i].stackSpace,g.player[i].stackSize,g.stackColor)
        text(g.buttonSpace[i],g.actionLabels[i]) #provided there are 4 butns
        canvas.create_rectangle(g.potSpace,width=g.potOutlineWidth)
        text(g.potSpace,g.potSize)

    #DRAW PLAYER CARDS IF THEY HAVENT FOLDED
        if(g.player[i].folded==False):
            for j in xrange(2):
                drawCard(g.player[i].cardSpace[j],g.player[i].card[j])
    for i in xrange(len(g.tableCards)):
        drawCard(g.tableCardSpace[i],g.tableCards[i])

    drawBlinds()

    caption = 'Amount due:'+ str(g.activePlayer.amountDue)
    canvas.create_text(g.screenWidth,0,text=caption,
                       anchor = NE, font = 'Arial 24')
    if(g.diagUp == True):
        drawDiag()

# - - - - - - - - - - - - - -  - R U N - - - - - - - - - - - - - - - - - - - -
def run():
    # create the root and the canvas
    global canvas
    global g
    class Struct: pass
    g = Struct()
    g.screenHeight = 800
    g.screenWidth = 1200
    root = Tk()
    canvas = Canvas(root, width=g.screenWidth, height=g.screenHeight)
    canvas.pack()
    init()
    # set up events
    root.bind("<KeyPress>", keyPressed)
    root.bind("<Button-1>", mousePressed)
    #timerFired()
    # and launch the app
    root.mainloop()  # This call BLOCKS (so your program waits until you close the window!)

run()
