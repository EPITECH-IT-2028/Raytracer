#include <gtest/gtest.h>
#include <filesystem>
#include "ParserConfigFile.hpp"
#include "exceptions/RaytracerException.hpp"

class ParserConfigFileTest : public ::testing::Test {
  protected:
    void SetUp() override {
      for (const auto &entry :
           std::filesystem::directory_iterator("./plugins")) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
          _plugins.push_back(entry.path().string());
        }
      }
    }

    void TearDown() override {
    }

    libconfig::Config _cfg;
    std::string _cfgFile;
    std::vector<std::string> _plugins;
};

TEST_F(ParserConfigFileTest, InvalidCameraPositionFields) {
  _cfgFile = "tests/camera/invalidFields/position/invalidX.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_EQ(camera.origin.x, 0);

  _cfgFile = "tests/camera/invalidFields/position/invalidY.cfg";
  Raytracer::ParserConfigFile parser2(_cfgFile, _plugins);
  Raytracer::Camera camera2;
  Raytracer::ShapeComposite sc2;
  Raytracer::LightComposite lc2;

  EXPECT_EQ(camera2.origin.y, 0);

  _cfgFile = "tests/camera/invalidFields/position/invalidZ.cfg";
  Raytracer::ParserConfigFile parser3(_cfgFile, _plugins);
  Raytracer::Camera camera3;
  Raytracer::ShapeComposite sc3;
  Raytracer::LightComposite lc3;

  EXPECT_EQ(camera3.origin.z, 0);
}
