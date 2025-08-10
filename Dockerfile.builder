FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

# Установка зависимостей для сборки gRPC и Protobuf
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc

WORKDIR /app


### Копируем исходный код приложения
COPY CMakeLists.txt /app/CMakeLists.txt
# health_service
COPY ./health_service /app/health_service
# # ca_service
# COPY ./ca_service /app/ca_service
# # back_gateway
# COPY ./back_gateway /app/back_gateway
# Копируем общие файлы
COPY ./common /app/common
COPY ./proto /app/proto
# Копируем RapidJSON
COPY ./third_party/rapidjson /app/third_party/rapidjson
###

### Генерируем gRPC/Protobuf-код для C++
RUN rm -rf build
RUN mkdir -p build/protoc
RUN find proto -name "*.proto" > build/protoc/proto_files.txt
RUN xargs protoc --proto_path=proto --cpp_out=build/protoc --grpc_out=build/protoc \
    --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin < build/protoc/proto_files.txt
###

### Сборка проекта
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build
###

### Копируем артефакты сборки
# health_service
RUN mkdir -p /app/build_artifacts/health_service && \
    cp -vf /app/build/health_service/health_service /app/build_artifacts/health_service/health_service
# # ca_service
# RUN mkdir -p /app/build_artifacts/ca_service && \
#     cp -vf /app/build/ca_service/ca_service /app/build_artifacts/ca_service/ca_service
# # back_gateway
# RUN mkdir -p /app/build_artifacts/back_gateway && \
#     cp -vf /app/build/back_gateway/back_gateway /app/build_artifacts/back_gateway/back_gateway
###

CMD ["sh", "-c", "echo '✅ Build completed. Run the container to copy artifacts in /build/'"]
