#include "ParserConfigFile.hpp"
#include <libconfig.h++>
#include <stdexcept>
#include <string>
#include "AmbientLight.hpp"
#include "Cylinder.hpp"
#include "DirectionalLight.hpp"
#include "Plane.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"
#include "Vector3D.hpp"
#include "materials/Reflections.hpp"

/**
 * @brief Constructs a ParserConfigFile to load and validate a raytracer configuration file.
 *
 * Attempts to read the specified `.cfg` file using libconfig and prepares the parser with the provided plugin list.
 * Throws a std::runtime_error if the file does not have a `.cfg` extension, cannot be read, or contains parse errors.
 *
 * @param filename Path to the configuration file (must end with `.cfg`).
 * @param plugins List of plugin identifiers to be used by the parser.
 */
Raytracer::ParserConfigFile::ParserConfigFile(
    const std::string &filename, const std::vector<std::string> &plugins)
    : _plugins(plugins) {
  if (!filename.ends_with(".cfg")) {
    throw std::runtime_error(
        "[ERROR] - Config file isn't in correct format (needs to be a *.cfg)");
  }
  try {
    _cfg.readFile(filename);
  } catch (const libconfig::FileIOException &fioex) {
    throw std::runtime_error("[ERROR] - Parsing error in file: " + filename);
  } catch (const libconfig::ParseException &pex) {
    std::string file = pex.getFile();
    std::string errorMessage = "[ERROR] - Parse error in " + file + " ; " +
                               std::to_string(pex.getLine()) + " - " +
                               pex.getError();
    throw std::runtime_error(errorMessage);
  }
}

