cmake_minimum_required(VERSION 3.16)

project(RecipeWizard VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Qml Sql Core Gui QuickControls2)
qt_standard_project_setup(REQUIRES 6.11)
if(COMMAND qt_policy)
    qt_policy(SET QTP0001 NEW)
    qt_policy(SET QTP0004 NEW)
endif()
add_library(RecipeCore STATIC
    src/DatabaseManager.cpp
    src/DatabaseManager.h

    src/RecipeRepository.cpp
    src/RecipeRepository.h

    src/RecipeService.cpp
    src/RecipeService.h

    src/RecipeModel.cpp
    src/RecipeModel.h

    src/Recipe.h
    src/RecipeException.h
)
target_include_directories(RecipeCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(RecipeCore PUBLIC Qt6::Core Qt6::Sql)

file(GLOB_RECURSE IMAGE_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "qml/images/*.jpg"
    "qml/images/*.jpeg"
    "qml/images/*.png"
)

qt_add_executable(appRecipeWizard src/main.cpp)

qt_add_qml_module(appRecipeWizard
    URI RecipeWizard
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/pages/HomePage.qml
        qml/pages/FavoritesPage.qml
        qml/pages/RecipeDetailsPage.qml
        qml/pages/AddRecipePage.qml
        qml/pages/RecipeCard.qml
        qml/pages/CookingOverlay.qml
        qml/pages/LaboratoryPage.qml
    RESOURCES
        ${IMAGE_FILES}
)

target_link_libraries(appRecipeWizard PRIVATE
    RecipeCore
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
)

enable_testing()
add_subdirectory(tests)
