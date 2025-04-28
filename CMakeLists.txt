cmake_minimum_required(VERSION 3.28)
project(RBT_Group_Project)

set(CMAKE_CXX_STANDARD 17)

# Add SDL2 manually
include_directories("C:/SDL2-2.30.8/x86_64-w64-mingw32/include")
link_directories("C:/SDL2-2.30.8/x86_64-w64-mingw32/lib")

set(CMAKE_EXE_LINKER_FLAGS "-mconsole")

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

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
