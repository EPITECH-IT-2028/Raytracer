#pragma once

#include <libconfig.h++>
#include <string>
#include <unordered_set>
#include "Camera.hpp"
#include "Factory.hpp"
#include "LightComposite.hpp"
#include "Object.hpp"
#include "ShapeComposite.hpp"
#include "string"

namespace Raytracer {
  /**
   * @brief Parses configuration files for the Raytracer scene.
   *
   * This class uses libconfig++ to read and interpret .cfg files,
   * populating Camera, ShapeComposite, and LightComposite objects
   * with the data found in the configuration. It supports parsing
   * camera settings, primitives (spheres, cylinders, planes, etc.),
   * lights (ambient, point, directional), and importing other scenes.
   * It also handles parsing .obj files for object primitives.
   */
  class ParserConfigFile {
    public:
      /**
       * @brief Constructor for ParserConfigFile.
       * @param filename The path to the configuration file to parse.
       * @param plugins A list of plugin paths (used by the factory).
       * @throws ParseError if the filename does not end with ".cfg".
       * @throws ConfigError if there's an I/O error reading the file or a parsing error in the file.
       */
      ParserConfigFile(const std::string &filename,
                       const std::vector<std::string> &plugins);

      /**
       * @brief Default destructor for ParserConfigFile.
       */
      ~ParserConfigFile() = default;

      /**
       * @brief Main parsing function.
       * Parses camera, primitives, lights, and scenes from the config file.
       * @param camera Reference to the Camera object to populate.
       * @param sc Reference to the ShapeComposite object to populate.
       * @param lc Reference to the LightComposite object to populate.
       */
      void parseConfigFile(Camera &, ShapeComposite &, LightComposite &);
      
      /**
       * @brief Secondary parsing function for imported scenes.
       * Parses only primitives and lights (no camera) when loading scenes referenced
       * by other config files.
       * @param sc Reference to the ShapeComposite object to populate.
       * @param lc Reference to the LightComposite object to populate.
       */
      void parseConfigFile(ShapeComposite &, LightComposite &);
      /**
       * @brief Parses camera settings from a libconfig setting.
       * @param camera Reference to the Camera object to populate.
       * @param setting The libconfig setting containing camera data.
       * @throws ParseError if required camera settings are missing or invalid.
       */
      void parseCamera(Camera &, const libconfig::Setting &);
      /**
       * @brief Parses primitive shapes from a libconfig setting.
       * @param sc Reference to the ShapeComposite object to populate.
       * @param setting The libconfig setting containing primitives data.
       * @throws ParseError if primitive settings are missing or invalid.
       */
      void parsePrimitives(ShapeComposite &, const libconfig::Setting &);
      /**
       * @brief Parses light sources from a libconfig setting.
       * @param lc Reference to the LightComposite object to populate.
       * @param setting The libconfig setting containing lights data.
       * @throws ParseError if light settings are missing or invalid.
       */
      void parseLights(LightComposite &, const libconfig::Setting &);
      /**
       * @brief Parses scene import directives from a libconfig setting.
       * @param sc Reference to the ShapeComposite object to populate from imported scenes.
       * @param lc Reference to the LightComposite object to populate from imported scenes.
       * @param setting The libconfig setting containing scene import data.
       * @throws ParseError if an import loop is detected or an imported file cannot be parsed.
       */
      void parseScenes(ShapeComposite &, LightComposite &, const libconfig::Setting &);

      /**
       * @brief Parses an OBJ file and populates an Object instance.
       * @param obj_file The path to the .obj file.
       * @param object Reference to the Object to populate.
       * @throws std::runtime_error if TinyObjLoader encounters an error or fails to load the file.
       */
      void parseObj(const std::string &obj_file, Object &object);

    private:
      libconfig::Config _cfg; ///< libconfig Config object.
      std::vector<std::string> _plugins; ///< List of plugin paths.
      std::unordered_set<std::string> _fileAlreadyParse; ///< Set of already parsed file paths to prevent import loops.
      Factory _factory = Factory(); ///< Factory for creating shapes and materials.
      std::string _currentFilePath; ///< Path of the currently parsed configuration file.
  

      /**
       * @brief Parses (x, y, z) coordinates from a libconfig setting.
       * @param setting The libconfig setting.
       * @return std::tuple<float, float, float> The parsed coordinates.
       * @throws ParseError if x, y, or z fields are missing.
       */
      static std::tuple<float, float, float> parseCoordinates(
          const libconfig::Setting &setting);
      /**
       * @brief Parses a Math::Point3D from a libconfig setting.
       * @param setting The libconfig setting.
       * @return Math::Point3D The parsed point.
       */
      static Math::Point3D parsePoint3D(const libconfig::Setting &setting);
      /**
       * @brief Parses a Math::Vector3D from a libconfig setting.
       * @param setting The libconfig setting.
       * @return Math::Vector3D The parsed vector.
       */
      static Math::Vector3D parseVector3D(const libconfig::Setting &setting);
      /**
       * @brief Parses a color (r, g, b) from a libconfig setting.
       * @param colorSetting The libconfig setting for color.
       * @return Math::Vector3D The parsed color vector (values between 0 and 1).
       * @throws ParseError if r, g, or b fields are missing or out of range [0,1].
       */
      static Math::Vector3D parseColor(const libconfig::Setting &colorSetting);
      /**
       * @brief Parses a string value for a "type" field from a libconfig setting.
       * @param setting The libconfig setting.
       * @return std::string The parsed string.
       * @throws libconfig::SettingNotFoundException if the "type" field is missing.
       */
      static std::string parseString(const libconfig::Setting &setting);

