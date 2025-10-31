cmake_minimum_required(VERSION 3.12)
project(SFMLProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/program)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Manual SFML setup using local include and lib directories
set(SFML_ROOT ${CMAKE_SOURCE_DIR})
set(SFML_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SFML_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)

# Include directories
include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

# Find source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cxx" "src/*.cc")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries manually
target_link_libraries(${PROJECT_NAME}
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

# Copy assets to program directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assests
    ${CMAKE_SOURCE_DIR}/program/assests
    COMMENT "Copying assets to program directory"
)
