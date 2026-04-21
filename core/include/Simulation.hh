#pragma once
#include <Body.hh>
#include <vector>
#include <memory>
#include <algorithm>

class Simulation {
    public:
        Simulation() {};
        ~Simulation() {};

        void addBody(std::unique_ptr<Body> body) {
            bodies.push_back(std::move(body));
        }
        void addBodies(std::vector<std::unique_ptr<Body>> newBodies){
            for (auto& body: newBodies) {
                addBody(std::move(body));
            }
        };
        
        void removeBody(std::unique_ptr<Body>& body);
        void update(double dt);

        std::vector<glm::vec3> getVelocities();
        std::vector<float> getSpeeds();
        std::vector<glm::vec3> getPositions();
        std::vector<glm::vec4> getPositionsAndSpeed();
        void addTrailPoints();
        std::vector<glm::vec3> getTrails();

    private:
        std::vector<std::unique_ptr<Body>> bodies;
    };
