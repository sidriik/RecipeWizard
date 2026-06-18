cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(recipe_tests test_main.cpp)

target_link_libraries(recipe_tests PRIVATE
    RecipeCore
    doctest::doctest
    Qt6::Core
    Qt6::Sql
)

add_test(NAME unit_tests COMMAND recipe_tests)