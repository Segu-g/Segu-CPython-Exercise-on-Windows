#define PY_SIZE_T_CLEAN
#include <Python.h>

namespace Python::BasicExtension
{

    struct MyInt_Object
    {
        PyObject_HEAD;
        int value;
        PyObject *dict;
    };

    PyObject *MyInt_add(MyInt_Object *self, PyObject *obj);

    PyObject *MyInt_int(MyInt_Object *self);

    PyObject *new_MyInt_Object(PyTypeObject *subtype, PyObject *args, PyObject *keywds);

    int init_MyInt_Object(MyInt_Object *self, PyObject *args, PyObject *keywds);

    void MyInt_dealloc(MyInt_Object *self);

    int basicext_exec(PyObject *module);

    PyMODINIT_FUNC PyInit_basicext(void);
}