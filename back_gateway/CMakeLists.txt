
cmake_minimum_required(VERSION 3.12)
project(back_gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем все исходники
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")



# Найти все proto-файлы в ${CMAKE_SOURCE_DIR}/proto
file(GLOB PROTO_FILES "${CMAKE_SOURCE_DIR}/proto/*.proto")
if(PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
else()
    set(PROTO_SRCS "")
    set(PROTO_HDRS "")
endif()

add_executable(back_gateway
    ${SOURCES}
    ${HEADERS}
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)
target_link_libraries(back_gateway
    userver::core
    protobuf::libprotobuf
)
        target_include_directories(back_gateway
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_SOURCE_DIR}/proto
        )
