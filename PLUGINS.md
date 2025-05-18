# Raytracer Plugin System

This document explains the plugin system used in the Raytracer project, allowing for dynamic loading of new shapes, lights, and materials.

## Overview

The Raytracer utilizes a plugin system to extend its capabilities without modifying the core application. Plugins are implemented as shared libraries (e.g., `.so` files on Linux, `.dylib` on macOS) that are loaded at runtime. Each plugin can provide implementations for new types of shapes, lights, or materials.

The core of the plugin system resides in the `Factory` class (`src/Factory.cpp`). This class is responsible for:
1.  Discovering and loading plugin files from a specified directory (typically `./plugins/`).
2.  Identifying factory functions within each plugin.
3.  Registering these factory functions to allow the creation of plugin-defined objects.

## Plugin Types

The system supports three main types of plugins:

1.  **Shapes**: Geometric objects that can be rendered in the scene (e.g., spheres, planes, cones).
2.  **Lights**: Light sources that illuminate the scene (e.g., point lights, directional lights, ambient lights).
3.  **Materials**: Properties that define how a shape's surface interacts with light (e.g., diffuse, reflective, refractive, transparent).

## Core Interfaces

To create a new plugin, you need to implement one or more of the following interfaces:

### 1. `Raytracer::IShape` (`src/shapes/IShape.hpp`)

   -   **Purpose**: Defines the contract for all drawable shapes.
   -   **Key Methods**:
        -   `hits(const Raytracer::Ray &ray) const`: Calculates ray-shape intersection.
        -   `getNormal(const Math::Point3D &hitPoint) const`: Returns the surface normal at a point.
        -   `translate(const Math::Vector3D &offset)`: Translates the shape.
        -   Getter/setter methods for properties like center, color, shininess, and material.
   -   **Base Class**: `Raytracer::AShape` (`src/shapes/AShape.hpp`) is an abstract base class that implements `IShape` and provides common functionality. New shapes should typically inherit from `AShape`.

### 2. `Raytracer::ILight` (`src/lights/ILight.hpp`)

   -   **Purpose**: Defines the contract for all light sources.
   -   **Key Methods**:
        -   `computeLighting(...) const`: Calculates the light's contribution to a point's color.
        -   `getType() const`: Returns the type of the light (e.g., "point", "directional").
        -   `getDirection() const`: Returns the light's direction (if applicable).
        -   `getColor() const`: Returns the light's color.
        -   `getIntensity() const`: Returns the light's intensity.

### 3. `Raytracer::IMaterials` (`src/materials/IMaterials.hpp`)

   -   **Purpose**: Defines the contract for how materials compute their appearance.
   -   **Key Method**:
        -   `computeMaterial(...) const`: Calculates the final color of a surface point, considering lighting, view direction, and material properties. This method often involves casting secondary rays for effects like reflection or refraction, using the `RayColorFunc` callback.

## Implementing a New Plugin

To implement a new plugin (e.g., a new shape called "MyCustomShape"):

1.  **Create Header and Source Files**:
    -   `MyCustomShape.hpp`
    -   `MyCustomShape.cpp`

2.  **Implement the Interface**:
    -   In `MyCustomShape.hpp`, declare your class, inheriting from the appropriate base class (e.g., `Raytracer::AShape`).
    -   ```cpp
        #pragma once
        #include "shapes/AShape.hpp" // Or other relevant base

        namespace Raytracer {
            class MyCustomShape : public AShape {
            public:
                MyCustomShape(/* constructor params */);
                ~MyCustomShape() override = default;

                // Implement IShape methods
                std::tuple<double, Math::Vector3D, const IShape *> hits(
                    const Raytracer::Ray &ray) const override;
                Math::Vector3D getNormal(const Math::Point3D &hitPoint) const override;
                // ... other necessary methods or overrides ...

            private:
                // Member variables
            };
        }
        ```
    -   In `MyCustomShape.cpp`, provide the implementations for these methods.

