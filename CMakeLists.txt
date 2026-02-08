cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

# set version to cpp 23+
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#compile commands for cmake + nix + nvim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

#find packages in my system
find_package(CLI11 CONFIG REQUIRED)
find_package(FTXUI REQUIRED)

#add source code to executable
add_executable(app
    src/main.cpp
    src/ripgrep.cpp
)

# add header files
target_include_directories(app
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# symlink compile commands to root
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
#link libraries to my executable
target_link_libraries(app
    PRIVATE
        CLI11::CLI11
        ftxui::screen
        ftxui::dom
        ftxui::component
)
target_precompile_headers(app PRIVATE
    # Boost headers (the heavy ones)
    <boost/json.hpp>
    #<boost/json/src.hpp>
    <boost/process.hpp>

    # Standard library
    <filesystem>
    <format>
    <string>
    <vector>
    <iostream>
    <sstream>
    <memory>
    <algorithm>

    # C headers if you use them
    <fcntl.h>
    <sys/wait.h>
    <unistd.h>
)
