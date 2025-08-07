# Dockerfile для контейнера сборки
FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

# Установка недостающих зависимостей
# Пробуем разные варианты date библиотеки
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    python3-pip \
    python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Установка библиотеки date из исходников
RUN git clone https://github.com/HowardHinnant/date.git /tmp/date && \
    cd /tmp/date && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr && \
    make -j$(nproc) && \
    make install && \
    rm -rf /tmp/date

# Рабочая директория
WORKDIR /app

# Копируем только файлы конфигурации для кэширования зависимостей
COPY CMakeLists.txt ./
COPY iot_gateway/CMakeLists.txt ./iot_gateway/
COPY ca_service/CMakeLists.txt ./ca_service/

# Копирование исходных файлов
COPY iot_gateway/src/ ./iot_gateway/src/
COPY ca_service/src/ ./ca_service/src/

# Настройка и сборка проекта
RUN mkdir -p build && cmake -B build -DCMAKE_BUILD_TYPE=Debug && make -C build

# Команда по умолчанию - запуск сборки
CMD ["sh", "-c", "echo 'Build completed. Binaries available at /app/build/'"]
