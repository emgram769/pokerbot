/* This file contains the implementation of the poker bot. */

#include <stdio.h>
#include <pokerbot.h>
#include <time.h>
#include <stdlib.h>

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
    *s = (suit)(c / NUM_RANKS);

    return 0;
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

static inline hand best_hand(card * cards)
{
	//look for pair, triple, or quadruple
	int 
	for (int i = 0; i < NUM_BOARD_CARDS + NUM_HELD_CARDS; i++)
	{

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
    /* Get a random variable. */
    int r = rand();
    return (double)(r % 1000)/1000;
}

int init_pokerbot(void) {
    /* Sets up random stuff.  Only call this once. */
    srand(time(NULL));
    return 0;
}

int main(int argc, char* argv[]) {
    printf("Hi I'm a poker bot!\n");
    card pair_test[7] = {1, 2, 15, 4, 3, 5, 7};
    /* Should be 1 not 0. */
    printf("Is pair %d\n", is_pair(pair_test));
    return init_pokerbot();
}

