#include <Octree.hh>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>


struct OctreeNode {

    struct Node {
        std::array<std::shared_ptr<OctreeNode>, 8> children;
    };

    struct Leaf {
        std::vector<std::shared_ptr<Body>> bodies;
    };


    glm::dvec3 position;
    double halfLength;

    double totalMass;
    glm::dvec3 centerOfMassPosition;

    std::variant<Leaf, Node> data; 
};
 
void Octree::buildTree(std::vector<std::shared_ptr<Body>> bodies){

    // retrieve half length of the root node
    glm::dvec3 minPos = {DBL_MAX, DBL_MAX, DBL_MAX};
    glm::dvec3 maxPos = {-DBL_MAX, -DBL_MAX, -DBL_MAX};
    double totalMass = 0.0;
    glm::dvec3 COMPos = {0.0,0.0,0.0};

    for (auto& body: bodies) {
        glm::dvec3 pos = body->getPosition();
        double mass = body->getMass();
        minPos = glm::min(minPos, pos);
        maxPos = glm::max(maxPos, pos);
        totalMass += mass;
        COMPos += mass * pos;
    };

    COMPos /= totalMass;

    double halfLength = glm::compMax((maxPos - minPos) / 2.0);
    glm::dvec3 rootPos = (minPos + maxPos) / 2.0;
    // add small increment so that no body falls outside
    rootNode->halfLength = halfLength + 1e-6;
    rootNode->totalMass = totalMass;
    rootNode->centerOfMassPosition = COMPos;
    rootNode->position = rootPos;
    rootNode->data = OctreeNode::Leaf{};
}