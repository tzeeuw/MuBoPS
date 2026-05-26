#include <Octree.hh>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/component_wise.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

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



Octree::Octree(int maxDepth): maxDepth(maxDepth) {rootNode = std::make_unique<OctreeNode>();};
Octree::~Octree() = default;
 
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

    // now insert all the bodies
    for (auto& body: bodies) {
        insert(body, rootNode.get(), 0);
    }

    // calculate all the center of masses for the nodes
    computeCOM(rootNode.get());
}


void Octree::computeCOM(OctreeNode* node) {
    double totalMass = 0.0;
    glm::dvec3 COMPos = {0.0,0.0,0.0};

    // based on the node we either calculate the leaf mass, or recursively calculate the mass
    std::visit([&](auto& data) {
        using T = std::decay_t<decltype(data)>;

        // if node is a leaf, calculate COM of the bodies
        if constexpr (std::is_same_v<T, OctreeNode::Leaf>) {
            for (auto& body: data.bodies){
                glm::dvec3 pos = body->getPosition();
                double mass = body->getMass();
                totalMass += mass;
                COMPos += mass * pos;
            }
        } else if constexpr (std::is_same_v<T, OctreeNode::Node>) {

            // if node is not a leaf, calculate COM of the children
            for (auto& child: data.children){
                OctreeNode* childptr = child.get();
                computeCOM(childptr);
                double childmass = childptr->totalMass;
                totalMass += childmass;
                COMPos += childptr->centerOfMassPosition * childmass;
            } 
        }
    }, node->data);

    // prevent division by 0
    if (totalMass == 0.0){
        COMPos = node->position;
    }
    else {
        COMPos /= totalMass;
    }
    node->totalMass = totalMass;
    node->centerOfMassPosition = COMPos;
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
                    std::shared_ptr<Body> body1 = data.bodies[0];

                    node->data = OctreeNode::Node{};
                    createChildren(node);

                    OctreeNode* newNode1 = findClosestChildNode(body1, node);
                    insert(body1, newNode1, depth + 1);

                    OctreeNode* newNode2 = findClosestChildNode(body, node);
                    insert(body, newNode2, depth + 1);
                }
        }

        } else if constexpr (std::is_same_v<T, OctreeNode::Node>) {
            // push the body down one node dependend on its position
            OctreeNode* newNode = findClosestChildNode(body, node);
            Octree::insert(body, newNode, depth + 1);

        }
    }, node->data);
}


OctreeNode* Octree::findClosestChildNode(std::shared_ptr<Body> body, OctreeNode* node){
    // the next child is based on in which quadrant the body lays which is only dependent on the sign off the position
    glm::dvec3 bodyPos = body->getPosition();

    // calculates if the body is larger or smaller than the node position to determine in what quadrant it lies
    bool px = bodyPos.x > node->position.x;
    bool py = bodyPos.y > node->position.y;
    bool pz = bodyPos.z > node->position.z;

    // index is now based on which quadrant it lies in corresponding to the create children method
    int index = (px ? 1 : 0) | (py ? 2 : 0) | (pz ? 4 : 0);

    auto data = std::get<OctreeNode::Node>(node->data);

    return data.children[index].get();
}


void Octree::createChildren(OctreeNode* node) {
    glm::dvec3 nodePos = node->position;
    double halfLength = node->halfLength;
    double childHalfLength = halfLength/2.0;
    OctreeNode::Node children;
    int x = 1;
    int y = 1;
    int z = 1;

    // create the 8 different children
    for (int i=0; i<8; i++){

        // based on i, determine in which quadrant the child is
        // i => 4 is in positive z region
        // odd i is in positive x region
        // i=2,3,6,7 is in positive y region
        int x = (i & 1) ? 1 : -1;
        int y = (i & 2) ? 1 : -1;
        int z = (i & 4) ? 1 : -1;
        glm::dvec3 childPos = nodePos + childHalfLength * glm::dvec3(x,y,z);
        children.children[i] = std::make_shared<OctreeNode>();
        children.children[i]->position = childPos;
        children.children[i]->halfLength = childHalfLength;
        children.children[i]->data = OctreeNode::Leaf{};
    }

    node->data = children;
}

