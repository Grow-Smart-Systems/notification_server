
FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

# Установка зависимостей для сборки gRPC и Protobuf
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc

WORKDIR /app

# копируем исходный код приложения
COPY CMakeLists.txt /app/CMakeLists.txt
COPY ./health_service /app/health_service
COPY ./ca_service /app/ca_service
COPY ./back_gateway /app/back_gateway

# копируем общие файлы
COPY ./common /app/common
COPY ./proto /app/proto

# Копируем RapidJSON
COPY ./third_party/rapidjson /app/third_party/rapidjson

RUN rm -rf build
RUN mkdir -p build/protoc

# RUN mkdir -p build/health_service
# COPY health_service/.env /app/build/health_service/.env

# Генерируем gRPC/Protobuf-код для C++
RUN find proto -name "*.proto" > build/protoc/proto_files.txt
RUN xargs protoc --proto_path=proto --cpp_out=build/protoc --grpc_out=build/protoc \
    --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin < build/protoc/proto_files.txt

# Сборка проекта (без установки userver)
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build

RUN mkdir -p /app/build_artifacts/health_service && \
    cp -vf /app/build/health_service/health_service /app/build_artifacts/health_service/health_service

EXPOSE 8080

# ▶️ Команда по умолчанию
CMD ["sh", "-c", "echo '✅ Build completed. Binaries available at /app/build/'"]
