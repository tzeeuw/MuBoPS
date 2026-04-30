/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <ClassicalBody.hh>
#include <iostream>


void ClassicalBody::update(double dt) {

    // update position using veloctity
    glm::dvec3 velocity = this->getVelocity();
    glm::dvec3 position = this->getPosition();
    glm::dvec3 acceleration = this->getAcceleration();
    double G = 6.67430e-11; // gravitational constant
    double M = 5.972e8;

    glm::dvec3 newAcceleration = -G * M * position / glm::length(position) / glm::length(position) / glm::length(position);

    // use leapfrog integration to update velocity and position
    glm::dvec3 halfVelocity = velocity + 0.5 * acceleration * dt;
    glm::dvec3 newPosition = position + halfVelocity * dt;
    glm::dvec3 newVelocity = halfVelocity + 0.5 * newAcceleration * dt;

    // std::cout << "Acceleration: " << newAcceleration.x << ", " << newAcceleration.y << ", " << newAcceleration.z << std::endl;
    // std::cout << "Velocity: " << newVelocity.x << ", " << newVelocity.y << ", " << newVelocity.z << std::endl;
    // std::cout << "Position: " << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << std::endl;

    this->setPosition(newPosition);
    this->setVelocity(newVelocity);
    this->setAcceleration(newAcceleration);
}


void ClassicalBody::addTrailPoint(const glm::dvec3& position) {

    // add trail points based on circular map
    trail[trailIndex] = (static_cast<glm::vec3>(position));
    trailIndex = (trailIndex + 1) % (trailPoints);
}


std::vector<glm::vec4> ClassicalBody::getRenderData(const glm::dvec3& cameraPos) {

    // get position and speed for rendering
    glm::vec3 relativePosition = static_cast<glm::vec3>(getPosition() - cameraPos);
    float speed = getSpeed();

    return {glm::vec4(relativePosition, speed)};
}
