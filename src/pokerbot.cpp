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

card makeCard(rank r, suit s)
{
	int res = (NUM_RANKS + 1) * (int)s;
	res += (int)r;
	return (card) res;
}

rank get_rank(card c)
{
	return (rank)(c % (NUM_RANKS + 1));
}

suit get_suit(card c)
{
	return (suit)(c / (NUM_RANKS + 1));
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
bool revrankComp (card a, card b) {return (int)get_rank(a) > (int)get_rank(b);}


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

static inline rank checkSF(card * cards, suit s)
{
	//first check for straights with ace low
	std::replace(cards, cards+NUM_BOARD_CARDS+NUM_HELD_CARDS, ACE_HIGH, ACE_LOW);

	//sort the cards by rank
	std::sort(cards, cards + NUM_BOARD_CARDS+NUM_HELD_CARDS, rankComp);


	rank highestrun = (rank)0;
	int conseq = 0;
	rank lastcard = get_rank(cards[0]);
	if (get_suit(cards[0]) == s)
	{
		conseq++;
	}
	for (int i = 1; i < NUM_HELD_CARDS + NUM_BOARD_CARDS; i++)
	{
		rank curr = get_rank(cards[i]);
		if (curr == lastcard) continue;//skip duplicate cards
		if (get_suit(cards[i]) != s) continue;
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

	conseq = 0;
	lastcard = get_rank(cards[0]);
	if (get_suit(cards[0]) == s)
	{
		conseq++;
	}
	for (int i = 1; i < NUM_HELD_CARDS + NUM_BOARD_CARDS; i++)
	{
		rank curr = get_rank(cards[i]);
		if (curr == lastcard) continue;//skip duplicate cards
		if (get_suit(cards[i]) != s) continue;
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
	if ((int)highestrun == 0)
		return (rank)0;
	else
	{
		for (int i= 0; i < 5; i++)
		{
			cards[i] = makeCard((rank)((int)highestrun - i), s);
		}
		return highestrun;
	}
}

void flushSort(card * cards, suit s)
{
	card * flush = (card *) malloc(sizeof(card)*7);
	int count = 0;
	for(int i = 0; i < NUM_BOARD_CARDS+NUM_HELD_CARDS; i++)
	{
		if (get_suit(cards[i])==s)
		{
			flush[count] = cards[i];
			count++;
		}
	}
	std::sort(flush, flush + count, revrankComp);
	for(int i = 0; i < count; i++)
	{
		cards[i] = flush[i];
	}
	free(flush);
}

void straightSort(card * cards, rank highest)
{
	card * strt = (card *)malloc(sizeof(card) * 5);
	for (int i = 0; i < 5; i ++)
	{
		for(int j = 0; j < NUM_HELD_CARDS+NUM_BOARD_CARDS; j++)
		{
			if(get_rank(cards[j])==(rank)((int)highest-i))
			{
				strt[i] = cards[j];
				break;
			}
		}
	}
	for(int i = 0; i < 5; i++)
	{
		cards[i] = strt[i];
	}
	free(strt);
}

//returns true iff h1 beats h2
hand_compare_res hand_beats(hand h1, hand h2)
{
	if((int)h1.type > (int)h2.type)
		return BEATS;
	else if ((int)h1.type == (int)h2.type)
	{
		for(int i = 0; i < 5; i++)
		{
			int curr1 = get_rank((*h1.cards)[i]);
			int curr2 = get_rank((*h2.cards)[i]);
			if (curr1 > curr2)
				return BEATS;
			if (curr1 < curr2)
				return LOSES;
			if (i==4)
				return TIES;
		}
	}

    return LOSES;

}

//returns a hand struct with the best hand that can be made from the 7 cards
//ONLY THE FIRST 5 CARDS IN THE RETURNED HAND ARE VALID.
hand best_hand(card * cards)
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

	if (straight && flush && (int)checkSF(cards,(suit)iflush) != 0)
	{
		return (hand){STRAIGHT_FLUSH, cards};
		//first 5 cards are the ones used in order.
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
	card third, fourth, fifth;
	switch (bcount)
	{
		case 4://quadruple of brank
				fifth = (card)0;
				for(int i =0; i< NUM_BOARD_CARDS+NUM_HELD_CARDS; i++)
				{
					rank curr = get_rank(cards[i]);
					if (curr == brank) continue;
					if ((int)curr > (int)get_rank(fifth))
						fifth = cards[i];
				}
				cards[0] = makeCard(brank, HEART);
				cards[1] = makeCard(brank, SPADE);
				cards[2] = makeCard(brank, CLUB);
				cards[3] = makeCard(brank, DIAMOND);
				cards[4] = fifth;
				return (hand){QUADRUPLE, cards};
				//ONLY FIRST 5 ARE VALID
			break;
		case 3://triple or full house
			xtraPairRank = pair_besides(cards, brank);
			if ((int)xtraPairRank == 0)
			{
				if (flush)
				{
					flushSort(cards, (suit)iflush);
					return (hand){FLUSH, cards};
					//ONLY FIRST 5 VALID
				}else
				{
					if (straight)
					{
						straightSort(cards, straightrank);
						return (hand){STRAIGHT, cards};
						//ONLY FIRST 5 ARE VALID
					}else
					{
						//there is no additional pair so the hand is a triple of brank
						int triplecount = 0;
						fourth = (rank)0;
						fifth = (rank)0;
						for(int i=0; i < NUM_BOARD_CARDS+NUM_HELD_CARDS; i++)
						{
							rank curr = get_rank(cards[i]);
							if (curr == brank)
							{
								cards[triplecount] = cards[i];
								triplecount++;
								continue;
							}
							if ((int)curr > (int)get_rank(fourth))
							{
								fifth = fourth;
								fourth = cards[i];
							}else if ((int)curr > (int)fifth)
								fifth = cards[i];
						}
						cards[3] = fourth;
						cards[4] = fifth;
						return (hand){TRIPLE, cards};
						//ONLY FIRST 5 VALID
					}
				}
			}else
			{
				//full house with 3 of brank and 2 of xtraPairRank
				card * FH = (card *)malloc(sizeof(card) * 5);
				int tricount = 0;
				int paircount = 0;
				for(int i = 0; i < NUM_BOARD_CARDS+ NUM_HELD_CARDS; i++)
				{
					rank curr = get_rank(cards[i]);
					if (curr == brank)
					{
						FH[tricount] = cards[i];
						tricount++;

					}else if (curr == xtraPairRank)
					{
						FH[3+paircount] = cards[i];
						paircount++;
					}
				}
				for(int i = 0; i < 5; i++)
				{
					cards[i] = FH[i];
				}
				free(FH);
				return (hand){FULL_HOUSE, cards};
				//ONLY FIRST 5 ARE VALID
			}


			break;
		case 2://pair or 2 pair

			if (flush)
			{
				flushSort(cards, (suit)iflush);
				return (hand){FLUSH, cards};
				//ONLY FIRST 5 VALID
			}
			if (straight)
			{
				straightSort(cards, straightrank);
				return (hand){STRAIGHT, cards};
				//ONLY FIRST 5 ARE VALID
			}
			xtraPairRank = pair_besides(cards, brank);
			if ((int)xtraPairRank == 0)
			{
				//there is no additional pair so the hand is a pair of brank
				int paircount = 0;
				third = (rank)0;
				fourth = (rank)0;
				fifth = (rank)0;
				for(int i=0; i < NUM_BOARD_CARDS+NUM_HELD_CARDS; i++)
				{
					rank curr = get_rank(cards[i]);
					if (curr == brank)
					{
						cards[paircount] = cards[i];
						paircount++;
						continue;
					}
					if ((int)curr > (int)get_rank(third))
					{
						fifth = fourth;
						fourth = third;
						third = cards[i];
					}else if ((int)curr > (int)get_rank(fourth))
					{
						fifth = fourth;
						fourth = cards[i];
					}else if ((int)curr > (int) get_rank(fifth))
						fifth = cards[i];
				}
				cards[2] = third;
				cards[3] = fourth;
				cards[4] = fifth;
				return (hand){PAIR, cards};
				//ONLY FIRST 5 ARE VALID
			}else
			{
				//two pair of brank and xtraPairRank
				card * TP = (card *)malloc(sizeof(card) * 4);
				int p1count = 0;
				int p2count = 0;
				fifth = (card)0;
				for(int i =0; i< NUM_BOARD_CARDS+NUM_HELD_CARDS; i++)
				{
					rank curr = get_rank(cards[i]);
					if (curr == brank)
					{
						TP[p1count] = cards[i];
						p1count++;
						continue;
					}
					if(curr == xtraPairRank)
					{
						TP[2+p2count] = cards[i];
						p2count++;
						continue;
					}
					if ((int)curr > (int)get_rank(fifth))
						fifth = cards[i];
				}
				for(int i = 0; i < 4; i++)
				{
					cards[i] = TP[i];
				}
				free(TP);
				cards[4] = fifth;
				return (hand){TWO_PAIR, cards};
				//ONLY FIRST 5 VALID
			}

			break;
		case 1://all cards have distinct rank
			if (flush)
			{
				flushSort(cards, (suit)iflush);
				return (hand){FLUSH, cards};
				//ONLY FIRST 5 VALID
			}
			if (straight)
			{
				straightSort(cards, straightrank);
				return (hand){STRAIGHT, cards};
				//ONLY FIRST 5 ARE VALID
			}

			//Only High Card
			std::sort(cards, cards + NUM_BOARD_CARDS+NUM_HELD_CARDS, revrankComp);
			return (hand){HIGH_CARD, cards};

			break;
	}
    return (hand){HIGH_CARD, cards};
}

int count_cards(card *cards) {
    // Count the number of cards we know
    int num_cards_known = 0;
    for (int i = 0; i < NUM_BOARD_CARDS; ++i) {
        if (is_known(cards[i])) {
            num_cards_known++;
        }
    }
    return num_cards_known;
}

int chen_score(card c) {
    if (get_rank(c) < 1) {
        printf("There's been a huge mistake. \n");
    }
    switch (get_rank(c)) {
        case ACE_HIGH: return 10; break;
        case ACE_LOW: return 10; break;
        case KING: return 8; break;
        case QUEEN: return 7; break;
        case JACK: return 6; break;
        default: return get_rank(c) / 2; break;
    }
}

int chen_formula(card first, card second) {
    int base_score = std::max(chen_score(first), chen_score(second));
    if (base_score < 1) {exit(1);}
    if (get_rank(first) == get_rank(second)) {
        base_score = std::max(5, base_score * 2);
    }
    if (get_suit(first) == get_suit(second)) {
        base_score += 2;
    }
    int first_rank = get_rank(first);
    int second_rank = get_rank(second);
    int gap = std::min(
    std::min(
        std::abs(first_rank - second_rank),
        std::abs(first_rank - second_rank % NUM_RANKS)
    ), std::min(
        std::abs(first_rank % NUM_RANKS - second_rank % NUM_RANKS),
        std::abs(first_rank % NUM_RANKS - second_rank)
    ));
    switch (gap) {
        case 0: break;
        case 1: base_score++; break;
        case 2: base_score--; break;
        case 3: base_score -= 2; break;
        case 4: base_score -= 4; break;
        default: base_score -= 5; break;
    }
    //printf("score: %d", base_score - gap);
    return base_score - gap;
}

// Returns 0 if the hero can tie or win, else 1
int chens_pocket_algo(card *hero, card *adversary) {
    int hero_score = chen_formula(hero[0], hero[1]);
    int adversary_score = chen_formula(adversary[0], adversary[1]);
    return adversary_score > hero_score;
}

// We simulate the board to determine a winner.
// If 0 the hero wins or ties, if 1 the adversary wins
int simulate_winner(card *hero, card *adversary) {
    // Not a possible situation.
    if (count_cards(hero) < 2) {
        return -1;
    }
    // If we are in pockets, we use Chen's algorithm.
    if (count_cards(hero) == 2) {
        return chens_pocket_algo(hero, adversary);
    }

    // Otherwise, we continue calculating with simulations.
    return 0;

}

int count_cards(card *c);
// We model one opponent and simulate their hold cards with monte carlo
double simulate_hands(game_state * game) {
    if (!count_cards(game->board)) {
        unsigned int counted = 0;
        unsigned int won = 0;
        for (int i = 1; i < NUM_DECK_CARDS; i++) {
            if ((card)i == game->hero->cards[0] || (card)i == game->hero->cards[1] || !get_rank((card)i)) {
                continue;
            } else {
                for (int j = 1; j < NUM_DECK_CARDS; j++) {
                    if ((card)j == game->hero->cards[0] || (card)j == game->hero->cards[1] || j == i || !get_rank((card)j)) {
                        continue;
                    } else {
                        card adversary[NUM_HAND_CARDS] = {(card)i, (card)j, 0, 0, 0};
                        int we_lost = simulate_winner(game->hero->cards, (card *)&adversary);
                        if (!we_lost) won++;
                        counted++;
                    }
                }
            }
        }
        return (1.0 * (double)won) / counted;
    }

    if (count_cards(game->board) == 3) {
    }

    int r = rand();
    return (double)(r % 1000)/1000;
}

unsigned long long bot_choice(game_state * game) {
    return rand();
}

double bot_strength(game_state * game) {
    return simulate_hands(game);
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
void print_hand_compare_res(hand_compare_res hcr)
{
	switch (hcr)
	{
		case LOSES:
			printf("%s\n", "First hand LOSES to second hand.");
			break;
		case TIES:
			printf("%s\n", "First hand TIES with second hand.");
			break;
		case BEATS:
			printf("%s\n", "First hand BEATS second hand.");
			break;
	}
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

    card * h = make7Hand((card)6,(card)8,(card)7,(card)2,(card)5,(card)12,(card)46);
    card * h2 = make7Hand((card)6,(card)25,(card)7,(card)15,(card)35,(card)32,(card)47);
    card * h3 = make7Hand((card)16,(card)23,(card)22,(card)9,(card)41,(card)50,(card)4);
    card * h4 = make7Hand((card)2,(card)17,(card)37,(card)15,(card)30,(card)13,(card)47);

    printHand(h,7);
    hand stHand = best_hand(h);
    printHandRank(stHand.type);
    std::cout << ", ";
    printHand((*stHand.cards),5);
    std::cout << "\n\n";

    printHand(h2,7);
    hand stHand2 = best_hand(h2);
    printHandRank(stHand2.type);
    std::cout << ", ";
    printHand((*stHand2.cards),5);
	std::cout << "\n\n";

    printHand(h3,7);
    stHand = best_hand(h3);
    printHandRank(stHand.type);
    std::cout << ", ";
    printHand((*stHand.cards),5);
    std::cout << "\n\n";

    printHand(h4,7);
    hand stHand4 = best_hand(h4);
    printHandRank(stHand4.type);
    std::cout << ", ";
    printHand((*stHand4.cards),5);

    std::cout << "\n";
    print_hand_compare_res(hand_beats(stHand2,stHand4));

    game_state *game = (game_state*)malloc(sizeof(game_state));
    game->hero = (player*)calloc(1, sizeof(player));
    game->hero->cards[0] = 2;
    game->hero->cards[1] = 7;
    std::cout << "running\n";
    std::cout << simulate_hands(game);

    return 0;
}

