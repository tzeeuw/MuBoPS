## [0.5.0] Upcoming release
## Added
- Added rectangles and squares to the simulation


## [0.4.1] 05-11-2026
## Added
- Added MacOS support
- Added a SkyBox
## Changed
- Splitted renderer class into Input, Shaders, Camera and new Renderer class.


## [0.4.0] 01-05-2026
## Added
- Added tree body storage based on octree to the simulation
- Added visualisation of the octree during rendering using hotkeys
## Changed
- Changed gravity calculations to the Barnes-Hut algorithm using an octree
## Bug Fixes
- Fixed a bug where acceleration would go to infinite due to two bodies having 0 distance between eachother. It now uses a minimal small distance.


## [0.3.1] 01-05-2026
## Changed
- rendering is now based on floating origin for higher precision calculations
- adding units scaling, bodies scale based on selected unit scale
### Removed
- removed trail points from classical bodies


## [0.3.0] 30-04-2026
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


## [0.2.1] - 29-04-2026
## Added
- pointsize rendering based on camera distance


## [0.2.0] - 29-04-2026
## Added
- Added python bindings for classical bodies simulation
- Added documentation to header files using Doxygen

### Changed
- Refactored code into python binding friendly format


## [0.1.1] - 29-04-2026
### Changed
- Refactored code into python binding friendly format
- Added test python bindings and C++ package can now be imported into python


## [0.1.0] - 22-04-2026
### Added
- Quantum wavefunction visualization of the H2+ molecule based on the Schrodinger equation
- Interactive 3D camera rotation
- Velocity-based color coding for classical bodies
- Gravitational based accretion disk for classical bodies (will be updated in later versions) with leapfrog integration
- Trail rendering for classical bodies
