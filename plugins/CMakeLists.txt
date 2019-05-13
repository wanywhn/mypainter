cmake_minimum_required(VERSION 3.12)
project(paintertools)

set(CMAKE_CXX_STANDARD 14)
set(QTPKG Core Widgets)
list(TRANSFORM QTPKG PREPEND "Qt5")
foreach(item ${QTPKG})
    find_package(${item})
endforeach()

list(TRANSFORM QTPKG REPLACE "Qt5" "Qt5::")




add_library(${PROJECT_NAME}
        SHARED
#    basictoolsplugin.cpp
#    basictoolsplugin.h
        math/pen_pencil.cpp
        math/pen_pencil.h
        math/bezier.c
        math/bezier.h
        )
add_library(earser SHARED math/Earser.cpp math/Earser.h)

set(LIBS ${PROJECT_NAME} earser)
foreach(it ${LIBS})
target_include_directories(${it} PRIVATE .)
target_include_directories(${it} PRIVATE math)

foreach(item ${QTPKG})
    target_link_libraries(${it} PRIVATE ${item})
endforeach()
set_target_properties(${it} PROPERTIES AUTOMOC TRUE)
    endforeach()
