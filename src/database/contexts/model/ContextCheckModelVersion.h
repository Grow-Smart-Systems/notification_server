#ifndef CONTEXT_CHECK_MODEL_VERSION_H
#define CONTEXT_CHECK_MODEL_VERSION_H

#include "BaseContextModelVersion.h"

namespace Database::Contexts 
{
    //! @brief Контекст для работы с таблицей model_version
    class ContextCheckModelVersion final : public BaseContextModelVersion
    {
    public:
        //! @brief Номер версии схемы
        using BaseContextModelVersion::version;

        ContextCheckModelVersion() = default;

        bool CheckRequiredFields() const override
        {
            // Проверяем, что версия схемы задана
            return version > 0;
        }
    };
} // namespace Database::Contexts

#endif // CONTEXT_CHECK_MODEL_VERSION_H
