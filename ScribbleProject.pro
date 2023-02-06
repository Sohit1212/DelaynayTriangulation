TEMPLATE = app
TARGET = delaunay_app

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += include/

HEADERS += \
    include/edge.h \
    include/line.h \
    include/mainwindow.h \
    include/scribblearea.h \
    include/triangle.h \
    include/vertex.h

SOURCES += \
    src/edge.cpp \
    src/line.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/scribblearea.cpp \
    src/triangle.cpp \
    src/vertex.cpp

DISTFILES += \
    ReadMe.md
