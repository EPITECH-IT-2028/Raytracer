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

TEST_F(ParserConfigFileTest, InvalidLightsAmbientFields) {
  _cfgFile = "tests/lights/invalidFields/ambient/invalidIntensity.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/ambient/negativeIntensity.cfg";
  Raytracer::ParserConfigFile parser1(_cfgFile, _plugins);
  Raytracer::Camera camera1;
  Raytracer::ShapeComposite sc1;
  Raytracer::LightComposite lc1;

  EXPECT_THROW(parser1.parseConfigFile(camera1, sc1, lc1), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/ambient/invalidColorR.cfg";
  Raytracer::ParserConfigFile parser2(_cfgFile, _plugins);
  Raytracer::Camera camera2;
  Raytracer::ShapeComposite sc2;
  Raytracer::LightComposite lc2;

  EXPECT_THROW(parser2.parseConfigFile(camera2, sc2, lc2), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/ambient/invalidColorG.cfg";
  Raytracer::ParserConfigFile parser3(_cfgFile, _plugins);
  Raytracer::Camera camera3;
  Raytracer::ShapeComposite sc3;
  Raytracer::LightComposite lc3;

  EXPECT_THROW(parser3.parseConfigFile(camera3, sc3, lc3), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/ambient/invalidColorB.cfg";
  Raytracer::ParserConfigFile parser4(_cfgFile, _plugins);
  Raytracer::Camera camera4;
  Raytracer::ShapeComposite sc4;
  Raytracer::LightComposite lc4;

  EXPECT_THROW(parser4.parseConfigFile(camera4, sc4, lc4), Raytracer::ParseError);
}
