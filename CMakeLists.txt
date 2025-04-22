cmake_minimum_required(VERSION 3.28)
project(RBT_Group_Project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.30.8/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(RBT_Group_Project
        BearGame.h
        Color.h
        Line.h
        Point.h
        Token.h
        SDL_Plotter.cpp
        SDL_Plotter.h
        main.cpp
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})