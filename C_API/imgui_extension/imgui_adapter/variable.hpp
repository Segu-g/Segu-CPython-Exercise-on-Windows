#define PY_SIZE_T_CLEAN
#include <Python.h>

namespace Python::ImGuiAdapter::Variable
{
    struct FloatVariable
    {
        PyObject_HEAD;
        float value;
    };

    PyObject *print_FloatVariable(FloatVariable *self);

    PyObject *new_FloatVariable(PyTypeObject *subtype, PyObject *args, PyObject *keywds);
    int init_FloatVariable(FloatVariable *self, PyObject *args, PyObject *keywrds);
    void dealloc_FloatVariable(FloatVariable *self);

    int variable_exec(PyObject *module);

    PyMODINIT_FUNC PyInit_variable();
}