3.  **Create the Factory Function**:
    -   In `MyCustomShape.cpp`, you **must** define an `extern "C"` function that creates an instance of your new class and returns a pointer to the base interface type.
    -   The name of this function is crucial for the `Factory` to discover it:
        -   For shapes: `addShape()`
        -   For lights: `addLight()`
        -   For materials: `addMaterial()`
    -   Example for `MyCustomShape`:
        ```cpp
        // In MyCustomShape.cpp
        #include "MyCustomShape.hpp"

        // ... (Implementations of MyCustomShape methods) ...

        extern "C" {
            Raytracer::IShape *addShape() {
                // You might want to return a default-constructed object
                // or one configured in a specific way.
                return new Raytracer::MyCustomShape(/* default params */);
            }
        }
        ```

4.  **Compile as a Shared Library**:
    -   Update your `CMakeLists.txt` (or build system) to compile this new shape (or light/material) into a shared library.
    -   Example `CMakeLists.txt` addition (simplified):
        ```cmake
        # Find the source files for your plugin
        add_library(mycustomshape SHARED src/shapes/custom/MyCustomShape.cpp)
        # Ensure it links against any necessary Raytracer libraries/components
        # target_link_libraries(mycustomshape PRIVATE RaytracerCore) # Example

        # Place the output .so/.dylib in the plugins directory
        set_target_properties(mycustomshape PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
        ```
    -   The compiled shared library (e.g., `mycustomshape.so`) should be placed in the `plugins/` directory relative to the Raytracer executable.

## How Plugins are Loaded

1.  When the Raytracer starts, the `Factory::initFactories` method is called.
2.  This method scans the `plugins/` directory for shared library files.
3.  For each library found:
    -   It uses `dlopen()` to load the library into memory.
    -   It uses `dlsym()` to look for the predefined factory function names (`addShape`, `addLight`, `addMaterial`).
    -   If a factory function is found, it's registered in a map within the `Factory`, associating the plugin's name (derived from the filename) with the function pointer.
    -   Example: If `plugins/sphere.so` contains `addShape()`, the factory will register a way to create `Sphere` objects using the key "sphere".

## Using Plugins in Configuration Files

Once a plugin is loaded, you can use the new shapes, lights, or materials in your scene configuration files (`.cfg`). The name used in the configuration file should match the plugin's filename (without the extension).

For example, if you create `plugins/mycustomshape.so`:

```libconfig
// In your scene.cfg
primitives: {
  objects: (
    {
      type = "mycustomshape"; // Matches the plugin filename
      center = { x = 0; y = 0; z = -10; };
      // ... other parameters for MyCustomShape ...
      material = {
        type = "diffuse"; // Or a custom material plugin
        color = { r = 255; g = 0; b = 0; };
      };
    }
  );
};
```

The `ParserConfigFile` class, when parsing the scene, will use the `Factory` to create instances of "mycustomshape" by calling the registered `addShape()` function from your plugin.

## Important Considerations

-   **Naming Conventions**: The factory function names (`addShape`, `addLight`, `addMaterial`) are fixed. The plugin filename (e.g., `sphere.so`, `pointlight.so`, `reflective.so`) determines the `type` string used in configuration files.
-   **Dependencies**: If your plugin has external dependencies, ensure they are correctly handled by your build system and are available at runtime.
-   **Error Handling**: Implement robust error handling in your plugin code. The `Factory` will skip plugins that fail to load or if symbols are not found, but internal plugin errors can still crash the application.
-   **Memory Management**: The factory functions typically return raw pointers (`IShape*`, `ILight*`, `IMaterials*`). The Raytracer system (likely `ShapeComposite`, `LightComposite`, or the parser) should take ownership of these pointers, often by wrapping them in `std::shared_ptr`. Ensure your plugin's destructor correctly cleans up any resources if you allocate them.
-   **Recompilation**: After adding or modifying a plugin, you need to recompile that specific plugin (shared library). You don't necessarily need to recompile the entire Raytracer application unless core interfaces have changed.

This plugin architecture provides a flexible way to extend the Raytracer's functionality with new and exciting features.
