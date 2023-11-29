cmake_minimum_required(VERSION 3.15)

project(Draw_Demo)

file(GLOB ALL_SOURCES "*.cpp" "*.c")
file(GLOB ALL_INCLUDES "*.h")

add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_INCLUDES})