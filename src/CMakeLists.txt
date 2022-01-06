find_package(
    Qt5
    CONFIG
    REQUIRED
    Core
    Gui
    Widgets
    PrintSupport
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(spiral)

target_sources(spiral 
  PRIVATE
  constants.cpp
  draglayout.cpp
  dragresizelabel.cpp
  main.cpp
  mainwindow.cpp
  textbox.cpp
  mrichtextedit.cpp
  mtextedit.cpp
)

# Icon QRC files
target_sources(spiral
        PRIVATE
        ../icon.qrc
        ../moveicon.qrc
        ../tango.qrc)

target_include_directories(spiral
PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/structure>
)

target_link_libraries(
    spiral
    project_warnings
    project_options
    Qt5::Widgets
    Qt5::PrintSupport
)
