Положите сюда SSL сертификат (`server.crt`) и приватный ключ (`server.key`).
Для теста можно сгенерировать так:

```bash
openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt -days 365 -nodes -subj "/CN=localhost"
```