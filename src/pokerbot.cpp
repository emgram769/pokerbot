/* This file contains the implementation of the poker bot. */

#include <stdio.h>
#include <pokerbot.h>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

using std::string;

/* Gets the suit and rank of a card and returns them through
 * passed in references. Returns 0 on success and -1 on failure.*/
int get_card(card c, suit * s, rank * r) {
    /* Bounds check on the card. */
    if (c > (NUM_SUITS * NUM_RANKS)) {
        return -1;
    }
    
    /* Rank is 0 if unkown, and 1-13 if known. */
    *r = (rank)(c % (NUM_RANKS + 1));

    /* Suit is always 0 - 3. */
    *s = (suit)(c / (NUM_RANKS+1));

    return 0;
}

rank get_rank(card c)
{
	return (rank)(c % (NUM_RANKS + 1));
}

suit get_suit(card c)
{
	return (suit)(c / (NUM_RANKS+1));
}

/* Checks if a card is known. */
static inline bool is_known(card c) {
    if (c > (NUM_SUITS * NUM_RANKS)) {
        /* Should raise an error. */
        return 0;
    } else if (c % (NUM_RANKS + 1) == 0) {
        return 0;
    } else {
        return 1;
    }
}

int compare_function(const void *a,const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

/* Sort the cards to make the best hand.  The indices are guaranteed
 * to be include (calculated by the calling function). The number of
 * indices is n. */
static inline void sift_best_cards(card * cards, int * indices, int n) {
    /* Sort the indices. */
    qsort(indices, n, sizeof(int), compare_function);

    for (int i = 0; i < n; ++i) {
        /* Swap to the front. Indices must be sorted for this work. */
        cards[i] = cards[indices[i]];
    }

    int * best_cards = (int*)malloc(sizeof(int) * (NUM_HAND_CARDS - n));
    for (int i = n; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {
    }
}

/* Returns if 7 cards can make a pair. */
static inline bool is_pair(card * cards) {
    /* The rank of the best pair. */
    rank best_pair = UNKNOWN;
    /* Indices of the best pair. */
    int b1, b2;

    /* Iterate to the second to last card, searching for a pair. */
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS - 1; ++i) {
        int candidate_index = i;
        rank r1; suit s1;
        get_card(cards[i], &s1, &r1);
        
        /* Only better pairs matter. */
        if (r1 < best_pair) continue;

        for (int j = i + 1; j < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++j) {
            rank r2; suit s2;
            get_card(cards[j], &s2, &r2);
            if (r2 == r1) {
                best_pair = r1;
                b1 = i;
                b2 = j;
            }
        }
    }

    return 0;
}

/* Returns if 7 cards can make a two pair. */
static inline bool is_two_pair(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }

    return 0;
}

/* Returns if 7 cards can make three of a kind. */
static inline bool is_triple(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    
    return 0;
}

/* Returns if 7 cards can make a straight. */
static inline bool is_straight(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    
    return 0;
}

/* Returns if 7 cards can make a flush. */
static inline bool is_flush(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    return 0;
}

/* Returns if 7 cards can make a full house. */
static inline bool is_full_house(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    return 0;
}

/* Returns if 7 cards can make a four of a kind. */
static inline bool is_quadruple(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    return 0;
}

/* Returns if 7 cards can make a straight flush. */
static inline bool is_straight_flush(card * cards) {
    for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; ++i) {

    }
    return 0;
}

//returns the rank of any pair that can be made without using cards of rank "besides"
//returns rank "Unknown" if there is no pair
static inline rank pair_besides(card * cards, rank besides)
{
	rank brank = (rank) 0;
	for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; i++)
	{
		//skip the card if its rank = besides
		if (get_rank (cards[i]) == besides)
		{
			continue;
		}
		//otherwise count occurrences
		int currcount = 0;
		for (int j = 0; j < NUM_BOARD_CARDS + NUM_HELD_CARDS; j++)
		{
			if (get_rank(cards[i]) == get_rank(cards[j]))
			{
				currcount++;
				if (currcount >= 2 && ((int)get_rank(cards[j]) > (int)brank))
				{
					brank = get_rank(cards[j]);
				}
			}
		}
	}
	return brank;
}

