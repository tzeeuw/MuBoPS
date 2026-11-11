#pragma once
#include <Body.hh>
#include <Units.hh>
#include <glm/glm.hpp>

class RectangularBody : public Body {
    public:
        RectangularBody(glm::dvec3 position, glm::dvec3 sides);
        ~RectangularBody(){}

        void update(double dt, Units& units) override;

        glm::dvec3 getSideLengths() {return sideLengths;}
        std::vector<glm::vec4> getRenderData(const glm::dvec3& cameraPos, Units& units) override {return {};};
        double getHalfDiagonal() {return halfDiagonal;}
        void setCollisionVector(glm::dvec3 colVector) {collisionVector = colVector;}
        glm::dvec3 getCollisionVector() {return collisionVector;}


    private:
        glm::dvec3 sideLengths;
        double halfDiagonal;
        glm::dvec3 collisionVector;
};
