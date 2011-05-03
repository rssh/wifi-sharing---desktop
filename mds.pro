#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T10:50:29
#
#-------------------------------------------------

QT       += core gui network
RESOURCES = mds.qrc

TARGET = mds
TEMPLATE = app

CONFIG += qt release

win32 {
   hupnp_src_home = c:/packages/herqq/herqq-1.0.0/hupnp
   hupnp_bin_home = c:/packages/herqq/herqq-build-desktop/hupnp
}
else {
   hupnp_src_home = /home/rssh/packages/herqq/hupnp/trunk/herqq/hupnp
   hupnp_bin_home = /home/rssh/packages/herqq/hupnp/trunk/herqq/hupnp
}

INCLUDEPATH += $$hupnp_src_home/include

LIBS += -L"$$hupnp_bin_home/bin" -lHUpnp \
        -L"$$hupnp_bin_home/lib/qtsoap-2.7-opensource/lib"

QMAKE_LIBDIR += $$hupnp_bin_home/bin $$hupnp_bin_home/lib/qtsoap-2.7-opensource/lib


win32 {
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
        mainwindow.cpp \
    upnpdeviceslistmodel.cpp

HEADERS  += mainwindow.h \
    upnpdeviceslistmodel.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    mdsmonitor.sh
