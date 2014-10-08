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
    double strength = bot_strength(NULL);
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

