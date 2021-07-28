#include "variable.hpp"
#include <iostream>

namespace Python::ImGuiAdapter::Variable
{

    PyObject *print_FloatVariable(FloatVariable *self)
    {
        std::cout << self->value << std::endl;
        Py_RETURN_NONE;
    }

    PyMethodDef FloatVariable_methods[] = {
        {"print", reinterpret_cast<PyCFunction>(print_FloatVariable), METH_NOARGS,
         PyDoc_STR("print: -> None")},
        {nullptr, nullptr},
    };

    PyTypeObject FloatVariable_Type = {
        PyVarObject_HEAD_INIT(nullptr, 0)                    //
        "imgui_adapter.variable.FloatVariable",              // tp_name
        sizeof(FloatVariable),                               // tp_basicsize
        0,                                                   // tp_itemsize
        reinterpret_cast<destructor>(dealloc_FloatVariable), // tp_dealloc
        0,                                                   // tp_vectorcall_offset
        nullptr,                                             // tp_getattr
        nullptr,                                             // tp_setattr
        nullptr,                                             // tp_as_async
        nullptr,                                             // tp_repr
        nullptr,                                             // tp_as_number
        nullptr,                                             // tp_as_sequence
        nullptr,                                             // tp_as_mapping
        nullptr,                                             // tp_hash
        nullptr,                                             // tp_call
        nullptr,                                             // tp_str
        nullptr,                                             // tp_getattro
        nullptr,                                             // tp_setattro
        nullptr,                                             // tp_as_buffer
        0,                                                   // tp_flags
        nullptr,                                             // tp_doc
        nullptr,                                             // tp_traverse
        nullptr,                                             // tp_clear
        nullptr,                                             // tp_richcompare
        0,                                                   // tp_weaklistoffset
        nullptr,                                             // tp_iter
        nullptr,                                             // tp_iternext
        FloatVariable_methods,                               // tp_methods
        nullptr,                                             // tp_members
        nullptr,                                             // tp_getset
        nullptr,                                             // tp_base
        nullptr,                                             // tp_dict
        nullptr,                                             // tp_descr_get
        nullptr,                                             // tp_descr_set
        0,                                                   // tp_dictoffset
        reinterpret_cast<initproc>(init_FloatVariable),      // tp_init
        nullptr,                                             // tp_alloc
        reinterpret_cast<newfunc>(new_FloatVariable),        // tp_new
        nullptr,                                             // tp_free
        nullptr,                                             // tp_is_gc
        nullptr,                                             // tp_bases
        nullptr,                                             // tp_mro
        nullptr,                                             // tp_cache
        nullptr,                                             // tp_subclasses
        nullptr,                                             //tp_weaklist
        nullptr                                              //tp_finalize
    };

    PyObject *new_FloatVariable(PyTypeObject *subtype, PyObject *args, PyObject *keywds)
    {
        PyObject *self;
        self = reinterpret_cast<PyObject *>(subtype->tp_alloc(subtype, 0));

        if (self == nullptr)
        {
            return PyErr_NoMemory();
        }

        return self;
    }

    int init_FloatVariable(FloatVariable *self, PyObject *args, PyObject *keywrds)
    {
        float value;
        const char *format = "f";
        static char *keywrdlist[] = {"value", nullptr};
        if (PyArg_ParseTupleAndKeywords(args, keywrds, format, keywrdlist, &value) == 0)
        {
            return -1;
        }

        self->value = value;
        return 0;
    }

    void dealloc_FloatVariable(FloatVariable *self)
    {
        PyTypeObject *tp = Py_TYPE(self);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    int variable_exec(PyObject *module)
    {
        if (PyType_Ready(&FloatVariable_Type) != 0)
        {
            PyErr_SetImportError(
                Py_BuildValue("s", "error message"),
                Py_BuildValue("s", "imgui_adapter.variable"),
                Py_BuildValue("s", __FILE__));
            return -1;
        }

        PyModule_AddObject(
            module,
            "FloatVariable",
            reinterpret_cast<PyObject *>(&FloatVariable_Type));
        return 0;
    }

    PyModuleDef_Slot variable_slots[] = {
        {Py_mod_exec, variable_exec},
        {0, nullptr},
    };

    PyMethodDef variable_method[] = {
        {nullptr, nullptr},
    };

    PyDoc_STRVAR(module_doc, "This text is dummy document of module");

    PyModuleDef variable_module = {
        PyModuleDef_HEAD_INIT,
        "imgui_adapter.variable",
        module_doc,
        0,
        variable_method,
        variable_slots,
        nullptr,
        nullptr,
        nullptr,
    };

    PyMODINIT_FUNC
    PyInit_variable()
    {
        return PyModuleDef_Init(&variable_module);
    }
}