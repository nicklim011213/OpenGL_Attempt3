cmake_minimum_required(VERSION 3.15)
project(OpenGl_Attempt3)

include_directories(${CMAKE_SOURCE_DIR}/Rendering)
include_directories(${CMAKE_SOURCE_DIR}/Settings)
include_directories(${CMAKE_SOURCE_DIR}/Utilities)
include_directories(${CMAKE_SOURCE_DIR}/External_Includes)

find_package(assimp CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

add_executable(Engine GraphicsEngine_Attempt3.cpp Rendering/Renderable.cpp Rendering/Texture.cpp Rendering/Shader.cpp)
target_link_libraries(Engine PRIVATE assimp::assimp)
target_link_libraries(Engine PRIVATE GLEW::GLEW)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE glm::glm)
target_link_libraries(Engine PRIVATE Boost::filesystem Boost::system)

target_compile_definitions(Engine PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")