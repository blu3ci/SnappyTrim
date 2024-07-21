set(SOURCE_FILES
    main.cpp

    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/forms/mainwindow.ui
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${MAJOR_QT_VERSION}::Core
    Qt${MAJOR_QT_VERSION}::Gui
    Qt${MAJOR_QT_VERSION}::Widgets
    )

set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE ON 
    MACOSX_BUNDLE ON
    )