#include "cextension.hpp"
#include <iostream>

namespace Python::PyPkg::CExtension
{
    int cextension_init(PyObject *module)
    {
        return 0;
    }

    PyModuleDef_Slot cextension_slots[] = {
        {Py_mod_exec, reinterpret_cast<void *>(cextension_init)},
        {0, nullptr},
    };

    PyMethodDef cextension_methods[] = {
        {"cout", cext_cout, METH_VARARGS,
         PyDoc_STR("cout(str) -> None\n output a argument.")},
        {nullptr, nullptr},
    };

    PyObject *cext_cout(PyObject *self, PyObject *args)
    {
        const char *message;
        if (!PyArg_ParseTuple(args, "s", &message))
        {
            return nullptr;
        }

        std::cout << message << std::endl;

        Py_RETURN_NONE;
    }

    PyDoc_STRVAR(module_doc, "Test of C Extension in Python package.");

    PyModuleDef cextension_module = {
        PyModuleDef_HEAD_INIT,
        "pypkg.cextention",
        module_doc,
        0,
        cextension_methods,
    };

    PyMODINIT_FUNC
    PyInit_cextension(void)
    {
        return PyModuleDef_Init(&cextension_module);
    }
}
