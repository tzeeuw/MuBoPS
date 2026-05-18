#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <Body.hh>
#include <memory>
#include <vector>
#include <array>

struct OctreeNode;


class Octree {
    public:

        Octree() {};
        ~Octree() = default;

        struct MassAggregate {
            glm::dvec3 centerOfMass;
            double totalMass;
        };

        void buildTree(std::vector<std::shared_ptr<Body>> bodies);

        
        std::vector<MassAggregate> BarnesHut(glm::dvec3 position, double theta);
        
        std::vector<std::shared_ptr<Body>> SPH(glm::dvec3 position, double cutOffRadius);
        
        
    private:
        
        void insert(std::shared_ptr<Body> body, OctreeNode* node, int depth);
        void createChildren(OctreeNode* node);
        void computeCOM(OctreeNode* node);
        OctreeNode* findClosestChildNode(std::shared_ptr<Body> body, OctreeNode* node);

        std::unique_ptr<OctreeNode> rootNode;
        int maxDepth;
};