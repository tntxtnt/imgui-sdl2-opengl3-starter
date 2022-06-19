cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(imgui-sdl2-opengl3-starter CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} WIN32
  src/main.cpp
  src/app.h
  src/app.cpp
)

find_package(fmt REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
if (UNIX AND NOT APPLE)
  set(OPENGL_LIB GL)
else()
  set(OPENGL_LIB opengl32)
endif()
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    fmt::fmt-header-only
    SDL2::SDL2 SDL2::SDL2main
    imgui::imgui
    ${OPENGL_LIB}
)
if (MINGW)
  target_link_options(${PROJECT_NAME} PRIVATE "-lmingw32")
endif()

file(COPY "res/fonts/wideosevka-extendedmedium.ttf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/fonts")
file(COPY "res/fonts/wideosevka-LICENSE.md" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res/fonts")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
