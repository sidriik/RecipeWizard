#pragma once

#include <QString>


struct Recipe
{
    int id = -1;

    QString title;
    QString ingredients;
    QString tags;
    QString imageUrl;

    int calories = 0;

    double proteins = 0.0;
    double fats = 0.0;
    double carbs = 0.0;

    int servings = 1;

    QString instructions;
};