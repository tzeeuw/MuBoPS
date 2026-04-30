/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <iostream>

void Simulation::removeBody(std::shared_ptr<Body>& body) {

    // find the body in the vector and remove it
    auto it = std::find_if(bodies.begin(), bodies.end(), [&](const std::shared_ptr<Body>& b) {
        return b.get() == body.get();
    });

    if (it != bodies.end()) {
        bodies.erase(it);
    }
}


void Simulation::update(double dt) {

    // update each body in the simulation by calling its update method
    for (auto& body: bodies) {
        body->update(dt, units);
    }
}

void Simulation::addTrailPoints() {

    // add trailpoints to classical bodies
    for (auto& body: bodies) {
        ClassicalBody* classicalBody = dynamic_cast<ClassicalBody*>(body.get());
        if (classicalBody) {
            classicalBody->addTrailPoint(classicalBody->getPosition());
        }
    }
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


// (DEPRICATED)
std::vector<glm::vec3> Simulation::getTrails() {

    // get all trail points from classical bodies and combine them into a single vector for rendering
    std::vector<glm::vec3> trails;
    for (auto& body: bodies) {
        ClassicalBody* classicalBody = dynamic_cast<ClassicalBody*>(body.get());

        if (classicalBody) {
            const std::vector<glm::vec3>& trail = classicalBody->getTrail();
            trails.insert(trails.end(), trail.begin(), trail.end());
        }
    }
    return trails;
}
