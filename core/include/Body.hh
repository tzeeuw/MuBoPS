#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Body {
    public:
        Body() {return;};
        virtual ~Body() {return;};


        void setName(std::string name) {
            this->name = name;
        }
        const std::string& getName() {return this->name;}

        void setMass(double mass) {this->mass = mass;}
        double getMass() {return this->mass;}

        void setPosition(glm::dvec3 position) {
            this->position = position;
        }
        const glm::dvec3& getPosition() {return this->position;}

        void setVelocity(glm::dvec3 velocity) {
            this->velocity = velocity;
        }
        const glm::dvec3& getVelocity() {return this->velocity;}

        void setAcceleration(glm::dvec3 acceleration) {
            this->acceleration = acceleration;
        }
        const glm::dvec3& getAcceleration() {return this->acceleration;}

        void setRadius(double radius) {this->radius = radius;}
        double getRadius() {return this->radius;}


        virtual void update(double dt) = 0;

    private:
        std::string name;
        double mass;
        double radius;
    
        glm::dvec3 position;
        glm::dvec3 velocity;
        glm::dvec3 acceleration;

};
    