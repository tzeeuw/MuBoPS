/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file Octree.hh
 * @brief Class representing an octree data structure for spatial partitioning in the MuBoPS simulation.
 * 
 * The Octree class manages a hierarchical tree structure that subdivides 3D space into octants.
 * It is used to optimize the calculation of gravitational forces between bodies by grouping distant bodies together and treating them as a single mass aggregate.
 * The class provides methods for building the tree from a collection of bodies, retrieving data for rendering, and performing Barnes-Hut and SPH calculations.
 */

#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <Body.hh>
#include <memory>
#include <vector>
#include <array>
#include <utility>

struct OctreeNode;

/**
 * @class Octree
 * @brief Manages an octree data structure for spatial partitioning in the simulation.
 * 
 * The Octree class provides methods for building the tree from a collection of bodies, retrieving data for rendering, and performing Barnes-Hut and SPH calculations.
 * It uses a variant to store either a leaf node (containing bodies) or an internal node (containing child nodes).
 * The class also includes helper methods for inserting bodies, computing centers of mass, and traversing the tree for various calculations.
 */
class Octree {
    public:

        Octree(int maxDepth = 6);
        ~Octree();

        struct MassAggregate {
            glm::dvec3 centerOfMass;
            double totalMass;
        };

        /// @brief Builds the octree from a collection of bodies, calculating the center of mass for each node.
        /// @param bodies Vector of shared pointers to Body objects to be included in the octree
        void buildTree(std::vector<std::shared_ptr<Body>> bodies);
        void debugPrint();

        /// @brief Retrieves render data for the octree, including positions and intensities for visualization.
        /// @param cameraPos Position of the camera for view-dependent calculations
        /// @param leavesOnly Flag to indicate whether to include only leaf nodes in the render data
        /// @return Vector of pairs containing position and intensity values for rendering
        std::vector<std::pair<glm::dvec3, double>> getRenderData(glm::dvec3 cameraPos, bool leavesOnly = false);

        /// @brief  Performs the Barnes-Hut algorithm to calculate mass aggregates for a given position in space.
        /// @param position Position in space for which to calculate the mass aggregate
        /// @param theta Threshold parameter for the Barnes-Hut approximation (smaller values yield more accurate results but require more computation)
        /// @return Vector of mass aggregates for the specified position
        void BarnesHut(std::shared_ptr<Body>& body, double theta, double eps, double G);
        
        
        std::vector<std::shared_ptr<Body>> SPH(glm::dvec3 position, double cutOffRadius);
        
        
    private:
        
        void insert(std::shared_ptr<Body> body, OctreeNode* node, int depth);
        void createChildren(OctreeNode* node);
        void computeCOM(OctreeNode* node);
        OctreeNode* findClosestChildNode(std::shared_ptr<Body> body, OctreeNode* node);
        void getRenderDataHelper(OctreeNode* node, glm::dvec3 cameraPos, bool leavesOnly, std::vector<std::pair<glm::dvec3, double>>& renderData);
        void printTreeHelper(OctreeNode* node, int depth);
        void BarnesHutHelper(OctreeNode* node, glm::dvec3 position, double theta, glm::dvec3& newAcceleration, double eps);


        std::unique_ptr<OctreeNode> rootNode;
        int maxDepth;
};