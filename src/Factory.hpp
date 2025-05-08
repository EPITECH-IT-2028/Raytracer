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
      ~Factory() = default;

      template <typename T>
      void registerShape(const std::string& name, std::function<T*()> addShape) {
        if constexpr (std::is_base_of_v<IShape, T>) {
          _shapeFactories[name] = [addShape]() {
            return std::shared_ptr<IShape>(addShape());
          };
        } else {
          throw std::runtime_error("T must be derived from IShape");
        }
      }

      template <typename T>
      void registerLight(const std::string& name, std::function<T*()> addLight) {
        if constexpr (std::is_base_of_v<ILight, T>) {
          _lightFactories[name] = [addLight]() {
            return std::shared_ptr<ILight>(addLight());
          };
        } else {
          throw std::runtime_error("T must be derived from ILight");
        }
      }

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

        return nullptr;
      }

      void initFactories(const std::vector<std::string> &plugins);

    private:
      std::map<std::string, std::function<std::shared_ptr<IShape>()>>
          _shapeFactories;
      std::map<std::string, std::function<std::shared_ptr<ILight>()>>
          _lightFactories;
  };
}  // namespace Raytracer
