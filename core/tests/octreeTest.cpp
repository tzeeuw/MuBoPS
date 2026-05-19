#include <Renderer.hh>
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <Units.hh>
#include <random>
#include <algorithm>
#include <memory>
#include <cmath>
#include <iostream>

int main() {
    Units units = Units();
    units.setScale("Stellar");
    Simulation sim = Simulation(units);
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
    for (int i = 0; i < 100; i++){
        auto body = std::make_shared<ClassicalBody>();
        double angle = angleDist(mt);
        // double phi = phiDist(mt);
        double radius = std::sqrt(minRadius*minRadius + radDist(mt)*(maxRadius*maxRadius - minRadius*minRadius));
        glm::dvec3 position = glm::dvec3(std::cos(angle) * radius,  std::sin(angle) * radius, 0);
        // body->setGravity(true, true);
        body->setPosition(position);
        body->setMass(1.0);

        // calculate the velocity size
        double velSize = std::sqrt(units.G * 1.989e30 / units.massScale / radius)/radius;
        std::cout << velSize << std::endl;
        glm::dvec3 velocity = glm::dvec3(-position[1], position[0], position[2]) * velSize;
        body->setVelocity(velocity);
        body->setAcceleration({0.0, 0.0, 0.0});
        bodies.push_back(body);
    }


    sim.addBodies(bodies);

    renderer.setupWindow(800, 800);
    renderer.setupShaders();
    renderer.setupObjects();
    renderer.setupCamera();
    renderer.startRenderLoop();

    return 0;
}