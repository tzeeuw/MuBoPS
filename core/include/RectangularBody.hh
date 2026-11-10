#pragma once
#include <Body.hh>
#include <Units.hh>
#include <glm/glm.hpp>

class RectangularBody : public Body {
    public:
        RectangularBody(glm::dvec3 position, glm::dvec3 sides): sideLengths(sides){setPosition(position);}
        ~RectangularBody(){}

        void update(double dt, Units& units) override {};

        glm::dvec3 getSideLengths() {return sideLengths;}
        std::vector<glm::vec4> getRenderData(const glm::dvec3& cameraPos, Units& units) override {return {};};

    private:
        glm::dvec3 sideLengths;
};
