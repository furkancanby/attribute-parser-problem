cmake_minimum_required(VERSION 3.22)

project(attribute_parser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

find_package(Boost REQUIRED)

link_directories(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}
)