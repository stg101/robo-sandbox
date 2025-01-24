cmake_minimum_required(VERSION 3.12) 
project(RoboSandboxProject VERSION 1.0.0)
project(RoboSandboxProject C CXX)


execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE SIMAVR_OBJ_DIRNAME)
string(STRIP ${SIMAVR_OBJ_DIRNAME} SIMAVR_OBJ_DIRNAME)


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/box2d)
add_subdirectory(${CMAKE_SOURCE_DIR}/testbed/tests/diff_robot_arduino)
add_subdirectory(${CMAKE_SOURCE_DIR}/testbed/tests/diff_robot_avr)

include_directories (
    ${CMAKE_SOURCE_DIR}/testbed
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/bodies
    ${CMAKE_SOURCE_DIR}/src/actuators
    ${CMAKE_SOURCE_DIR}/src/sensors
    ${CMAKE_SOURCE_DIR}/src/robots
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/simavr/simavr/cores
    ${CMAKE_SOURCE_DIR}/simavr/examples/parts
    ${CMAKE_SOURCE_DIR}/simavr/simavr/sim
    ${CMAKE_SOURCE_DIR}/box2d/include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)


file(
    GLOB HELLO_EMU_SRCS
    "${CMAKE_SOURCE_DIR}/simavr/examples/parts/button.c"
    "${CMAKE_SOURCE_DIR}/simavr/simavr/sim/sim_elf.c"
    "${CMAKE_SOURCE_DIR}/testbed/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/bodies/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/actuators/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/sensors/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/robots/*.cpp"
    "${CMAKE_SOURCE_DIR}/testbed/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/testbed/tests/diff_robot_arduino/*.cpp"
    "${CMAKE_SOURCE_DIR}/testbed/tests/diff_robot_avr/*.cpp"
    "${CMAKE_SOURCE_DIR}/testbed/tests/env_test/*.cpp"
)

add_custom_target(
    simavr_lib
    COMMAND make libsimavr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/simavr/simavr
)

add_executable(RoboSandboxProject ${HELLO_EMU_SRCS})
add_dependencies(RoboSandboxProject box2d simavr_lib)


target_link_libraries(
    RoboSandboxProject
    elf
    box2d
    glfw
    imgui
    sajson
    glad
    ${CMAKE_SOURCE_DIR}/simavr/simavr/obj-${SIMAVR_OBJ_DIRNAME}/libsimavr.a
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

set_target_properties(RoboSandboxProject PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


# Copy font files, etc
add_custom_command(
        TARGET RoboSandboxProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/testbed/data/
                ${CMAKE_CURRENT_BINARY_DIR}/data/)