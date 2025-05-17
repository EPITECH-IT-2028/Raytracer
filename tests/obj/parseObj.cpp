#include <gtest/gtest.h>
#include <filesystem>
#include "ParserConfigFile.hpp"

class ParserConfigFileTest : public ::testing::Test {
  protected:
    void SetUp() override {
      for (const auto& entry :
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

TEST_F(ParserConfigFileTest, ParseValidObjFileWithMaterials) {
    Raytracer::ParserConfigFile parser("tests/obj/dummy.cfg", _plugins);
    Raytracer::Object object;
    std::string objPath = "tests/obj/cube.obj";

    ASSERT_NO_THROW(parser.parseObj(objPath, object));

    ASSERT_EQ(object.getVertices().size(), 8);
    EXPECT_DOUBLE_EQ(object.getVertices()[0].x, 0.0);
    EXPECT_DOUBLE_EQ(object.getVertices()[0].y, 1.0);
    EXPECT_DOUBLE_EQ(object.getVertices()[0].z, 0.0);
    EXPECT_DOUBLE_EQ(object.getVertices()[7].x, 1.0);
    EXPECT_DOUBLE_EQ(object.getVertices()[7].y, 2.0);
    EXPECT_DOUBLE_EQ(object.getVertices()[7].z, 1.0);

    ASSERT_EQ(object.getNormals().size(), 6);
    EXPECT_DOUBLE_EQ(object.getNormals()[0].x, 0.0);
    EXPECT_DOUBLE_EQ(object.getNormals()[0].y, 0.0);
    EXPECT_DOUBLE_EQ(object.getNormals()[0].z, 1.0);
    EXPECT_DOUBLE_EQ(object.getNormals()[1].x, 0.0);
    EXPECT_DOUBLE_EQ(object.getNormals()[1].y, 0.0);
    EXPECT_DOUBLE_EQ(object.getNormals()[1].z, -1.0);

    ASSERT_EQ(object.getFaces().size(), 12);

    ASSERT_EQ(object.getFaces()[0].vertex.size(), 3);
    EXPECT_EQ(object.getFaces()[0].vertex[0], 0);
    EXPECT_EQ(object.getFaces()[0].vertex[1], 6);
    EXPECT_EQ(object.getFaces()[0].vertex[2], 4);

    ASSERT_EQ(object.getFaces()[0].normal.size(), 3);
    EXPECT_EQ(object.getFaces()[0].normal[0], 1);
    EXPECT_EQ(object.getFaces()[0].normal[1], 1);
    EXPECT_EQ(object.getFaces()[0].normal[2], 1);
    EXPECT_EQ(object.getFaces()[0].material_name, "red");

    ASSERT_EQ(object.getFaces()[10].vertex.size(), 3);
    EXPECT_EQ(object.getFaces()[10].vertex[0], 1);
    EXPECT_EQ(object.getFaces()[10].vertex[1], 5);
    EXPECT_EQ(object.getFaces()[10].vertex[2], 7);

    ASSERT_EQ(object.getFaces()[10].normal.size(), 3);
    EXPECT_EQ(object.getFaces()[10].normal[0], 0);
    EXPECT_EQ(object.getFaces()[10].normal[1], 0);
    EXPECT_EQ(object.getFaces()[10].normal[2], 0);
    EXPECT_EQ(object.getFaces()[10].material_name, "red");

    ASSERT_FALSE(object.getMaterials().empty());

    ASSERT_NE(object.getMaterials().find("red"), object.getMaterials().end());
    const auto& redMaterial = object.getMaterials().at("red");
    EXPECT_NEAR(redMaterial.ambient.x, 0.4449, 1e-5);
    EXPECT_NEAR(redMaterial.ambient.y, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.ambient.z, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.diffuse.x, 0.7714, 1e-5);
    EXPECT_NEAR(redMaterial.diffuse.y, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.diffuse.z, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.specular.x, 0.8857, 1e-5);
    EXPECT_NEAR(redMaterial.specular.y, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.specular.z, 0.0, 1e-5);
    EXPECT_NEAR(redMaterial.shininess, 136.4300, 1e-5);
    EXPECT_EQ(redMaterial.illumination, 2);
    EXPECT_NEAR(redMaterial.transparency, 1.0, 1e-5);

    ASSERT_NE(object.getMaterials().find("white"), object.getMaterials().end());
    const auto& whiteMaterial = object.getMaterials().at("white");
    EXPECT_NEAR(whiteMaterial.ambient.x, 0.4000, 1e-5);
    EXPECT_NEAR(whiteMaterial.diffuse.x, 1.0000, 1e-5);
    EXPECT_NEAR(whiteMaterial.specular.x, 0.3000, 1e-5);
    EXPECT_NEAR(whiteMaterial.shininess, 60.0000, 1e-5);
    EXPECT_EQ(whiteMaterial.illumination, 2);
    EXPECT_NEAR(whiteMaterial.transparency, 1.0, 1e-5);
}

TEST_F(ParserConfigFileTest, ParseObjFileNotFound) {
    Raytracer::ParserConfigFile parser("tests/obj/dummy.cfg", _plugins);
    Raytracer::Object object;
    std::string objPath = "tests/obj/non_existent_cube.obj";

    EXPECT_THROW(parser.parseObj(objPath, object), std::runtime_error);
}