//returns an int from 0 to 4 that represents 
//the suit of any flush that can be made, or 4 if there is no flush.
static inline int flushOf(card * cards)
{
	int SpadeCount = 0;
	int ClubCount = 0;
	int HeartCount = 0;
	int DiamondCount = 0;
	for (int i = 0; i <NUM_BOARD_CARDS + NUM_HELD_CARDS; i++)
	{
		suit s = get_suit(cards[i]);
		switch (s) 
		{
			case DIAMOND:
				DiamondCount++;
				break;
			case HEART:
				HeartCount++;
				break;
			case CLUB:
				ClubCount++;
				break;
			case SPADE:
				SpadeCount++;
				break;
		}
	}
	if (SpadeCount >= 5) return (int)SPADE;
	else if (ClubCount >= 5) return (int)CLUB;
	else if (HeartCount >= 5) return (int)HEART;
	else if (DiamondCount >= 5) return (int)DIAMOND;
	else return 4;
}


bool rankComp (card a, card b) {return (int)get_rank(a) < (int)get_rank(b);}
//returns the rank of the highest card in the highest run of 5 cards if there
// is a run of 5 cards. If there is not, return "Unknown"
static inline rank straightOf(card * cards)
{
	//first check for straights with ace low
	std::replace(cards, cards+NUM_BOARD_CARDS+NUM_HELD_CARDS, ACE_HIGH, ACE_LOW);

	//sort the cards by rank 
	std::sort(cards, cards + NUM_BOARD_CARDS+NUM_HELD_CARDS, rankComp);

	
	rank highestrun = (rank)0;
	int conseq = 1;
	rank lastcard = get_rank(cards[0]);
	for (int i = 1; i < NUM_HELD_CARDS + NUM_BOARD_CARDS; i++)
	{
		rank curr = get_rank(cards[i]);
		if (curr == lastcard) continue;//skip duplicate cards
		if((int)curr > 1 && (int)curr == (int)lastcard + 1)
		{
			conseq++;
			if (conseq >= 5 && (int)highestrun < (int)curr)
			{
				highestrun = curr;
			}
		}
		else
		{
			conseq=1;
		}
		lastcard = curr;
	}
	//now check for straights with ace high
	std::replace(cards, cards+NUM_BOARD_CARDS+NUM_HELD_CARDS, ACE_LOW, ACE_HIGH);
	//sort the cards by rank 
	std::sort(cards, cards + NUM_BOARD_CARDS+NUM_HELD_CARDS, rankComp);

	conseq = 1;
	lastcard = get_rank(cards[0]);
	for (int i = 1; i < NUM_HELD_CARDS + NUM_BOARD_CARDS; i++)
	{
		rank curr = get_rank(cards[i]);
		if (curr == lastcard) continue;//skip duplicate cards
		if((int)curr > 1 && (int)curr == (int)lastcard + 1)
		{
			conseq++;
			if (conseq >= 5 && (int)highestrun < (int)curr)
			{
				highestrun = curr;
			}
		}
		else
		{
			conseq=1;
		}
		lastcard = curr;
	}
	return highestrun;
}

