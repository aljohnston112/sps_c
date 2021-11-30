#include "node.hpp"
#include "probtree.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

using namespace std;

int main(int argc, char** argv){

}

PYBIND11_MODULE(sps_c, m){
    py::bind_vector<std::vector<double>>(m, "VectorD");
    py::bind_map<std::map<double, double>>(m, "MapDD");
    py::class_<ProbTree<double>>(m, "ProbTree")
        .def(py::init())
        .def("process", &ProbTree<double>::process)
        .def("predict", &ProbTree<double>::predict)
        ;
}

