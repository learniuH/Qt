cmake_minimum_required(VERSION 3.16)

project(QTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 添加这一行，让 AUTOUIC 能够找到 serial/ 下的 .ui 文件
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Serial")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QTest
        MANUAL_FINALIZATION
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ProtoEditor/protomodel.cpp
        ProtoEditor/protomodel.h
        ProtoEditor/protoview.cpp
        ProtoEditor/protoview.h
        ProtoEditor/typedelegate.cpp
        ProtoEditor/typedelegate.h
        GraphicsItems/joystickitem.cpp
        GraphicsItems/joystickitem.h
        GraphicsItems/switchItem.h
        GraphicsItems/switchItem.cpp
        Serial/serialsettings.ui
        Serial/serial.h
        Serial/serial.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QTest APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QTest SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QTest
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QTest PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)

# widget promoted
target_include_directories(QTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ProtoEditor
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QTest)
endif()
set_target_properties(QTest PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
# widget promoted
include_directories(
    ${PROJECT_SOURCE_DIR}
)
install(TARGETS QTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTest)
endif()
