cmake_minimum_required(VERSION 3.7)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add definitions and flags
add_definitions(-std=c++17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# Project name
project(SDL2Test)

# Set the CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} src)

# Add the executable
add_executable(SnakeGame 
    src/main.cpp 
    src/game.cpp 
    src/controller.cpp 
    src/renderer.cpp 
    src/snake.cpp
    src/gameoverhandler.cpp
)

# Link SDL2 and SDL2_image
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
