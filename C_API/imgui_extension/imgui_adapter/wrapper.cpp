#include "wrapper.hpp"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Python::ImGuiAdapter::Wrapper
{
    using Python::ImGuiAdapter::Variable::FloatVariable;

    PyObject *
    enter_glfw_window(PyObject *self, PyObject *arg)
    {
        const char *title = PyUnicode_AsUTF8(arg);
        if (title == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get text.");
            return nullptr;
        }

        glfwSetErrorCallback(
            [](int error, const char *description)
            {
                std::cerr << "Glfw Error " << error << std::endl;
                std::cerr << description << std::endl;
            });

        if (!glfwInit())
        {
            PyErr_Format(PyExc_RuntimeError, "Faild to initialize glfw.");
            return nullptr;
        }

        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        GLFWwindow *window = glfwCreateWindow(1280, 720, title, nullptr, nullptr);
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Faild to create glfw window.");
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (gl3wInit() != 0)
        {
            PyErr_Format(PyExc_RuntimeError, "Failed to initialize OpenGL loader!\n");
            return nullptr;
        }

        return PyCapsule_New(window, "_window", [](PyObject *cupsule) {});
    }

    PyObject *exit_glfw_window(PyObject *self, PyObject *arg)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow *>(PyCapsule_GetPointer(arg, "_window"));
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window pointer from PyCupsle.");
            return nullptr;
        }
        glfwDestroyWindow(window);
        glfwTerminate();
        Py_RETURN_NONE;
    }

    PyObject *enter_imgui_window(PyObject *self, PyObject *arg)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow *>(PyCapsule_GetPointer(arg, "_window"));
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window pointer from PyCupsle.");
            return nullptr;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        Py_RETURN_NONE;
    }

    PyObject *exit_imgui_window(PyObject *self, PyObject *arg)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow *>(PyCapsule_GetPointer(arg, "_window"));
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window pointer from PyCupsle.");
            return nullptr;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        Py_RETURN_NONE;
    }

    PyObject *glfw_window_should_close(PyObject *self, PyObject *arg)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow *>(PyCapsule_GetPointer(arg, "_window"));
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window pointer from PyCupsle.");
            return nullptr;
        }

        if (glfwWindowShouldClose(window))
        {
            Py_RETURN_TRUE;
        }
        else
        {
            Py_RETURN_FALSE;
        }
    }

    PyObject *glfw_poll_events(PyObject *self)
    {
        glfwPollEvents();
        Py_RETURN_NONE;
    }

    PyObject *enter_frame(PyObject *self)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Py_RETURN_NONE;
    }

    PyObject *exit_frame(PyObject *self, PyObject *arg)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow *>(PyCapsule_GetPointer(arg, "_window"));
        if (window == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window pointer from PyCupsle.");
            return nullptr;
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        Py_RETURN_NONE;
    }

    PyObject *enter_window(PyObject *self, PyObject *arg)
    {
        const char *window_name = PyUnicode_AsUTF8(arg);
        if (window_name == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get window name.");
            return nullptr;
        }

        ImGui::Begin(window_name);

        Py_RETURN_NONE;
    }

    PyObject *exit_window(PyObject *self)
    {
        ImGui::End();
        Py_RETURN_NONE;
    }

    PyObject *show_text(PyObject *self, PyObject *arg)
    {
        const char *text = PyUnicode_AsUTF8(arg);
        if (text == nullptr)
        {
            PyErr_Format(PyExc_RuntimeError, "Cannot get text.");
            return nullptr;
        }
        ImGui::Text(text);

        Py_RETURN_NONE;
    }

    PyObject *show_float_slider(PyObject *self, PyObject *args)
    {
        char *label = nullptr;
        PyObject *obj = nullptr;
        const char *format = "sO";
        if (!PyArg_ParseTuple(args, format, &label, &obj))
        {
            return nullptr;
        }
        PyObject *variable_module = PyImport_ImportModule("imgui_adapter.variable");
        if (variable_module == nullptr)
        {
            return nullptr;
        }
        PyTypeObject *FloatVariable_Type = reinterpret_cast<PyTypeObject *>(PyObject_GetAttrString(variable_module, "FloatVariable"));

        if (!Py_IS_TYPE(obj, FloatVariable_Type))
        {
            PyErr_SetString(PyExc_TypeError, FloatVariable_Type->tp_name);
            return nullptr;
        }
        FloatVariable *var = reinterpret_cast<FloatVariable *>(obj);
        ImGui::SliderFloat(label, &(var->value), 0.0f, 1.0f);
        Py_RETURN_NONE;
    }

    PyObject *get_framerate(PyObject *self)
    {
        ImGuiIO &io = ImGui::GetIO();
        return Py_BuildValue("f", io.Framerate);
    }

    PyMethodDef wrapper_methods[] = {
        {"enter_glfw_window", enter_glfw_window, METH_O,
         PyDoc_STR("enter_glfw_window: str -> GLFWWindow")},
        {"exit_glfw_window", exit_glfw_window, METH_O,
         PyDoc_STR("exit_glfw_window: GLFWWindow => None")},
        {"enter_imgui_window", enter_imgui_window, METH_O,
         PyDoc_STR("enter_imgui_window: GLFWWindow -> None")},
        {"exit_imgui_window", exit_imgui_window, METH_O,
         PyDoc_STR("exit_imgui_window: GLFWWindow -> None")},
        {"glfw_window_should_close", glfw_window_should_close, METH_O,
         PyDoc_STR("glfw_window_should_close: GLFWWindow -> bool")},
        {"glfw_poll_events", reinterpret_cast<PyCFunction>(glfw_poll_events), METH_NOARGS,
         PyDoc_STR("glfw_poll_events: -> None")},
        {"enter_frame", reinterpret_cast<PyCFunction>(enter_frame), METH_NOARGS,
         PyDoc_STR("enter_frame: -> None")},
        {"exit_frame", exit_frame, METH_O,
         PyDoc_STR("exit_frame: GLFWWindow -> None")},
        {"enter_window", enter_window, METH_O,
         PyDoc_STR("enter_window: str -> None")},
        {"exit_window", reinterpret_cast<PyCFunction>(exit_window), METH_NOARGS,
         PyDoc_STR("exit_window: -> None")},
        {"show_text", show_text, METH_O,
         PyDoc_STR("show_text: str -> None")},
        {"show_float_slider", reinterpret_cast<PyCFunction>(show_float_slider), METH_VARARGS,
         PyDoc_STR("show_float_slider: FloatVariable -> None")},
        {"get_framerate", reinterpret_cast<PyCFunction>(get_framerate), METH_NOARGS,
         "get_framerate -> float"},
        {nullptr, nullptr},
    };

    int wrapper_exec(PyObject *module)
    {
        PyObject *typing_module = PyImport_ImportModule("typing");
        if (typing_module == nullptr)
        {
            PyErr_SetImportError(
                Py_BuildValue("s", "error message"),
                Py_BuildValue("s", "imgui_adapter.wrapper"),
                Py_BuildValue("s", __FILE__));
            return -1;
        }

        PyObject *new_type = PyObject_GetAttrString(typing_module, "NewType");

        PyObject *builtins_module = PyImport_ImportModule("builtins");
        if (builtins_module == nullptr)
        {
            Py_XDECREF(new_type);
            Py_DECREF(typing_module);
            PyErr_SetImportError(
                Py_BuildValue("s", "error message"),
                Py_BuildValue("s", "imgui_adapter.wrapper"),
                Py_BuildValue("s", __FILE__));
            return -1;
        }

        PyObject *object = PyObject_GetAttrString(builtins_module, "object");

        PyObject *args_tuple = Py_BuildValue("(s, O)", "GLFWWindow", object);

        PyObject *GLWFWindow_type_obj = PyObject_CallObject(new_type, args_tuple);

        if (GLWFWindow_type_obj == nullptr)
        {
            Py_XDECREF(args_tuple);
            Py_XDECREF(object);
            Py_DECREF(builtins_module);
            Py_XDECREF(new_type);
            Py_DECREF(typing_module);
            PyErr_SetImportError(
                Py_BuildValue("s", "error message"),
                Py_BuildValue("s", "imgui_adapter.wrapper"),
                Py_BuildValue("s", __FILE__));
            return -1;
        }

        PyModule_AddObject(
            module,
            "GLFWWindow",
            GLWFWindow_type_obj);

        Py_XDECREF(args_tuple);
        Py_XDECREF(object);
        Py_DECREF(builtins_module);
        Py_DECREF(typing_module);
        Py_XDECREF(new_type);

        return 0;
    }

    PyModuleDef_Slot wrapper_slots[] = {
        {Py_mod_exec, wrapper_exec},
        {0, nullptr},
    };

    PyDoc_STRVAR(module_doc, "Test of C Extension in Python package.");

    PyModuleDef wrapper_module = {
        PyModuleDef_HEAD_INIT,
        "imgui_adapter.wrapper",
        module_doc,
        0,
        wrapper_methods,
        wrapper_slots,
    };

    PyMODINIT_FUNC PyInit_wrapper(void)
    {
        return PyModuleDef_Init(&wrapper_module);
    }
}