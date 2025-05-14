cmake_minimum_required(VERSION 3.14)

project(Raytracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_COMMANDS_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -Wextra -Werror -std=c++20)

add_executable(raytracer
  src/main.cpp
  src/shapes/ShapeComposite.cpp
  src/lights/LightComposite.cpp
  src/ParserConfigFile.cpp
  src/Factory.cpp
  src/Rectangle.cpp
  src/Camera.cpp
  src/Scene.cpp
  src/Renderer.cpp
)

add_library(math_objects OBJECT
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
)

set_target_properties(math_objects PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
)

add_library(sphere SHARED
  src/shapes/Sphere.cpp
)

add_library(plane SHARED
  src/shapes/Plane.cpp
)

add_library(cylinder SHARED
  src/shapes/Cylinder.cpp
)

add_library(cylinderInf SHARED
  src/shapes/CylinderInf.cpp
)

add_library(cone SHARED
  src/shapes/Cone.cpp
)

add_library(coneInf SHARED
  src/shapes/ConeInf.cpp
)

add_library(object SHARED
  src/shapes/Object.cpp
)

add_library(triangle SHARED
  src/shapes/Triangle.cpp
)

add_library(directional SHARED
  src/lights/DirectionalLight.cpp
)

add_library(ambient SHARED
  src/lights/AmbientLight.cpp
)

add_library(point SHARED
  src/lights/PointLight.cpp
)


add_library(reflection SHARED
  src/materials/Reflections.cpp
)

set_target_properties(sphere PROPERTIES PREFIX "")

set_target_properties(plane PROPERTIES PREFIX "")

set_target_properties(cylinder PROPERTIES PREFIX "")

set_target_properties(cylinderInf PROPERTIES PREFIX "")

set_target_properties(cone PROPERTIES PREFIX "")

set_target_properties(coneInf PROPERTIES PREFIX "")

set_target_properties(object PROPERTIES PREFIX "")

set_target_properties(triangle PROPERTIES PREFIX "")

set_target_properties(directional PROPERTIES PREFIX "")

set_target_properties(ambient PROPERTIES PREFIX "")

set_target_properties(point PROPERTIES PREFIX "")

set_target_properties(reflection PROPERTIES PREFIX "")

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/maths
    ${CMAKE_SOURCE_DIR}/src/shapes
    ${CMAKE_SOURCE_DIR}/src/lights
    ${CMAKE_SOURCE_DIR}/src/materials
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sphere PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(sphere PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
  ${CMAKE_SOURCE_DIR}/src/materials
)

target_link_libraries(plane PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(plane PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
  ${CMAKE_SOURCE_DIR}/src/materials
)

target_link_libraries(cylinder PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(cylinder PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
  ${CMAKE_SOURCE_DIR}/src/materials
)

target_link_libraries(cylinderInf PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(cylinderInf PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
  ${CMAKE_SOURCE_DIR}/src/materials
)

target_link_libraries(cone PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(cone PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(coneInf PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(coneInf PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(object PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(object PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(triangle PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(triangle PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(directional PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(directional PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(ambient PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(ambient PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(point PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(point PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/maths
        ${CMAKE_SOURCE_DIR}/src/shapes
        ${CMAKE_SOURCE_DIR}/src/lights
)

target_link_libraries(reflection PRIVATE $<TARGET_OBJECTS:math_objects>)

target_include_directories(reflection PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/maths
  ${CMAKE_SOURCE_DIR}/src/shapes
  ${CMAKE_SOURCE_DIR}/src/lights
)

if(APPLE)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
    target_link_libraries(raytracer config++)
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
    find_path(LIBCONFIGPP_INCLUDE_DIR NAMES libconfig.h++ HINTS /usr/local/opt/libconfig/include)
    find_library(LIBCONFIGPP_LIBRARY NAMES config++ HINTS /usr/local/opt/libconfig/lib)
    target_include_directories(raytracer PRIVATE ${LIBCONFIGPP_INCLUDE_DIR})
    target_link_libraries(raytracer ${LIBCONFIGPP_LIBRARY} $<TARGET_OBJECTS:math_objects>)
  else()
    message(FATAL_ERROR "Unknown macOS architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
  find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics $<TARGET_OBJECTS:math_objects>)

  set_target_properties(sphere PROPERTIES SUFFIX ".so")
  set_target_properties(plane PROPERTIES SUFFIX ".so")
  set_target_properties(cylinder PROPERTIES SUFFIX ".so")
  set_target_properties(cylinderInf PROPERTIES SUFFIX ".so")
  set_target_properties(cone PROPERTIES SUFFIX ".so")
  set_target_properties(coneInf PROPERTIES SUFFIX ".so")
  set_target_properties(object PROPERTIES SUFFIX ".so")
  set_target_properties(triangle PROPERTIES SUFFIX ".so")
  set_target_properties(directional PROPERTIES SUFFIX ".so")
  set_target_properties(ambient PROPERTIES SUFFIX ".so")
  set_target_properties(point PROPERTIES SUFFIX ".so")
  set_target_properties(reflection PROPERTIES SUFFIX ".so")
elseif(UNIX AND NOT APPLE)
  find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics config++ $<TARGET_OBJECTS:math_objects>)
endif()

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/sphere.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/plane.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/cylinder.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/cylinderInf.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/cone.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/coneInf.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/object.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/triangle.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/directional.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/ambient.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/point.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/reflection.so ${PROJECT_SOURCE_DIR}/plugins/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
