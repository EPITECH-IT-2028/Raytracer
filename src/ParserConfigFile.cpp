#include "ParserConfigFile.hpp"
#include <iostream>
#include <libconfig.h++>
#include <string>
#include <tuple>
#include <vector>
#include "AmbientLight.hpp"
#include "Cone.hpp"
#include "ConeInf.hpp"
#include "Cylinder.hpp"
#include "CylinderInf.hpp"
#include "DirectionalLight.hpp"
#include "LightComposite.hpp"
#include "Object.hpp"
#include "Plane.hpp"
#include "PointLight.hpp"
#include "Reflections.hpp"
#include "Refractions.hpp"
#include "ShapeComposite.hpp"
#include "Sphere.hpp"
#include "Transparency.hpp"
#include "Triangle.hpp"
#include "Vector3D.hpp"
#include "exceptions/RaytracerException.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void Raytracer::ParserConfigFile::parseObj(const std::string &obj_file,
                                           Object &object) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                  obj_file.c_str(), nullptr);

  if (!warn.empty()) {
    std::cerr << "TinyObjLoader warning: " << warn << std::endl;
  }
  if (!err.empty()) {
    throw std::runtime_error("TinyObjLoader error: " + err);
  }
  if (!success) {
    throw std::runtime_error("Failed to load .obj file: " + obj_file);
  }

  std::vector<Object::Face> faces;
  std::vector<Math::Point3D> vertices;
  std::vector<Math::Vector3D> normals;
  std::unordered_map<std::string, Object::Mtl> materials2;

  for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
    double x = attrib.vertices[3 * i + 0];
    double y = attrib.vertices[3 * i + 1];
    double z = attrib.vertices[3 * i + 2];
    vertices.emplace_back(Math::Point3D(x, y, z));
  }

  for (size_t i = 0; i < attrib.normals.size() / 3; i++) {
    double x = attrib.normals[3 * i + 0];
    double y = attrib.normals[3 * i + 1];
    double z = attrib.normals[3 * i + 2];
    normals.emplace_back(Math::Vector3D(x, y, z));
  }

  for (const auto &material : materials) {
    Object::Mtl mtl;
    mtl.ambient = Math::Vector3D(material.ambient[0], material.ambient[1],
                                 material.ambient[2]);
    mtl.diffuse = Math::Vector3D(material.diffuse[0], material.diffuse[1],
                                 material.diffuse[2]);
    mtl.specular = Math::Vector3D(material.specular[0], material.specular[1],
                                  material.specular[2]);
    mtl.shininess = material.shininess;
    mtl.transparency = material.dissolve;
    mtl.illumination = material.illum;
    materials2[material.name] = mtl;
  }

  for (const auto &shape : shapes) {
    for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
      Object::Face face;

      for (size_t j = 0; j < 3; j++) {
        int vertex_index = shape.mesh.indices[i + j].vertex_index;
        face.vertex.push_back(vertex_index);

        int normal_index = shape.mesh.indices[i + j].normal_index;
        if (normal_index >= 0) {
          face.normal.push_back(normal_index);
        }
      }

      int material_id = shape.mesh.material_ids[i / 3];
      if (material_id >= 0 &&
          material_id < static_cast<int>(materials.size())) {
        face.material_name = materials[material_id].name;
      }
      faces.push_back(face);
    }
  }
  object.setVertices(vertices);
  object.setNormals(normals);
  object.setFaces(faces);
  object.setMaterials(materials2);
}

Raytracer::ParserConfigFile::ParserConfigFile(
    const std::string &filename, const std::vector<std::string> &plugins)
    : _plugins(plugins), _currentFilePath(filename) {
  if (!filename.ends_with(".cfg")) {
    throw ParseError(
        "Config file isn't in correct format (needs to be a *.cfg)");
  }
  try {
    _cfg.readFile(filename.c_str());
  } catch (const libconfig::FileIOException &fioex) {
    throw ConfigError(
        std::string("I/O error reading config file: ") + fioex.what(),
        filename);
  } catch (const libconfig::ParseException &pex) {
    throw ConfigError(pex.getError(), pex.getFile() ? pex.getFile() : filename,
                      pex.getLine());
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
    throw ParseError(std::string("Camera config: ") + nfex.getPath() +
                     " not found or invalid.");
  } catch (const libconfig::SettingTypeException &stex) {
    throw ParseError(std::string("Camera config: ") + stex.getPath() +
                     " has incorrect type.");
  }
}

