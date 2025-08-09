FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

WORKDIR /app

# Копируем CMake-файлы (для ускорения пересборки при неизменных зависимостях)
COPY CMakeLists.txt ./
COPY iot_gateway/CMakeLists.txt ./iot_gateway/
COPY ca_service/CMakeLists.txt ./ca_service/
COPY back_gateway/CMakeLists.txt ./back_gateway/
COPY common/ ./common/

# Копируем исходные файлы
COPY iot_gateway/src/ ./iot_gateway/src/
COPY ca_service/src/ ./ca_service/src/
COPY back_gateway/src/ ./back_gateway/src/

# Копируем файлы протоколов
COPY proto/ ./proto/

# Сборка проекта (без установки userver)
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build

EXPOSE 8080

# ▶️ Команда по умолчанию
CMD ["sh", "-c", "echo '✅ Build completed. Binaries available at /app/build/'"]
