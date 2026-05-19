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
    octree.buildTree(bodies);
    // octree.debugPrint();
    updateGravity(dt);
    for (auto& body: bodies) {
        body->update(dt, units);
    }
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

        if (dist == 0.0) {
            continue;
        }

        newAcceleration += units.G * cbody2->getMass() * dpos / (dist * dist * dist);
        cbody1->setNewAcceleration(newAcceleration);
    }
}

// void Simulation::updateGravity(double dt) {

//     for (auto& body1: bodies) {
//         ClassicalBody* cbody1 = dynamic_cast<ClassicalBody*>(body1.get());
//         if (!cbody1 || !cbody1->affectedByGravity){
//             continue;
//         }

//         glm::dvec3 newAcceleration(0.0);
//         glm::dvec3 position1 = cbody1->getPosition();

//         for (auto& body2: bodies) {
//             if (body1 == body2) {
//                 continue;
//             }
//             ClassicalBody* cbody2 = dynamic_cast<ClassicalBody*>(body2.get());
//             if (!cbody2 || !cbody2->exertsGravity){
//                 continue;
//             }

//             glm::dvec3 position2 = cbody2->getPosition();
//             glm::dvec3 dpos = position2 - position1;
//             double dist = glm::length(dpos);

//             if (dist == 0.0) {
//                 continue;
//             }

//             newAcceleration += units.G * cbody2->getMass() * dpos / (dist * dist * dist);
//         }

//         cbody1->setNewAcceleration(newAcceleration);
//     }
//     return;
// }


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
