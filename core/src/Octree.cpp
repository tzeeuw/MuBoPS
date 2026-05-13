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


void Octree::insert(std::shared_ptr<Body> body, OctreeNode* node, int depth) {


    std::visit([&](auto& data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, OctreeNode::Leaf>) {
            // if node is empty and is a leaf add the body
            if (std::size(data.bodies) == 0){
                data.bodies.push_back(body);
            }
            // if node is a leaf but not emtpy, only add body if max depth is reached
            else{
                if (depth == maxDepth){
                    data.bodies.push_back(body);
                }
                // create new children and push bodies based on position
                else{
                    node->data = OctreeNode::Node{};
                    createChildren(node);
                }
        }

        } else if constexpr (std::is_same_v<T, OctreeNode::Node>) {

            // push the body down one node dependend on its position
            Octree::insert(body, newNode);

        }
    }, node->data);
}


void createChildren(OctreeNode* node) {
    glm::dvec3 nodePos = node->position;
    double halfLength = node->halfLength;
    double childHalfLength = halfLength/2.0;
    OctreeNode::Node children;
    int x = 1;
    int y = 1;
    int z = 1;

    for (int i=0; i<8; i++){
        int x = (i & 1) ? 1 : -1;
        int y = (i & 2) ? 1 : -1;
        int z = (i & 4) ? 1 : -1;
        glm::dvec3 childPos = nodePos + childHalfLength * glm::dvec3(x,y,z);
        children.children[i] = std::make_shared<OctreeNode>();
        children.children[i]->position = childPos;
        children.children[i]->halfLength = childHalfLength;
        children.children[i]->data = OctreeNode::Leaf{};
    }
}