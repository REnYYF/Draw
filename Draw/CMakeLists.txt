cmake_minimum_required(VERSION 3.15)

project(Draw_Demo)

#QT相关的配置
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}  "*.cpp" "*.c")
file(GLOB ALL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} "*.h")


add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_INCLUDES} res.qrc)

find_package(Qt5 COMPONENTS core gui Widgets Svg Xml REQUIRED)

target_link_libraries(${PROJECT_NAME}
Qt5::Core
Qt5::Gui
Qt5::Widgets
Qt5::Svg
Qt5::Xml
)
