TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt link_pkgconfig

PKGCONFIG += gtk-2.0 libgnomecanvas-2.0

#this is mostly for autocomplete and code-helps
INCLUDEPATH += /usr/include/gtk-2.0
INCLUDEPATH += /usr/lib/gtk-2.0/include
INCLUDEPATH += /usr/include/pango-1.0
INCLUDEPATH += /usr/include/atk-1.0
INCLUDEPATH += /usr/include/cairo
INCLUDEPATH += /usr/include/pixman-1
INCLUDEPATH += /usr/include/libdrm
INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
INCLUDEPATH += /usr/include/libpng16
INCLUDEPATH += /usr/include/pango-1.0
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/glib-2.0/include
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/libpng16
INCLUDEPATH += /usr/include/harfbuzz
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/libpng16
INCLUDEPATH += /usr/include/harfbuzz
INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
INCLUDEPATH += /usr/include/libgnomecanvas-2.0

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
    src/xo-callbacks.c \
    src/xo-copywindow.c


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
    src/xournal.h \
    src/xo-copywindow.h \
    src/utils.h

DISTFILES += \
    xournal.glade \
    uren.txt

