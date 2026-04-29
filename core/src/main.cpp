/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Renderer.hh>
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <random>
#include <algorithm>
#include <memory>
#include <cmath>

int main() {
    Simulation sim = Simulation();
    Renderer renderer = Renderer(sim);


    // add some bodies to the simulation
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> angleDist(0, 2*3.14159);
    std::uniform_real_distribution<double> radDist(0, 1);
    std::uniform_real_distribution<double> zDist(-0.1, 0.1);
    std::vector<std::shared_ptr<Body>> bodies;

    double minRadius = 0.1;
    double maxRadius = 0.8;
    for (int i = 0; i < 5000; i++){
        auto body = std::make_shared<ClassicalBody>(15);
        double angle = angleDist(mt);
        // double phi = phiDist(mt);
        double radius = std::sqrt(minRadius*minRadius + radDist(mt)*(maxRadius*maxRadius - minRadius*minRadius));
        glm::dvec3 position = glm::dvec3(std::cos(angle) * radius,  std::sin(angle) * radius, 0);
        body->setPosition(position);

        // calculate the velocity size
        double velSize = std::sqrt(6.67430e-11 * 5.972e8/radius)/radius;
        glm::dvec3 velocity = glm::dvec3(-position[1], position[0], position[2]) * velSize;
        body->setVelocity(velocity);
        body->setAcceleration({0.0, 0.0, 0.0});
        bodies.push_back(body);
    }

    // auto qbody = std::make_unique<QuantumBody>(30,30,30);
    // bodies.push_back(std::move(qbody));
    sim.addBodies(bodies);


    renderer.setupWindow(800, 800);
    renderer.setupShaders();
    renderer.setupObjects();
    renderer.setupCamera();
    renderer.startRenderLoop();

    return 0;
}
