#pragma once
#define PY_SIZE_T_CLEAN
#include <Python.h>
#include "variable.hpp"

namespace Python::ImGuiAdapter::Wrapper
{
    PyObject *enter_glfw_window(PyObject *self, PyObject *arg);
    PyObject *exit_glfw_window(PyObject *self, PyObject *arg);
    PyObject *enter_imgui_window(PyObject *self, PyObject *arg);
    PyObject *exit_imgui_window(PyObject *self, PyObject *arg);

    PyObject *glfw_window_should_close(PyObject *self, PyObject *arg);
    PyObject *glfw_poll_events(PyObject *self);

    PyObject *enter_frame(PyObject *self);
    PyObject *exit_frame(PyObject *self, PyObject *arg);

    PyObject *enter_window(PyObject *self, PyObject *arg);
    PyObject *exit_window(PyObject *self);

    PyObject *show_text(PyObject *self, PyObject *arg);
    PyObject *show_float_slider(PyObject *self, PyObject *args);

    PyObject *get_framerate(PyObject *self);

    PyMODINIT_FUNC PyInit_wrapper(void);
}