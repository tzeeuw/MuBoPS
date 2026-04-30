/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file QuantumBody.hh
 * @brief Class representing a quantum body in the MuBoPS simulation.
 * 
 * The QuantumBody class extends the abstract Body class to represent a quantum particle.
 * It includes properties and methods for handling the wavefunction, probability density,
 * and grid-based representation of the quantum state. The class supports both time-dependent
 * and time-independent simulations.
 */

#pragma once
#include <Body.hh>
#include <vector>
#include <complex>

/**
 * @class QuantumBody
 * @brief Represents a quantum particle in the simulation.
 * 
 * Extends the Body class to include properties and methods specific to quantum mechanics,
 * such as the wavefunction, probability density, and grid representation. The update() method
 * advances the quantum state based on the Schrödinger equation, and getRenderData() provides
 * visualization data based on the probability density.
 */
class QuantumBody : public Body {
    public:

        /// @brief Constructs a QuantumBody with specified grid dimensions and time-dependence.
        /// @param Nx Number of grid points in the x-dimension
        /// @param Ny Number of grid points in the y-dimension
        /// @param Nz Number of grid points in the z-dimension
        /// @param timeDep Flag indicating whether to perform a time-dependent simulation
        QuantumBody(int Nx = 20, int Ny = 20, int Nz = 20, bool timeDep = false) : Nx(Nx), Ny(Ny), Nz(Nz), dx(1.6/(Nx-1)), dy(1.6/(Ny-1)), dz(1.6/(Nz-1)), timeDep(timeDep) {
            psi.resize(Nx*Ny*Nz);
            probability.resize(Nx*Ny*Nz);
            grid.resize(Nx*Ny*Nz);

            createGrid();
            initializeWavefunction();
        }

        ~QuantumBody() {return;}

        /// @brief Creates a 3D grid of positions for the wavefunction based on the specified dimensions and spacing.
        void createGrid();

        /// @brief Initializes the wavefunction with a Gaussian distribution centered at the origin.
        void initializeWavefunction();

        /// @brief Advances the quantum state by one timestep using the Schrödinger equation.
        /// @param dt Timestep in seconds
        void update(double dt) override;

        /// @brief Returns render data for this body based on the probability density.
        /// @return Vector of vec4 where xyz = position and w = normalized intensity (probability density)
        std::vector<glm::vec4> getRenderData(const glm::dvec3& cameraPos) override;

        /// @brief Calculates the probability density from the wavefunction values.
        void calculateProbability();

        /// @brief Returns the current probability density values.
        /// @return Vector of doubles representing the probability density at each grid point
        std::vector<double> getProbability();

    private:
        int Nx, Ny, Nz;                                 ///< Number of grid points in each dimension
        double dx, dy, dz;                              ///< Grid spacing in each dimension
        bool timeDep;                                   ///< Flag for time-dependent simulation
        bool calcProb = false;                          ///< Flag to indicate when to calculate probability density
        double a0 = 0.18;                               ///< Bohr radius in simulation units
        int Z=1;                                        ///< Atomic number for Coulomb potential            

        float d = 0.5 / std::sqrt(3);                   ///< Tetha for the corners of the grid cube, calculated to fit within a unit cube centered at the origin
        glm::dvec3 h1 = glm::dvec3(d,d,d);              ///< Pre-calculated corner positions of the grid cube for efficient rendering
        glm::dvec3 h2 = glm::dvec3(d,-d,-d);            
        glm::dvec3 h3 = glm::dvec3(-d,d,-d);
        glm::dvec3 h4 = glm::dvec3(-d,-d,d);
        glm::dvec3 origin = glm::dvec3(0.0,0.0,0.0);    ///< Origin of the grid, centered at the origin of the simulation space
        
        std::vector<glm::dvec3> grid;                   ///< 3D grid of positions corresponding to the wavefunction values, used for rendering and calculations
        std::vector<std::complex<double>> psi;          ///< Complex wavefunction values defined on the 3D grid, representing the quantum state of the particle
        std::vector<double> probability;                ///< Probability density calculated from the wavefunction, used for rendering intensity and analysis
};
