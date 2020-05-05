cmake_minimum_required( VERSION 3.10 )


# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project Name
project( C++_Web_Server_from_scratch )

# flags

# files

# include
include_directories( ./src )

# target
add_executable( web_server ./src/main.cpp )

# external libs
add_library(Server ./src/server.cpp)
target_link_libraries( web_server Server )
