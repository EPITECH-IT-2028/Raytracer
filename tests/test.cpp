#include <gtest/gtest.h>
#include "ParserConfigFile.hpp"
#include "exceptions/RaytracerException.hpp"

class ParserConfigFileTest : public ::testing::Test {
  protected:
    void SetUp() override {
      // _cfgFile = "tests/test_scenes/camera_setup.cfg";
      _plugins = {
          "plugins/ambient.so",     "plugins/cone.so",  "plugins/cylinder.so",
          "plugins/directional.so", "plugins/plane.so", "plugins/reflection.so",
          "plugins/sphere.so",
      };
    }

    void TearDown() override {
    }

    libconfig::Config _cfg;
    std::string _cfgFile;
    std::vector<std::string> _plugins;
};

TEST_F(ParserConfigFileTest, TestParseCamera) {
  _cfgFile = "tests/test_scenes/parseCamera.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  try {
    parser.parseConfigFile(camera, sc, lc);
  } catch (const Raytracer::ParseError &e) {
    FAIL() << "Failed to parse camera configuration: " << e.what();
  } catch (const Raytracer::ConfigError &e) {
    FAIL() << "Failed to parse camera configuration (ConfigError): "
           << e.what();
  }

  EXPECT_EQ(camera.getWidth(), 400);
  EXPECT_EQ(camera.getHeight(), 400);
  EXPECT_DOUBLE_EQ(camera.getFieldOfView(), 45.0);
}

TEST_F(ParserConfigFileTest, TestParsePrimitives) {
  _cfgFile = "tests/test_scenes/parsePrimitives.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  try {
    parser.parseConfigFile(camera, sc, lc);
  } catch (const Raytracer::ParseError &e) {
    FAIL() << "Failed to parse primitives configuration: " << e.what();
  } catch (const Raytracer::ConfigError &e) {
    FAIL() << "Failed to parse primitives configuration (ConfigError): "
           << e.what();
  }

  EXPECT_EQ(sc.getShapes().size(), 4);
}

TEST_F(ParserConfigFileTest, TestParseLights) {
  _cfgFile = "tests/test_scenes/parseLights.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  try {
    parser.parseConfigFile(camera, sc, lc);
  } catch (const Raytracer::ParseError &e) {
    FAIL() << "Failed to parse lights configuration: " << e.what();
  } catch (const Raytracer::ConfigError &e) {
    FAIL() << "Failed to parse lights configuration (ConfigError): "
           << e.what();
  }

  EXPECT_EQ(lc.getLights().size(), 2);
}
