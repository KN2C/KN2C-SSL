#-------------------------------------------------
#
# Project created by QtCreator 2012-01-29T16:05:56
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kn2cssl
TEMPLATE = app
#CONFIG   += qt warn_on incremental link_prl
#MAKEFILE_GENERATOR = UNIX
#TARGET_PLATFORM  = unix
#QMAKE_INCREMENTAL_STYLE = sublib
QMAKE_CXXFLAGS += -std=c++11

#CONFIG += debug
#CONFIG   += release
#DEFINES += QT_NO_DEBUG_OUTPUT

#QMAKE_CXXFLAGS += --coverage
#QMAKE_LFLAGS += --coverage

#QMAKE_CFLAGS    -= -O2 -O1
#QMAKE_CXXFLAGS  -= -O2 -O1
##QMAKE_CFLAGS  += -O3 -m64
##QMAKE_LFLAGS  += -O3 -m64
#QMAKE_CFLAGS  += -O3
#QMAKE_LFLAGS  += -O3
#QMAKE_CXXFLAGS += -O3

#QMAKE_CXXFLAGS += -O3
#QMAKE_CXXFLAGS += -m64
#QMAKE_CXXFLAGS += -pipe
#QMAKE_CXXFLAGS += -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mpclmul # -mavx -mavx2 -maes -mfsgsbase -mrdrnd
#QMAKE_CXXFLAGS += -mfpmath=sse
#QMAKE_CXXFLAGS += -fargument-noalias-global
#QMAKE_CXXFLAGS += -fomit-frame-pointer
##QMAKE_CXXFLAGS += -fwhole-program #fail!
#QMAKE_CXXFLAGS += -march=native
#QMAKE_CXXFLAGS += -mtune=native

#load(qt_config)

DESTDIR = ../bin
OBJECTS_DIR = ../tmp/.obj
MOC_DIR = ../tmp/.moc
RCC_DIR = ../tmp/.rcc
UI_DIR = ../tmp/.ui

win32: LIBS += -L$$PWD/../ -lprotobuf
win32: INCLUDEPATH += $$PWD/../
win32: DEPENDPATH += $$PWD/../
unix:  LIBS += -lprotobuf

INCLUDEPATH += ssl
INCLUDEPATH += ssl/sslvision
INCLUDEPATH += ssl/sslvision/messages
INCLUDEPATH += ssl/sslrefbox
INCLUDEPATH += etc
INCLUDEPATH += ui
INCLUDEPATH += util
INCLUDEPATH += qextserialport
INCLUDEPATH += geom
INCLUDEPATH += output
INCLUDEPATH += grSim
INCLUDEPATH += ai

SOURCES +=	main.cpp \
    grSim/grSim_Replacement.pb.cc \
    grSim/grSim_Packet.pb.cc \
    grSim/grSim_Commands.pb.cc \
    ssl/sslreceiver.cpp \
    ssl/sslrefbox.cpp \
    ssl/sslvision.cpp \
    ssl/sslvision/messages/messages_robocup_ssl_wrapper.pb.cc \
    ssl/sslvision/messages/messages_robocup_ssl_refbox_log.pb.cc \
    ssl/sslvision/messages/messages_robocup_ssl_geometry.pb.cc \
    ssl/sslvision/messages/messages_robocup_ssl_detection.pb.cc \
    ui/mainwindow.cpp \
    ui/renderarea.cpp \
    util/fpscounter.cpp \
    ai/soccer.cpp \
    ssl/worldmodel.cpp \
    output/wpacket.cpp \
    output/transmitter.cpp \
    output/grsim.cpp \
    output/grpacket.cpp \
    ssl/sslrefbox/game_state.cpp \
    ssl/mobileobject.cpp \
    ssl/ball.cpp \
    ssl/robot.cpp \
    output/controller.cpp \
    ai/ai.cpp \
    geom/angle_deg.cpp \
    geom/circle_2d.cpp \
    geom/composite_region_2d.cpp \
    geom/convex_hull.cpp \
    geom/delaunay_triangulation.cpp \
    geom/line_2d.cpp \
    geom/matrix_2d.cpp \
    geom/polygon_2d.cpp \
    geom/ray_2d.cpp \
    geom/rect_2d.cpp \
    geom/sector_2d.cpp \
    geom/segment_2d.cpp \
    geom/triangle_2d.cpp \
    geom/vector_2d.cpp \
    ai/agent.cpp \
    ai/navigation.cpp \
    ai/play.cpp \
    ai/tactic.cpp \
    ai/play/playtest.cpp \
    ai/tactic/tactictest.cpp \
    ai/mapsearchnode.cpp \
    ai/tactic/tacticgoalie.cpp \
    ai/tactic/tacticdefender.cpp \
    ai/tactic/tacticstop.cpp \
    ai/tactic/tactichalt.cpp \
    ai/tactic/tacticattacker.cpp \
    ai/knowledge.cpp \
    ai/tactic/tacticballtracker.cpp \
    ai/tactic/tacticcircle.cpp \
    ai/tactic/tacticformation.cpp \
    ai/play/playstop.cpp \
    ai/play/playhalt.cpp \
    ai/play/playfreekickopp.cpp \
    ai/play/playfreekickour.cpp \
    ai/play/playgameon.cpp \
    ai/play/playkickoffopp.cpp \
    ai/play/playkickoffour.cpp \
    ai/play/playpenaltyopp.cpp \
    ai/play/playpenaltyour.cpp \
    ai/tactic/tactictestkickrange.cpp \
    ai/tactic/tactictestkickprecision.cpp \
    ai/play/playcalibration.cpp \
    ai/tactic/tactictestfriction.cpp \
    ai/play/playgameondefensive.cpp

