#include "dynamicext.hpp"
#include <cstddef>

namespace Python::DynamicExt
{
    PyMethodDef MyInt_methods[] = {
        {"add", (PyCFunction)MyInt_add, METH_O,
         PyDoc_STR("add(self, int) -> void")},
        {nullptr, nullptr}};

    PyNumberMethods MyInt_number_methods = {
        nullptr,              // nb_add
        nullptr,              // nb_multiply
        nullptr,              // nb_subtract
        nullptr,              // nb_remainder
        nullptr,              // nb_divmod
        nullptr,              // nb_power
        nullptr,              // nb_negative
        nullptr,              // nb_positive
        nullptr,              // nb_absolute
        nullptr,              // nb_bool
        nullptr,              // nb_invert
        nullptr,              // nb_lshift
        nullptr,              // nb_rshift
        nullptr,              // nb_and
        nullptr,              // nb_xor
        nullptr,              // nb_or
        (unaryfunc)MyInt_int, // nb_int
        nullptr,              // *nb_reserved
        nullptr,              // nb_float
        nullptr,              // nb_inplace_add
        nullptr,              // nb_inplace_subtract
        nullptr,              // nb_inplace_multiply
        nullptr,              // nb_inplace_remainder
        nullptr,              // nb_inplace_power
        nullptr,              // nb_inplace_lshift
        nullptr,              // nb_inplace_rshift
        nullptr,              // nb_inplace_and
        nullptr,              // nb_inplace_xor
        nullptr,              // nb_inplace_or
        nullptr,              // nb_floor_divide
        nullptr,              // nb_true_divide
        nullptr,              // nb_inplace_floor_divide
        nullptr,              // nb_inplace_true_divide
        nullptr,              // nb_index
        nullptr,              // nb_matrix_multiply
        nullptr,              // nb_inplace_matrix_multiply
    };

    PyTypeObject MyInt_Type = {
        PyVarObject_HEAD_INIT(nullptr, 0)                                    //
        "dynamicext.MyInt",                                                  // tp_name
        sizeof(MyInt_Object),                                                // tp_basicsize
        0,                                                                   // tp_itemsize
        reinterpret_cast<destructor>(MyInt_dealloc),                         // tp_dealloc
        0,                                                                   // tp_vectorcall_offset
        nullptr,                                                             // tp_getattr
        nullptr,                                                             // tp_setattr
        nullptr,                                                             // tp_as_async
        nullptr,                                                             // tp_repr
        &MyInt_number_methods,                                               // tp_as_number
        nullptr,                                                             // tp_as_sequence
        nullptr,                                                             // tp_as_mapping
        nullptr,                                                             // tp_hash
        nullptr,                                                             // tp_call
        nullptr,                                                             // tp_str
        PyObject_GenericGetAttr,                                             // tp_getattro
        PyObject_GenericSetAttr,                                             // tp_setattro
        nullptr,                                                             // tp_as_buffer
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_LONG_SUBCLASS, // tp_flags
        nullptr,                                                             // tp_doc
        nullptr,                                                             // tp_traverse
        nullptr,                                                             // tp_clear
        nullptr,                                                             // tp_richcompare
        0,                                                                   // tp_weaklistoffset
        nullptr,                                                             // tp_iter
        nullptr,                                                             // tp_iternext
        MyInt_methods,                                                       // tp_methods
        nullptr,                                                             // tp_members
        nullptr,                                                             // tp_getset
        nullptr,                                                             // tp_base
        nullptr,                                                             // tp_dict
        nullptr,                                                             // tp_descr_get
        nullptr,                                                             // tp_descr_set
        offsetof(MyInt_Object, dict),                                        // tp_dictoffset
        (initproc)init_MyInt_Object,                                         // tp_init
        nullptr,                                                             // tp_alloc
        (newfunc)new_MyInt_Object,                                           // tp_new
        nullptr,                                                             // tp_free
        nullptr,                                                             // tp_is_gc
        nullptr,                                                             // tp_bases
        nullptr,                                                             // tp_mro
        nullptr,                                                             // tp_cache
        nullptr,                                                             // tp_subclasses
        nullptr,                                                             //tp_weaklist
        nullptr                                                              //tp_finalize
    };

    PyObject *new_MyInt_Object(PyTypeObject *subtype, PyObject *args, PyObject *keywds)
    {
        MyInt_Object *self;
        self = (MyInt_Object *)subtype->tp_alloc(subtype, 0);
        // self = PyObject_New(MyInt_Object, subtype);
        if (self == nullptr)
        {
            PyErr_NoMemory();
            return nullptr;
        }
        self->dict = PyDict_New();
        if (self->dict == nullptr)
        {
            PyErr_SetString(PyExc_RuntimeError, "Faild to get a PyDictObject of MyInt_Object.");
            return nullptr;
        }
        return reinterpret_cast<PyObject *>(self);
    }

    int init_MyInt_Object(MyInt_Object *self, PyObject *args, PyObject *keywds)
    {
        int value;
        const char *format = "i";
        static char *keywdlist[] = {"value", nullptr};
        if (PyArg_ParseTupleAndKeywords(args, keywds, format, keywdlist, &value) == 0)
        {
            return -1;
        }
        self->value = value;
        return 0;
    }

    PyObject *MyInt_add(MyInt_Object *self, PyObject *obj)
    {
        Py_INCREF(obj);
        if (PyObject_IsInstance(obj, (PyObject *)&MyInt_Type) == 0)
        {
            Py_DECREF(obj);
            PyErr_Format(PyExc_TypeError, "The second argument of dynamicext.MyInt.add shuold be dynamicext.MyInt.");
            return nullptr;
        }
        self->value += ((MyInt_Object *)obj)->value;
        Py_DECREF(obj);
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject *MyInt_int(MyInt_Object *self)
    {
        return Py_BuildValue("i", self->value);
    }

    void MyInt_dealloc(MyInt_Object *self)
    {
        PyTypeObject *tp = Py_TYPE(self);
        tp->tp_free(self);
        Py_DECREF(tp);
    };

    int dynamicext_exec(PyObject *module)
    {
        if (PyType_Ready(&MyInt_Type) < 0)
        {
            PyErr_SetImportError(
                Py_BuildValue("s", "error message"),
                Py_BuildValue("s", "dynamicext"),
                Py_BuildValue("s", __FILE__));
            return -1;
        }
        PyModule_AddObject(module, "MyInt", (PyObject *)&MyInt_Type);
        return 0;
    };

    PyModuleDef_Slot dynamicext_slots[] = {
        {Py_mod_exec, dynamicext_exec},
        {0, nullptr},
    };

    PyMethodDef dynamicext_methods[] = {
        {nullptr, nullptr},
    };

    PyDoc_STRVAR(module_doc, "This text is dummy document of module");

    struct PyModuleDef dynamicext_module = {
        PyModuleDef_HEAD_INIT,
        "dynamicext",
        module_doc,
        0,
        dynamicext_methods,
        dynamicext_slots,
        nullptr,
        nullptr,
        nullptr,
    };

    PyMODINIT_FUNC
    PyInit_dynamicext(void)
    {
        return PyModuleDef_Init(&dynamicext_module);
    }
}
