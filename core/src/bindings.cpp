/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Simulation.hh>
#include <Body.hh>
#include <QuantumBody.hh>
#include <Renderer.hh>
#include <ClassicalBody.hh>
#include <glm/glm.hpp>

namespace py = pybind11;

PYBIND11_MODULE(mubops_core, m) {
    m.doc() = "MuBoPS python bindings";
    
    py::class_<Body, std::shared_ptr<Body>>(m, "body");
    py::class_<ClassicalBody, Body, std::shared_ptr<ClassicalBody>>(m, "ClassicalBody")
        .def(py::init<>())
        .def("setPosition", [](ClassicalBody& body, double x, double y, double z) {
            body.setPosition(glm::dvec3(x,y,z));
        })
        .def("setVelocity", [](ClassicalBody& body, double x, double y, double z) {
            body.setVelocity(glm::dvec3(x,y,z));
        })
        .def("setMass", &ClassicalBody::setMass)
        .def("setRadius", &ClassicalBody::setRadius)
        .def("setAcceleration", [](ClassicalBody& body, double x, double y, double z) {
            body.setAcceleration(glm::dvec3(x,y,z));
        });

    py::class_<Simulation>(m, "Simulation")
        .def(py::init<>())
        .def("addBody", [](Simulation& sim, std::shared_ptr<ClassicalBody> body) {
            sim.addBody(body);
        })
        .def("update", &Simulation::update);

    py::class_<Renderer>(m, "Renderer")
        .def(py::init<Simulation&>())
        .def("setupWindow", &Renderer::setupWindow)
        .def("setupObjects", &Renderer::setupObjects)
        .def("setupCamera", &Renderer::setupCamera)
        .def("setupShaders", &Renderer::setupShaders)
        .def("startRenderLoop", &Renderer::startRenderLoop);
}
