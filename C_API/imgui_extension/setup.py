from setuptools import setup, Extension
import distutils.sysconfig
import distutils.util
from setuptools import msvc


distutils.sysconfig.get_python_inc()
distutils.sysconfig.get_python_lib()
arch = distutils.util.get_platform()
env = msvc.EnvironmentInfo(arch)

include_dirs = (
    env.OSIncludes
    + env.VCIncludes
    + env.UCRTIncludes
    + [
        "../../libraries/imgui",
        "../../libraries/imgui/backends",
        "../../libraries/imgui/examples/libs/gl3w",
        "../../libraries/glfw-3.3.4.bin.WIN64/include",
    ]
)


library_dirs = env.OSLibpath + ["../../libraries/glfw-3.3.4.bin.WIN64/lib-vc2019"]
libraries = (
    env.OSLibraries
    + env.VCLibraries
    + env.UCRTLibraries
    + ["glfw3", "gdi32", "opengl32", "imm32"]
    + [
        "kernel32",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "odbc32",
        "odbccp32",
    ]
)

sources = [
    "imgui_adapter/wrapper.cpp",
    "../../libraries/imgui/imgui_draw.cpp",
    "../../libraries/imgui/imgui_tables.cpp",
    "../../libraries/imgui/imgui_widgets.cpp",
    "../../libraries/imgui/imgui.cpp",
    "../../libraries/imgui/backends/imgui_impl_opengl3.cpp",
    "../../libraries/imgui/backends/imgui_impl_glfw.cpp",
    "../../libraries/imgui/examples/libs/gl3w/GL/gl3w.c",
]


ext_modules = [
    Extension(
        name="imgui_adapter.wrapper",
        sources=sources,
        language="c++",
        include_dirs=include_dirs,
        library_dirs=library_dirs,
        libraries=libraries,
        extra_compile_args=["/std:c++17", "-DIMGUI_IMPL_OPENGL_LOADER_GL3W"],
    ),
    Extension(
        name="imgui_adapter.variable",
        sources=["imgui_adapter/variable.cpp"],
        language="c++",
        extra_compile_args=["/std:c++17"],
    ),
]


def main():
    setup(
        name="imgui_adapter",
        version="1.0.0",
        description="a Example ",
        packages=["imgui_adapter"],
        package_dir={"imgui_adapter": "./imgui_adapter"},
        package_data={"imgui_adapter": ["*.pyi"]},
        ext_modules=ext_modules,
        test_suite="tests",
        tests_require=["pytest"],
    )


if __name__ == "__main__":
    main()
    # pass
