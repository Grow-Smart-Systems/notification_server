cmake_minimum_required(VERSION 3.14)
project(server  
    VERSION 1.0
    DESCRIPTION "Notification server using userver framework"
    LANGUAGES CXX)

# Отключаем модуль ClickHouse для userver
set(USERVER_FEATURE_CLICKHOUSE OFF CACHE BOOL "Disable ClickHouse module in userver" FORCE)

# Add Protobuf dependency
find_package(Protobuf REQUIRED)

# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()

add_subdirectory(iot_gateway)   
add_subdirectory(ca_service)