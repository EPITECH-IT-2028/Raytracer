#pragma once

#include "AShape.hpp"
namespace raytracer {
class Torus : public Raytracer::AShape {
public:
  Torus() = default;
  ~Torus() = default;

  std::size_t getRadius() {return _radius;}
  std::size_t getDistance() {return _distance;}

  void setRadius(std::size_t radius) { _radius = radius;}
  void setDistance(std::size_t distance) { _distance = distance;}

private:
  std::size_t _radius;
  std::size_t _distance;
};
}
