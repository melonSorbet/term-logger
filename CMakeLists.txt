cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile commands for cmake + nix + nvim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Source files
add_executable(app src/main.cpp)

# Dependencies
find_package(CLI11 CONFIG REQUIRED)
find_package(FTXUI REQUIRED)

# Link libraries
target_link_libraries(app 
    PRIVATE 
        CLI11::CLI11
        ftxui::screen
        ftxui::dom
        ftxui::component
)

# Optional: symlink compile_commands.json to source dir
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(
        copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )
endif()
