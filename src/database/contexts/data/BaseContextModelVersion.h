#ifndef BASE_CONTEXT_MODEL_VERSION_H
#define BASE_CONTEXT_MODEL_VERSION_H

#include <QString>

namespace Database::Contexts {
    //! @brief Контекст для работы с таблицей model_version
    struct BaseContextModelVersion {
        //! @brief Уникальный идентификатор записи
        int id{0};
        //! @brief Номер версии схемы
        int version{0};
        //! @brief Дата обновления (ISO 8601)
        QString updatedAt;

        BaseContextModelVersion() = default;
        BaseContextModelVersion(const BaseContextModelVersion& other)
            : id(other.id), version(other.version), updatedAt(other.updatedAt) {}
        BaseContextModelVersion(BaseContextModelVersion&& other) noexcept
            : id(other.id), version(other.version), updatedAt(std::move(other.updatedAt)) {}
        BaseContextModelVersion& operator=(const BaseContextModelVersion& other) {
            if (this != &other) {
                id = other.id;
                version = other.version;
                updatedAt = other.updatedAt;
            }
            return *this;
        }
        BaseContextModelVersion& operator=(BaseContextModelVersion&& other) noexcept {
            if (this != &other) {
                id = other.id;
                version = other.version;
                updatedAt = std::move(other.updatedAt);
            }
            return *this;
        }
    };
} // namespace Database::Contexts

#endif // BASE_CONTEXT_MODEL_VERSION_H
