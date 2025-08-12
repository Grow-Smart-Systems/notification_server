# Server

Микросервисная архитектура для системы уведомлений с использованием userver framework.

## 🏗 Архитектура

Компоненты:

- **IoT Gateway** — шлюз для IoT устройств (порт 8080)
- **CA Service** — центр сертификации (порт 8081)
- **Health Service** — проверка состояния сервисов (порт 5000)
- **PostgreSQL** — база данных (порт 5432)
- **Redis** — кэш и брокер сообщений (порт 6379)

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

#### 2.1. Сборка артефактов (builder)
Сбор выполняет контейнер `userver-latest`, копируя бинарники в локальный каталог `build/` (смонтирован в контейнер).

```bash
docker compose up --build userver-latest
```

После завершения контейнер остановится, а артефакты будут лежать в `./build/`.

#### 2.2. Запуск сервисов
Пример для health_service:

```bash
docker compose up -d health_service
```

Можно запускать несколько сервисов (при их добавлении):

```bash
docker compose up -d health_service iot_gateway ca_service
```

#### 2.3. Общий цикл разработки
1. Внести изменения в исходный код
2. Пересобрать артефакты: `docker compose up --build userver-latest`
3. Перезапустить/запустить нужные сервисы: `docker compose up -d health_service`

#### 2.4. Важно
- Контейнер `userver-latest` используется только для сборки.
- При изменении кода всегда повторяйте сборку.
- Артефакты не живут внутри рантайм-контейнера — они примонтированы из хоста.

### 3. Проверка работы

```bash
# Логи всех сервисов
docker compose logs

# Логи конкретного сервиса
docker compose logs health_service

# Проверка HTTP/GRPC доступности (если применимо)
curl http://localhost:5000 || true
```

### 4. Команды управления

```bash
# Остановка всех сервисов
docker compose down

# Остановка с удалением volumes
docker compose down -v

# Пересборка конкретного сервиса (если будет отдельный build context)
docker compose build health_service

# Полная пересборка без кэша
docker compose build --no-cache

# Принудительная пересборка и запуск
docker compose up --build -d health_service

# Логи в реальном времени
docker compose logs -f health_service

# Просмотр процессов
docker compose top
```

## Конфигурация

### Переменные окружения (пример для health_service)

- `TZ` — часовой пояс (например Europe/Moscow)
- `HEALTH_SERVICE_SERVICES_JSON_PATH` — путь к JSON со списком сервисов внутри контейнера (напр. `/app/config/services.json`)
- `HEALTH_SERVICE_REQUEST_INTERVAL` — интервал между опросами (секунды)
- `HEALTH_SERVICE_LOG_FILE_PATH` — путь к лог-файлу (напр. `/app/health_service.log`)

### Порты

- `8080` — IoT Gateway
- `8081` — CA Service
- `5000` — Health Service
- `5432` — PostgreSQL
- `6379` — Redis

### Volumes (из docker-compose.yaml)

- `./health_service/config:/app/config` — конфиги health_service
- `./health_service/certs:/app/certs` — сертификаты сервиса
- `./common/certs:/app/common/certs` — общий CA
- `./build/health_service:/app/` — собранные бинарники (артефакты)

## Разработка

### Структура проекта

```
notification_server/
├── docker-compose.yaml
├── Dockerfile.builder
├── CMakeLists.txt
├── health_service/
├── iot_gateway/
├── ca_service/
├── common/
├── proto/
└── scripts/
```

### Оптимизация сборки
- Используется кэширование слоёв Docker.
- Пересобирайте только после изменений исходников.

## Troubleshooting

```bash
# Очистка кэша и пересборка
docker system prune -a
docker compose build --no-cache

# Логи
docker compose logs health_service

# Интерактивная отладка
docker compose run --rm health_service bash
```

## Лицензия

[Укажите лицензию проекта]

## Контакты

- GitHub: [Grow-Smart-Systems](https://github.com/Grow-Smart-Systems)
- Проект: [notification_server](https://github.com/Grow-Smart-Systems/notification_server)