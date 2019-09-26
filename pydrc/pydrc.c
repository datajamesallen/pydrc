#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "oofit.h"

// function
static PyObject* test(PyObject* self, PyObject* args) {
	printf("test\n");
	return Py_None;
}

// function Definition struct
static PyMethodDef myMethods[] = {
	{"test",test, METH_NOARGS, "Prints test"},
	{NULL,NULL,0,NULL}
};

// Module definition struct
static struct PyModuleDef pydrc = {
	PyModuleDef_HEAD_INIT,
	"_pydrc",
	"dose reponse fitting for python",
	-1,
	myMethods
};

// Module initialization
PyMODINIT_FUNC PyInit__pydrc(void) {
	return PyModule_Create(&pydrc);
}