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

TEST_F(ParserConfigFileTest, InvalidLightAmbient) {
  _cfgFile = "tests/lights/invalidFields/invalidDiffuse.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_THROW(parser.parseConfigFile(camera, sc, lc), Raytracer::ParseError);

  _cfgFile = "tests/lights/invalidFields/negativeDiffuse.cfg";
  Raytracer::ParserConfigFile parser1(_cfgFile, _plugins);
  Raytracer::Camera camera1;
  Raytracer::ShapeComposite sc1;
  Raytracer::LightComposite lc1;

  EXPECT_THROW(parser1.parseConfigFile(camera1, sc1, lc1), Raytracer::ParseError);
}
