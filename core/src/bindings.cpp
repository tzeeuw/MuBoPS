/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <pybind11/pybind11.h>
#include <Simulation.hh>
#include <QuantumBody.hh>
#include <Simulation.hh>

namespace py = pybind11;

PYBIND11_MODULE(mubops_core, m) {
    m.doc() = "MuBoPS python bindings";
    m.def("test", [&]() {
        return "test";
    });
}