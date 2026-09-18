project(chip8)

add_executable(${PROJECT_NAME}
    main.cpp
    chip8.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
