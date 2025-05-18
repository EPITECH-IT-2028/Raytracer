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
  /**
   * @brief A factory class for creating shapes, lights, and materials.
   *
   * This factory uses registered creation functions (typically from plugins or
   * compiled-in types) to instantiate objects based on a string identifier.
   * It supports IShape, ILight, and IMaterials types.
   */
  class Factory {
    public:
      /**
       * @brief Default constructor.
       */
      Factory() = default;
      /**
       * @brief Default destructor.
       */
      ~Factory() = default;

      /**
       * @brief Registers a shape creation function.
       * @tparam T The concrete shape type, must derive from IShape.
       * @param name The string identifier for this shape type.
       * @param addShape A function pointer or lambda that creates an instance of T.
       * @throw std::runtime_error if T does not derive from IShape.
       */
      template <typename T>
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

      /**
       * @brief Registers a light creation function.
       * @tparam T The concrete light type, must derive from ILight.
       * @param name The string identifier for this light type.
       * @param addLight A function pointer or lambda that creates an instance of T.
       * @throw std::runtime_error if T does not derive from ILight.
       */
      template <typename T>
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

      /**
       * @brief Registers a material creation function.
       * @tparam T The concrete material type, must derive from IMaterials.
       * @param name The string identifier for this material type.
       * @param addMaterial A function pointer or lambda that creates an instance of T.
       * @throw std::runtime_error if T does not derive from IMaterials.
       */
      template <typename T>
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

      /**
       * @brief Creates an instance of a registered type (shape, light, or material).
       * @tparam T The base type of the object to create (IShape, ILight, or IMaterials).
       * @param type The string identifier of the concrete type to create.
       * @return std::shared_ptr<T> A shared pointer to the created object.
       * @throw std::runtime_error if the type is not registered or T is not a supported base type.
       */
      template <typename T>
      std::shared_ptr<T> create(const std::string& type) {
        // Check if T is a shape type
        if constexpr (std::is_base_of_v<IShape, T>) {
          auto it = _shapeFactories.find(type);
          if (it != _shapeFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Shape is not registered");
          }
        }
        // Check if T is a light type
        else if constexpr (std::is_base_of_v<ILight, T>) {
          auto it = _lightFactories.find(type);
          if (it != _lightFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Light is not registered");
          }
        }
        // Check if T is a material type
        else if constexpr (std::is_base_of_v<IMaterials, T>) {
          auto it = _materialFactories.find(type);
          if (it != _materialFactories.end()) {
            return std::static_pointer_cast<T>(it->second());
          } else {
            throw std::runtime_error("Material is not registered: " + type);
          }
        }
        return nullptr;
      }

      /**
       * @brief Initializes factories by loading creators from plugins.
       * @param plugins A list of plugin file paths to load.
       */
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