      /**
       * @brief Internal helper to parse primitives, lights, and scenes.
       * @param sc ShapeComposite to populate.
       * @param lc LightComposite to populate.
       * @param setting The root libconfig setting.
       */
      void parseInternal(ShapeComposite &, LightComposite &, const libconfig::Setting &);

      /**
       * @brief Parses an MTL file associated with an OBJ file. (Currently not fully implemented within this snippet)
       * @param mtl_file The path to the .mtl file.
       * @param object Reference to the Object to populate with material data.
       */
      void parseMtl(const std::string &mtl_file, Object &object);

      /**
       * @brief Parses sphere definitions from a libconfig setting.
       * @param sc ShapeComposite to add spheres to.
       * @param spheresSetting The libconfig setting for spheres.
       */
      void parseSpheres(ShapeComposite &sc,
                        const libconfig::Setting &spheresSetting);
      /**
       * @brief Parses cylinder definitions from a libconfig setting.
       * @param sc ShapeComposite to add cylinders to.
       * @param cylindersSetting The libconfig setting for cylinders.
       */
      void parseCylinders(ShapeComposite &sc,
                          const libconfig::Setting &cylindersSetting);
      /**
       * @brief Parses infinite cylinder definitions from a libconfig setting.
       * @param sc ShapeComposite to add infinite cylinders to.
       * @param cylindersInfSetting The libconfig setting for infinite cylinders.
       */
      void parseCylindersInf(ShapeComposite &sc,
                             const libconfig::Setting &cylindersInfSetting);
      /**
       * @brief Parses cone definitions from a libconfig setting.
       * @param sc ShapeComposite to add cones to.
       * @param conesSetting The libconfig setting for cones.
       */
      void parseCones(ShapeComposite &sc,
                      const libconfig::Setting &conesSetting);
      /**
       * @brief Parses infinite cone definitions from a libconfig setting.
       * @param sc ShapeComposite to add infinite cones to.
       * @param conesInfSetting The libconfig setting for infinite cones.
       */
      void parseConesInf(ShapeComposite &sc,
                         const libconfig::Setting &conesInfSetting);
      /**
       * @brief Parses plane definitions from a libconfig setting.
       * @param sc ShapeComposite to add planes to.
       * @param planesSetting The libconfig setting for planes.
       */
      void parsePlanes(ShapeComposite &sc,
                       const libconfig::Setting &planesSetting);
      /**
       * @brief Parses object (OBJ file) definitions from a libconfig setting.
       * @param sc ShapeComposite to add objects to.
       * @param objectsSetting The libconfig setting for objects.
       */
      void parseObjects(ShapeComposite &sc,
                        const libconfig::Setting &objectsSetting);
      /**
       * @brief Parses triangle definitions from a libconfig setting.
       * @param sc ShapeComposite to add triangles to.
       * @param trianglesSetting The libconfig setting for triangles.
       */
      void parseTriangles(ShapeComposite &sc,
                          const libconfig::Setting &trianglesSetting);

      /**
       * @brief Parses directional light definitions from a libconfig setting.
       * @param lc LightComposite to add directional lights to.
       * @param lightsSetting The libconfig setting for directional lights.
       */
      void parseDirectionalLights(LightComposite &lc,
                                  const libconfig::Setting &lightsSetting);
      /**
       * @brief Parses ambient light definition from a libconfig setting.
       * @param lc LightComposite to add ambient light to.
       * @param ambientInfo The libconfig setting for ambient light.
       */
      void parseAmbientLight(LightComposite &lc,
                             const libconfig::Setting &ambientInfo);
      /**
       * @brief Parses diffuse light multiplier from a libconfig setting.
       * @param lc LightComposite to set diffuse multiplier for.
       * @param diffuseInfo The libconfig setting for diffuse light.
       */
      void parseDiffuseLight(LightComposite &lc,
                             const libconfig::Setting &diffuseInfo);
      /**
       * @brief Parses point light definitions from a libconfig setting.
       * @param lc LightComposite to add point lights to.
       * @param pointInfo The libconfig setting for point lights.
       */
      void parsePointLight(LightComposite &lc,
                           const libconfig::Setting &pointInfo);
      /**
       * @brief Checks if settings within a group are allowed.
       * @param setting The libconfig setting group to check.
       * @param requiredFields A set of allowed field names.
       * @throws ParseError if an unknown setting is found.
       */
      void checkSettings(
          const libconfig::Setting &setting,
          const std::unordered_set<std::string> &requiredFields) const;
  };
}  // namespace Raytracer