void Raytracer::ParserConfigFile::parseCamera(Camera &camera,
                                              const libconfig::Setting &root) {
  try {
    const libconfig::Setting &resolutionInfo = root["camera"]["resolution"];
    const libconfig::Setting &positionInfo = root["camera"]["position"];
    const libconfig::Setting &fovInfo = root["camera"];
    int width, height, posX, posY, posZ;
    double fov;
    resolutionInfo.lookupValue("width", width);
    resolutionInfo.lookupValue("height", height);
    positionInfo.lookupValue("x", posX);
    positionInfo.lookupValue("y", posY);
    positionInfo.lookupValue("z", posZ);
    fov = fovInfo.lookup("fieldOfView");
    camera.setHeight(height);
    camera.setWidth(width);
    camera.origin.x = posX;
    camera.origin.y = posY;
    camera.origin.z = posZ;
    camera.setFieldOfView(fov);
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  } catch (const libconfig::SettingTypeException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

/**
 * @brief Parses and adds primitive shapes from the configuration to the scene.
 *
 * Reads sphere, cylinder, and plane definitions from the configuration root and creates corresponding shape objects using the factory. Each shape's properties such as position, size, color, and optional material (for spheres) are set based on the configuration. The created shapes are added to the provided ShapeComposite.
 *
 * @param sc The composite object to which parsed shapes are added.
 * @param root The root configuration setting containing primitive definitions.
 *
 * @throws std::runtime_error If required settings are missing, have invalid types, or object creation fails.
 */
void Raytracer::ParserConfigFile::parsePrimitives(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &root) {
  try {
    // SPHERES
    if (root.exists("primitives") && root["primitives"].exists("spheres")) {
      const libconfig::Setting &spheresInfo = root["primitives"]["spheres"];
      for (int i = 0; i < spheresInfo.getLength(); i++) {
        const libconfig::Setting &sphere = spheresInfo[i];
        const libconfig::Setting &colorInfo = sphere["color"];
        std::shared_ptr<Sphere> newSphere =
            _factory.create<Raytracer::Sphere>("sphere");
        if (newSphere == nullptr) {
          throw std::runtime_error(
              "[ERROR] - Failed during creation of sphere.");
        }
        double posX, posY, posZ, radius;
        double red, green, blue;
        sphere.lookupValue("x", posX);
        sphere.lookupValue("y", posY);
        sphere.lookupValue("z", posZ);
        sphere.lookupValue("r", radius);
        colorInfo.lookupValue("r", red);
        colorInfo.lookupValue("g", green);
        colorInfo.lookupValue("b", blue);
        if (sphere.exists("material")) {
          const libconfig::Setting &materialInfo = sphere["material"];
          std::string materialType;
          materialInfo.lookupValue("type", materialType);
          if (materialType == "reflective") {
            auto reflectiveMaterial =
                _factory.create<Raytracer::Reflections>("reflection");
            newSphere->setMaterial(reflectiveMaterial);
          }
        }
        Math::Vector3D color(red, green, blue);
        Math::Point3D center(posX, posY, posZ);
        newSphere->setColor(color);
        newSphere->setRadius(radius);
        newSphere->setCenter(center);
        sc.addShape(newSphere);
      }
    }

    // CYLINDERS
    if (root.exists("primitives") && root["primitives"].exists("cylinders")) {
      const libconfig::Setting &cylindersInfo = root["primitives"]["cylinders"];
      for (int i = 0; i < cylindersInfo.getLength(); i++) {
        const libconfig::Setting &cylinder = cylindersInfo[i];
        const libconfig::Setting &colorInfo = cylinder["color"];
        auto newCylinder = _factory.create<Raytracer::Cylinder>("cylinder");
        if (newCylinder == nullptr) {
          throw std::runtime_error(
              "[ERROR] - Failed during creation of cylinder.");
        }
        double posX, posY, posZ, red, green, blue;
        double radius, height;
        cylinder.lookupValue("x", posX);
        cylinder.lookupValue("y", posY);
        cylinder.lookupValue("z", posZ);
        cylinder.lookupValue("r", radius);
        cylinder.lookupValue("h", height);
        colorInfo.lookupValue("r", red);
        colorInfo.lookupValue("g", green);
        colorInfo.lookupValue("b", blue);
        Math::Vector3D color(red, green, blue);
        Math::Point3D center(posX, posY, posZ);
        newCylinder->setColor(color);
        newCylinder->setRadius(radius);
        newCylinder->setHeight(height);
        newCylinder->setCenter(center);
        sc.addShape(newCylinder);
      }
    }

    // PLANES
    if (root.exists("primitives") && root["primitives"].exists("planes")) {
      const libconfig::Setting &planesInfo = root["primitives"]["planes"];
      for (int i = 0; i < planesInfo.getLength(); i++) {
        const libconfig::Setting &planeSetting = planesInfo[i];
        if (planeSetting.getLength() < 3 || !planeSetting[0].isString() ||
            !(planeSetting[1].isNumber()) || !planeSetting.exists("color"))
          throw std::runtime_error(
              "[ERROR] - Invalid plane format in config: requires axis "
              "(string), position (number), and color group.");
        const std::string axis = planeSetting[0];
        const double position = planeSetting[1];
        const libconfig::Setting &colorInfo = planeSetting["color"];

        double red, green, blue;
        colorInfo.lookupValue("r", red);
        colorInfo.lookupValue("g", green);
        colorInfo.lookupValue("b", blue);
        Math::Vector3D color(red, green, blue);

        auto newPlane = _factory.create<Raytracer::Plane>("plane");
        if (newPlane == nullptr)
          throw std::runtime_error(
              "[ERROR] - Failed during creation of plane object.");
        if (axis == "X") {
          newPlane->setNormal(Math::Vector3D(1, 0, 0));
          newPlane->setCenter(Math::Point3D(position, 0, 0));
        } else if (axis == "Y") {
          newPlane->setNormal(Math::Vector3D(0, 1, 0));
          newPlane->setCenter(Math::Point3D(0, position, 0));
        } else if (axis == "Z") {
          newPlane->setNormal(Math::Vector3D(0, 0, 1));
          newPlane->setCenter(Math::Point3D(0, 0, position));
        } else {
          throw std::runtime_error("[ERROR] - Invalid axis for plane.");
        }
        newPlane->setColor(color);
        sc.addShape(newPlane);
      }
    }
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  } catch (const libconfig::SettingTypeException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

void Raytracer::ParserConfigFile::parseLights(Raytracer::LightComposite &lc,
                                              const libconfig::Setting &root) {
  try {
    // DIRECTIONALS
    if (root.exists("lights") && root["lights"].exists("directional")) {
      const libconfig::Setting &directionalsInfo =
          root["lights"]["directional"];
      for (int i = 0; i < directionalsInfo.getLength(); i++) {
        const libconfig::Setting &directional = directionalsInfo[i];
        auto newDirectional =
            _factory.create<Raytracer::DirectionalLight>("directional");
        if (newDirectional == nullptr) {
          throw std::runtime_error(
              "[ERROR] - Failed during creation of directional light.");
        }
        double posX, posY, posZ;
        directional.lookupValue("x", posX);
        directional.lookupValue("y", posY);
        directional.lookupValue("z", posZ);
        Math::Vector3D direction(posX, posY, posZ);
        newDirectional->setDirection(direction.normalize());
        newDirectional->setType("DirectionalLight");
        lc.addLight(newDirectional);
      }
    }
    // AMBIENT
    if (root.exists("lights") && root["lights"].exists("ambient")) {
      const libconfig::Setting &ambientInfo = root["lights"]["ambient"];
      const libconfig::Setting &colorInfo = root["lights"]["ambient"]["color"];
      auto newAmbient = _factory.create<AmbientLight>("ambient");
      if (newAmbient == nullptr) {
        throw std::runtime_error(
            "[ERROR] - Failed during creation of ambient light.");
      }
      double red, green, blue, intensity;
      ambientInfo.lookupValue("intensity", intensity);
      colorInfo.lookupValue("r", red);
      colorInfo.lookupValue("g", green);
      colorInfo.lookupValue("b", blue);
      newAmbient->setColor(Math::Vector3D(red, green, blue));
      newAmbient->setIntensity(intensity);
      newAmbient->setType("AmbientLight");
      lc.addLight(newAmbient);
    }
    // DIFFUSE
    if (root.exists("lights") && root["lights"].exists("diffuse")) {
      const libconfig::Setting &diffuseInfo = root["lights"];
      double diffuseMultiplier = diffuseInfo.lookup("diffuse");
      lc.setDiffuse(diffuseMultiplier);
    }
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  } catch (const libconfig::SettingTypeException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

void Raytracer::ParserConfigFile::parseConfigFile(Camera &camera,
                                                  ShapeComposite &sc,
                                                  LightComposite &lc) {
  const libconfig::Setting &root = _cfg.getRoot();
  _factory.initFactories(_plugins);
  // CAMERA
  try {
    parseCamera(camera, root);
  } catch (const std::runtime_error &error) {
    throw std::runtime_error(error.what());
  }
  // PRIMITIVES
  try {
    parsePrimitives(sc, root);
  } catch (const std::runtime_error &error) {
    throw std::runtime_error(error.what());
  }
  // LIGHTS
  try {
    parseLights(lc, root);
  } catch (const std::runtime_error &error) {
    throw std::runtime_error(error.what());
  }
}
