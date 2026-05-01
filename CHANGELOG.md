## [0.3.1] 2026-05-01
## Changed
- rendering is now based on floating origin for higher precision calculations
- adding units scaling, bodies scale based on selected unit scale
### Removed
- removed trail points from classical bodies


## [0.3.0] 2026-04-30
### Added
- Added camera movement based on free camera
    - Camera can now be moved with WASD for xy movement
    - Spacebar and left ctrl move in the z direction
    - Q and E control the roll
    - Mouse controlls yaw and pitch
    - Middle mouse button can zoom in and out

### Changed
- Changed camera definition to the proper model, view, projection
- Changed background to a dark midnight blue

### Bug Fixes
- Fixed depth rendering of points


## [0.2.1] - 2026-04-29
## Added
- pointsize rendering based on camera distance


## [0.2.0] - 2026-04-29
## Added
- Added python bindings for classical bodies simulation
- Added documentation to header files using Doxygen

### Changed
- Refactored code into python binding friendly format


## [0.1.1] - 2026-04-29
### Changed
- Refactored code into python binding friendly format
- Added test python bindings and C++ package can now be imported into python


## [0.1.0] - 2026-04-22
### Added
- Quantum wavefunction visualization of the H2+ molecule based on the Schrodinger equation
- Interactive 3D camera rotation
- Velocity-based color coding for classical bodies
- Gravitational based accretion disk for classical bodies (will be updated in later versions) with leapfrog integration
- Trail rendering for classical bodies
