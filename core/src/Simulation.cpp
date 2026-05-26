/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <iostream>
#include <GLFW/glfw3.h>

double avg = 0.0;

void Simulation::removeBody(std::shared_ptr<Body>& body) {

    // find the body in the vector and remove it
    auto it = std::find_if(bodies.begin(), bodies.end(), [&](const std::shared_ptr<Body>& b) {
        return b.get() == body.get();
    });

    if (it != bodies.end()) {
        bodies.erase(it);
    }
}

void Simulation::initGravityPairs() {
    for (auto& body1: bodies) {
        ClassicalBody* cbody1 = dynamic_cast<ClassicalBody*>(body1.get());
        if (!cbody1 || !cbody1->affectedByGravity) {
            continue;
        }
        for (auto& body2: bodies) {
            if (body1 == body2) {
                continue;
            }
            ClassicalBody* cbody2 = dynamic_cast<ClassicalBody*>(body2.get());
            if (!cbody2 || !cbody2->exertsGravity){
                continue;
            }

            gravityPairs.push_back(std::pair(body1, body2));
        }
    }
}


void Simulation::update(double dt) {

    // update each body in the simulation by calling its update method
    // double t1 = glfwGetTime();
    octree.buildTree(bodies);
    // double t2 = glfwGetTime();
    // octree.debugPrint();
    // updateGravity(dt);
    for (auto& body: bodies) {
        updateGravityBH(body, 0.8, 1e-2);
        body->update(dt, units);
    }
    // double t3 = glfwGetTime();
    // std::cout << "Tree building time: " << t2 - t1 << "s,  BH: " << t3 - t2 << "s" << std::endl;
}

void Simulation::updateGravity(double dt) {
    for (auto& bodyPair: gravityPairs) {
        auto& cbody1 = bodyPair.first;
        auto& cbody2 = bodyPair.second;
        
        glm::dvec3 newAcceleration = cbody1->getNewAcceleration();
        glm::dvec3 pos1 = cbody1->getPosition();
        glm::dvec3 pos2 = cbody2->getPosition();

        glm::dvec3 dpos = pos2 - pos1;
        double dist = glm::length(dpos);

        if (dist < 1e-2) {
            dist = 1e-2;
        }

        newAcceleration += units.G * cbody2->getMass() * dpos / (dist * dist * dist);
        cbody1->setNewAcceleration(newAcceleration);
    }
}

void Simulation::updateGravityBH(std::shared_ptr<Body>& body, double theta, double eps){
    // double t4 = glfwGetTime();
    // glm::dvec3 newAcceleration = body->getNewAcceleration();
    // glm::dvec3 bodyPos = body->getPosition();
    octree.BarnesHut(body, theta, eps, units.G);
    // avg += masses.size();
    // double t5 = glfwGetTime();
    // for (auto& mass: masses){
    //     glm::dvec3 dpos = mass.centerOfMass - bodyPos;
    //     double dist = std::sqrt(glm::length(dpos)*glm::length(dpos) + eps*eps);

    //     newAcceleration += units.G * mass.totalMass * dpos / (dist * dist * dist);
    // }
    // body->setNewAcceleration(newAcceleration);
    // double t6 = glfwGetTime();
    // std::cout << "Mass aggregating: " << t5 - t4 << "s, acceleration: " << t6 - t5 << "s" << std::endl;
}


std::vector<glm::vec4> Simulation::getQuantumRenderData(const glm::dvec3& cameraPos) {

    // retrieve the render data for quantum bodies, which includes position and normalized probability density for rendering
    std::vector<glm::vec4> renderData;

    // get all quantum bodies
    for (auto& body: bodies) {
        QuantumBody* quantumBody = dynamic_cast<QuantumBody*>(body.get());
        if (quantumBody){
            std::vector<glm::vec4> bodyData = body->getRenderData(cameraPos, units);
            renderData.insert(renderData.end(), bodyData.begin(), bodyData.end());
        }
    }

    // find min and max value for 4th data point
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;

    for (auto& dataPoint : renderData) {
        minVal = std::min(minVal, dataPoint[3]);
        maxVal = std::max(maxVal, dataPoint[3]);
    }


    // normalize the 4th data point to be between 0 and 1 for rendering
    for (auto& dataPoint : renderData) {
        dataPoint[3] = (dataPoint[3] - minVal) / (maxVal - minVal);
    }
    return renderData;
}

std::vector<glm::vec4> Simulation::getClassicalRenderData(const glm::dvec3& cameraPos) {

    // retrieve the render data for classical bodies, which includes position and normalized speed for rendering
    std::vector<glm::vec4> renderData;

    // find all classical bodies
    for (auto& body: bodies) {
        ClassicalBody* classicalBody = dynamic_cast<ClassicalBody*>(body.get());
        if (classicalBody){
            std::vector<glm::vec4> bodyData = body->getRenderData(cameraPos, units);
            renderData.insert(renderData.end(), bodyData.begin(), bodyData.end());
        }
    }

    // find min and max value for 4th data point
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;

    for (auto& dataPoint : renderData) {
        minVal = std::min(minVal, dataPoint[3]);
        maxVal = std::max(maxVal, dataPoint[3]);
    }

    // normalize the 4th data point to be between 0 and 1 for rendering
    for (auto& dataPoint : renderData) {
        dataPoint[3] = (dataPoint[3] - minVal) / (maxVal - minVal);
    }
    return renderData;
}
