#pragma once
#include <Body.hh>
#include <vector>

class ClassicalBody : public Body {
    public:
        ClassicalBody(int trailPoints = 10) : trailPoints(trailPoints) {
            trail.resize(trailPoints);
        }
        ~ClassicalBody() {return;};

        void update(double dt) override;

        const std::vector<glm::vec3>& getTrail() const {return this->trail;}
        void addTrailPoint(const glm::dvec3& position);

    private:
        int trailIndex = 0;
        int trailPoints;
        std::vector<glm::vec3> trail;
};
