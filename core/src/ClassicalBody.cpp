#include <ClassicalBody.hh>


void ClassicalBody::update(double dt) {
    // update position using veloctity
    glm::dvec3 velocity = this->getVelocity();
    glm::dvec3 position = this->getPosition();

    // angle is theta = arctan(vy/vx) = arctan(y/x) = length(position) / length(velocity) * dt
    double radius = glm::length(position);
    double angle =glm::length(velocity) * dt / radius;

    double x = cos(angle) * position.x - sin(angle) * position.y;
    double y = sin(angle) * position.x + cos(angle) * position.y;

    double vx = cos(angle) * velocity.x - sin(angle) * velocity.y;
    double vy = sin(angle) * velocity.x + cos(angle) * velocity.y;

    glm::dvec3 newPosition = {x, y, 0};
    glm::dvec3 newVelocity = {vx, vy, 0};
    this->setPosition(newPosition);
    this->setVelocity(newVelocity);

    // for now we move the body in a straight line
    // glm::dvec3 velocity = this->getVelocity();
    // glm::dvec3 position = this->getPosition();
    // glm::dvec3 newPosition = position + velocity * dt;
    // this->setPosition(newPosition);
}
