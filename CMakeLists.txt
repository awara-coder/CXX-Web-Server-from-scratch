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
add_library(HTTPParser ./src/http_parser.cpp)
add_library(File ./src/file.cpp)
add_library(Worker ./src/worker.cpp)

target_link_libraries( Worker PUBLIC HTTPParser)
target_link_libraries( web_server PUBLIC Server)
target_link_libraries( HTTPParser PUBLIC File)
target_link_libraries( Server PUBLIC Worker)