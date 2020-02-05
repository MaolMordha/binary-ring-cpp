cmake_minimum_required(VERSION 3.0.0)
project(BinaryRing CXX)

set_property(TARGET ${BINARY_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${BINARY_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_INCLUDE_PATH} include)

set (SOURCE_DIR source)
set (SOURCE
    ${SOURCE_DIR}/Core.cpp
    ${SOURCE_DIR}/Exceptions/Exception.cpp
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Particle.cpp
    ${SOURCE_DIR}/ParticleCluster.cpp
)

set (BINARY_NAME binary_ring)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(Threads)

add_executable(${BINARY_NAME} ${SOURCE})

target_link_libraries(${BINARY_NAME} sfml-graphics ${CMAKE_THREAD_LIBS_INIT})
