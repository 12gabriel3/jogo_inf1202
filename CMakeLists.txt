cmake_minimum_required(VERSION 3.10)

# set the project name
project(OMago)

# add include and link dirs
include_directories(allegro/include include)
link_directories(allegro/lib)

# add libraries
add_library(graphics ./src/graphics.c)
target_link_libraries(graphics allegro_monolith)

add_library(input ./src/input.c)
target_link_libraries(input allegro_monolith)

add_library(geometry ./src/geometry.c)
target_link_libraries(geometry input)

add_library(character ./src/character.c)
target_link_libraries(character input geometry graphics)

add_library(env ./src/env.c)
target_link_libraries(env geometry graphics)

add_library(level ./src/level.c)
target_link_libraries(level geometry env character allegro_monolith)

add_library(game ./src/game.c)
target_link_libraries(game menu level input allegro_monolith)

add_library(menu ./src/menu.c)
target_link_libraries(menu game input allegro_monolith)

# add the executable
add_executable(OMago ./src/main.c)
target_link_libraries(OMago allegro_monolith graphics input geometry character level game)