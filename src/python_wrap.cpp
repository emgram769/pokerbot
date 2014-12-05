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
 * Bind Python function names to our C functions
 */
static PyMethodDef pokerbot_methods[] = {
    {"botChoice", py_botChoice, METH_VARARGS},
    {"botStrength", py_botStrength, METH_VARARGS},
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

