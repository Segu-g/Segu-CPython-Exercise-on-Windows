from setuptools import setup, Extension

ext_modules = [
    Extension(
        name="pypkg.cextension",
        sources=["pypkg/cextension.cpp"],
        language="c++",
        extra_compile_args=["/std:c++17"],
    )
]


def main():
    setup(
        name="pypkg",
        version="1.0.0",
        description="a Example ",
        packages=["pypkg"],
        package_dir={"pypkg": "./pypkg"},
        ext_modules=ext_modules,
    )


if __name__ == "__main__":
    main()