std::tuple<float, float, float> Raytracer::ParserConfigFile::parseCoordinates(
    const libconfig::Setting &setting) {
  float x, y, z;
  if (!setting.lookupValue("x", x) || !setting.lookupValue("y", y) ||
      !setting.lookupValue("z", z))
    throw ParseError(
        std::string("Missing coordinate field(s) (x, y, or z) in setting: ") +
        setting.getPath());
  return {x, y, z};
}

Math::Point3D Raytracer::ParserConfigFile::parsePoint3D(
    const libconfig::Setting &setting) {
  auto [x, y, z] = parseCoordinates(setting);
  return {x, y, z};
}

Math::Vector3D Raytracer::ParserConfigFile::parseVector3D(
    const libconfig::Setting &setting) {
  auto [x, y, z] = parseCoordinates(setting);
  return {x, y, z};
}

std::string Raytracer::ParserConfigFile::parseString(
    const libconfig::Setting &setting) {
  std::string str;
  if (!setting.lookupValue("type", str))
    throw libconfig::SettingNotFoundException("Missing 'type' field.");
  return str;
}

Math::Vector3D Raytracer::ParserConfigFile::parseColor(
    const libconfig::Setting &setting) {
  float r, g, b;
  if (!setting.lookupValue("r", r) || !setting.lookupValue("g", g) ||
      !setting.lookupValue("b", b))
    throw ParseError(
        std::string("Missing color field(s) (r, g, or b) in setting: ") +
        setting.getPath());
  if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1)
    throw ParseError(
        std::string("Color values out of range [0, 1] in setting: ") +
        setting.getPath());
  return {r, g, b};
}

