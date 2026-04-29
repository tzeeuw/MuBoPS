/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <QuantumBody.hh>
#include <algorithm>
#include <cmath>



void QuantumBody::calculateProbability(){
    std::transform(psi.begin(), psi.end(), probability.begin(), [](std::complex<double> z) {
        return std::norm(z);
    });

    if (!timeDep) {
        calcProb = true;
    } 
}

std::vector<double> QuantumBody::getProbability(){

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

    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                grid[i*Ny*Nz + j*Nz + k] = glm::dvec3(x + i*dx, y + j*dy, z + k*dz);
            }
        }
    }
};


void QuantumBody::initializeWavefunction(){
    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                int index = i*Ny*Nz + j*Nz + k;
                glm::dvec3 pos = grid[index];
                double distance1 = glm::length(pos - h1);
                double distance2 = glm::length(pos - h2);
                double distance3 = glm::length(pos - h3);
                double distance4 = glm::length(pos - h4);

                double psi1 = 1 / std::sqrt(M_PI) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance1/a0);
                double psi2 = 1 / std::sqrt(M_PI) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance2/a0);
                double psi3 = 1 / std::sqrt(M_PI) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance3/a0);
                double psi4 = 1 / std::sqrt(M_PI) * std::pow(Z/a0, 3.0/2.0) * std::exp(-Z*distance4/a0);

                psi[index] = {psi1 + psi2 - psi3 - psi4, 0};
            }
        }
    }
}


void QuantumBody::update(double dt) {
    if (!timeDep){
        return;
    }
    return;
}


std::vector<glm::vec4> QuantumBody::getRenderData() {
    std::vector<glm::vec4> renderData(Nx*Ny*Nz);
    std::vector<double> prob = getProbability();

    for (int i = 0; i<Nx; i++) {
        for (int j = 0; j<Ny; j++) {
            for (int k = 0; k<Nz; k++) {
                int index = i*Ny*Nz + j*Nz + k;
                renderData[index] = glm::vec4(static_cast<glm::vec3>(grid[index]), prob[index]);
            }
        }
    }

    return renderData;
}