#include "ParserConfigFile.hpp"
#include <libconfig.h++>
#include <stdexcept>
#include <string>
#include "AmbientLight.hpp"
#include "Cylinder.hpp"
#include "DirectionalLight.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"
#include "Vector3D.hpp"

Raytracer::ParserConfigFile::ParserConfigFile(const std::string &filename) {
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

void Raytracer::ParserConfigFile::parsePrimitives(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &root) {
  try {
    // SPHERES
    if (root.exists("primitives") && root["primitives"].exists("spheres")) {
      const libconfig::Setting &spheresInfo = root["primitives"]["spheres"];
      for (int i = 0; i < spheresInfo.getLength(); i++) {
        const libconfig::Setting &sphere = spheresInfo[i];
        const libconfig::Setting &colorInfo = sphere["color"];
        _factory.registerShape<Sphere>("sphere");
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
        _factory.registerShape<Cylinder>("cylinder");
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
        _factory.registerLight<DirectionalLight>("directional");
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
        newDirectional->direction = direction.normalize();
        lc.addLight(newDirectional);
      }
    }
    // AMBIENT
    if (root.exists("lights") && root["lights"].exists("ambient")) {
      const libconfig::Setting &ambientInfo = root["lights"]["ambient"];
      const libconfig::Setting &colorInfo = root["lights"]["ambient"]["color"];
      _factory.registerLight<AmbientLight>("ambient");
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
