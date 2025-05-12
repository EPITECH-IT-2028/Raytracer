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

TEST_F(ParserConfigFileTest, TestParseInvalidConfig) {
  _cfgFile = "tests/test_scenes/invalidConfig.cfg";

  EXPECT_THROW(Raytracer::ParserConfigFile parser(_cfgFile, _plugins);, Raytracer::ConfigError);
}

TEST_F(ParserConfigFileTest, TestParseMissingField) {
  _cfgFile = "tests/test_scenes/missingField.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseInvalidField) {
  _cfgFile = "tests/test_scenes/invalidField.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseIncompleteCameraConfig1) {
  _cfgFile = "tests/test_scenes/camera/missingResolution.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseIncompleteCameraConfig2) {
  _cfgFile = "tests/test_scenes/camera/missingPosition.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseIncompleteCameraConfig3) {
  _cfgFile = "tests/test_scenes/camera/missingRotation.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseIncompleteCameraConfig4) {
  _cfgFile = "tests/test_scenes/camera/missingFOV.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);
}

TEST_F(ParserConfigFileTest, TestParseIncompleteCameraConfig5) {
  _cfgFile = "tests/test_scenes/camera/missingResolutionWidth.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_EQ(camera.getWidth(), 0);

  _cfgFile = "tests/test_scenes/camera/missingResolutionHeight.cfg";
  Raytracer::ParserConfigFile parser2(_cfgFile, _plugins);
  Raytracer::Camera camera2;
  Raytracer::ShapeComposite sc2;
  Raytracer::LightComposite lc2;

  EXPECT_EQ(camera2.getHeight(), 0);
}
