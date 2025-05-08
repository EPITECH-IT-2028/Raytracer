#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "ILight.hpp"
#include "IShape.hpp"

namespace Raytracer {
  class Factory {
    public:
      Factory() = default;
      /**
 * @brief Destroys the Factory instance.
 *
 * Cleans up resources used by the Factory. Default destructor; no custom cleanup is performed.
 */
~Factory() = default;

      template <typename T>
      /**
       * @brief Registers a factory function for creating shape instances of type T.
       *
       * Associates the given name with a factory function that produces shared pointers to objects of type T, which must inherit from IShape. Throws a runtime error if T does not derive from IShape.
       *
       * @tparam T The shape type to register, must inherit from IShape.
       * @param name The unique string identifier for the shape type.
       * @param addShape A function that returns a raw pointer to a new instance of T.
       */
      void registerShape(const std::string& name,
                         std::function<T*()> addShape) {
        if constexpr (std::is_base_of_v<IShape, T>) {
          _shapeFactories[name] = [addShape]() {
            return std::shared_ptr<IShape>(addShape());
          };
        } else {
          throw std::runtime_error("T must be derived from IShape");
        }
      }

      template <typename T>
      /**
       * @brief Registers a factory function for creating light objects of type T.
       *
       * Associates the given name with a factory function that constructs instances of T, which must inherit from ILight. Throws a runtime error if T does not derive from ILight.
       *
       * @tparam T The light type to register, must inherit from ILight.
       * @param name The unique string identifier for the light type.
       * @param addLight A function that returns a raw pointer to a new instance of T.
       */
      void registerLight(const std::string& name,
                         std::function<T*()> addLight) {
        if constexpr (std::is_base_of_v<ILight, T>) {
          _lightFactories[name] = [addLight]() {
            return std::shared_ptr<ILight>(addLight());
          };
        } else {
          throw std::runtime_error("T must be derived from ILight");
        }
      }

      template <typename T>
      /**
       * @brief Registers a material factory function under a given name.
       *
       * Associates the specified name with a factory function that creates instances of type T, which must inherit from IMaterials. Throws a runtime error if T does not derive from IMaterials.
       *
       * @tparam T The material type to register, must inherit from IMaterials.
       * @param name The unique string identifier for the material type.
       * @param addMaterial A function that returns a raw pointer to a new instance of T.
       */
      void registerMaterial(const std::string& name,
                            std::function<T*()> addMaterial) {
        if constexpr (std::is_base_of_v<IMaterials, T>) {
          _materialFactories[name] = [addMaterial]() {
            return std::shared_ptr<IMaterials>(addMaterial());
          };
        } else {
          throw std::runtime_error("T must be derived from IMaterials");
        }
      }

      template <typename T>
      /**
       * @brief Creates an instance of a registered shape, light, or material by type name.
       *
       * Looks up the factory function for the specified type name and returns a shared pointer to the created object.
       * The template parameter T must inherit from IShape, ILight, or IMaterials. Throws a runtime error if the type is not registered.
       *
       * @param type The string identifier of the registered type to instantiate.
       * @return std::shared_ptr<T> Shared pointer to the created object, or nullptr if T does not inherit from a supported base class.
       *
       * @throws std::runtime_error If the requested type is not registered.
       */
      std::shared_ptr<T> create(const std::string& type) {
        // Check if T is a shape type
        if constexpr (std::is_base_of_v<IShape, T>) {
          auto it = _shapeFactories.find(type);
          if (it != _shapeFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Shape is not registred");
          }
        }
        // Check if T is a light type
        else if constexpr (std::is_base_of_v<ILight, T>) {
          auto it = _lightFactories.find(type);
          if (it != _lightFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Light is not registred");
          }
        }
        // Check if T is a material type
        else if constexpr (std::is_base_of_v<IMaterials, T>) {
          auto it = _materialFactories.find(type);
          if (it != _materialFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Material is not registred");
          }
        }
        return nullptr;
      }

      void initFactories(const std::vector<std::string>& plugins);

    private:
      std::map<std::string, std::function<std::shared_ptr<IShape>()>>
          _shapeFactories;
      std::map<std::string, std::function<std::shared_ptr<ILight>()>>
          _lightFactories;
      std::map<std::string, std::function<std::shared_ptr<IMaterials>()>>
          _materialFactories;
  };
}  // namespace Raytracer
