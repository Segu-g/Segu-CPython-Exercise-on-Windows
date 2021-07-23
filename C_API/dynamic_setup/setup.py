from setuptools import setup, Extension
from pathlib import Path

ext_modules = [
    Extension(
        name="dynamicext",
        sources=[str(Path(__file__).parent / "dynamicext" / "dynamicext.cpp")],
        language="c++",
        extra_compile_args=["/std:c++17"],
    )
]


def main():
    setup(
        name="dynamicext",
        version="1.0.0",
        ext_modules=ext_modules,
    )


if __name__ == "__main__":
    main()
