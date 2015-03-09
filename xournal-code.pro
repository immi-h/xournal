TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

PKGCONFIG += gtk-2.0

SOURCES += \
    src/xo-clipboard.c \
    src/xo-file.c \
    src/xo-image.c \
    src/xo-interface.c \
    src/xo-misc.c \
    src/xo-paint.c \
    src/xo-print.c \
    src/xo-selection.c \
    src/xo-shapes.c \
    src/xo-support.c \
    src/main.c \
    src/xo-callbacks.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    src/xo-callbacks.h \
    src/xo-clipboard.h \
    src/xo-file.h \
    src/xo-image.h \
    src/xo-interface.h \
    src/xo-misc.h \
    src/xo-paint.h \
    src/xo-print.h \
    src/xo-selection.h \
    src/xo-shapes.h \
    src/xo-support.h \
    src/xournal.h

DISTFILES += \
    xournal.glade

