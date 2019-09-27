#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "oofit.h"

// function
static PyObject* fit4pdrc(PyObject* self, PyObject* args) {
	// first test with a list of responses for a set doses list
	PyObject *pList;
	PyObject *pItem;
	Py_ssize_t n;
	int i;

	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
    	PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
    	return NULL;
	}

	n = PyList_Size(pList);
	for (i=0; i<n; i++) {
		pItem = PyList_GetItem(pList, i);
		if(!PyFloat_Check(pItem)) {
			PyErr_SetString(PyExc_TypeError, "list items must be floats.");
        	return NULL;
    	}
	}
	printf("test\n");
	Py_INCREF(Py_None);
	ret = Py_None;
	assert(! PyErr_Occurred());
	assert(ret)
	goto finally;
except:
	Py_XDECREF(ret)
	ret = NULL;
finally:
	return ret;
}

// function Definition struct
static PyMethodDef myMethods[] = {
	{"fitdrc",fitdrc, METH_NOARGS, "Prints test"},
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