project(snake)

add_executable(${PROJECT_NAME}
    main.cpp
    snake_game.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)