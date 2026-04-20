#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <iostream>

void Simulation::removeBody(std::unique_ptr<Body>& body) {
    // find the body in the vector and remove it
    auto it = std::find_if(bodies.begin(), bodies.end(), [&](const std::unique_ptr<Body>& b) {
        return b.get() == body.get();
    });

    if (it != bodies.end()) {
        bodies.erase(it);
    }
}


void Simulation::update(double dt) {
    for (auto& body: bodies) {
        body->update(dt);
    }
}

void Simulation::addTrailPoints() {
    for (auto& body: bodies) {
        ClassicalBody* classicalBody = dynamic_cast<ClassicalBody*>(body.get());
        if (classicalBody) {
            classicalBody->addTrailPoint(classicalBody->getPosition());
        }
    }
}

std::vector<glm::vec3> Simulation::getPositions() {
    std::vector<glm::vec3> positions;
    for (auto& body: bodies) {
        glm::vec3 position = static_cast<glm::vec3>(body->getPosition());
        positions.push_back(position);
    }
    return positions;
}


std::vector<glm::vec3> Simulation::getTrails() {
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
