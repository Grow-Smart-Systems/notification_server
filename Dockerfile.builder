FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

# Установка зависимостей для сборки gRPC и Protobuf
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc

### Формируем переменные окружения для сборки
ARG APP_FOLDER=/app
ARG BUILD_TYPE=Release
#
ENV APP_FOLDER=${APP_FOLDER}
ENV BUILD_TYPE=${BUILD_TYPE}
WORKDIR ${APP_FOLDER}
###

### Копируем исходный код приложения
COPY CMakeLists.txt ${APP_FOLDER}/CMakeLists.txt
# health_service
COPY ./health_service ${APP_FOLDER}/health_service
# # ca_service
COPY ./ca_service ${APP_FOLDER}/ca_service
# # back_gateway
COPY ./back_gateway ${APP_FOLDER}/back_gateway
# Копируем общие файлы
COPY ./common ${APP_FOLDER}/common
COPY ./proto ${APP_FOLDER}/proto
COPY ./scripts ${APP_FOLDER}/scripts
# Копируем RapidJSON
COPY ./third_party/rapidjson ${APP_FOLDER}/third_party/rapidjson
###

### Проект готов к сборке.
