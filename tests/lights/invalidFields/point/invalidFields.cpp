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

TEST_F(ParserConfigFileTest, InvalidLightsPointFields) {
  _cfgFile = "tests/lights/invalidFields/point/invalidX.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidY.cfg";
  Raytracer::ParserConfigFile parser2(_cfgFile, _plugins);
  Raytracer::Camera camera2;
  Raytracer::ShapeComposite sc2;
  Raytracer::LightComposite lc2;

  EXPECT_THROW(parser2.parseConfigFile(camera2, sc2, lc2), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidZ.cfg";
  Raytracer::ParserConfigFile parser3(_cfgFile, _plugins);
  Raytracer::Camera camera3;
  Raytracer::ShapeComposite sc3;
  Raytracer::LightComposite lc3;

  EXPECT_THROW(parser3.parseConfigFile(camera3, sc3, lc3), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidColorR.cfg";
  Raytracer::ParserConfigFile parser5(_cfgFile, _plugins);
  Raytracer::Camera camera5;
  Raytracer::ShapeComposite sc5;
  Raytracer::LightComposite lc5;

  EXPECT_THROW(parser5.parseConfigFile(camera5, sc5, lc5), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidColorG.cfg";
  Raytracer::ParserConfigFile parser6(_cfgFile, _plugins);
  Raytracer::Camera camera6;
  Raytracer::ShapeComposite sc6;
  Raytracer::LightComposite lc6;

  EXPECT_THROW(parser6.parseConfigFile(camera6, sc6, lc6), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidColorB.cfg";
  Raytracer::ParserConfigFile parser7(_cfgFile, _plugins);
  Raytracer::Camera camera7;
  Raytracer::ShapeComposite sc7;
  Raytracer::LightComposite lc7;

  EXPECT_THROW(parser7.parseConfigFile(camera7, sc7, lc7), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/point/invalidIntensity.cfg";
  Raytracer::ParserConfigFile parser8(_cfgFile, _plugins);
  Raytracer::Camera camera8;
  Raytracer::ShapeComposite sc8;
  Raytracer::LightComposite lc8;

  EXPECT_THROW(parser8.parseConfigFile(camera8, sc8, lc8), Raytracer::ParseError);
}
