/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file Simulation.hh
 * @brief Class representing the core simulation logic for MuBoPS.
 * 
 * The Simulation class manages the collection of bodies in the simulation, updates their states based on physics, and provides data for rendering.
 * It serves as the central point for advancing the simulation and retrieving information about the current state of all bodies.
 */
#pragma once
#include <Body.hh>
#include <vector>
#include <memory>
#include <algorithm>


/**
 * @class Simulation
 * @brief Class representing the core simulation logic for MuBoPS.
 */
class Simulation {
    public:
        Simulation() {};
        ~Simulation() {};

        /// @name Body management
        /// @{
        void addBody(std::unique_ptr<Body> body) {
            bodies.push_back(std::move(body));
        }
        void addBodies(std::vector<std::unique_ptr<Body>> newBodies){
            for (auto& body: newBodies) {
                addBody(std::move(body));
            }
        };
        
        void removeBody(std::unique_ptr<Body>& body);
        /// @}


        void update(double dt);
        void addTrailPoints();

        /// @name Data retrieval for rendering
        /// @{
        std::vector<glm::vec4> getClassicalRenderData();
        std::vector<glm::vec4> getQuantumRenderData();
        std::vector<glm::vec3> getTrails();
        /// @}

    private:
        std::vector<std::unique_ptr<Body>> bodies;      ///< Collection of all bodies in the simulation
    };
