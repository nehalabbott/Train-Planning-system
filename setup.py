from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "metro_routing", # The name of the module you will import in app.py
        ["main.cpp"],    # Your C++ source file
        include_dirs=[pybind11.get_include()], # Automatically finds pybind11
        language="c++",
        extra_compile_args=["/O2", "/std:c++17"] # Optimization and C++17 for Windows MSVC
    ),
]

setup(
    name="metro_routing",
    ext_modules=ext_modules,
)