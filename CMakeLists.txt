cmake_minimum_required(VERSION 3.16)

project(SagarIsland VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 5.15 REQUIRED COMPONENTS
    Core
    Quick
    Qml
    QuickControls2
)

set(PROJECT_SOURCES
    main.cpp
    qml.qrc
    src/FixedAspectRatioWindow.h
    src/FixedAspectRatioWindow.cpp
)

add_executable(SagarIsland ${PROJECT_SOURCES})

target_link_libraries(SagarIsland
  PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::Qml
  Qt5::QuickControls2
)

set_target_properties(SagarIsland PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SagarIsland
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
