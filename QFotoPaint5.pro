#-------------------------------------------------
#
# Project created by QtCreator 2022-09-01T12:00:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFotoPaint5

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    ajustelineal.cpp \
    bajorrelieve.cpp \
    capturarvideo.cpp \
    detectarcaras.cpp \
    detectarcarasvideo.cpp \
    ecualizarhistograma.cpp \
    falsocolor.cpp \
    mainwindow.cpp \
    imagenes.cpp \
    dialognueva.cpp \
    brillocontraste.cpp \
    matsatlum.cpp \
    minimomaximo.cpp \
    modelos_color.cpp \
    movimiento.cpp \
    perfilado.cpp \
    perspectiva.cpp \
    pincharestirar.cpp \
    rojoverdeazul.cpp \
    rotarcualquiera.cpp \
    suavizados.cpp \
    ver_informacion.cpp \
    video.cpp \
    rotaravideo.cpp \
    mediaponderada.cpp \
    acercade.cpp \
    mediadevideo.cpp

HEADERS  += mainwindow.h \
    ajustelineal.h \
    bajorrelieve.h \
    capturarvideo.h \
    detectarcaras.h \
    detectarcarasvideo.h \
    ecualizarhistograma.h \
    falsocolor.h \
    imagenes.h \
    dialognueva.h \
    brillocontraste.h \
    matsatlum.h \
    minimomaximo.h \
    modelos_color.h \
    movimiento.h \
    perfilado.h \
    perspectiva.h \
    pincharestirar.h \
    rojoverdeazul.h \
    rotarcualquiera.h \
    suavizados.h \
    ver_informacion.h \
    video.h \
    rotaravideo.h \
    mediaponderada.h \
    acercade.h \
    mediadevideo.h

FORMS    += mainwindow.ui \
    ajustelineal.ui \
    bajorrelieve.ui \
    capturarvideo.ui \
    detectarcaras.ui \
    detectarcarasvideo.ui \
    dialognueva.ui \
    brillocontraste.ui \
    ecualizarhistograma.ui \
    falsocolor.ui \
    matsatlum.ui \
    minimomaximo.ui \
    modelos_color.ui \
    movimiento.ui \
    perfilado.ui \
    perspectiva.ui \
    pincharestirar.ui \
    rojoverdeazul.ui \
    rotarcualquiera.ui \
    suavizados.ui \
    rotaravideo.ui \
    mediaponderada.ui \
    acercade.ui \
    mediadevideo.ui \
    ver_informacion.ui

INCLUDEPATH += "C:\OpenCV\OpenCV4.6.0G\include"

LIBS += -L"C:\OpenCV\OpenCV4.6.0G\lib"\
        -llibopencv_world460

RESOURCES += recursos.qrc

RC_ICONS = imagenes/icono.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
