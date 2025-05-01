#include "ParserConfigFile.hpp"
#include <libconfig.h++>
#include <stdexcept>
#include <iostream>
#include <string>
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
    std::string errorMessage = "[ERROR] - Parse error at " + file  + ";" + std::to_string(pex.getLine()) + " - " + pex.getError();
    throw std::runtime_error(errorMessage);
  }
}

void Raytracer::ParserConfigFile::parseCamera(Camera &camera, const Setting &root) {
  try {
    const Setting &resolutionInfo = root["camera"]["resolution"];
    const Setting &positionInfo = root["camera"]["position"];
    const Setting &fovInfo = root["camera"];
    int width, height, posX, posY, posZ, fov;
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
  }
}

void Raytracer::ParserConfigFile::parsePrimivites(Raytracer::ShapeComposite &sc, const Setting &root) {
  try {
    const Setting &spheresInfo = root["primivites"]["spheres"];
    // const Setting &planesInfo = root["primivites"]["planes"];

    for (int i = 0; i < spheresInfo.getLength(); i ++) {
      const Setting &sphere = spheresInfo[i];
      const Setting &colorInfo = spheresInfo[i]["color"];
      _factory.registerShape<Sphere>("sphere");
      auto newSphere = _factory.create<Raytracer::Sphere>("sphere");
      if (newSphere == nullptr) {
        throw std::runtime_error("[ERROR] - Failed during creation of sphere.");
      }
      int posX, posY, posZ, red, green, blue;
      double radius;
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
    // for (int i = 0; i < planesInfo.getLength(); i ++) {
    //   const Setting &planeInfo = planesInfo[i];
    //   const Setting &colorInfo = planesInfo[i]["color"];
    //   auto newPlane = _factory.create<Raytracer::Plane>("plane");
    //   sc.addShape(newPlane)
    // }
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw std::runtime_error(nfex.what());
  }
}

void Raytracer::ParserConfigFile::parseLights(Raytracer::LightComposite &lc, const Setting &root) {
  try {
    const Setting &directionalsInfo = root["lights"]["directional"];

    for (int i = 0; i < directionalsInfo.getLength(); i ++) {
      const Setting &directional = directionalsInfo[i]; 
      _factory.registerLight<DirectionalLight>("directional");
      auto newDirectional = _factory.create<Raytracer::DirectionalLight>("directional");
      if (newDirectional == nullptr) {
        throw std::runtime_error("[ERROR] - Failed during creation of directional light.");
      }
      int posX, posY, posZ;
      directional.lookupValue("x", posX);
      directional.lookupValue("y", posY);
      directional.lookupValue("z", posZ);
      Math::Vector3D direction(posX, posY, posZ);
      newDirectional->direction = direction.normalize();
      lc.addLight(newDirectional);
    }
  } catch (const libconfig::SettingNotFoundException &nfex) {
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
    parsePrimivites(sc, root);
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
