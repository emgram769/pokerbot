#include <Python.h>
#include <pokerbot.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*
 * This function makes the bot's choice.
 */
static PyObject* py_botChoice(PyObject* self, PyObject* args)
{
    unsigned long long choice = bot_choice(NULL);
    return Py_BuildValue("K", choice);
}

/*
 * This function returns our bot's predicted strength.
 */
static PyObject* py_botStrength(PyObject* self, PyObject* args)
{
    int c1, c2, c3, c4, c5, c6, c7;
    if (!PyArg_ParseTuple(args, "iiiiiii", &c1, &c2, &c3, &c4, &c5, &c6, &c7)) {
        return NULL;
    }
    game_state *game = (game_state*)malloc(sizeof(game_state));
    game->hero = (player*)calloc(1, sizeof(player));
    game->hero->cards[0] = c1;
    game->hero->cards[1] = c2;
    game->board[0] = c3;
    game->board[1] = c4;
    game->board[2] = c5;
    game->board[3] = c6;
    game->board[4] = c7;
    double strength = bot_strength(game);
    return Py_BuildValue("d", strength);
}

/*
 * This function returns the best hand
 */
static PyObject* py_bestHand(PyObject* self, PyObject* args)
{
    int h11, h12, h21, h22, b1, b2, b3, b4, b5;


    if (!PyArg_ParseTuple(args, "iiiiiiiii", &h11, &h12, &h21, &h22, &b1, &b2, &b3, &b4, &b5)) {
        return NULL;
    }
    card * h1 = make7Hand(h11, h12, b1, b2, b3, b4, b5);
    card * h2 = make7Hand(h21, h22, b1, b2, b3, b4, b5);
    hand bh1 = best_hand(h1);
    hand bh2 = best_hand(h2);
    hand_compare_res result = hand_beats(bh1, bh2);

    return Py_BuildValue("i", (int)result);
}


/*
 * Bind Python function names to our C functions
 */
static PyMethodDef pokerbot_methods[] = {
    {"botChoice", py_botChoice, METH_VARARGS},
    {"botStrength", py_botStrength, METH_VARARGS},
    {"bestHand", py_bestHand, METH_VARARGS},
    {NULL, NULL} /* Null terminate the array of functions. */
};

/*
 * Python calls this to let us initialize our module.
 */
void initpokerbot()
{
    (void) Py_InitModule("pokerbot", pokerbot_methods);
}

#ifdef __cplusplus
}  // extern "C"
#endif

