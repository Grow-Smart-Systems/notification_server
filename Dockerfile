FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

# Установим зависимости
RUN apt-get update && \
    apt-get install -y wget gdebi-core

# Скачаем deb-пакет userver (замени ссылку на актуальную версию с GitHub Releases)
ENV USERVER_DEB_URL=https://github.com/userver-framework/userver/releases/download/v2.10/ubuntu22.04-libuserver-all-dev_2.10_amd64.deb

RUN wget $USERVER_DEB_URL -O /tmp/userver.deb && \
    gdebi --non-interactive /tmp/userver.deb && \
    rm /tmp/userver.deb

# Проверка установки (опционально)
RUN userver --version || true

# Копируем исходники проекта
WORKDIR /app
COPY . /app


# Сборка приложения с автоматическим поиском userverConfig.cmake
RUN rm -rf build && mkdir build && \
    cd build && \
    cmake ..
RUN cd build && make -j$(nproc)

RUN apt install -y tree
RUN tree /app/build/ca_service

RUN chmod +x /app/build/ca_service && \
    chmod +x /app/build/iot_gateway


# Открываем порт для сервиса
EXPOSE 8080
