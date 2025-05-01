#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "DirectionalLight.hpp"
#include "ILight.hpp"
#include "IShape.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

namespace Raytracer {
class Factory {
public:
  Factory() = default;
  ~Factory() = default;

  template<typename T>
  void registerShape(const std::string& name) {
    _shapeFactories[name] = []() { return std::make_shared<T>(); };
  }

  template<typename T>
  void registerLight(const std::string& name) {
    _lightFactories[name] = []() { return std::make_shared<T>(); };
  }

  template<typename T>
  std::shared_ptr<T> create(const std::string &type) {
    // Check if T is a shape type
    if constexpr (std::is_base_of_v<IShape, T>) {
      auto it = _shapeFactories.find(type);
      if (it != _shapeFactories.end()) {
        return std::static_pointer_cast<T>(it->second());
      }
    }
    // Check if T is a light type
    else if constexpr (std::is_base_of_v<ILight, T>) {
      auto it = _lightFactories.find(type);
      if (it != _lightFactories.end()) {
        return std::static_pointer_cast<T>(it->second());
      }
    }
    
    return nullptr;
  }
  
private:
  std::map<std::string, std::function<std::shared_ptr<IShape>()>> _shapeFactories;
  std::map<std::string, std::function<std::shared_ptr<ILight>()>> _lightFactories;
};
}
