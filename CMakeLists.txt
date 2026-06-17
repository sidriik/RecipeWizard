cmake_minimum_required(VERSION 3.16)
project(RecipeWizard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# 1. Сначала ищем пакеты Qt
find_package(Qt6 REQUIRED COMPONENTS Quick Qml Sql Core Gui QuickControls2)

cmake_policy(SET CMP0071 NEW)
if(POLICY QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()
if(POLICY QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

# 2. Теперь настраиваем политики и стандартные параметры (теперь ошибки не будет)
if(POLICY QTP0001)
    cmake_policy(SET QTP0001 NEW)
endif()

# 3. Основной исполняемый файл
add_executable(appRecipeWizard src/main.cpp)

# 4. Находим картинки (RELATIVE важен для путей!)
file(GLOB_RECURSE IMAGE_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "qml/images/*.jpg"
    "qml/images/*.jpeg"
    "qml/images/*.png"
)

# 5. Описываем QML модуль
qt_add_qml_module(appRecipeWizard
    URI RecipeWizard
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/pages/HomePage.qml
        qml/pages/FavoritesPage.qml

        qml/pages/RecipeDetailsPage.qml
        qml/pages/AddRecipePage.qml
        qml/pages/RecipeCard.qml
        qml/pages/CookingOverlay.qml
        qml/pages/LaboratoryPage.qml
    SOURCES
        src/DatabaseManager.h src/DatabaseManager.cpp
        src/RecipeModel.h src/RecipeModel.cpp
        src/Recipe.h
        src/RecipeRepository.h src/RecipeRepository.cpp
        src/RecipeService.h src/RecipeService.cpp
    RESOURCES
        ${IMAGE_FILES}
)

# 6. Линковка библиотек
target_link_libraries(appRecipeWizard PRIVATE
    Qt6::Quick
    Qt6::Sql
    Qt6::Core
    Qt6::Gui
    Qt6::QuickControls2
)