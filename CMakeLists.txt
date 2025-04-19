cmake_minimum_required(VERSION 3.14)

project("SimpleP3D" LANGUAGES C)

include(FetchContent)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_LIBRARY_TYPE "Link glfw static or dynamic" STATIC)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        3.3.8
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message("Fetching glfw")
FetchContent_MakeAvailable(glfw)

option(glew-cmake_BUILD_SHARED "Build the shared glew library" OFF)
option(glew-cmake_BUILD_STATIC "Build the static glew library" ON)
option(ONLY_LIBS "Do not build executables" OFF)
FetchContent_Declare(
  glew-cmake
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message("Fetching glew-cmake")
FetchContent_MakeAvailable(glew-cmake)

add_executable(${PROJECT_NAME} "main.c" "gltext.h")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw libglew_static)