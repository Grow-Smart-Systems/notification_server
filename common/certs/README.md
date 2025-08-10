# Каталог CA

## Общее

Скопируйте сюда файлы `ca.key` и `ca.crt`.

## Генерация центра сертификации (CA)

Сгенерируйте приватный ключ и публичный сертификат CA:
```bash
openssl genrsa -out ca.key 4096
openssl req -x509 -new -nodes -key ca.key -sha256 -days 3650 -out ca.crt -subj "/CN=GrowSmartCA"
```

