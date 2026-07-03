cmake_minimum_required(VERSION 3.5)

project(weather-app-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 COMPONENTS Core Gui Widgets Network Svg QUIET)

if(Qt6_FOUND)
set(QT_VERSION_MAJOR 6)
else()

find_package(Qt5 COMPONENTS Core Gui Widgets Network Svg REQUIRED)
set(QT_VERSION_MAJOR 5)
endif()



set(PROJECT_SOURCES
main.cpp
weatherapp.cpp
weatherapp.h
resources.qrc
)



if(QT_VERSION_MAJOR EQUAL 6)
qt_add_executable(weather-app-qt
MANUAL_FINALIZATION
${PROJECT_SOURCES}
)
else()
add_executable(weather-app-qt
${PROJECT_SOURCES}
)
endif()



if(QT_VERSION_MAJOR EQUAL 6)
target_link_libraries(weather-app-qt PRIVATE
Qt6::Core
Qt6::Gui
Qt6::Widgets
Qt6::Network
Qt6::Svg
)
else()
target_link_libraries(weather-app-qt PRIVATE
Qt5::Core
Qt5::Gui
Qt5::Widgets
Qt5::Network
Qt5::Svg
)
endif()



set_target_properties(weather-app-qt PROPERTIES
MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
MACOSX_BUNDLE TRUE
WIN32_EXECUTABLE TRUE
)


if(QT_VERSION_MAJOR EQUAL 6)
qt_finalize_executable(weather-app-qt)
endif()
