cmake_minimum_required(VERSION 3.5)

project(BlurIT VERSION 0.1 LANGUAGES CXX)

# COMMENT OUT IF NOT DEBUGGING
# Enable Address Sanitizer for debug builds
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Enable Address Sanitizer
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    #set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
#endif()

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Without this line, cmake does not find the package (building without Qt Creator)
set(CMAKE_PREFIX_PATH "C:/Qt/6.6.1/msvc2019_64")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia Widgets MultimediaWidgets)

# Set the source and destination paths for assets && styles
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/Assets")
set(STYLES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Styles")
set(STYLES_DEST_DIR "${CMAKE_BINARY_DIR}/Styles")
set(OPENCV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/opencv")
set(OPENCV_DEST_DIR "${CMAKE_BINARY_DIR}/opencv")
set(DEST_DIR "${CMAKE_BINARY_DIR}")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(files_to_copy
    "${SOURCE_DIR}/Assets/res10_300x300_ssd_iter_140000.caffemodel"
    "${SOURCE_DIR}/Assets/deploy.prototxt.txt"
)

# Copy the assets && styles folder to the build directory during the build process
file(COPY ${ASSETS_SOURCE_DIR} DESTINATION ${ASSETS_DEST_DIR})
file(COPY ${STYLES_SOURCE_DIR} DESTINATION ${STYLES_DEST_DIR})
file(COPY ${OPENCV_SOURCE_DIR} DESTINATION ${OPENCV_DEST_DIR})
foreach(file ${files_to_copy})
    file(COPY ${file} DESTINATION ${DEST_DIR})
endforeach()

# Detect operating system and set OpenCV library build according to it
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OpenCV_DIR "${CMAKE_BINARY_DIR}/opencv/opencv/build-windows-opencl-MSVC2019x64/")
elseif(DEFINED OS_TYPE)
    set(OpenCV_DIR "${CMAKE_BINARY_DIR}/opencv/opencv/build-linux-opencl-GCC/")
endif()

find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR} NO_DEFAULT_PATH)
include_directories(${OpenCV_INCLUDE_DIRS})

# Define APP_TARGET
set(APP_TARGET BlurIT)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Modify the build process to include the icon file
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon1.rc")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Use qt_add_executable for Qt 6
    qt_add_executable(${APP_TARGET}
        MANUAL_FINALIZATION
        WIN32
        ${PROJECT_SOURCES}
        icon.qrc
        facedetector.h facedetector.cpp
        icon1.rc
        ${APP_ICON_RESOURCE_WINDOWS}
    )

else()
    # Use add_executable for Qt 5
    if(ANDROID)
        add_library(${APP_TARGET} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${APP_TARGET}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Set common target properties
set_target_properties(${APP_TARGET} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_link_libraries(${APP_TARGET} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets)
target_link_libraries(${APP_TARGET} PUBLIC ${OpenCV_LIBS})

include(GNUInstallDirs)
install(TARGETS ${APP_TARGET}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${APP_TARGET})
endif()
