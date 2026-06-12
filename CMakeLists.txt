cmake_minimum_required(VERSION 3.14)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)


include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(SnakeGame main.cpp Snake.cpp Game.cpp sqlite3.c DatabaseManager.cpp)


target_link_libraries(SnakeGame PRIVATE sfml-graphics sfml-window sfml-system)


target_compile_definitions(SnakeGame PRIVATE SFML_STATIC)


