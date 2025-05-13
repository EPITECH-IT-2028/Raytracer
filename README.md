# Raytracer :white_circle: :sunny:
Raytracer is a project we've done for Epitech. The goal of this project is simple : do a raytracer.

## Setup 
### 1. Install libconfig 
- On Ubuntu:
```bash
sudo apt-get install libconfig++-dev
```
- On MacOs:
```bash
brew install libconfig
```

### 2. Compile the project with CMake 
```bash
cmake -B .build && cmake --build .build
```

### 3. Run Raytracer
```bash
./raytracer ./scenes/example.cfg
```
## Features

#### Lights: 
- [x] Directional lights
- [x] Ambient lights
- [x] Point lights
- [x] Diffuse lights
- [x] Colored lights
- [x] Multiple point lights
- [x] Drop shadows

#### Primitives:
- [x] Spheres
- [x] Planes 
- [x] .OBJ files
- [x] Limited cones 
- [x] Limited cylinders 
- [ ] Cones 
- [ ] Cylinders 
- [ ] Mobius strip 
- [ ] Fractals
- [ ] Triangles
- [ ] Tanglecube
- [ ] Torus

#### Materials:
- [x] Reflections
- [x] Refractions
- [x] Transparency
- [x] Flat colors 
- [x] Phong reflection

#### Transformations:
- [x] Translations
- [x] Rotations
- [ ] Scale
- [ ] Shear 
- [ ] Transformation matrix 
- [ ] Scene 

#### Bonus:
- [x] Free cam

# Contributing
If you want to contribute to the project, please create a new branch and make a pull request.
## Authors
- [@Etienne](https://github.com/Etiennelbre)
- [@Arthur](https://github.com/ZifFiji)
- [@Nolann](https://github.com/MelmanC)
- [@Quentin](https://github.com/quentin-lpr)
