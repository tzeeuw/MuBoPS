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
#include <Units.hh>
#include <Octree.hh>


/**
 * @class Simulation
 * @brief Class representing the core simulation logic for MuBoPS.
 */
class Simulation {
    public:
        Simulation(Units& units, int octreeDepth = 6): units(units), octree(octreeDepth) {};
        ~Simulation() {};

        /// @name Body management
        /// @{
        void addBody(std::shared_ptr<Body> body) {
            bodies.push_back(body);
        }
        void addBodies(std::vector<std::shared_ptr<Body>> newBodies){
            for (auto& body: newBodies) {
                addBody(body);
            }
        };
        
        void removeBody(std::shared_ptr<Body>& body);
        /// @}
        void initGravityPairs();

        void update(double dt);
        void updateGravity(double dt);
        void updateGravityBH(std::shared_ptr<Body>& body, double theta, double eps);

        /// @name Data retrieval for rendering
        /// @{
        std::vector<glm::vec4> getClassicalRenderData(const glm::dvec3& cameraPos);
        std::vector<glm::vec4> getQuantumRenderData(const glm::dvec3& cameraPos);
        std::vector<std::pair<glm::dvec3, double>> getOctreeRenderData(glm::dvec3 cameraPos) {return octree.getRenderData(cameraPos);};

        /// @}
        
        
    private:
        Units& units;
        std::vector<std::shared_ptr<Body>> bodies;      ///< Collection of all bodies in the simulation
        std::vector<std::pair<std::shared_ptr<Body>, std::shared_ptr<Body>>> gravityPairs;
        Octree octree;
    };
