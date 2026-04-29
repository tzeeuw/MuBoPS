#pragma once
#include <Body.hh>
#include <vector>
#include <complex>

class QuantumBody : public Body {
    public:
        QuantumBody(int Nx = 20, int Ny = 20, int Nz = 20, bool timeDep = false) : Nx(Nx), Ny(Ny), Nz(Nz), dx(1.6/(Nx-1)), dy(1.6/(Ny-1)), dz(1.6/(Nz-1)), timeDep(timeDep) {
            psi.resize(Nx*Ny*Nz);
            probability.resize(Nx*Ny*Nz);
            grid.resize(Nx*Ny*Nz);

            createGrid();
            initializeWavefunction();
        }

        ~QuantumBody() {return;}

        void createGrid();
        void initializeWavefunction();
        void update(double dt) override;
        std::vector<glm::vec4> getRenderData() override;


        void calculateProbability();
        std::vector<double> getProbability();

    private:
        int Nx, Ny, Nz;
        double dx, dy, dz;
        bool timeDep;
        bool calcProb = false;
        double a0 = 0.18;
        int Z=1;

        float d = 0.5 / std::sqrt(3);
        glm::dvec3 h1 = glm::dvec3(d,d,d);
        glm::dvec3 h2 = glm::dvec3(d,-d,-d);
        glm::dvec3 h3 = glm::dvec3(-d,d,-d);
        glm::dvec3 h4 = glm::dvec3(-d,-d,d);
        glm::dvec3 origin = glm::dvec3(0.0,0.0,0.0);
        
        std::vector<glm::dvec3> grid;
        std::vector<std::complex<double>> psi;
        std::vector<double> probability;
};