#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "oofit.h"

// function
static PyObject* fit4pdrc(PyObject* self, PyObject* args) {
	// first test with a list of responses for a set doses list
	PyObject *doseList;
	PyObject *doseItem;
	PyObject *resList;
	PyObject *resItem;
	Py_ssize_t n;
	Py_ssize_t dose_n;
	Py_ssize_t res_n;
	int i;

	if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &doseList, &PyList_Type, &resList)) {
    	PyErr_SetString(PyExc_TypeError, "parameter must be a list of doses, followed by a list of responses.");
    	return NULL;
	}
	dose_n = PyList_Size(doseList);
	res_n = PyList_Size(resList);
	if (dose_n != res_n) {
		PyErr_SetString(PyExc_ValueError, "the dose and response list must be of equal size");
		return NULL;
	}
	n = dose_n;
	double * dose = malloc(sizeof(double) * dose_n);
	double * res = malloc(sizeof(double) * res_n);
	for (i=0; i<n; i++) {
		doseItem = PyList_GetItem(doseList, i);
		resItem = PyList_GetItem(resList, i);

		if(PyFloat_Check(resItem)) {
			res[i] = PyFloat_AsDouble(resItem);
		} else if(PyLong_Check(resItem)) {
			res[i] = PyFloat_AsDouble(resItem);
		//} else if(resItem == Py_None) {
		//	continue;
		} else {
			PyErr_SetString(PyExc_TypeError, "list items must be numeric.");
        	return NULL;
		}
		if(PyFloat_Check(doseItem)) {
			dose[i] = PyFloat_AsDouble(doseItem);
		} else if(PyLong_Check(doseItem)) {
			dose[i] = PyLong_AsDouble(doseItem);
		} else {
			PyErr_SetString(PyExc_TypeError, "list items must be numeric.");
        	return NULL;
		}
	}
	double ret_arr[6];
	fit4d(res, dose, n, ret_arr);
	free(res);
	free(dose);
	/*
	//uncomment this block for printing output (testing)
	
	printf("returned array: (");
	int j = 0;
	for (j=0; j<6; j++) {
		printf("%.7g ", ret_arr[j]);
	}
	printf(")\n");
	pfit(ret_arr);
	*/
	PyObject * ret_dict = Py_BuildValue("{s:d,s:d,s:d,s:d,s:d,s:d}", "c", ret_arr[0], "h", ret_arr[1], "b", ret_arr[2], "t", ret_arr[3], "p", ret_arr[4], "i", ret_arr[5]);
	return ret_dict;
}

// function Definition struct
static PyMethodDef myMethods[] = {
	{"fit4pdrc",fit4pdrc, METH_VARARGS, "fits a four parameter dose response curve"},
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