TEMPLATE        = app
unix:CONFIG     += qt warn_on release
win32:CONFIG    += windows warn_on release
greaterThan(QT_MAJOR_VERSION, 4) {
QT += widgets
}
isEmpty(PREFIX) {
PREFIX		= /usr/local
}
HEADERS         += qmbox.h
SOURCES         += qmbox.cpp
target.path	= $${PREFIX}/bin
TARGET          = qmbox
QTDIR_build:REQUIRES="contains(QT_CONFIG, full-config)"
INSTALLS	+= target
