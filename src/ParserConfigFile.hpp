#pragma once

#include <libconfig.h++>
#include <string>
#include "Camera.hpp"
#include "Factory.hpp"
#include "LightComposite.hpp"
#include "ShapeComposite.hpp"
#include "string"

namespace Raytracer {
  class ParserConfigFile {
    public:
      ParserConfigFile(const std::string &filename,
                       const std::vector<std::string> &plugins);

      ~ParserConfigFile() = default;

      void parseConfigFile(Camera &, ShapeComposite &, LightComposite &);
      void parseCamera(Camera &, const libconfig::Setting &);
      void parsePrimitives(ShapeComposite &, const libconfig::Setting &);
      void parseLights(LightComposite &, const libconfig::Setting &);

    private:
      libconfig::Config _cfg;
      std::vector<std::string> _plugins;
      Factory _factory = Factory();
  };
}  // namespace Raytracer
