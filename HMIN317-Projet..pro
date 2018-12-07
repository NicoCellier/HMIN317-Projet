QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    megaman.cpp \
    terrain.cpp

SOURCES += \
    mainwidget.cpp

HEADERS += \
    mainwidget.h \
    megaman.h \
    terrain.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target

DISTFILES += \
    heightmap-1.png \
    heightmap-2.png \
    heightmap-3.png
