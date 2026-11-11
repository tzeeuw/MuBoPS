/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Simulation.hh>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <RectangularBody.hh>
#include <iostream>
#include <GLFW/glfw3.h>

double avg = 0.0;

// retrieves index of max value in a 3d vec
int maxIndex(const glm::dvec3& v) {
    return (v.x >= v.y) ? ((v.x >= v.z) ? 0 : 2) : ((v.y >= v.z) ? 1 : 2);
}


void Simulation::removeBody(std::shared_ptr<Body>& body) {

    // find the body in the vector and remove it
    auto it = std::find_if(bodies.begin(), bodies.end(), [&](const std::shared_ptr<Body>& b) {
        return b.get() == body.get();
    });

    if (it != bodies.end()) {
        bodies.erase(it);
    }
}

void Simulation::initGravityPairs() {
    for (auto& body1: bodies) {
        ClassicalBody* cbody1 = dynamic_cast<ClassicalBody*>(body1.get());
        if (!cbody1 || !cbody1->affectedByGravity) {
            continue;
        }
        for (auto& body2: bodies) {
            if (body1 == body2) {
                continue;
            }
            ClassicalBody* cbody2 = dynamic_cast<ClassicalBody*>(body2.get());
            if (!cbody2 || !cbody2->exertsGravity){
                continue;
            }

            gravityPairs.push_back(std::pair(body1, body2));
        }
    }
}


void Simulation::update(double dt) {

    // update each body in the simulation by calling its update method
    // double t1 = glfwGetTime();
    octree.buildTree(bodies);
    // double t2 = glfwGetTime();
    // octree.debugPrint();
    // updateGravity(dt);

    // VERY UGLY FIX LATER BUT FOR NOW A CHECK
    RectangularBody* rectBody = dynamic_cast<RectangularBody*>(bodies[0].get());
    RectangularBody* rectBody2 = dynamic_cast<RectangularBody*>(bodies[1].get());
    if (rectBody && rectBody2) {
        bool collides = checkCollision(rectBody, rectBody2);
        if (collides) {
            std::cout << "they are colliding!" << std::endl;
        }
    }
    for (auto& body: bodies) {
        // updateGravityBH(body, 0.8, 1e-2);
        body->update(dt, units);
    }
    // double t3 = glfwGetTime();
    // std::cout << "Tree building time: " << t2 - t1 << "s,  BH: " << t3 - t2 << "s" << std::endl;
}

void Simulation::updateGravity(double dt) {
    for (auto& bodyPair: gravityPairs) {
        auto& cbody1 = bodyPair.first;
        auto& cbody2 = bodyPair.second;
        
        glm::dvec3 newAcceleration = cbody1->getNewAcceleration();
        glm::dvec3 pos1 = cbody1->getPosition();
        glm::dvec3 pos2 = cbody2->getPosition();

        glm::dvec3 dpos = pos2 - pos1;
        double dist = glm::length(dpos);

        if (dist < 1e-2) {
            dist = 1e-2;
        }

        newAcceleration += units.G * cbody2->getMass() * dpos / (dist * dist * dist);
        cbody1->setNewAcceleration(newAcceleration);
    }
}

void Simulation::updateGravityBH(std::shared_ptr<Body>& body, double theta, double eps){
    // double t4 = glfwGetTime();
    glm::dvec3 newAcceleration = body->getNewAcceleration();
    glm::dvec3 bodyPos = body->getPosition();
    // octree.BarnesHut(body, theta, eps, units.G);
    std::vector<Octree::MassAggregate> masses = octree.BarnesHut(bodyPos, theta);
    // avg += masses.size();
    double t5 = glfwGetTime();
    for (auto& mass: masses){
        glm::dvec3 dpos = mass.centerOfMass - bodyPos;
        double dist = std::sqrt(glm::length(dpos)*glm::length(dpos) + eps*eps);

        newAcceleration += units.G * mass.totalMass * dpos / (dist * dist * dist);
    }
    body->setNewAcceleration(newAcceleration);
    // double t6 = glfwGetTime();
    // std::cout << "Mass aggregating: " << t5 - t4 << "s, acceleration: " << t6 - t5 << "s" << std::endl;
}


std::vector<glm::vec4> Simulation::getQuantumRenderData(const glm::dvec3& cameraPos) {

    // retrieve the render data for quantum bodies, which includes position and normalized probability density for rendering
    std::vector<glm::vec4> renderData;

    // get all quantum bodies
    for (auto& body: bodies) {
        QuantumBody* quantumBody = dynamic_cast<QuantumBody*>(body.get());
        if (quantumBody){
            std::vector<glm::vec4> bodyData = body->getRenderData(cameraPos, units);
            renderData.insert(renderData.end(), bodyData.begin(), bodyData.end());
        }
    }

    // find min and max value for 4th data point
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;

    for (auto& dataPoint : renderData) {
        minVal = std::min(minVal, dataPoint[3]);
        maxVal = std::max(maxVal, dataPoint[3]);
    }


    // normalize the 4th data point to be between 0 and 1 for rendering
    for (auto& dataPoint : renderData) {
        dataPoint[3] = (dataPoint[3] - minVal) / (maxVal - minVal);
    }
    return renderData;
}

