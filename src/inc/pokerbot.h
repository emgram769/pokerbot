/* This file defines the interface for the poker bot. */

#ifndef _POKERBOT_H
#define _POKERBOT_H

#define NUM_SUITS   4
#define NUM_RANKS   13

enum suit { DIAMOND, CLUB, HEART, SPADE };
enum rank { ACE = 1, TWO, THREE, FOUR, FIVE,
            SIX, SEVEN, EIGHT, NINE, TEN,
            JACK, QUEEN, KING };

/* Cards are defined as follows:
 *  suit s = card_num / NUM_RANKS;
 *  rank r = card_num % 14;
 *  if (r == 0) the card is unknown.
 */
typedef unsigned int card;

typedef struct _player {
    void *history;                  /* TODO: make this meaningful. */
    card cards[2];                  /* Contains the cards of the player. */
    unsigned long long chips;       /* Current number of chips. */
    unsigned long long bet;         /* Most recent bet made by the player. */
    unsigned int pos;               /* Position of the player. */
} player;

typedef struct _game_state{
    unsigned long long pot;         /* Count of the chips in the pot. */
    unsigned long long curr_bet;    /* The current highest bet for the round. */
    unsigned int curr_pos;          /* Current player betting. */
    card board[5];                  /* Contains the flop, turn and river. */
    player *hero;                   /* The bot's own player data. */
    player **players;               /* An array of players. */
} game_state;

/* This function returns the betting choice the bot makes. */
unsigned long long bot_choice(game_state * game);

/* This function returns the bots probability of winning the hand. */
double bot_strength(game_state * game);

#endif /* _POKERBOT_H */

