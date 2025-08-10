cmake_minimum_required(VERSION 3.12)
project(iot_gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Рекурсивно собрать все .cpp и .hpp файлы из текущего каталога и вложенных
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")


file(GLOB PROTO_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/proto/*.proto")
if(PROTO_FILES)
    protobuf_generate(
        LANGUAGE cpp
        PROTOS ${PROTO_FILES}
        OUT_VAR PROTO_GEN_SRCS
    )
else()
    set(PROTO_GEN_SRCS "")
endif()

if(SOURCES)
    add_executable(iot_gateway
        ${SOURCES}
        ${HEADERS}
        ${PROTO_GEN_SRCS}
    )
    target_link_libraries(iot_gateway
        userver-core
        protobuf::libprotobuf
    )
    target_include_directories(iot_gateway
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/proto
    )
endif()