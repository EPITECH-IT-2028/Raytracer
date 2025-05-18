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

  /**
   * @brief Represents a 3D object loaded from an OBJ file.
   *
   * This class inherits from AShape and stores geometry (vertices, faces, normals)
   * and material information loaded from an OBJ file and its associated MTL file.
   * It provides methods for ray intersection testing and retrieving object data.
   */
  class Object : public AShape {
    public:

      /**
       * @brief Structure to hold material properties.
       */
      struct Mtl {
        Math::Vector3D ambient;     ///< Ambient color component.
        Math::Vector3D diffuse;     ///< Diffuse color component.
        Math::Vector3D specular;    ///< Specular color component.
        double shininess = 10.0;    ///< Shininess exponent for specular highlights.
        double transparency = 1.0;  ///< Transparency value (0.0 opaque, 1.0 fully transparent).
        int illumination = 2;       ///< Illumination model.
      };

      /**
       * @brief Structure to represent a face (triangle) of the object.
       */
      struct Face {
        std::vector<int> vertex;      ///< Indices of the vertices forming the face.
        std::vector<int> normal;      ///< Indices of the normals for each vertex.
        std::string material_name;  ///< Name of the material applied to this face.
      };

      /**
       * @brief Default constructor for Object.
       * Initializes an empty object.
       */
      Object() 
          : _obj_file{}, _vertices{}, _faces{}, _materials{} {}

      /**
       * @brief Default destructor for Object.
       */
      ~Object() = default;

      /**
       * @brief Calculates the intersection of a ray with the object's faces.
       * @param ray The ray to test for intersection.
       * @return A tuple containing the distance to the hit point, the color of the hit face's material, and a pointer to this object if hit, otherwise a very large distance and null pointer.
       */
      std::tuple<double, Math::Vector3D, const Raytracer::IShape *> hits(
          const Raytracer::Ray &ray) const override;

      /**
       * @brief Sets the path to the OBJ file.
       * @param obj_file The OBJ file path.
       */
      void setObjFile(const std::string &obj_file) {_obj_file = obj_file;}
      /**
       * @brief Sets the materials for the object.
       * @param materials An unordered_map of material names to Mtl structs.
       */
      void setMaterials(const std::unordered_map<std::string, Mtl> &materials) {
          _materials = materials;
      }
      /**
       * @brief Sets the vertices of the object.
       * @param vertices A vector of Point3D representing the vertices.
       */
      void setVertices(const std::vector<Math::Point3D> &vertices) {
          _vertices = vertices;
      }
      /**
       * @brief Sets the normals of the object.
       * @param normals A vector of Vector3D representing the normals.
       */
      void setNormals(const std::vector<Math::Vector3D> &normals) {
          _normals = normals;
      }
      /**
       * @brief Sets the faces of the object.
       * @param faces A vector of Face structs representing the faces.
       */
      void setFaces(const std::vector<Face> &faces) {
          _faces = faces;
      }

      /**
       * @brief Gets the path to the OBJ file.
       * @return const std::string& A const reference to the OBJ file path.
       */
      const std::string &getObjFile() const {return _obj_file;}

      /**
       * @brief Gets the vertices of the object.
       * @return const std::vector<Math::Point3D>& A const reference to the vector of vertices.
       */
      const std::vector<Math::Point3D>& getVertices() const { return _vertices; }
      /**
       * @brief Gets the normals of the object.
       * @return const std::vector<Math::Vector3D>& A const reference to the vector of normals.
       */
      const std::vector<Math::Vector3D>& getNormals() const { return _normals; }
      /**
       * @brief Gets the faces of the object.
       * @return const std::vector<Face>& A const reference to the vector of faces.
       */
      const std::vector<Face>& getFaces() const { return _faces; }
      /**
       * @brief Gets the materials of the object.
       * @return const std::unordered_map<std::string, Mtl>& A const reference to the map of materials.
       */
      const std::unordered_map<std::string, Mtl>& getMaterials() const { return _materials; }

      /**
       * @brief Gets the normal vector at a given point on the object's surface.
       * @note This implementation is a placeholder and returns a zero vector.
       *       A proper implementation would require finding the specific face hit
       *       and interpolating vertex normals or using face normals.
       * @param hitPoint The point on the object's surface.
       * @return Math::Vector3D The normal vector (currently a zero vector).
       */
      Math::Vector3D getNormal(
          const Math::Point3D &hitPoint) const override {(void)hitPoint; return Math::Vector3D(0, 0, 0);};

    private:
      std::string _obj_file; ///< Path to the OBJ file.
      std::vector<Math::Point3D> _vertices; ///< Vector of vertices.
      std::vector<Math::Vector3D> _normals; ///< Vector of normals.
      std::vector<Face> _faces; ///< Vector of faces.
      std::unordered_map<std::string, Mtl> _materials; ///< Map of materials.
  };

}  // namespace Raytracer
