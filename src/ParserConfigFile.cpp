#include "ParserConfigFile.hpp"
#include <libconfig.h++>
#include <stdexcept>
#include <iostream>
#include <string>
#include "Cylinder.hpp"
#include "DirectionalLight.hpp"
#include "ShapeComposite.hpp"

Raytracer::ParserConfigFile::ParserConfigFile(const std::string &filename) {
  if (!filename.ends_with(".cfg")) {
    throw std::runtime_error("[ERROR] - Config file isn't in correct format (needs to be a *.cfg)");
  }
  try {
    _cfg.readFile(filename);
  } catch (const FileIOException &fioex) {
    std::cerr << "I/o error while reading file." << std::endl;
    throw std::runtime_error("[ERROR] - Parsing error in file: " + filename);
  } catch (const ParseException &pex) {
    std::string file = pex.getFile();
    std::string errorMessage = "[ERROR] - Parse error in " + file  + " ; " + std::to_string(pex.getLine()) + " - " + pex.getError();
    throw std::runtime_error(errorMessage);
  }
}

void Raytracer::ParserConfigFile::parseCamera(Camera &camera, const Setting &root) {
  try {
    const Setting &resolutionInfo = root["camera"]["resolution"];
    const Setting &positionInfo = root["camera"]["position"];
    const Setting &fovInfo = root["camera"];
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

void Raytracer::ParserConfigFile::parsePrimitives(Raytracer::ShapeComposite &sc, const Setting &root) {
  try {
    const Setting &spheresInfo = root["primitives"]["spheres"];
    // SPHERES 
    if (root["primitives"].exists("spheres")) {
      for (int i = 0; i < spheresInfo.getLength(); i ++) {
        const Setting &sphere = spheresInfo[i];
        const Setting &colorInfo = sphere["color"];
        _factory.registerShape<Sphere>("sphere");
        std::shared_ptr<Sphere> newSphere = _factory.create<Raytracer::Sphere>("sphere");
        if (newSphere == nullptr) {
          throw std::runtime_error("[ERROR] - Failed during creation of sphere.");
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
        std::cout << newSphere->getColor().x << " "<< newSphere->getColor().y << " "<< newSphere->getColor().z << std::endl; 
        newSphere->setRadius(radius);
        newSphere->setCenter(center);
        std::cout << newSphere->getCenter().x << " "<< newSphere->getCenter().y << " "<< newSphere->getCenter().z << std::endl; 
        sc.addShape(newSphere);
      }
    }
    
    // CYLINDERS 
    if (root["primitives"].exists("cylinders")) {
      const Setting &cylindersInfo = root["primitives"]["cylinders"];
      for (int i = 0; i < cylindersInfo.getLength(); i++) {
        const Setting &cylinder = cylindersInfo[i];
        const Setting &colorInfo = cylinder["color"];
        _factory.registerShape<Cylinder>("cylinder");
        auto newCylinder = _factory.create<Raytracer::Cylinder>("cylinder");
        if (newCylinder == nullptr) {
          throw std::runtime_error("[ERROR] - Failed during creation of cylinder.");
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

void Raytracer::ParserConfigFile::parseLights(Raytracer::LightComposite &lc, const Setting &root) {
  try {
    // DIRECTIONALS 
    if (root["lights"].exists("directional")) {
      const Setting &directionalsInfo = root["lights"]["directional"];
      for (int i = 0; i < directionalsInfo.getLength(); i++) {
        const Setting &directional = directionalsInfo[i]; 
        _factory.registerLight<DirectionalLight>("directional");
        auto newDirectional = _factory.create<Raytracer::DirectionalLight>("directional");
        if (newDirectional == nullptr) {
          throw std::runtime_error("[ERROR] - Failed during creation of directional light.");
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
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  } catch (const libconfig::SettingTypeException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

void Raytracer::ParserConfigFile::parseConfigFile(Camera &camera, ShapeComposite &sc, LightComposite &lc) {
  const Setting &root = _cfg.getRoot();
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
