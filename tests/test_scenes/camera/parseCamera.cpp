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
  _cfgFile = "tests/test_scenes/camera/parseCamera.cfg";
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
