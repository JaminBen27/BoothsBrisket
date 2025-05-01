cmake_minimum_required(VERSION 3.28)
project(RBT_Group_Project)

set(CMAKE_CXX_STANDARD 17)

# SDL2 via Homebrew (standard path for Intel Macs)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

add_executable(RBT_Group_Project
        BearGame.h
        Color.h
        Line.h
        Point.h
        Token.h
        SDL_Plotter.cpp
        SDL_Plotter.h
        main.cpp
        GroupName.h
        Team.h
        Constants.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
