# Multi-Body Physics Simulaation tool (MuBoPS)
Multi-Body Physics Simulation tool (MuBoPS) is a project aimed to simulated Multi-Body physics across many branches in physics.
The goal is to simulate systems from small scale (particle physics) to large scale (accretion disks or black hole mergers), based on a centralized system.

## Getting Started
### Installing the project
MacOS is currently not supported for this project but is planned. \
To install the project, cmake or any other C++ compiler is required. OpenGL is also required for the project to work as it is used for visualisation. Other external libraries are included in the project and do not require seperate installation. \
The example below is based on cmake. \
First clone the repository
```
git clone https://github.com/tzeeuw/MuBoPS.git
cd MuBoPS
```
Then build the project with cmake
```
cmake -S .\ -B .\build
cmake --build .\build
```
To run the core program \
On Windows
```
.\build\core\Debug\core
```
On Linux
```
.\build\core.exe
```

## Project information
This project is made as a hobby, the goal is to be accurate, but also have good visualisation of the physics problem as I like good graphics. I'm developing it solo and thus updates might not be frequent.

### Features:
* Centralized Body
    * Classical body used for classical computations

### Features in developments:
* Accretion disks 

### Planned features:
* Atomic and molecular wavefunctions
* Fluid dynamics

### Far Future planned features:
* MacOS support

The project will be open source and will be build in C++ and Python.
