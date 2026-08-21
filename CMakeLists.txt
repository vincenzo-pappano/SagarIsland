cmake_minimum_required(VERSION 3.16)

project(SagarIsland VERSION 1.0.0 LANGUAGES CXX)

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
    Location
    Positioning
    Widgets
)

set(PROJECT_SOURCES
    main.cpp
    qml.qrc
    src/FixedAspectRatioWindow.h
    src/FixedAspectRatioWindow.cpp
)

add_executable(SagarIsland ${PROJECT_SOURCES}
    src/LoggerConfig.h
    src/Logger.h
    src/Logger.cpp)

target_link_libraries(SagarIsland
  PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::Qml
  Qt5::QuickControls2
  Qt5::Location
  Qt5::Positioning
  Qt5::Widgets
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT GIT_COMMIT_HASH)
    set(GIT_COMMIT_HASH "unknown")
endif()

target_compile_definitions(SagarIsland
    PRIVATE
        GIT_COMMIT_ID="${GIT_COMMIT_HASH}"
        PROJECT_VERSION_STRING="${PROJECT_VERSION}"
)

set_target_properties(SagarIsland PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SagarIsland
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
