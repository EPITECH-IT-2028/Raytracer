#pragma once

#include <tuple>
#include <unordered_map>
#include <vector>
#include "AShape.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace Raytracer {

  class Object : public AShape {
    public:

      struct Mtl {
        Math::Vector3D ambient;
        Math::Vector3D diffuse;
        Math::Vector3D specular;
        double shininess = 10.0;
        double transparency = 1.0;
        int illumination = 2;
      };

      struct Face {
        std::vector<int> vertex;
        std::vector<int> normal;
        std::string material_name;
      };

      Object() 
          : _obj_file{}, _vertices{}, _faces{}, _materials{} {}

      ~Object() = default;

      std::tuple<double, Math::Vector3D, const Raytracer::IShape *> hits(
          const Raytracer::Ray &ray) const override;

      void setObjFile(const std::string &obj_file) {_obj_file = obj_file;}
      void setMaterials(const std::unordered_map<std::string, Mtl> &materials) {
          _materials = materials;
      }
      void setVertices(const std::vector<Math::Point3D> &vertices) {
          _vertices = vertices;
      }
      void setNormals(const std::vector<Math::Vector3D> &normals) {
          _normals = normals;
      }
      void setFaces(const std::vector<Face> &faces) {
          _faces = faces;
      }

      const std::string &getObjFile() const {return _obj_file;}

      Math::Vector3D getNormal(
          const Math::Point3D &hitPoint) const override {(void)hitPoint; return Math::Vector3D(0, 0, 0);};

    private:
      std::string _obj_file;
      std::vector<Math::Point3D> _vertices;
      std::vector<Math::Vector3D> _normals;
      std::vector<Face> _faces;
      std::unordered_map<std::string, Mtl> _materials;
  };

}  // namespace Raytracer
