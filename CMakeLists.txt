cmake_minimum_required(VERSION 3.12) 
project(MyProject VERSION 1.0.0)
project(MyProject C CXX)


execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE SIMAVR_OBJ_DIRNAME)
string(STRIP ${SIMAVR_OBJ_DIRNAME} SIMAVR_OBJ_DIRNAME)


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/box2d)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

include_directories (
    ${CMAKE_SOURCE_DIR}/src
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/simavr/simavr/cores
    ${CMAKE_SOURCE_DIR}/simavr/examples/parts
    ${CMAKE_SOURCE_DIR}/simavr/simavr/sim
    ${CMAKE_SOURCE_DIR}/box2d/include
    /usr/local/include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

file(
    GLOB HELLO_EMU_SRCS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/tests/simavr_hello/*.cpp"
)

# file(
#     GLOB HELLO_EMU_SRCS
#     "${CMAKE_SOURCE_DIR}/src/cli.cpp"
#     "${CMAKE_SOURCE_DIR}/src/*.c"
#     "${CMAKE_SOURCE_DIR}/simavr/examples/parts/ssd1306_virt.c"
# )

add_custom_target(
    simavr_lib
    COMMAND make libsimavr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/simavr/simavr
)

add_executable(MyProject ${HELLO_EMU_SRCS})
add_dependencies(MyProject box2d simavr_lib)

target_link_libraries(
    MyProject
    box2d
    glfw
    imgui
    sajson
    glad
    ${CMAKE_SOURCE_DIR}/simavr/simavr/obj-${SIMAVR_OBJ_DIRNAME}/libsimavr.a
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
