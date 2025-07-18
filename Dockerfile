FROM ubuntu:22.04

# Установим базовые утилиты и зависимости
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    libssl-dev \
    pkg-config \
    qtbase5-dev \
    libpq-dev \
    libcurl4-openssl-dev \
    wget

# Установка userver
RUN git clone https://github.com/userver-framework/userver.git /userver \
 && cd /userver \
 && mkdir build && cd build \
 && cmake .. \
 && make -j$(nproc) \
 && make install

# Копируем исходники проекта
WORKDIR /app
COPY . /app

# Сборка приложения
RUN mkdir build && cd build \
 && cmake .. \
 && make -j$(nproc)

# Открываем порт для сервиса
EXPOSE 8080

# Запуск сервиса (пример для userver)
CMD ["./build/userver_service", "--config", "config/config.yaml"]