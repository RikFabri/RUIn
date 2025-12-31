cmake_minimum_required(VERSION 4.0.3)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(RUIn)

file(GLOB WIDGET_SOURCES
    "source/widgets/*.cpp"
    "source/widgets/*.h"
)

file(GLOB BINDING_SOURCES
    "source/widgets/databinding/*.cpp"
    "source/widgets/databinding/*.h"
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

file(GLOB UTIL_SOURCES
    "source/utils/*.cpp"
    "source/utils/*.h"
)

file(GLOB INCLUDE_FILES
    "source/include/*.h"
)

file(GLOB RESOURCE_FILES
    "source/resources/*.cpp"
    "source/resources/*.h"
)

source_group("include" FILES ${INCLUDE_FILES})

source_group("widgets" FILES ${WIDGET_SOURCES})
source_group("widgets\\databinding" FILES ${BINDING_SOURCES})
source_group("layouting" FILES ${LAYOUTING_SOURCES})
source_group("math" FILES ${MATH_SOURCES})
source_group("utils" FILES ${UTIL_SOURCES})
source_group("resources" FILES ${RESOURCE_FILES})
source_group("external\\tinyxml" FILES ${TINYXML_SOURCES})

add_library(RUIn SHARED source/RUIN.cpp source/UIManager.cpp source/BindingDatabase.cpp
    ${WIDGET_SOURCES}
    ${BINDING_SOURCES}
    ${LAYOUTING_SOURCES}
    ${TINYXML_SOURCES}
    ${MATH_SOURCES}
    ${INCLUDE_FILES}
    ${RESOURCE_FILES}
    ${UTIL_SOURCES}
)

target_compile_definitions(RUIn PRIVATE RUIN_BUILD)

target_include_directories(RUIn PRIVATE 
source/external/tinyxml2/
source/math/
source/utils/
)

if (MSVC)
set_property(TARGET RUIn PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(RUIn PRIVATE /W4)
endif()

# support find_package
include(GNUInstallDirs)

target_include_directories(RUIn PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS RUIn EXPORT RUInTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES source/include/RUIN.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT RUInTargets
    FILE RUInTargets.cmake
    NAMESPACE RUIn::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RUIn
)

export(EXPORT RUInTargets
    NAMESPACE RUIn::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/RUInTargets.cmake)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RUInConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RUInConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RUIn
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RUInConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RUInConfig.cmake
    INSTALL_DESTINATION .
)

install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/RUInConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RUIn
) 