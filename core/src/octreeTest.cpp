#include <Renderer.hh>
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <Units.hh>
#include <Octree.hh>



int main(){

    std::vector<std::shared_ptr<Body>> bodies;

    for (int i = 0; i < 4; i++){
        int x = (i & 1) ? 1 : -1;
        int y = (i & 2) ? 1 : -1;
        auto body = std::make_shared<ClassicalBody>();
        body->setPosition({static_cast<double>(x), static_cast<double>(y), 0.0});
        body->setMass(1.0);
        body->setVelocity({0.0, 0.0, 0.0});
        body->setAcceleration({0.0, 0.0, 0.0});
        bodies.push_back(body);
    }

    Octree tree(3);
    tree.buildTree(bodies);
    tree.debugPrint();
    return 0;
}