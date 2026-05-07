/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <ClassicalBody.hh>
#include <iostream>
#include <Units.hh>


void ClassicalBody::update(double dt, Units& units) {

    // update position using veloctity
    glm::dvec3 velocity = this->getVelocity();
    glm::dvec3 position = this->getPosition();
    glm::dvec3 acceleration = this->getAcceleration();
    glm::dvec3 newAcceleration = this->getNewAcceleration();
    
    // std::cout << units.G << " " << glm::length(position) << " " << glm::length(newAcceleration) << " a"<< std::endl;

    // use leapfrog integration to update velocity and position
    glm::dvec3 halfVelocity = velocity + 0.5 * acceleration * dt;
    glm::dvec3 newPosition = position + halfVelocity * dt;
    glm::dvec3 newVelocity = halfVelocity + 0.5 * newAcceleration * dt;

    // std::cout << glm::length(newVelocity) << std::endl;
    this->setPosition(newPosition);
    this->setVelocity(newVelocity);
    this->setAcceleration(newAcceleration);
    this->setNewAcceleration({0.0,0.0,0.0});
}



std::vector<glm::vec4> ClassicalBody::getRenderData(const glm::dvec3& cameraPos, Units& units) {

    // get position and speed for rendering
    glm::vec3 relativePosition = static_cast<glm::vec3>((getPosition() - cameraPos));
    float speed = getSpeed();

    return {glm::vec4(relativePosition, speed)};
}
