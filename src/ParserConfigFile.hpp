#pragma once

#include "Camera.hpp"
#include "Factory.hpp"
#include "ShapeComposite.hpp"
#include "LightComposite.hpp"
#include "string"
#include <libconfig.h++>
#include <string>

namespace Raytracer {
class ParserConfigFile {
public:
  ParserConfigFile(const std::string &filename);

  ~ParserConfigFile() = default;

  void parseConfigFile(Camera &, ShapeComposite &, LightComposite &);
  void parseCamera(Camera &, const libconfig::Setting &);
  void parsePrimitives(ShapeComposite &, const libconfig::Setting &);
  void parseLights(LightComposite &, const libconfig::Setting &);
private:
  libconfig::Config _cfg;
  Factory _factory = Factory();
};
}
