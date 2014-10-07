#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*
 * This function makes the bot's choice.
 */
static PyObject* py_botChoice(PyObject* self, PyObject* args)
{
    char *s = "I choose 3!";
    return Py_BuildValue("s", s);
}

/*
 * This function returns our bot's predicted strength.
 */
static PyObject* py_botStrength(PyObject* self, PyObject* args)
{
    // double x, y;
    // PyArg_ParseTuple(args, "dd", &x, &y);
    return Py_BuildValue("d", 9000.00001);
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

