#pragma once

#include <libconfig.h++>
#include <string>
#include <unordered_set>
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

      static std::tuple<float, float, float> parseCoordinates(
          const libconfig::Setting &setting);
      static Math::Point3D parsePoint3D(const libconfig::Setting &setting);
      static Math::Vector3D parseVector3D(const libconfig::Setting &setting);
      static Math::Vector3D parseColor(const libconfig::Setting &colorSetting);

      void parseSpheres(ShapeComposite &sc,
                        const libconfig::Setting &spheresSetting);
      void parseCylinders(ShapeComposite &sc,
                          const libconfig::Setting &cylindersSetting);
      void parseCones(ShapeComposite &sc,
                      const libconfig::Setting &conesSetting);
      void parsePlanes(ShapeComposite &sc,
                       const libconfig::Setting &planesSetting);

      void parseDirectionalLights(LightComposite &lc,
                                  const libconfig::Setting &lightsSetting);
      void parseAmbientLight(LightComposite &lc,
                             const libconfig::Setting &ambientInfo);
      void parseDiffuseLight(LightComposite &lc,
                             const libconfig::Setting &diffuseInfo);
      void checkSettings(const libconfig::Setting &setting,
                         const std::unordered_set<std::string> &requiredFields) const;
  };
}  // namespace Raytracer
