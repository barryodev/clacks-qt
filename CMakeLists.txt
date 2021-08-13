cmake_minimum_required(VERSION 3.5)

project(ClacksQT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.1.2\\mingw81_64")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql Network Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql Network Xml REQUIRED)

set(PROJECT_SOURCES
        main.h
        main.cpp
        clacks.cpp
        clacks.h
        clacks.ui
        addfeeddialog.h
        addfeeddialog.cpp
        addfeeddialog.ui
        removedialog.h
        removedialog.cpp
        removedialog.ui
        editdialog.h
        editdialog.cpp
        editdialog.ui
        feedloader.h
        feedloader.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ClacksQT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(ClacksQT SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ClacksQT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ClacksQT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql  Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Xml)

set_target_properties(ClacksQT PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ClacksQT)
endif()
