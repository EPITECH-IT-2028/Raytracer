#include "ParserConfigFile.hpp"
#include <libconfig.h++>
#include <stdexcept>
#include <string>
#include "Plane.hpp"
#include "AmbientLight.hpp"
#include "Cylinder.hpp"
#include "DirectionalLight.hpp"
#include "Plane.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"
#include "Vector3D.hpp"

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

Math::Point3D Raytracer::ParserConfigFile::parsePoint3D(
    const libconfig::Setting &setting, const std::string &x_name,
    const std::string &y_name, const std::string &z_name) {
  float x, y, z;
  if (!setting.lookupValue(x_name, x) || !setting.lookupValue(y_name, y) ||
      !setting.lookupValue(z_name, z)) {
    throw libconfig::SettingNotFoundException(
        "Missing one or more coordinate fields (x, y, z) for a point/vector.");
  }
  return {x, y, z};
}

Math::Vector3D Raytracer::ParserConfigFile::parseVector3D(
    const libconfig::Setting &setting, const std::string &x_name,
    const std::string &y_name, const std::string &z_name) {
  float x, y, z;
  if (!setting.lookupValue(x_name, x) || !setting.lookupValue(y_name, y) ||
      !setting.lookupValue(z_name, z)) {
    throw libconfig::SettingNotFoundException(
        "Missing one or more coordinate fields (x, y, z) for a point/vector.");
  }
  return {x, y, z};
}

Math::Vector3D Raytracer::ParserConfigFile::parseColor(
    const libconfig::Setting &colorSetting) {
  float r, g, b;
  if (!colorSetting.lookupValue("r", r) || !colorSetting.lookupValue("g", g) ||
      !colorSetting.lookupValue("b", b)) {
    throw libconfig::SettingNotFoundException(
        "Missing one or more color fields (r, g, b).");
  }
  return {r, g, b};
}

void Raytracer::ParserConfigFile::parseSpheres(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &spheresSetting) {
  for (int i = 0; i < spheresSetting.getLength(); i++) {
    const libconfig::Setting &sphere = spheresSetting[i];
    auto newSphere = _factory.create<Raytracer::Sphere>("sphere");
    if (!newSphere)
      throw std::runtime_error("[ERROR] - Failed during creation of sphere.");

    newSphere->setCenter(parsePoint3D(sphere));
    newSphere->setRadius(sphere.lookup("r").operator double());
    newSphere->setColor(parseColor(sphere["color"]));

    // Optional options
    if (sphere.exists("translate")) {
      Math::Vector3D translation = parseVector3D(sphere["translate"]);
      newSphere->translate(translation);
    }
    sc.addShape(newSphere);
  }
}

void Raytracer::ParserConfigFile::parseCylinders(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &cylindersSetting) {
  for (int i = 0; i < cylindersSetting.getLength(); i++) {
    const libconfig::Setting &cylinder = cylindersSetting[i];
    auto newCylinder = _factory.create<Raytracer::Cylinder>("cylinder");
    if (!newCylinder)
      throw std::runtime_error("[ERROR] - Failed during creation of cylinder.");

    newCylinder->setCenter(parsePoint3D(cylinder));
    newCylinder->setRadius(cylinder.lookup("r").operator double());
    newCylinder->setHeight(cylinder.lookup("h").operator double());
    newCylinder->setColor(parseColor(cylinder["color"]));

    // Optional options
    if (cylinder.exists("translate")) {
      Math::Vector3D translation = parseVector3D(cylinder["translate"]);
      newCylinder->translate(translation);
    }
    sc.addShape(newCylinder);
  }
}

void Raytracer::ParserConfigFile::parsePlanes(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &planesSettings) {
  for (int i = 0; i < planesSettings.getLength(); i++) {
    const libconfig::Setting &plane = planesSettings[i];
    auto newPlane = _factory.create<Raytracer::Plane>("plane");
    if (!newPlane)
      throw std::runtime_error("[ERROR] - Failed during creation of plane.");

    newPlane->setCenter(parsePoint3D(plane));
    newPlane->setNormal(parseVector3D(plane["normal"]));
    newPlane->setColor(parseColor(plane["color"]));

    // Optional options
    if (plane.exists("translate")) {
      Math::Vector3D translation = parseVector3D(plane["translate"]);
      newPlane->translate(translation);
    }
    sc.addShape(newPlane);
  }
}

void Raytracer::ParserConfigFile::parsePrimitives(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &root) {
  try {
    // SPHERES
    if (root.exists("primitives") && root["primitives"].exists("spheres"))
      parseSpheres(sc, root["primitives"]["spheres"]);

    // CYLINDERS
    if (root.exists("primitives") && root["primitives"].exists("cylinders"))
      parseCylinders(sc, root["primitives"]["cylinders"]);

    // PLANES
    if (root.exists("primitives") && root["primitives"].exists("planes"))
      parsePlanes(sc, root["primitives"]["planes"]);
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  } catch (const libconfig::SettingTypeException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

void Raytracer::ParserConfigFile::parseDirectionalLights(
    Raytracer::LightComposite &lc, const libconfig::Setting &lightsSetting) {
  for (int i = 0; i < lightsSetting.getLength(); i++) {
    const libconfig::Setting &light = lightsSetting[i];
    auto newDirectional =
        _factory.create<Raytracer::DirectionalLight>("directional");
    if (newDirectional == nullptr) {
      throw std::runtime_error(
          "[ERROR] - Failed during creation of directional light.");
    }

    double posX, posY, posZ;
    light.lookupValue("x", posX);
    light.lookupValue("y", posY);
    light.lookupValue("z", posZ);

    Math::Vector3D direction(posX, posY, posZ);
    newDirectional->direction = direction.normalize();
    lc.addLight(newDirectional);
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
    //if (root.exists("lights") && root["lights"].exists("directional"))
    //  parseDirectionalLights(lc, root["lights"]["directional"]);
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
