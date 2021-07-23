#define PY_SIZE_T_CLEAN
#include <Python.h>

namespace Python::PyPkg::CExtension
{
    int cextension_init(PyObject *module);

    PyObject *cext_cout(PyObject *self, PyObject *args);

    PyMODINIT_FUNC PyInit_cextension(void);
}