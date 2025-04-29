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
  src/shapes/ShapeComposite.cpp
  src/Rectangle.cpp
  src/Camera.cpp
  src/Scene.cpp
)

target_include_directories(raytracer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/maths
    ${CMAKE_SOURCE_DIR}/src/shapes
)

if(APPLE)
  set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
  find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
  target_link_libraries(raytracer sfml-system sfml-window sfml-graphics)
endif()

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
