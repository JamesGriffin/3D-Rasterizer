cmake_minimum_required(VERSION 3.5)
project(rasterizer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -funroll-loops -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(src)

set(SOURCE_FILES
    src/display.cpp
    src/main.cpp
    src/matrix4.cpp
    src/mesh.cpp
    src/renderer.cpp
    src/starfield.cpp
    src/triangle.cpp
    src/vector4.cpp
    src/vertex.cpp
)

add_executable(rasterizer ${SOURCE_FILES})

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)

target_link_libraries(rasterizer
    ${SDL2_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
)

file(COPY obj/ DESTINATION "${PROJECT_BINARY_DIR}/obj")
file(COPY fonts/ DESTINATION "${PROJECT_BINARY_DIR}/fonts")

add_custom_target(run
    COMMAND rasterizer
    DEPENDS rasterizer
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
