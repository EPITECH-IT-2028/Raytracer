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

TEST_F(ParserConfigFileTest, InvalidPlanesFields) {
  _cfgFile = "tests/primitives/planes/invalidFields/invalidNormal.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);

  _cfgFile = "tests/primitives/planes/invalidFields/invalidOffset.cfg";
  Raytracer::ParserConfigFile parser2(_cfgFile, _plugins);
  Raytracer::Camera camera2;
  Raytracer::ShapeComposite sc2;
  Raytracer::LightComposite lc2;

  EXPECT_THROW(parser2.parseConfigFile(camera2, sc2, lc2), Raytracer::ParseError);
}


