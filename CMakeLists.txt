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

file(GLOB TINYXML_SOURCES
    "source/external/tinyxml2/*.cpp"
    "source/external/tinyxml2/*.h"
)

file(GLOB MATH_SOURCES
    "source/math/*.cpp"
    "source/math/*.h"
)

file(GLOB INCLUDE_FILES
    "source/include/*.h"
)

source_group("include" FILES ${INCLUDE_FILES})

source_group("widgets" FILES ${WIDGET_SOURCES})
source_group("layouting" FILES ${LAYOUTING_SOURCES})
source_group("math" FILES ${MATH_SOURCES})
source_group("external\\tinyxml" FILES ${TINYXML_SOURCES})

add_library(RUIn SHARED source/RUIN.cpp source/UIManager.cpp 
    ${WIDGET_SOURCES}
    ${LAYOUTING_SOURCES}
    ${TINYXML_SOURCES}
    ${MATH_SOURCES}
    ${INCLUDE_FILES}
)

target_include_directories(RUIn PRIVATE 
source/external/tinyxml2/
source/math/
)