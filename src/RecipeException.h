#pragma once

#include <QString>
#include <stdexcept>

/**
 * @brief Исключение для ошибок при работе с рецептами и базой данных.
 */
class RecipeException : public std::runtime_error
{
public:
    /**
     * @brief Создать исключение с текстом ошибки.
     * @param message Описание ошибки.
     */
    explicit RecipeException(const QString &message)
        : std::runtime_error(message.toStdString())
    {
    }
};
