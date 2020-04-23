cmake_minimum_required(VERSION 3.15)

project(locked-up)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

find_package(SFML 2.5 COMPONENTS window graphics network audio REQUIRED)

include_directories(include ${SFML_INCLUDE})

add_executable(locked-up ${SOURCES} ${HEADERS})
target_link_libraries(locked-up sfml-window sfml-graphics sfml-network sfml-audio)

set_property(TARGET locked-up PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
