#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <Body.hh>
#include <memory>
#include <vector>
#include <array>
#include <utility>

struct OctreeNode;


class Octree {
    public:

        Octree(int maxDepth = 6);
        ~Octree();

        struct MassAggregate {
            glm::dvec3 centerOfMass;
            double totalMass;
        };

        void buildTree(std::vector<std::shared_ptr<Body>> bodies);
        void debugPrint();

        std::vector<std::pair<glm::dvec3, double>> getRenderData(glm::dvec3 cameraPos, bool leavesOnly = false);

        
        std::vector<MassAggregate> BarnesHut(glm::dvec3 position, double theta);
        
        std::vector<std::shared_ptr<Body>> SPH(glm::dvec3 position, double cutOffRadius);
        
        
    private:
        
        void insert(std::shared_ptr<Body> body, OctreeNode* node, int depth);
        void createChildren(OctreeNode* node);
        void computeCOM(OctreeNode* node);
        OctreeNode* findClosestChildNode(std::shared_ptr<Body> body, OctreeNode* node);
        void getRenderDataHelper(OctreeNode* node, glm::dvec3 cameraPos, bool leavesOnly, std::vector<std::pair<glm::dvec3, double>>& renderData);
        void printTreeHelper(OctreeNode* node, int depth);

        std::unique_ptr<OctreeNode> rootNode;
        int maxDepth;
};