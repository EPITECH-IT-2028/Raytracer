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

TEST_F(ParserConfigFileTest, TestParsePrimitives) {
  _cfgFile = "tests/primitives/parsePrimitives.cfg";
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
