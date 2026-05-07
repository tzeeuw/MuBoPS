/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file Body.hh
 * @brief Abstract base class for all bodies in the MuBoPS simulation.
 * 
 * The Body class defines the common interface for all simulation objects.
 * Derived classes must implement update() for physics and getRenderData() 
 * for visualization. Bodies cannot be instantiated directly.
 */

#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Units.hh>


/**
 * @class Body
 * @brief Abstract base class representing any object in the simulation.
 * 
 * Provides common properties (position, velocity, acceleration, mass, radius)
 * and defines the interface that all derived body types must implement.
 */
class Body {
    public:
        Body() {return;};
        
        /// Virtual destructor to ensure correct cleanup of derived classes
        virtual ~Body() {return;};

        /// @name Identity
        /// @{
        void setName(std::string name) { this->name = name; }
        const std::string& getName() { return this->name; }
        /// @}

        /// @name Physical properties
        /// @{
        void setMass(double mass) { this->mass = mass; }
        double getMass() { return this->mass; }

        void setRadius(double radius) { this->radius = radius; }
        double getRadius() { return this->radius; }
        /// @}

        /// @name Kinematics
        /// @{
        void setPosition(glm::dvec3 position) { this->position = position; }
        const glm::dvec3& getPosition() { return this->position; }

        void setVelocity(glm::dvec3 velocity) { this->velocity = velocity; }
        const glm::dvec3& getVelocity() { return this->velocity; }

        /// @brief Returns the scalar speed (magnitude of velocity) as a float
        float getSpeed() { return static_cast<float>(glm::length(this->getVelocity())); }

        void setAcceleration(glm::dvec3 acceleration) { this->acceleration = acceleration; }
        const glm::dvec3& getAcceleration() { return this->acceleration; }

        void setNewAcceleration(glm::dvec3 newAcceleration) {this->newAcceleration = newAcceleration; }
        const glm::dvec3& getNewAcceleration() {return this->newAcceleration; }
        /// @}

        /**
         * @brief Advances the body's state by one timestep.
         * @param dt Timestep in seconds
         */
        virtual void update(double dt, Units& units) = 0;

        /**
         * @brief Returns render data for this body.
         * @return Vector of vec4 where xyz = position and w = normalized intensity
         *         (speed for classical bodies, probability density for quantum bodies)
         */
        virtual std::vector<glm::vec4> getRenderData(const glm::dvec3& cameraPos, Units& units) = 0;

    private:
        std::string name;           ///< Human-readable identifier
        double mass;                ///< Mass in kg
        double radius;              ///< Radius in simulation units
        glm::dvec3 position;        ///< Position in simulation units
        glm::dvec3 velocity;        ///< Velocity in simulation units per second
        glm::dvec3 acceleration;    ///< Acceleration in simulation units per second squared
        glm::dvec3 newAcceleration;
};
