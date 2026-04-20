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

std::vector<glm::vec3> Simulation::getVelocities() {
    std::vector<glm::vec3> velocities;
    for (auto& body: bodies) {
        glm::vec3 velocity = static_cast<glm::vec3>(body->getVelocity());
        velocities.push_back(velocity);
    }
    return velocities;
}

std::vector<float> Simulation::getSpeeds() {
    std::vector<float> speeds;
    for (auto& body: bodies) {
        float speed = body->getSpeed();
        speeds.push_back(speed);
    }
    return speeds;
}


std::vector<glm::vec4> Simulation::getPositionsAndSpeed() {
    std::vector<glm::vec4> positionsAndSpeed;

    for (auto& body: bodies) {
        glm::vec3 position = static_cast<glm::vec3>(body->getPosition());
        float speed = body->getSpeed();
        positionsAndSpeed.push_back(glm::vec4(position, speed));
    }

    return positionsAndSpeed;
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