//returns a hand struct with the best hand that can be made from the 7 cards
static inline hand best_hand(card * cards)
{
	bool straight = false;
	bool flush = false;
	//look for straight
	rank straightrank = straightOf(cards);
	if ((int) straightrank != 0)
	{
		straight = true;
	}
	//look for flush
	int  iflush = flushOf(cards);
	if (iflush != 4)
	{
		flush = true;
	}

//!!!!!!NOT A CORRECT CHECK FOR STAIGHT FLUSH
	if (straight && flush)
	{
		return (hand){STRAIGHT_FLUSH, cards};
		//NEEDS TO RETURN CARDS IN CORRECT ORDER
	}

	//look for pair, triple, or quadruple
	rank brank = (rank) 0; //unknown rank
	int bcount = 0;
	for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; i++)
	{
		int currcount = 0;
		for (int j = 0;  j < NUM_BOARD_CARDS + NUM_HELD_CARDS; j++)
		{
			if (get_rank(cards[i]) == get_rank(cards[j]))
			{
				currcount++;
				if (currcount > bcount
					|| (currcount == bcount && ((int)get_rank(cards[i]) > (int)brank))
				   )
				{
					bcount = currcount;
					brank = get_rank(cards[i]);
				}
			}
		}
	}

	rank xtraPairRank;
	switch (bcount)
	{
		case 4://quadruple of brank
				return (hand){QUADRUPLE, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			break;
		case 3://triple or full house
			xtraPairRank = pair_besides(cards, brank);
			if ((int)xtraPairRank == 0)
			{
				if (flush)
				{
					return (hand){FLUSH, cards};
					//NEEDS TO RETURN CARDS IN CORRECT ORDER
				}else
				{
					if (straight)
					{
						return (hand){STRAIGHT, cards};
						//NEEDS TO RETURN CARDS IN CORRECT ORDER
					}else
					{
						//there is no additional pair so the hand is a triple of brank
						return (hand){TRIPLE, cards};
						//NEEDS TO RETURN CARDS IN CORRECT ORDER
					}
				}
			}else
			{
				//full house with 3 of brank and 2 of xtraPairRank
				return (hand){FULL_HOUSE, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}


			break;
		case 2://pair or 2 pair

			if (flush)
			{
				return (hand){FLUSH, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}
			if (straight)
			{
				return (hand){STRAIGHT, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}
			printf("%s\n", "got here 1");
			xtraPairRank = pair_besides(cards, brank);
			printf("%s\n", "got here 2");
			if ((int)xtraPairRank == 0)
			{
				//there is no additional pair so the hand is a pair of brank
				return (hand){PAIR, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}else
			{
				//two pair of brank and xtraPairRank
				return (hand){TWO_PAIR, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}

			break;
		case 1://all cards have distinct rank
			if (flush)
			{
				return (hand){FLUSH, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}
			if (straight)
			{
				return (hand){STRAIGHT, cards};
				//NEEDS TO RETURN CARDS IN CORRECT ORDER
			}

			//Only High Card
			return (hand){HIGH_CARD, cards};
			//NEEDS TO RETURN CARDS IN CORRECT ORDER

			break;
	}
}

void simulate_hands(game_state * game) {

    for (int i = 0; i < NUM_BOARD_CARDS; ++i) {
        if (is_known(game->board[i])) {
        } else {

        }
    }

    return;
}

unsigned long long bot_choice(game_state * game) {
    return rand();
}

double bot_strength(game_state * game) {
    int r = rand();
    return (double)(r % 1000)/1000;
}

int init_pokerbot(void) {
    srand(time(NULL));
    return 0;
}

//makes a card a string such that the Queen of Diamonds is "QD", 10 of Hearts is "10H"
string cardToString(card c)
{
	string res;
	string r;
	string s;

	switch ((int)get_rank(c))
	{
		case 0:
			r = "U";
			break;
		case 1:
		case 14:
			r = "A";
			break;
		case 11:
			r = "J";
			break;
		case 12:
			r = "Q";
			break;
		case 13:
			r = "K";
			break;

		default:
			r = std::to_string((int)get_rank(c));
	}

	switch (get_suit(c))
	{
		case DIAMOND:
			s = "D";
			break;
		case CLUB:
			s = "C";
			break;
		case HEART:
			s = "H";
			break;
		case SPADE:
			s = "S";
			break;
	}

	res = r+s;
	return res;
}

//print function for hands. useful for testing
void printHand(card* cards, int len)
{
	printf("[");
	for(int i = 0; i < len; i++)
	{
		string suf = "";
		if (i == len -1)
		{
			suf = "]\n";
		}
		else
		{
			suf = ", ";
		}
		suf = cardToString(cards[i]) + suf;
		std::cout << suf;
	}
}

//print handrankenum
void printHandRank (hand_rank hr)
{
	string res;
	switch (hr)
	{
		case HIGH_CARD:
			res = "High Card";
			break;
		case PAIR:
			res = "Pair";
			break;
		case TWO_PAIR:
			res = "Two Pair";
			break;
		case TRIPLE:
			res = "Three of a Kind";
			break;
		case QUADRUPLE:
			res = "Four of a Kind";
			break;
		case FULL_HOUSE:
			res = "Full House";
			break;
		case FLUSH:
			res = "Flush";
			break;
		case STRAIGHT:
			res = "Straight";
			break;
		case STRAIGHT_FLUSH:
			res = "Straight Flush";
			break;
	}
	std::cout << res;
}

//make 7 card hand
card* make7Hand(card c0, card c1,card c2,card c3,card c4,card c5,card c6)
{
	card * res = (card *)malloc(sizeof(card) * 7);
	res[0]=c0;
	res[1]=c1;
	res[2]=c2;
	res[3]=c3;
	res[4]=c4;
	res[5]=c5;
	res[6]=c6;
	return res;
}

int main(int argc, char* argv[]) {
    printf("Hi I'm a poker bot!\n");
    init_pokerbot();

    card * h = make7Hand((card)6,(card)8,(card)7,(card)4,(card)5,(card)22,(card)46);

    printHand(h,7);
    hand stHand = best_hand(h);
    printHandRank(stHand.type);
    std::cout << "\n";
    return 0;
}

