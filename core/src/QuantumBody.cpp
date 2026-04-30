/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <QuantumBody.hh>
#include <algorithm>
#include <cmath>

double pi = 3.14159265358979323846;

void QuantumBody::calculateProbability(){

    // calculate probability from wavefunction
    std::transform(psi.begin(), psi.end(), probability.begin(), [](std::complex<double> z) {
        return std::norm(z);
    });

    // if not time dependent, we can calculate probability once and reuse it
    if (!timeDep) {
        calcProb = true;
    } 
}

std::vector<double> QuantumBody::getProbability(){

    // if probability has not been calculated yet, calculate it
    if (!calcProb){
        calculateProbability();
    }

    return probability;
};


void QuantumBody::createGrid(){
    // start in corner
    double x = -0.8;
    double y = -0.8;
    double z = -0.8;

    // create grid of positions for the wavefunction
    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                grid[i*Ny*Nz + j*Nz + k] = glm::dvec3(x + i*dx, y + j*dy, z + k*dz);
            }
        }
    }
};


void QuantumBody::initializeWavefunction(){

    // for each point in the grid, calculate the wavefunction value based on the 1S orbital of the hydrogen atom
    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                int index = i*Ny*Nz + j*Nz + k;
                glm::dvec3 pos = grid[index];
                double distance1 = glm::length(pos - h1);
                double distance2 = glm::length(pos - h2);
                double distance3 = glm::length(pos - h3);
                double distance4 = glm::length(pos - h4);

                // for now 4 hydrogen 1S orbitals
                double psi1 = 1 / std::sqrt(pi) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance1/a0);
                double psi2 = 1 / std::sqrt(pi) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance2/a0);
                double psi3 = 1 / std::sqrt(pi) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance3/a0);
                double psi4 = 1 / std::sqrt(pi) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance4/a0);

                psi[index] = {psi1 + psi2 - psi3 - psi4, 0};
            }
        }
    }
}


void QuantumBody::update(double dt) {

    // no update for time-independent simulation
    if (!timeDep){
        return;
    }
    return;
}


std::vector<glm::vec4> QuantumBody::getRenderData(const glm::dvec3& cameraPos) {

    // create render data based on probability density
    std::vector<glm::vec4> renderData(Nx*Ny*Nz);
    std::vector<double> prob = getProbability();

    // for each point in the grid, create a vec4 where xyz is the position and w is the normalized probability density
    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                int index = i*Ny*Nz + j*Nz + k;
                renderData[index] = glm::vec4(static_cast<glm::vec3>(grid[index] - cameraPos), prob[index]);
            }
        }
    }

    return renderData;
}
