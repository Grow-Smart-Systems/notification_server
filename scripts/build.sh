#!/bin/bash
set -e

# build folder
root_folder=${APP_FOLDER}
build_folder="$root_folder/build"

start_time=$(date +%s)
echo "[BUILD] Start build: $(date)"

mkdir -p $build_folder/protoc
find $root_folder/proto -name "*.proto" > $build_folder/protoc/proto_files.txt

xargs protoc --proto_path=$root_folder/proto --cpp_out=$build_folder/protoc --grpc_out=$build_folder/protoc \
    --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin < $build_folder/protoc/proto_files.txt

cd $root_folder
rm -rfv $build_folder/CMakeFiles

# Используем стандартный Makefile и подробный лог
cmake -B $build_folder -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build $build_folder -j$(nproc)

end_time=$(date +%s)
duration=$((end_time - start_time))

echo "[BUILD] Project built successfully in ${duration} seconds!"
echo "[BUILD] Build artifacts are located in: $build_folder"