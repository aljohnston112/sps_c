from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension("sps_c",
        ["src/main.cpp"],)
]

setup(
    name="sps_c",
    cmdclass={"build_ext": build_ext},
    ext_modules=ext_modules
)
