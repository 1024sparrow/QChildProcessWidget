cmake_minimum_required(VERSION 3.10.2)
project(webviewExample_project CXX)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-lpthread")
SET(CMAKE_CXX_STANDARD 14)

add_subdirectory(webviewExample)
