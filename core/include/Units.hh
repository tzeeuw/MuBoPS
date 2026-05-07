#pragma once
#include <string>

class Units {
    public:
        double lengthScale;
        double timeScale;
        double massScale;
        double G;
        double a0;

        void setScale(const std::string& scale) {
            if (scale == "SI") {
                lengthScale = 1.0;
                timeScale = 1.0;
                massScale = 1.0;
            }
            if (scale == "Stellar") {
                lengthScale = 1.496e11; // AU
                massScale = 1.989e30; // solar mass
                timeScale = 24*60*60; // days
            }
            if (scale == "Quantum") {
                lengthScale = 3 * 5.291e-11; // 3*bohr radius
                massScale = 9.109e-31; // electron mass
                timeScale = 1e-9; // nanosecond
            }

            a0 = 5.291e-11 / lengthScale; // convert bohr radius
            G = 6.67430e-11 * (massScale * timeScale * timeScale) / (lengthScale*lengthScale*lengthScale);
        }

};
