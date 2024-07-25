include(FindPackageHandleStandardArgs)

set(FFmpeg_REQUIRED_COMPONENTS)

macro(find_ffmpeg_component component header)
    list(APPEND FFmpeg_REQUIRED_COMPONENTS FFmpeg_${component}_INCLUDE_DIR FFmpeg_${component}_LIBRARY)

    find_path(FFmpeg_${component}_INCLUDE_DIR 
        NAMES "lib${component}/${header}" 
        PATH_SUFFIXES FFmpeg
    )

    find_library(FFmpeg_${component}_LIBRARY 
        NAMES "${component}" 
    )

    if (FFmpeg_${component}_INCLUDE_DIR AND FFmpeg_${component}_LIBRARY)
        set(FFmpeg_${component}_FOUND TRUE)

        mark_as_advanced("FFmpeg_${component}_LIBRARY")
        mark_as_advanced("FFmpeg_${component}_INCLUDE_DIR")

        if (NOT TARGET FFmpeg_${component})
            add_library(FFmpeg_${component} UNKNOWN IMPORTED)
            add_library(FFmpeg::${component} ALIAS FFmpeg_${component})
            set_target_properties(FFmpeg_${component} PROPERTIES 
                IMPORTED_LOCATION  "${FFmpeg_${component}_LIBRARY}" 
                INTERFACE_INCLUDE_DIRECTORIES "${FFmpeg_${component}_INCLUDE_DIR}"
            )

            message("FFmpeg - FOUND ${component}")
        endif()
    else()
        set(FFmpeg_${component}_FOUND FALSE)

        message(FATAL_ERROR "FFmpeg - COULD NOT FIND ${component}")
    endif()
endmacro()

find_ffmpeg_component("avcodec" "avcodec.h")
find_ffmpeg_component("avfilter" "avfilter.h")
find_ffmpeg_component("avformat" "avformat.h")
find_ffmpeg_component("avdevice" "avdevice.h")
find_ffmpeg_component("avutil" "avutil.h")
find_ffmpeg_component("swresample" "swresample.h")
find_ffmpeg_component("postproc" "postprocess.h")
find_ffmpeg_component("swscale" "swscale.h")

find_package_handle_standard_args(FFmpeg REQUIRED_VARS 
    ${FFmpeg_REQUIRED_COMPONENTS}
)

if (FFmpeg_FOUND)
    mark_as_advanced(FFmpeg_LIBRARY)
endif()