std::vector<std::pair<glm::dvec3, double>> Octree::getRenderData(glm::dvec3 cameraPos, bool leavesOnly){

    std::vector<std::pair<glm::dvec3, double>> renderData;

    // pass renderData as reference to conserve memory
    getRenderDataHelper(rootNode.get(), cameraPos, leavesOnly, renderData);

    return renderData;
}

void Octree::getRenderDataHelper(OctreeNode* node, glm::dvec3 cameraPos, bool leavesOnly, std::vector<std::pair<glm::dvec3, double>>& renderData){
    std::visit([&](auto& data) {
        using T = std::decay_t<decltype(data)>;

        // if it is a leave just add the render data
        if constexpr (std::is_same_v<T, OctreeNode::Leaf>) {
            renderData.push_back(std::pair<glm::dvec3, double>(node->position - cameraPos, node->halfLength));
        }
        // if it is a node add the node data if leavesonly is false and add all child data
        else if constexpr (std::is_same_v<T, OctreeNode::Node>) {
            if (!leavesOnly){
                renderData.push_back(std::pair<glm::dvec3, double>(node->position - cameraPos, node->halfLength));
            }

            // for each child add the render data
            for (auto& child: data.children){
                OctreeNode* childptr = child.get();
                getRenderDataHelper(childptr, cameraPos, leavesOnly, renderData);
            }
        }
    }, node->data);
}


void Octree::debugPrint(){
    printTreeHelper(rootNode.get(), 0);
}

void Octree::printTreeHelper(OctreeNode* node, int depth){
    // based on depth print number of spaces
    std::string prefix(4*depth, ' ');
    std::cout << prefix;

    std::visit([&](auto& data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, OctreeNode::Leaf>) {
            std::cout << "Leaf(" << glm::to_string(node->position) << ", " << node->totalMass << ", " << glm::to_string(node->centerOfMassPosition) << ")" << std::endl
            << prefix << "    bodies:" << std::endl;
            for (auto& body: data.bodies){
                std::cout << prefix << "    postion: " << glm::to_string(body->getPosition()) << ", mass: " << body->getMass() << std::endl;
            }
        } else if constexpr(std::is_same_v<T, OctreeNode::Node>) {
            std::cout << "Node(" << glm::to_string(node->position) << ", " << node->totalMass << ", " << glm::to_string(node->centerOfMassPosition) << ")" << std::endl;
            for (auto& child: data.children){
                OctreeNode* childptr = child.get();
                printTreeHelper(childptr, depth + 1);
            }
        }
    }, node->data);
}



void Octree::BarnesHut(std::shared_ptr<Body>& body, double theta, double eps, double G){
    glm::dvec3 newAcceleration = body->getNewAcceleration();
    glm::dvec3 position = body->getPosition();
    BarnesHutHelper(rootNode.get(), position, theta, newAcceleration, eps);
    body->setNewAcceleration(G * newAcceleration);
}

void Octree::BarnesHutHelper(OctreeNode* node, glm::dvec3 position, double theta, glm::dvec3& newAcceleration, double eps){

    std::visit([&](auto& data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, OctreeNode::Leaf>) {
            for (auto& body: data.bodies){
                glm::dvec3 bodyPos = body->getPosition();

                // skip the self contribution
                if (glm::distance(position, bodyPos) < 1.0e-6){
                    continue;
                }
                double bodyMass = body->getMass();
                glm::dvec3 dpos = bodyPos - position;
                double dist = std::sqrt(glm::length(dpos)*glm::length(dpos) + eps*eps);
                newAcceleration += bodyMass * dpos / (dist * dist * dist);
            }
        } else if constexpr (std::is_same_v<T, OctreeNode::Node>) {
            double distance = glm::distance(position, node->centerOfMassPosition);
            double s = 2*node->halfLength;

            // avoid division by 0 when computing s/d
            if (distance < 1.0e-6){
                distance = 1.0e-6;
            }
            if (s/distance > theta){
                for (auto& child: data.children){
                    BarnesHutHelper(child.get(), position, theta, newAcceleration, eps);
                }
            } else {
                glm::dvec3 nodePos = node->centerOfMassPosition;
                double nodeMass = node->totalMass;
                glm::dvec3 dpos = nodePos - position;
                double dist = std::sqrt(glm::length(dpos)*glm::length(dpos) + eps*eps);
                newAcceleration += nodeMass * dpos / (dist * dist * dist);
            }
        }
    }, node->data);
}