std::vector<glm::vec4> Simulation::getClassicalRenderData(const glm::dvec3& cameraPos) {

    // retrieve the render data for classical bodies, which includes position and normalized speed for rendering
    std::vector<glm::vec4> renderData;

    // find all classical bodies
    for (auto& body: bodies) {
        ClassicalBody* classicalBody = dynamic_cast<ClassicalBody*>(body.get());
        if (classicalBody){
            std::vector<glm::vec4> bodyData = body->getRenderData(cameraPos, units);
            renderData.insert(renderData.end(), bodyData.begin(), bodyData.end());
        }
    }

    // find min and max value for 4th data point
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;

    for (auto& dataPoint : renderData) {
        minVal = std::min(minVal, dataPoint[3]);
        maxVal = std::max(maxVal, dataPoint[3]);
    }

    // normalize the 4th data point to be between 0 and 1 for rendering
    for (auto& dataPoint : renderData) {
        dataPoint[3] = (dataPoint[3] - minVal) / (maxVal - minVal);
    }
    return renderData;
}


std::vector<std::pair<glm::dvec3, glm::dvec3>> Simulation::getRectRenderData(glm::dvec3 cameraPos) {
    std::vector<std::pair<glm::dvec3, glm::dvec3>> renderData;

    for (auto& body: bodies) {
        RectangularBody* rectBody = dynamic_cast<RectangularBody*>(body.get());
        if (rectBody) {
            std::pair<glm::dvec3, glm::dvec3> bodyData;
            bodyData.first = rectBody->getPosition() - cameraPos;
            bodyData.second = rectBody->getSideLengths();
            renderData.insert(renderData.end(), bodyData);
        }
    }

    return renderData;
}



bool Simulation::checkCollision(RectangularBody* firstBody, RectangularBody* secondBody){
    
    // calculate distance and minimum distance for collision required
    glm::dvec3 firstPosition = firstBody->getPosition();
    glm::dvec3 secondPosition = secondBody->getPosition();
    double distance = glm::length(firstPosition - secondPosition);
    double collisionDistance = firstBody->getHalfDiagonal() + secondBody->getHalfDiagonal();
    
    // return false if bodies are far enough apart
    if (distance > collisionDistance) {
        return false;
    }
    
    // this computation is done keeping in mind that bodies will rotate, meaning that the collision vectors of each body needs to be calculated
    // distanceVector points from the SECOND body to the FIRST body
    glm::dvec3 distanceVector = firstPosition - secondPosition;
    glm::dvec3 firstSLHalf = firstBody->getSideLengths() / 2.0;
    glm::dvec3 secondSLHalf = secondBody->getSideLengths() / 2.0;

    // here we add some rotation matrix so that the vector is rotated into the coordinates of the rectangle, but that is for later
    // add rotation here

    // here we calculate which component is furthest away from its half sidelength
    // this tells us which component should be used to scale the distanceVector so that we find the point on the edge
    // of the object that corresponds with the collision
    glm::dvec3 firstSubtractedVector = glm::abs(distanceVector) - firstSLHalf;
    glm::dvec3 secondSubtractedVector = glm::abs(distanceVector) - secondSLHalf;
    int firstMaxIndex = maxIndex(firstSubtractedVector);
    int secondMaxIndex = maxIndex(secondSubtractedVector);

    // std::cout << firstSubtractedVector.x << firstSubtractedVector.y << firstSubtractedVector.z
    // << "\n" << secondSubtractedVector.x << secondSubtractedVector.y << secondSubtractedVector.z << std::endl;
    // std::cout << "firstMaxIndex: "<< firstMaxIndex << " secondMaxIndex: " << secondMaxIndex << std::endl;

    // find the scales for the two collisionVectors
    // the firstScale is times -1.0 because the distanceVector goes from SECOND to FIRST, so we have to invert it
    double firstScale = -1.0 * firstSLHalf[firstMaxIndex] / distanceVector[firstMaxIndex];
    double secondScale = secondSLHalf[secondMaxIndex] / distanceVector[secondMaxIndex];

    // calculate the collisionVectors
    glm::dvec3 firstCollisionVector = distanceVector * firstScale * glm::sign(distanceVector[firstMaxIndex]);
    glm::dvec3 secondCollisionVector = distanceVector * secondScale * glm::sign(distanceVector[secondMaxIndex]);


    // if the sum of the length collision vectors is greater or equal to the distance between the two bodies
    // means that they are clipping and thus collision occured
    double clippingLength = glm::length(firstCollisionVector) + glm::length(secondCollisionVector);

    // std::cout << clippingLength << std::endl;
    if (clippingLength < distance){
        return false;
    }

    // now set the collision vectors so that further calculations do not have to recompute it
    // the reason the computation is done here is because it is needed to be done here due to the rotation of the rectangles
    firstBody->setCollisionVector(firstCollisionVector);
    secondBody->setCollisionVector(secondCollisionVector);


    // using the seperating axes theorem or hyperplane separation theorem, we can check if the boxes collide
    // Later when adding rotations, we just have to add more axes instead so the fundamental codes stay the same


    return true;

}
