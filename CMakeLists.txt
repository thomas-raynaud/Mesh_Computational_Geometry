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
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Vertex.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Face.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Edge.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Mesh2D.cpp
    ${CMAKE_SOURCE_DIR}/src/meshes/Mesh3D.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/mesh_rendering.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/predicate.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/simplify.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/delaunay.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/MeshCrust.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/MeshParabola.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/MeshRuppert.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/visibility_march.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/2d/voronoi.cpp
    ${CMAKE_SOURCE_DIR}/src/techniques/3d/curvature.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/color.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/math.cpp
)

# Add resource files
file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(${CMAKE_SOURCE_DIR}/src)

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

# Catch
find_package(Catch2 REQUIRED)
#target_link_libraries(tests Catch2::Catch2)


add_executable(${PROJECT_NAME} ${SOURCES})


foreach(_comp ${OpenGL_COMPONENT})
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::${_comp})
endforeach()

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

foreach(_comp ${QT_COMPONENT})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::${_comp} )
endforeach()