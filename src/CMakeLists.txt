cmake_minimum_required(VERSION 3.12)
project(bezier)

set(CMAKE_CXX_STANDARD 14)
set(QTPKG Core Widgets Gui)
list(TRANSFORM QTPKG PREPEND "Qt5")
foreach(item ${QTPKG})
    find_package(${item})
endforeach()

list(TRANSFORM QTPKG REPLACE "Qt5" "Qt5::")




include_directories(.)
include_directories(math)

add_executable(${PROJECT_NAME}
        math/pen_pencil.cpp
        math/pen_pencil.h
        math/bezier.c
        math/bezier.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        paintwidget.cpp
        paintwidget.h
        undocommand.cpp
        undocommand.h
        interfaces.h
        Eraser.cpp Eraser.h)

foreach(item ${QTPKG})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${item})
endforeach()
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

#add_dependencies(${PROJECT_NAME} earser)
