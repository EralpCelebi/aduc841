cmake_minimum_required(VERSION 4.1.0)

project(aduc841 LANGUAGES C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(modules)
add_executable(aduc841 src/main.c src/aduc841.c)

target_link_libraries(aduc841 ${MODULES})
target_include_directories(aduc841 PUBLIC include)
