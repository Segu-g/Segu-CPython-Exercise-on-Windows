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
    PyObject *enter_glfw_window(PyObject *self)
    {
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

        GLFWwindow *window = glfwCreateWindow(1280, 720, "imgui test", nullptr, nullptr);
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

    PyModuleDef_Slot wrapper_slots[] = {
        {0, nullptr},
    };

    PyMethodDef wrapper_methods[] = {
        {"enter_glfw_window", reinterpret_cast<PyCFunction>(enter_glfw_window), METH_NOARGS,
         PyDoc_STR("enter_glfw_window: -> GLFWWindow")},
        {"exit_glfw_window", exit_glfw_window, METH_O,
         PyDoc_STR("exit_glfw_window: GLFWWindow => None")},
        {"enter_imgui_window", enter_imgui_window, METH_O,
         PyDoc_STR("enter_imgui_window: GLFWWindow -> None")},
        {"exit_imgui_window", exit_imgui_window, METH_O,
         PyDoc_STR("exit_imgui_window: GLFWWindow -> None")},
        {"glfw_window_should_close", glfw_window_should_close, METH_O,
         PyDoc_STR("glfw_window_should_close: GLFWWindow -> None")},
        {"glfw_poll_events", reinterpret_cast<PyCFunction>(glfw_poll_events), METH_NOARGS,
         PyDoc_STR("glfw_poll_events: -> None")},
        {"enter_frame", reinterpret_cast<PyCFunction>(enter_frame), METH_NOARGS,
         PyDoc_STR("enter_frame: -> None")},
        {"exit_frame", exit_frame, METH_O,
         PyDoc_STR("exit_frame: GLFWWindow -> None")},
        {nullptr, nullptr},
    };

    PyDoc_STRVAR(module_doc, "Test of C Extension in Python package.");

    PyModuleDef wrapper_module = {
        PyModuleDef_HEAD_INIT,
        "imgui_adapter.wrapper",
        module_doc,
        0,
        wrapper_methods,
    };

    PyMODINIT_FUNC PyInit_wrapper(void)
    {
        return PyModuleDef_Init(&wrapper_module);
    }
}