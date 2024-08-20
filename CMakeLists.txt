cmake_minimum_required(VERSION 3.20)
project(SnappyTrim VERSION 0.1.0 LANGUAGES CXX C DESCRIPTION "Quick and lightweight video trimming application")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MAJOR_QT_VERSION 6)
set(QT_INSTALL_DIR "$ENV{QT_DIR}" CACHE INTERNAL "Qt Intallation DIR ex: C:\\Qt\\6.7.1\\msvc2019_64 (uses environment var QT_DIR by default)")

if (QT_INSTALL_DIR)
    message("Added Qt Directory ${QT_INSTALL_DIR} to CMAKE_PREFIX_PATH")
else()
    message("No specified Qt Directory. Using default CMAKE_PREFIX_PATH config.")
endif()

find_package(Qt${MAJOR_QT_VERSION} 
    REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
)

find_package(FFmpeg)

if (Qt${MAJOR_QT_VERSION}_FOUND)
    message("Qt was found")
    message("Qt version: ${Qt${MAJOR_QT_VERSION}Widgets_VERSION}")
endif()

qt_standard_project_setup()

add_subdirectory(src)

install (TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "blu3ci")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME} ${PROJECT_VERSION}")

include(CPack)