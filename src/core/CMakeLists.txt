project(runtime)

add_library(${PROJECT_NAME} STATIC
    data.cpp
    game.cpp
    input.cpp
    objects.cpp
    renderer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)
