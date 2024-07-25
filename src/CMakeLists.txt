set(LIBRARY_TARGET "${PROJECT_NAME}-lib")

set(SOURCE_FILES
    core/videotrimmer.cpp
    core/videotrimmer.h

    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/forms/mainwindow.ui
    gui/widgets/videoplayer.cpp
    gui/widgets/videoplayer.h
)

add_library(${LIBRARY_TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${LIBRARY_TARGET} PRIVATE gui/widgets)

target_link_libraries(${LIBRARY_TARGET} PUBLIC
    Qt${MAJOR_QT_VERSION}::Core
    Qt${MAJOR_QT_VERSION}::Gui
    Qt${MAJOR_QT_VERSION}::Widgets
    Qt${MAJOR_QT_VERSION}::Multimedia
    Qt${MAJOR_QT_VERSION}::MultimediaWidgets

    FFmpeg::avcodec
    FFmpeg::avformat
    FFmpeg::avutil
)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_TARGET})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE ON 
    MACOSX_BUNDLE ON
)
