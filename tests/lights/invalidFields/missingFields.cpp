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

TEST_F(ParserConfigFileTest, MissingLightAmbient) {
  _cfgFile = "tests/lights/missingFields/missingAmbient.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_EQ(lc.getLights().size(), 0);
}

TEST_F(ParserConfigFileTest, MissingLightDiffuse) {
  _cfgFile = "tests/lights/missingFields/missingDiffuse.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_EQ(lc.getLights().size(), 0);
  EXPECT_EQ(lc.getIntensity(), 0);
}

TEST_F(ParserConfigFileTest, MissingLightDirectional) {
  _cfgFile = "tests/lights/missingFields/missingDirectional.cfg";
  Raytracer::ParserConfigFile parser(_cfgFile, _plugins);
  Raytracer::Camera camera;
  Raytracer::ShapeComposite sc;
  Raytracer::LightComposite lc;

  EXPECT_EQ(lc.getLights().size(), 0);
}
