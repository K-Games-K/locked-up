cmake_minimum_required(VERSION 3.15)

project(locked-up)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE PROTO_FILES proto/*.proto)

find_package(SFML 2.5 COMPONENTS graphics network audio REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_executable(locked-up ${SOURCE} ${HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(locked-up sfml-graphics sfml-network sfml-audio ${Protobuf_LIBRARIES})

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/deploy)
install(TARGETS locked-up RUNTIME DESTINATION . CONFIGURATIONS Release)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION . CONFIGURATIONS Release)