void Raytracer::ParserConfigFile::parseSpheres(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &spheresSetting) {
  for (int i = 0; i < spheresSetting.getLength(); i++) {
    const libconfig::Setting &sphere = spheresSetting[i];
    auto newSphere = _factory.create<Raytracer::Sphere>("sphere");
    if (!newSphere)
      throw ParseError("Failed to create sphere object from factory.");
    if (!sphere.exists("r"))
      throw ParseError(std::string("Sphere radius not found at ") +
                       sphere.getPath());
    if (!sphere.exists("color"))
      throw ParseError(std::string("Sphere color not found at ") +
                       sphere.getPath());
    newSphere->setCenter(parsePoint3D(sphere));
    if (sphere.lookup("r").operator double() <= 0)
      throw ParseError(std::string("Sphere radius must be positive at ") +
                       sphere.getPath());
    newSphere->setRadius(sphere.lookup("r").operator double());
    newSphere->setColor(parseColor(sphere["color"]));

    // Optional options
    if (sphere.exists("translate")) {
      Math::Vector3D translation = parseVector3D(sphere["translate"]);
      newSphere->translate(translation);
    }
    if (sphere.exists("material")) {
      std::string materialName = parseString(sphere["material"]);
      if (materialName == "reflective") {
        newSphere->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "refractive") {
        newSphere->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else if (materialName == "transparent") {
        newSphere->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else {
        throw ParseError(std::string("[ERROR] - Unknown material type: ") +
                         materialName);
      }
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
      throw ParseError("Failed to create cylinder object from factory.");
    if (!cylinder.exists("r"))
      throw ParseError(std::string("Cylinder radius not found at ") +
                       cylinder.getPath());
    if (!cylinder.exists("h"))
      throw ParseError(std::string("Cylinder height not found at ") +
                       cylinder.getPath());
    if (!cylinder.exists("color"))
      throw ParseError(std::string("Cylinder color not found at ") +
                       cylinder.getPath());

    newCylinder->setCenter(parsePoint3D(cylinder));
    if (cylinder.lookup("r").operator double() <= 0)
      throw ParseError(std::string("Cylinder radius must be positive at ") +
                       cylinder.getPath());
    newCylinder->setRadius(cylinder.lookup("r").operator double());
    newCylinder->setHeight(cylinder.lookup("h").operator double());
    newCylinder->setColor(parseColor(cylinder["color"]));

    // Optional options
    if (cylinder.exists("translate")) {
      Math::Vector3D translation = parseVector3D(cylinder["translate"]);
      newCylinder->translate(translation);
    }
    if (cylinder.exists("material")) {
      std::string materialName = parseString(cylinder["material"]);
      if (materialName == "reflective") {
        newCylinder->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "refractive") {
        newCylinder->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else if (materialName == "transparent") {
        newCylinder->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else {
        throw std::runtime_error("[ERROR] - Unknown material type.");
      }
    }
    sc.addShape(newCylinder);
  }
}

void Raytracer::ParserConfigFile::parseCylindersInf(
    Raytracer::ShapeComposite &sc,
    const libconfig::Setting &cylindersInfSetting) {
  for (int i = 0; i < cylindersInfSetting.getLength(); i++) {
    const libconfig::Setting &cylinderInf = cylindersInfSetting[i];
    auto newCylinderInf =
        _factory.create<Raytracer::CylinderInf>("cylinderInf");
    if (!newCylinderInf)
      throw ParseError("Failed to create cylinderInf object from factory.");
    if (!cylinderInf.exists("r"))
      throw ParseError(std::string("CylinderInf radius not found at ") +
                       cylinderInf.getPath());
    if (!cylinderInf.exists("color"))
      throw ParseError(std::string("CylinderInf color not found at ") +
                       cylinderInf.getPath());

    newCylinderInf->setCenter(parsePoint3D(cylinderInf));
    if (cylinderInf.lookup("r").operator double() <= 0)
      throw ParseError(std::string("CylinderInf radius must be positive at ") +
                       cylinderInf.getPath());
    newCylinderInf->setRadius(cylinderInf.lookup("r").operator double());
    newCylinderInf->setColor(parseColor(cylinderInf["color"]));

    // Optional options
    if (cylinderInf.exists("translate")) {
      Math::Vector3D translation = parseVector3D(cylinderInf["translate"]);
      newCylinderInf->translate(translation);
    }
    if (cylinderInf.exists("material")) {
      std::string materialName = parseString(cylinderInf["material"]);
      if (materialName == "reflective") {
        newCylinderInf->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "transparent") {
        newCylinderInf->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else if (materialName == "refractive") {
        newCylinderInf->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else {
        throw std::runtime_error("[ERROR] - Unknown material type.");
      }
    }
    sc.addShape(newCylinderInf);
  }
}

void Raytracer::ParserConfigFile::parseCones(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &conesSetting) {
  for (int i = 0; i < conesSetting.getLength(); i++) {
    const libconfig::Setting &cone = conesSetting[i];
    auto newCone = _factory.create<Raytracer::Cone>("cone");
    if (!newCone)
      throw ParseError("Failed to create cone object from factory.");
    if (!cone.exists("r"))
      throw ParseError(std::string("Cone radius not found at ") +
                       cone.getPath());
    if (!cone.exists("h"))
      throw ParseError(std::string("Cone height not found at ") +
                       cone.getPath());
    if (!cone.exists("color"))
      throw ParseError(std::string("Cone color not found at ") +
                       cone.getPath());

    newCone->setCenter(parsePoint3D(cone));
    if (cone.lookup("r").operator double() <= 0)
      throw ParseError(std::string("Cone radius must be positive at ") +
                       cone.getPath());
    newCone->setRadius(cone.lookup("r").operator double());
    newCone->setHeight(cone.lookup("h").operator double());
    newCone->setColor(parseColor(cone["color"]));

    // Optional options
    if (cone.exists("translate")) {
      Math::Vector3D translation = parseVector3D(cone["translate"]);
      newCone->translate(translation);
    }
    if (cone.exists("material")) {
      std::string materialName = parseString(cone["material"]);
      if (materialName == "reflective") {
        newCone->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "refractive") {
        newCone->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else if (materialName == "transparent") {
        newCone->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else {
        throw std::runtime_error("[ERROR] - Unknown material type.");
      }
    }
    sc.addShape(newCone);
  }
}

void Raytracer::ParserConfigFile::parseConesInf(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &conesInfSetting) {
  for (int i = 0; i < conesInfSetting.getLength(); i++) {
    const libconfig::Setting &coneInf = conesInfSetting[i];
    auto newConeInf = _factory.create<Raytracer::ConeInf>("coneInf");
    if (!newConeInf)
      throw ParseError("Failed to create coneInf object from factory.");
    if (!coneInf.exists("a"))
      throw ParseError(std::string("ConeInf angle not found at ") +
                       coneInf.getPath());
    if (!coneInf.exists("color"))
      throw ParseError(std::string("ConeInf color not found at ") +
                       coneInf.getPath());

    newConeInf->setCenter(parsePoint3D(coneInf));
    if (coneInf.lookup("a").operator double() <= 0)
      throw ParseError(std::string("Cone angle must be positive at ") +
                       coneInf.getPath());
    newConeInf->setAngle(coneInf.lookup("a").operator double());
    newConeInf->setColor(parseColor(coneInf["color"]));

    // Optional options
    if (coneInf.exists("translate")) {
      Math::Vector3D translation = parseVector3D(coneInf["translate"]);
      newConeInf->translate(translation);
    }
    if (coneInf.exists("material")) {
      std::string materialName = parseString(coneInf["material"]);
      if (materialName == "reflective") {
        newConeInf->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "transparent") {
        newConeInf->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else if (materialName == "refractive") {
        newConeInf->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else {
        throw std::runtime_error("[ERROR] - Unknown material type.");
      }
    }
    sc.addShape(newConeInf);
  }
}

void Raytracer::ParserConfigFile::parsePlanes(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &planesSettings) {
  for (int i = 0; i < planesSettings.getLength(); i++) {
    const libconfig::Setting &plane = planesSettings[i];
    auto newPlane = _factory.create<Raytracer::Plane>("plane");
    if (!newPlane)
      throw ParseError("Failed to create plane object from factory.");
    if (!plane.exists("normal"))
      throw ParseError(std::string("Plane normal not found at ") +
                       plane.getPath());
    if (!plane.exists("offset"))
      throw ParseError(std::string("Plane offset not found at ") +
                       plane.getPath());
    if (!plane.exists("color"))
      throw ParseError(std::string("Plane color not found at ") +
                       plane.getPath());

    Math::Vector3D normal = {0, 0, 0};
    std::string newNormal = plane.lookup("normal").operator std::string();
    if (newNormal == "X" || newNormal == "x")
      normal = {1, 0, 0};
    else if (newNormal == "Y" || newNormal == "y")
      normal = {0, 1, 0};
    else if (newNormal == "Z" || newNormal == "z")
      normal = {0, 0, 1};
    else
      throw ParseError(std::string("Plane normal must be X, Y, or Z at ") +
                       plane.getPath());
    newPlane->setNormal(normal);

    // Optional options
    if (plane.exists("translate")) {
      Math::Vector3D translation = parseVector3D(plane["translate"]);
      newPlane->translate(translation);
    }
    if (plane.exists("material")) {
      std::string materialName = parseString(plane["material"]);
      if (materialName == "reflective") {
        newPlane->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "refractive") {
        newPlane->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else if (materialName == "transparent") {
        newPlane->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else {
        throw std::runtime_error("[ERROR] - Unknown material type.");
      }
    }
    float center = plane.lookup("offset").operator double();
    Math::Point3D newCenter = {0, center, 0};
    newPlane->setCenter({0, center, 0});
    newPlane->setColor(parseColor(plane["color"]));
    sc.addShape(newPlane);
  }
}

void Raytracer::ParserConfigFile::parseObjects(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &objectsSetting) {
  for (int i = 0; i < objectsSetting.getLength(); i++) {
    const libconfig::Setting &object = objectsSetting[i];
    auto newObject = _factory.create<Raytracer::Object>("object");
    if (!newObject)
      throw ParseError("Failed to create object from factory.");
    if (!object.exists("obj_file"))
      throw ParseError(std::string("Object file not found at ") +
                       object.getPath());
    parseObj(object.lookup("obj_file").operator std::string(), *newObject);
    sc.addShape(newObject);
  }
}

void Raytracer::ParserConfigFile::parseTriangles(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &trianglesSetting) {
  for (int i = 0; i < trianglesSetting.getLength(); i++) {
    const libconfig::Setting &triangle = trianglesSetting[i];
    auto newTriangle = _factory.create<Raytracer::Triangle>("triangle");
    if (!newTriangle)
      throw ParseError("Failed to create triangle object from factory.");
    if (!triangle.exists("p1"))
      throw ParseError(std::string("Triangle p1 not found at ") +
                       triangle.getPath());
    if (!triangle.exists("p2"))
      throw ParseError(std::string("Triangle p2 not found at ") +
                       triangle.getPath());
    if (!triangle.exists("p3"))
      throw ParseError(std::string("Triangle p3 not found at ") +
                       triangle.getPath());
    if (!triangle.exists("color"))
      throw ParseError(std::string("Triangle color not found at ") +
                       triangle.getPath());

    newTriangle->setP1(parsePoint3D(triangle["p1"]));
    newTriangle->setP2(parsePoint3D(triangle["p2"]));
    newTriangle->setP3(parsePoint3D(triangle["p3"]));
    newTriangle->setColor(parseColor(triangle["color"]));

    // Optional options
    if (triangle.exists("translate")) {
      Math::Vector3D translation = parseVector3D(triangle["translate"]);
      newTriangle->translate(translation);
    }
    if (triangle.exists("material")) {
      std::string materialName = parseString(triangle["material"]);
      if (materialName == "reflective") {
        newTriangle->setMaterial(
            _factory.create<Raytracer::Reflections>("reflection"));
      } else if (materialName == "refractive") {
        newTriangle->setMaterial(
            _factory.create<Raytracer::Refractions>("refraction"));
      } else if (materialName == "transparent") {
        newTriangle->setMaterial(
            _factory.create<Raytracer::Transparency>("transparent"));
      } else {
        throw ParseError(std::string("[ERROR] - Unknown material type: ") +
                         materialName);
      }
    }
    sc.addShape(newTriangle);
  }
}

void Raytracer::ParserConfigFile::parsePrimitives(
    Raytracer::ShapeComposite &sc, const libconfig::Setting &root) {
  try {
    // SPHERES
    if (root.exists("primitives") && root["primitives"].exists("spheres")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "r", "color", "translate", "material"};
      checkSettings(root["primitives"]["spheres"], allowedSettings);
      parseSpheres(sc, root["primitives"]["spheres"]);
    }

    // CYLINDERS
    if (root.exists("primitives") && root["primitives"].exists("cylinders")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "r", "h", "color", "translate", "material"};
      checkSettings(root["primitives"]["cylinders"], allowedSettings);
      parseCylinders(sc, root["primitives"]["cylinders"]);
    }

    // CYLINDERSINF
    if (root.exists("primitives") &&
        root["primitives"].exists("cylindersInf")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "r", "color", "translate", "material"};
      checkSettings(root["primitives"]["cylindersInf"], allowedSettings);
      parseCylindersInf(sc, root["primitives"]["cylindersInf"]);
    }

    // CONES
    if (root.exists("primitives") && root["primitives"].exists("cones")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "r", "h", "color", "translate", "material"};
      checkSettings(root["primitives"]["cones"], allowedSettings);
      parseCones(sc, root["primitives"]["cones"]);
    }

    // CONESINF
    if (root.exists("primitives") && root["primitives"].exists("conesInf")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "a", "color", "translate", "material"};
      checkSettings(root["primitives"]["conesInf"], allowedSettings);
      parseConesInf(sc, root["primitives"]["conesInf"]);
    }

    // PLANES
    if (root.exists("primitives") && root["primitives"].exists("planes")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "normal", "offset", "color", "translate", "material"};
      checkSettings(root["primitives"]["planes"], allowedSettings);
      parsePlanes(sc, root["primitives"]["planes"]);
    }

    // OBJECTS
    if (root.exists("primitives") && root["primitives"].exists("objects")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "obj_file"};
      checkSettings(root["primitives"]["objects"], allowedSettings);
      parseObjects(sc, root["primitives"]["objects"]);
    }

    // TRIANGLES
    if (root.exists("primitives") && root["primitives"].exists("triangles")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "p1", "p2", "p3", "color", "translate", "material"};
      checkSettings(root["primitives"]["triangles"], allowedSettings);
      parseTriangles(sc, root["primitives"]["triangles"]);
    }
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw ParseError(std::string("Primitives config: ") + nfex.getPath() +
                     " not found or invalid.");
  } catch (const libconfig::SettingTypeException &stex) {
    throw ParseError(std::string("Primitives config: ") + stex.getPath() +
                     " has incorrect type.");
  }
}

void Raytracer::ParserConfigFile::parseAmbientLight(
    Raytracer::LightComposite &lc, const libconfig::Setting &ambientInfo) {
  const libconfig::Setting &colorInfo = ambientInfo["color"];
  auto newAmbient = _factory.create<AmbientLight>("ambient");
  if (newAmbient == nullptr)
    throw ParseError("Failed to create ambient light object from factory.");

  Math::Vector3D color = parseColor(colorInfo);
  double intensity = ambientInfo.lookup("intensity");
  if (intensity < 0 || intensity > 1)
    throw ParseError(
        std::string("Ambient light intensity out of range [0, 1] at ") +
        ambientInfo.getPath());

  newAmbient->setColor(color);
  newAmbient->setIntensity(intensity);
  newAmbient->setType("AmbientLight");
  lc.addLight(newAmbient);
}

void Raytracer::ParserConfigFile::parsePointLight(
    Raytracer::LightComposite &lc, const libconfig::Setting &pointInfo) {
  for (int i = 0; i < pointInfo.getLength(); i++) {
    const libconfig::Setting &point = pointInfo[i];
    const libconfig::Setting &colorInfo = point["color"];
    auto newPoint = _factory.create<Raytracer::PointLight>("point");
    if (newPoint == nullptr)
      throw ParseError("Failed to create point light object from factory.");
    Math::Point3D position = parsePoint3D(point);
    Math::Vector3D color = parseColor(colorInfo);
    double intensity = point.lookup("intensity");
    if (intensity < 0) {
      throw ParseError(
          "Failed to set intensity. Intensity must be over 0 but it was : " +
          std::to_string(intensity));
    }
    newPoint->setIntensity(intensity);
    newPoint->setPosition(position);
    newPoint->setColor(color);
    newPoint->setType("PointLight");
    lc.addLight(newPoint);
  }
}

void Raytracer::ParserConfigFile::parseDiffuseLight(
    Raytracer::LightComposite &lc, const libconfig::Setting &diffuseInfo) {
  double diffuseMultiplier = diffuseInfo;
  if (diffuseMultiplier < 0 || diffuseMultiplier > 1)
    throw ParseError(
        std::string("Diffuse light multiplier out of range [0, 1] at ") +
        diffuseInfo.getPath());

  lc.setDiffuse(diffuseMultiplier);
}

void Raytracer::ParserConfigFile::parseDirectionalLights(
    Raytracer::LightComposite &lc, const libconfig::Setting &lightsSetting) {
  for (int i = 0; i < lightsSetting.getLength(); i++) {
    const libconfig::Setting &directional = lightsSetting[i];
    auto newDirectional =
        _factory.create<Raytracer::DirectionalLight>("directional");
    if (newDirectional == nullptr)
      throw ParseError(
          "Failed to create directional light object from factory.");
    Math::Vector3D direction = parseVector3D(directional);

    newDirectional->setDirection(direction.normalize());
    newDirectional->setType("DirectionalLight");
    lc.addLight(newDirectional);
  }
}

void Raytracer::ParserConfigFile::parseLights(Raytracer::LightComposite &lc,
                                              const libconfig::Setting &root) {
  try {
    // AMBIENT
    if (root.exists("lights") && root["lights"].exists("ambient")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "intensity", "color"};
      checkSettings(root["lights"]["ambient"], allowedSettings);
      parseAmbientLight(lc, root["lights"]["ambient"]);
    }
    // POINT
    if (root.exists("lights") && root["lights"].exists("point")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "color", "x", "y", "z", "intensity"};
      checkSettings(root["lights"]["point"], allowedSettings);
      parsePointLight(lc, root["lights"]["point"]);
    }
    // DIFFUSE
    if (root.exists("lights") && root["lights"].exists("diffuse"))
      parseDiffuseLight(lc, root["lights"]["diffuse"]);
    // DIRECTIONALS
    if (root.exists("lights") && root["lights"].exists("directional")) {
      static const std::unordered_set<std::string> allowedSettings = {
          "x", "y", "z", "color"};
      checkSettings(root["lights"]["directional"], allowedSettings);
      parseDirectionalLights(lc, root["lights"]["directional"]);
    }
  } catch (const libconfig::SettingNotFoundException &nfex) {
    throw ParseError(std::string("Lights config: ") + nfex.getPath() +
                     " not found or invalid.");
  } catch (const libconfig::SettingTypeException &stex) {
    throw ParseError(std::string("Lights config: ") + stex.getPath() +
                     " has incorrect type.");
  }
}

void Raytracer::ParserConfigFile::checkSettings(
    const libconfig::Setting &settings,
    const std::unordered_set<std::string> &allowedSettings) const {
  if (settings.isList() || settings.isArray()) {
    for (int i = 0; i < settings.getLength(); i++) {
      const libconfig::Setting &object = settings[i];
      if (object.isGroup()) {
        for (int j = 0; j < object.getLength(); j++) {
          const libconfig::Setting &setting = object[j];
          std::string settingName = setting.getName();
          if (allowedSettings.find(settingName) == allowedSettings.end()) {
            throw ParseError(std::string("Unknown setting '") + settingName +
                             "' found in object configuration at " +
                             setting.getPath());
          }
        }
      }
    }
  } else if (settings.isGroup()) {
    for (int j = 0; j < settings.getLength(); j++) {
      const libconfig::Setting &setting = settings[j];
      std::string settingName = setting.getName();
      if (allowedSettings.find(settingName) == allowedSettings.end()) {
        throw ParseError(std::string("Unknown setting '") + settingName +
                         "' found in object configuration at " +
                         setting.getPath());
      }
    }
  }
}

void Raytracer::ParserConfigFile::parseScenes(ShapeComposite &sc,
                                              LightComposite &lc,
                                              const libconfig::Setting &root) {
  if (root.exists("scenes")) {
    static const std::unordered_set<std::string> allowedSettings = {
        "scenesPaths"};
    checkSettings(root["scenes"], allowedSettings);
    const libconfig::Setting &scenes = root["scenes"]["scenesPaths"];
    for (const auto &scene : scenes) {
      std::string path = scene.lookup("path");
      for (const auto &alreadyParsed : _fileAlreadyParse) {
        if (alreadyParsed == path) {
          throw ParseError("Import loop detected. File \"" + path +
                           "\" is already imported into the current scene.");
        }
      }
      _fileAlreadyParse.insert(path);
      ParserConfigFile parser(path, _plugins);
      parser._fileAlreadyParse = _fileAlreadyParse;
      parser.parseConfigFile(sc, lc);
    }
  }
}

void Raytracer::ParserConfigFile::parseInternal(
    ShapeComposite &sc, LightComposite &lc, const libconfig::Setting &root) {
  // PRIMITIVES
  try {
    parsePrimitives(sc, root);
  } catch (const RaytracerError &e) {
    throw;
  } catch (const libconfig::ConfigException &cfgex) {
    throw ParseError(
        std::string("General libconfig error during primitives parsing: ") +
        cfgex.what());
  }

  // LIGHTS
  try {
    parseLights(lc, root);
  } catch (const RaytracerError &e) {
    throw;
  } catch (const libconfig::ConfigException &cfgex) {
    throw ParseError(
        std::string("General libconfig error during lights parsing: ") +
        cfgex.what());
  }

  try {
    parseScenes(sc, lc, root);
  } catch (const RaytracerError &e) {
    throw;
  } catch (const libconfig::ConfigException &cfgex) {
    throw ParseError(
        std::string("General libconfig error during scenes parsing: ") +
        cfgex.what());
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
  } catch (const RaytracerError &e) {
    throw;
  } catch (const libconfig::ConfigException &cfgex) {
    throw ParseError(
        std::string("General libconfig error during camera parsing: ") +
        cfgex.what());
  }

  parseInternal(sc, lc, root);
}

void Raytracer::ParserConfigFile::parseConfigFile(ShapeComposite &sc,
                                                  LightComposite &lc) {
  const libconfig::Setting &root = _cfg.getRoot();
  _factory.initFactories(_plugins);

  parseInternal(sc, lc, root);
}
