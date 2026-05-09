cmake_minimum_required(VERSION 3.22)

project(Orion VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(orion
    src/main.cpp
    src/backend/FileSystemModel.hpp
    src/backend/FileSystemModel.cpp
)

qt_add_qml_module(orion
    URI Orion
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_link_libraries(orion
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

install(TARGETS orion
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
