/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file ClassicalBody.hh
 * @brief Concrete class representing a classical body in the MuBoPS simulation.
 * 
 * The ClassicalBody class implements the physics and rendering for a classical
 * point mass. It can maintain a trail of recent positions for visualization.
 */

#pragma once
#include <Body.hh>
#include <Units.hh>
#include <vector>

/**
 * @class ClassicalBody
 * @brief Represents a classical point mass in the simulation.
 * 
 * Implements Newtonian mechanics for motion and can store a trail of recent
 * positions for rendering purposes. The update() method advances the body's
 * state based on its current velocity and acceleration.
 */
class ClassicalBody : public Body {
    public:
        ClassicalBody(){};
        ~ClassicalBody() {return;};

        /// @brief Advances the body's state by one timestep using Newtonian mechanics.
        /// @param dt Timestep in seconds
        void update(double dt, Units& units) override;
        
        /// @brief Returns render data for this body, including its current position and trail points.
        /// @return Vector of vec4 where xyz = position and w = normalized intensity (speed)
        std::vector<glm::vec4>  getRenderData(const glm::dvec3& cameraPos, Units& units) override;

        void setGravity(bool affectedByGravity, bool exertsGravity){
            this->affectedByGravity = affectedByGravity;
            this->exertsGravity = exertsGravity;
        };
        
        bool exertsGravity = false;
        bool affectedByGravity = false;

    private:
};
