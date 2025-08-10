# # Найти protobuf
# find_package(Protobuf REQUIRED)

project(health_service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем все исходники
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Найти все proto-файлы в ${CMAKE_SOURCE_DIR}/proto
file(GLOB PROTO_FILES "${CMAKE_SOURCE_DIR}/proto/*.proto")

# Используем сгенерированные файлы из build/protoc
file(GLOB PROTO_GEN_SRCS
    "${CMAKE_SOURCE_DIR}/build/protoc/*.pb.cc"
    "${CMAKE_SOURCE_DIR}/build/protoc/*.grpc.pb.cc"
)
file(GLOB PROTO_GEN_HDRS
    "${CMAKE_SOURCE_DIR}/build/protoc/*.pb.h"
    "${CMAKE_SOURCE_DIR}/build/protoc/*.grpc.pb.h"
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/health_service)
add_executable(health_service
    ${SOURCES}
    ${HEADERS}
    ${PROTO_GEN_SRCS}
    ${PROTO_GEN_HDRS}
)
target_link_libraries(health_service
    userver::core
    protobuf::libprotobuf
    grpc++
    grpc
    protobuf
)
target_include_directories(health_service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/proto
        ${CMAKE_CURRENT_BINARY_DIR}/proto
        ${CMAKE_SOURCE_DIR}/third_party/rapidjson/include
)
