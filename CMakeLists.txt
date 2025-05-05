cmake_minimum_required(VERSION 3.14)

project(Raytracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_COMMANDS_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_compile_options(-Wall -Wextra -Werror -std=c++20)

add_executable(raytracer
  src/main.cpp
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
  src/shapes/ShapeComposite.cpp
  src/lights/DirectionalLight.cpp
  src/lights/LightComposite.cpp
  src/ParserConfigFile.cpp
  src/Factory.cpp
  src/Rectangle.cpp
  src/Camera.cpp
  src/Scene.cpp
  src/Renderer.cpp
)

add_library(sphere SHARED
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
  src/shapes/Sphere.cpp
)

add_library(plane SHARED
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
  src/shapes/Plane.cpp
)

add_library(cylinder SHARED
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
  src/shapes/Cylinder.cpp
)

add_library(directional SHARED
  src/lights/DirectionalLight.cpp
  src/maths/Vector3D.cpp
  src/maths/Point3D.cpp
)

set_target_properties(sphere PROPERTIES PREFIX "")

set_target_properties(plane PROPERTIES PREFIX "")

set_target_properties(cylinder PROPERTIES PREFIX "")

set_target_properties(directional PROPERTIES PREFIX "")

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/maths
    ${CMAKE_SOURCE_DIR}/src/shapes
    ${CMAKE_SOURCE_DIR}/src/lights
)

if(APPLE)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(CMAKE_PREFIX_PATH "/usr/local/opt/sfml@2" ${CMAKE_PREFIX_PATH})
  else()
    message(FATAL_ERROR "Unknown macOS architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
  find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics config++)

  set_target_properties(sphere PROPERTIES SUFFIX ".so")
  set_target_properties(plane PROPERTIES SUFFIX ".so")
  set_target_properties(cylinder PROPERTIES SUFFIX ".so")
  set_target_properties(directional PROPERTIES SUFFIX ".so")
elseif(UNIX AND NOT APPLE)
  find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics config++)
endif()

target_include_directories(sphere PUBLIC src)
target_include_directories(sphere PUBLIC src/maths)

target_include_directories(plane PUBLIC src)
target_include_directories(plane PUBLIC src/maths)

target_include_directories(cylinder PUBLIC src)
target_include_directories(cylinder PUBLIC src/maths)

target_include_directories(directional PUBLIC src)
target_include_directories(directional PUBLIC src/maths)

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/sphere.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/plane.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/cylinder.so ${PROJECT_SOURCE_DIR}/plugins/
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/directional.so ${PROJECT_SOURCE_DIR}/plugins/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
