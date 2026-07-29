project(tetris)

add_executable(${PROJECT_NAME}
    blocks.cpp
    level.cpp
    main.cpp
    tetris_game.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)