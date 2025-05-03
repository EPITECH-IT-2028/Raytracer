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
  src/shapes/Sphere.cpp
  src/shapes/Plane.cpp
  src/shapes/Cylinder.cpp
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
    target_link_libraries(raytracer config++)
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
    find_path(LIBCONFIGPP_INCLUDE_DIR NAMES libconfig.h++ HINTS /usr/local/opt/libconfig/include)
    find_library(LIBCONFIGPP_LIBRARY NAMES config++ HINTS /usr/local/opt/libconfig/lib)
    target_include_directories(raytracer PRIVATE ${LIBCONFIGPP_INCLUDE_DIR})
    target_link_libraries(raytracer ${LIBCONFIGPP_LIBRARY})
  else()
    message(FATAL_ERROR "Unknown macOS architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()
  find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics)
elseif(UNIX AND NOT APPLE)
  find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics config++)
endif()

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
