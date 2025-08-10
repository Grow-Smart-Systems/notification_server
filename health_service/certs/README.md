# Быстрая инструкция по генерации ключа и сертификата для health_service

1. Сгененируйте или скопируйте свой CA в каталог /common/certs/
2. Выполните команду в каталоге health_service/certs (текущий каталог):

```sh
openssl req -new -newkey rsa:4096 -nodes -keyout server.key -out server.csr -subj "/CN=health_service"
openssl x509 -req -in server.csr -CA ../../common/certs/ca.crt -CAkey ../common/certs/ca.key -CAcreateserial -out server.crt -days 3650 -sha256
```

В результате появятся файлы `server.key` и `server.crt`, готовые для использования в конфиге сервиса.
