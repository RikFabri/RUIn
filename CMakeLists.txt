cmake_minimum_required(VERSION 4.0.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(RUIn)

add_compile_definitions(RUIN_BUILD)

file(GLOB WIDGET_SOURCES
    "source/widgets/*.cpp"
    "source/widgets/*.h"
)

file(GLOB LAYOUTING_SOURCES
    "source/layouting/*.cpp"
    "source/layouting/*.h"
)

source_group("widgets" FILES ${WIDGET_SOURCES})
source_group("layouting" FILES ${LAYOUTING_SOURCES})

add_library(RUIn SHARED source/RUIN.cpp source/UIManager.cpp ${WIDGET_SOURCES} ${LAYOUTING_SOURCES})