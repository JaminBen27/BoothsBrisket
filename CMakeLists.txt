cmake_minimum_required(VERSION 3.27)
project(RBT_Group_Project)

set(CMAKE_CXX_STANDARD 17)

# SDL2 root path
set(SDL2_PATH "C:/SDL2-2.30.9/x86_64-w64-mingw32")

# Add the SDL2 include directory (only up to 'include', not 'SDL2')
include_directories(${SDL2_PATH}/include)

# Add the SDL2 lib directory
link_directories(${SDL2_PATH}/lib)

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

# Link the SDL2main and SDL2 libraries
target_link_libraries(RBT_Group_Project SDL2main SDL2)
