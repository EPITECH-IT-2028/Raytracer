#pragma once

#include "Camera.hpp"
#include "Factory.hpp"
#include "ShapeComposite.hpp"
#include "LightComposite.hpp"
#include "string"
#include <libconfig.h++>
#include <string>

using libconfig::Config;
using libconfig::FileIOException;
using libconfig::ParseException;
using libconfig::Setting;

namespace Raytracer {
class ParserConfigFile {
public:
  ParserConfigFile(const std::string &filename);

  ~ParserConfigFile() = default;

  void parseConfigFile(Camera &, ShapeComposite &, LightComposite &);
  void parseCamera(Camera &, const Setting &);
  void parsePrimitives(ShapeComposite &, const Setting &);
  void parseLights(LightComposite &, const Setting &);
private:
  Config _cfg;
  Factory _factory = Factory();
};
}
