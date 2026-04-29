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
        ClassicalBody(int trailPoints = 10) : trailPoints(trailPoints) {
            trail.resize(trailPoints);
        }

        ~ClassicalBody() {return;};

        /// @brief Advances the body's state by one timestep using Newtonian mechanics.
        /// @param dt Timestep in seconds
        void update(double dt) override;

        /// @brief Returns the current trail of positions for this body.
        /// @return Vector of vec3 representing the trail of positions
        const std::vector<glm::vec3>& getTrail() const {return this->trail;}
        
        /// @brief  Adds a new position to the trail, maintaining a fixed number of points.
        /// @param position The new position to add to the trail
        void addTrailPoint(const glm::dvec3& position);
        
        /// @brief Returns render data for this body, including its current position and trail points.
        /// @return Vector of vec4 where xyz = position and w = normalized intensity (speed)
        std::vector<glm::vec4>  getRenderData() override;

    private:
        int trailIndex = 0;                 ///< Index for the next trail point to overwrite
        int trailPoints;                    ///< Maximum number of points to keep in the trail
        std::vector<glm::vec3> trail;       ///< Circular buffer storing the recent positions of the body for rendering trails
};
