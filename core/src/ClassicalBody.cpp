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
    glm::dvec3 newPosition = position + velocity * dt + 0.5 * acceleration * dt * dt;
    glm::dvec3 newVelocity = velocity + 0.5 * (acceleration + newAcceleration) * dt;

    // std::cout << "Acceleration: " << newAcceleration.x << ", " << newAcceleration.y << ", " << newAcceleration.z << std::endl;
    // std::cout << "Velocity: " << newVelocity.x << ", " << newVelocity.y << ", " << newVelocity.z << std::endl;
    // std::cout << "Position: " << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << std::endl;

    this->setPosition(newPosition);
    this->setVelocity(newVelocity);
    this->setAcceleration(newAcceleration);
}
