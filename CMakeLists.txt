cmake_minimum_required(VERSION 3.20)
project(snappytrim VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "Quick and lightweight video trimming application")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MAJOR_QT_VERSION 6)
set(QT_INSTALL_DIR "$ENV{QT_DIR}" CACHE INTERNAL "Qt Intallation DIR ex: C:\\Qt\\6.7.1\\msvc2019_64 (uses environment var QT_DIR by default)")

list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")

if (QT_INSTALL_DIR)
    message("Added Qt Directory ${QT_INSTALL_DIR} to CMAKE_PREFIX_PATH")
else()
    message("No specified Qt Directory. Using default CMAKE_PREFIX_PATH config.")
endif()

find_package(
    Qt${MAJOR_QT_VERSION} 
    REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    )

if (Qt${MAJOR_QT_VERSION}_FOUND)
    message("Qt was found")
    message("Qt version: ${Qt${MAJOR_QT_VERSION}Widgets_VERSION}")
endif()

qt_standard_project_setup()

add_subdirectory(src)