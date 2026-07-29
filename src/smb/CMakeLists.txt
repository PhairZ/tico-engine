project(super_mario_bros)

add_executable(${PROJECT_NAME}
    main.cpp
    player.cpp
    smb_game.cpp
    sprites.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)