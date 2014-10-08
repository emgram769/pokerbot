/* This file contains the implementation of the poker bot. */

#include <stdio.h>
#include <pokerbot.h>
#include <time.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
    printf("Hi I'm a poker bot!\n");
    return init_pokerbot();
}

