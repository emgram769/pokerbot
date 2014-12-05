

from Tkinter import *
import random
import copy
import winsound

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
    g.SUITS = ['S','C','H','D']
    g.RANKS = range(2,15)
    g.FACE = range(2,11) + ['J','Q','K','A']
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
                winsound.Beep(700,300)
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
            bet(g.activePlayer,g.player.StackSize)
            g.player.stackSize = 0
            g.player.allIn = True
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
    g.notRaised = False

def makeRaise(amount):
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
    if(g.playCounter==0):
        deal()
        preFlop()
    elif(g.playCounter==1):
        #FLOP
        for i in xrange(3):
            g.tableCards.append(getCard())
        nextBetRound()
        
    elif(g.playCounter==2 or g.playCounter == 3):
        #turn and river
        g.tableCards.append(getCard())
        nextBetRound()
    else:
        showdown()
    g.playCounter+=1

def showdown():
    pass

def deal():
    #no players have folded yet
    g.numActivePlayers = g.numPlayers
    for i in xrange(4):
        g.player[i].folded = False
        #g.player[i].allIn = False
    g.deck = []
    g.littleBlind = (g.littleBlind+1)%g.numPlayers
    g.bigBlind = (g.bigBlind+1)%g.numPlayers
    
    #generate deck
    for i in xrange(len(g.SUITS)):
        for j in xrange(len(g.RANKS)):
            g.deck.append((g.RANKS[j],g.SUITS[i]))
    

        #deal 2 random cards to each player
        for j in xrange(2):
            #deal card and remove from deck
            g.player[i].card[j]=getCard()
    redrawAll()

def preFlop():
    g.potSize = 0
    g.tableCards = []
    
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
    g.activePlayer = nextNonFoldedPlayer(g.player[g.littleBlind])
    g.lastToCheck = prevNonFoldedPlayer(
                    g.player[(g.littleBlind-1)%g.numPlayers])
    
def bet(player, amount):
    player.stackSize -= amount
    g.potSize+=amount
    player.bet+=amount
    if(player.bet > g.highestBet): g.highestBet = player.bet
    for i in xrange(g.numPlayers):
        g.player[i].amountDue = g.highestBet - g.player[i].bet
        assert(g.player[i].amountDue>=0)
    
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
            g.player[i].stackSize+= g.potSize
            g.playCounter = 0
            nextRound()

def switchActivePlayer():
    #moves to next player, then checks for the next nonfolded player
    g.activePlayer=nextNonFoldedPlayer(nextPlayer(g.activePlayer))

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
    caption = g.FACE[card[0]-2]
    if card[1]== 'D' or card[1]== 'H':
        color = 'red'
    else:
        color = 'black'
    canvas.create_rectangle(pos,fill=g.cardColor)
    canvas.create_text(x,y,text=caption,anchor= CENTER,
                       font ='Arial 32', fill = color)
    (x,y) = center(pos)
    canvas.create_text(x,y,text=card[1],anchor= CENTER,
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