HEADERS  += \
    etc/settings.h \
    etc/constants.h \
    etc/base.h \
    grSim/grSim_Replacement.pb.h \
    grSim/grSim_Packet.pb.h \
    grSim/grSim_Commands.pb.h \
    ssl/worldmodel.h \
    ssl/sslreceiver.h \
    ssl/sslrefbox.h \
    ssl/sslrefbox/ref_protocol.h \
    ssl/sslrefbox/game_state.h \
    ssl/sslrefbox/commands.h \
    ssl/sslvision.h \
    ssl/sslvision/messages/messages_robocup_ssl_wrapper.pb.h \
    ssl/sslvision/messages/messages_robocup_ssl_refbox_log.pb.h \
    ssl/sslvision/messages/messages_robocup_ssl_geometry.pb.h \
    ssl/sslvision/messages/messages_robocup_ssl_detection.pb.h \
    ui/mainwindow.h \
    ui/renderarea.h \
    util/util.h \
    util/fpscounter.h \
    output/wpacket.h \
    output/transmitter.h \
    output/grsim.h \
    output/outputbuffer.h \
    output/grpacket.h \
    ssl/mobileobject.h \
    ssl/ball.h \
    ssl/robot.h \
    output/controller.h \
    ssl/position.h \
    ai/ai.h \
    ai/robotcommand.h \
    geom/angle_deg.h \
    geom/circle_2d.h \
    geom/composite_region_2d.h \
    geom/convex_hull.h \
    geom/delaunay_triangulation.h \
    geom/line_2d.h \
    geom/matrix_2d.h \
    geom/polygon_2d.h \
    geom/ray_2d.h \
    geom/rect_2d.h \
    geom/region_2d.h \
    geom/sector_2d.h \
    geom/segment_2d.h \
    geom/size_2d.h \
    geom/triangle_2d.h \
    geom/triangulation.h \
    geom/vector_2d.h \
    geom/geom.h \
    ai/soccer.h \
    ai/agent.h \
    ai/navigation.h \
    output/controller_struct.h \
    ai/play.h \
    ai/tactic.h \
    ai/play/playtest.h \
    ai/tactic/tactictest.h \
    3rdparty/stlastar.h \
    3rdparty/fsa.h \
    ai/mapsearchnode.h \
    ai/tactic/tacticgoalie.h \
    ai/tactic/tacticdefender.h \
    ai/tactic/tacticstop.h \
    ai/tactic/tactichalt.h \
    ai/tactic/tacticattacker.h \
    ai/knowledge.h \
    ai/tactic/tacticballtracker.h \
    ai/tactic/tacticcircle.h \
    ai/tactic/tacticformation.h \
    ai/tactics.h \
    ai/play/playstop.h \
    ai/play/playhalt.h \
    ai/play/playfreekickopp.h \
    ai/play/playfreekickour.h \
    ai/play/playgameon.h \
    ai/play/playkickoffopp.h \
    ai/play/playkickoffour.h \
    ai/play/playpenaltyopp.h \
    ai/play/playpenaltyour.h \
    ai/tactic/tactictestkickrange.h \
    ai/play/playcalibration.h \
    ai/tactic/tactictestkickprecision.h \
    ai/tactic/tactictestfriction.h \
    ai/play/playgameondefensive.h

FORMS    +=	ui/mainwindow.ui

OTHER_FILES += \
    ../config/settings.ini

RESOURCES += \
    resources.qrc

