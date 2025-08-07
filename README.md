# Notification Server

Микросервисная архитектура для системы уведомлений с использованием userver framework.

## 📋 Документация

- **[Быстрый старт](QUICKSTART.md)** - основные команды для работы с проектом
- **[API документация](API.md)** - описание REST API и схемы данных
- **[Развертывание в продакшене](DEPLOYMENT.md)** - инструкции для production

## 🏗 Архитектура

Проект состоит из следующих компонентов:

- **IoT Gateway** - шлюз для IoT устройств (порт 8080)
- **CA Service** - сервис центра сертификации (порт 8081)
- **PostgreSQL** - база данных (порт 5432)
- **Redis** - кэш и брокер сообщений (порт 6379)

## Требования

- Docker 20.10+
- Docker Compose 2.0+
- Git

## Быстрый старт

### 1. Клонирование репозитория

```bash
git clone https://github.com/Grow-Smart-Systems/notification_server.git
cd notification_server
```

### 2. Сборка и запуск

```bash
# Сборка всех образов
docker-compose build

# Запуск всех сервисов в фоновом режиме
docker-compose up -d

# Проверка статуса сервисов
docker-compose ps
```

### 3. Проверка работы

```bash
# Просмотр логов всех сервисов
docker-compose logs

# Просмотр логов конкретного сервиса
docker-compose logs iot_gateway
docker-compose logs ca_service

# Проверка доступности сервисов
curl http://localhost:8080  # IoT Gateway
curl http://localhost:8081  # CA Service
```

## Команды управления

### Остановка сервисов

```bash
# Остановка всех сервисов
docker-compose down

# Остановка с удалением volumes
docker-compose down -v
```

### Пересборка

```bash
# Пересборка конкретного сервиса
docker-compose build iot_gateway

# Полная пересборка без кэша
docker-compose build --no-cache

# Принудительная пересборка и перезапуск
docker-compose up --build -d
```

### Мониторинг

```bash
# Просмотр ресурсов
docker-compose top

# Просмотр логов в реальном времени
docker-compose logs -f

# Просмотр логов конкретного сервиса
docker-compose logs -f iot_gateway
```

## Разработка

### Структура проекта

```
notification_server/
├── docker-compose.yaml     # Конфигурация Docker Compose
├── Dockerfile.builder      # Dockerfile для сборки C++ приложений
├── CMakeLists.txt          # Главный файл CMake
├── iot_gateway/            # IoT Gateway сервис
│   ├── CMakeLists.txt
│   ├── config/
│   └── src/
│       └── main.cpp
├── ca_service/             # CA Service сервис
│   ├── CMakeLists.txt
│   ├── config/
│   └── src/
│       └── main.cpp
├── common/                 # Общие компоненты
├── proto/                  # Protocol Buffers схемы
└── scripts/               # Скрипты для развертывания
```

### Оптимизация сборки

Проект использует многослойное Docker кэширование для оптимизации времени сборки:

- **Первая сборка**: ~25 секунд (установка всех зависимостей)
- **Последующие сборки**: ~1 секунда (благодаря кэшированию)

Кэширование работает на уровне:
1. Системных пакетов (apt)
2. Внешних библиотек (date library)
3. userver зависимостей
4. CMake конфигурации
5. Исходного кода

### Добавление нового сервиса

1. Создайте директорию для сервиса:
```bash
mkdir new_service
mkdir new_service/src
mkdir new_service/config
```

2. Создайте CMakeLists.txt для сервиса:
```cmake
cmake_minimum_required(VERSION 3.14)
project(new_service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

if(SOURCES)
    add_executable(new_service ${SOURCES} ${HEADERS})
    target_link_libraries(new_service userver-core)
    target_include_directories(new_service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
```

3. Добавьте сервис в docker-compose.yaml:
```yaml
  new_service:
    build:
      context: .
      dockerfile: Dockerfile.builder
    command: ["/app/build/new_service/new_service"]
    depends_on:
      - postgres
      - redis
    environment:
      - TZ=Europe/Moscow
```

4. Добавьте в корневой CMakeLists.txt:
```cmake
add_subdirectory(new_service)
```

## Конфигурация

### Переменные окружения

- `TZ` - часовой пояс (по умолчанию: Europe/Moscow)
- `POSTGRES_URL` - URL подключения к PostgreSQL
- `REDIS_URL` - URL подключения к Redis

### Порты

- `8080` - IoT Gateway
- `8081` - CA Service  
- `5432` - PostgreSQL
- `6379` - Redis

### Volumes

- `./iot_gateway/config:/app/config` - конфигурация IoT Gateway
- `./ca_service/config:/app/config` - конфигурация CA Service

## Troubleshooting

### Проблемы сборки

1. **Ошибка "nothing starts, build outputs nothing"**:
   ```bash
   # Очистите Docker кэш
   docker system prune -a
   docker-compose build --no-cache
   ```

2. **Ошибка CMake cache compatibility**:
   ```bash
   # Удалите локальную папку build
   rm -rf build/
   docker-compose build --no-cache
   ```

3. **Ошибка "libdate-dev not found"**:
   - Библиотека date устанавливается автоматически из исходников в Dockerfile.builder

### Проблемы запуска

1. **Контейнер сразу останавливается**:
   ```bash
   # Проверьте логи
   docker-compose logs [service_name]
   
   # Запустите в интерактивном режиме
   docker-compose run --rm iot_gateway bash
   ```

2. **Порты заняты**:
   ```bash
   # Найдите процессы, использующие порты
   netstat -tlnp | grep :8080
   
   # Измените порты в docker-compose.yaml
   ports:
     - "8082:8080"  # внешний:внутренний
   ```

### Очистка

```bash
# Остановка и удаление всех контейнеров
docker-compose down -v

# Удаление неиспользуемых образов
docker image prune -a

# Полная очистка Docker
docker system prune -a --volumes
```

## Лицензия

[Укажите лицензию проекта]

## Контакты

- GitHub: [Grow-Smart-Systems](https://github.com/Grow-Smart-Systems)
- Проект: [notification_server](https://github.com/Grow-Smart-Systems/notification_server)