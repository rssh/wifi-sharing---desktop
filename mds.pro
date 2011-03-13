#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T10:50:29
#
#-------------------------------------------------

QT       += core gui webkit
RESOURCES = mds.qrc

TARGET = mds
TEMPLATE = app

hupnp_home = /home/rssh/packages/herqq/hupnp/trunk/herqq/hupnp

INCLUDEPATH += $$hupnp_home/include

LIBS += -L"$$hupnp_home/bin" -lHUpnp \
        -L"$$hupnp_home/lib/qtsoap-2.7-opensource/lib"

QMAKE_LIBDIR += $$hupnp_home/bin $$hupnp_home/lib/qtsoap-2.7-opensource/lib


WIN32 {
   debug {
     LIBS += -lQtSolutions_SOAP-2.7d
   }
   else {
     LIBS += -lQtSolutions_SOAP-2.7
   }
   LIBS += -lws2_32
}
else {
  LIBS += -lQtSolutions_SOAP-2.7
  !macx:QMAKE_LFLAGS += -Wl,--rpath=\\\\$\$ORIGIN
}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
