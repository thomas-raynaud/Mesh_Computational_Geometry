#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    meshcrust.cpp \
    point.cpp \
    opengldisplaymesh.cpp \
    voronoi.cpp \
    visibilitymarch.cpp \
    mesh3d.cpp \
    color.cpp \
    meshcurvature.cpp \
    meshdelaunay.cpp \
    meshparabola.cpp \
    predicate.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    mesh.h \
    meshcrust.h \
    point.h \
    color.h \
    mesh3d.h \
    meshdelaunay.h \
    meshparabola.h \
    predicate.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
LIBS = -lGLU

# Add the queen.off file
copydata.commands = $(COPY_DIR) $$PWD/queen.off $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
