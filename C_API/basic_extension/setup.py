from setuptools import setup, Extension
from pathlib import Path

ext_modules = [
    Extension(
        name="basicext",
        sources=[str(Path(__file__).parent / "basic_ext" / "basic_ext.cpp")],
        language="c++",
        extra_compile_args=["/std:c++17"],
    )
]


def main():
    setup(
        name="basicext",
        version="1.0.0",
        ext_modules=ext_modules,
    )


if __name__ == "__main__":
    main()
