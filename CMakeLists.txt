cmake_minimum_required(VERSION 3.20)

project(Mesh_Computational_Geometry)

message("Building Mesh Computational Geometry ...")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}")

set(SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/gldisplaywidget.cpp
    ${CMAKE_SOURCE_DIR}/mesh.cpp
    ${CMAKE_SOURCE_DIR}/meshcrust.cpp
    ${CMAKE_SOURCE_DIR}/meshruppert.cpp
    ${CMAKE_SOURCE_DIR}/point.cpp
    ${CMAKE_SOURCE_DIR}/opengldisplaymesh.cpp
    ${CMAKE_SOURCE_DIR}/simplify.cpp
    ${CMAKE_SOURCE_DIR}/voronoi.cpp
    ${CMAKE_SOURCE_DIR}/visibilitymarch.cpp
    ${CMAKE_SOURCE_DIR}/mesh3d.cpp
    ${CMAKE_SOURCE_DIR}/color.cpp
    ${CMAKE_SOURCE_DIR}/meshcurvature.cpp
    ${CMAKE_SOURCE_DIR}/meshdelaunay.cpp
    ${CMAKE_SOURCE_DIR}/meshparabola.cpp
    ${CMAKE_SOURCE_DIR}/predicate.cpp
    ${CMAKE_SOURCE_DIR}/mesh_refined_delaunay.cpp
)

# Add resource files
file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(${CMAKE_SOURCE_DIR})

# Configure OpenGL
set(OpenGL_COMPONENT
    EGL
    GLX
    OpenGL
)
find_package(OpenGL COMPONENTS ${OpenGL_COMPONENT} REQUIRED)

# Configure GLM
find_package(glm REQUIRED)

# Configure Qt
set(QT_VERSION 5.12.8)
set(QT_COMPONENT
    Core
    Concurrent
    Widgets
    OpenGL
)
find_package(Qt5 ${QT_VERSION} COMPONENTS ${QT_COMPONENT} REQUIRED)


add_executable(${PROJECT_NAME} ${SOURCES})


foreach(_comp ${OpenGL_COMPONENT})
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::${_comp})
endforeach()

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

foreach(_comp ${QT_COMPONENT})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::${_comp} )
endforeach()