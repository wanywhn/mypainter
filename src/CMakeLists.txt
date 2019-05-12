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
include_directories(../plugins/math)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_NAME}
        ../plugins/math/pen_pencil.cpp
        ../plugins/math/pen_pencil.h
        ../plugins/math/bezier.c
        ../plugins/math/bezier.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        paintwidget.cpp
        paintwidget.h
        )

foreach(item ${QTPKG})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${item})
endforeach()
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
