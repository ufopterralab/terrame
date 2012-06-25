######################################################################
# Automatically generated by qmake (2.01a) seg fev 7 23:16:58 2011
######################################################################
# RODRIGO
# MODIFICADO MANUALMENTE
LIB_LUA_PATH= ../../dependencies/lua
LIB_MYSQL_PATH= ../../dependencies/mysql
LIB_TERRALIB_PATH = ../../dependencies/terralib
LIB_QWT_PATH= ../../dependencies/qwt
LIB_QT_PATH= ../../dependencies/qt

#   QT += main
QT += core gui network

CONFIG -= console
CONFIG += app_bundle

#DEFINES + TME_WIN32
# TME_BUILD will ignore main method code defined for udpReceiver

DEFINES += TME_BUILD
DEFINES += TME_LUA_5_1 TME_TERRALIB_RC3


QMAKE_CXXFLAGS_RELEASE += -g
QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS +=

TEMPLATE = app
TARGET = TerraME

INCLUDEPATH += \
    . \
    ../../src/core \
    ../../src/observer \
    ../../src/observer/types \
    ../../src/observer/observer/components/legend \
    ../../src/observer/observer/components/painter \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/MySQL \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/MySQL/include \
    $${LIB_TERRALIB_PATH}/src/terralib/kernel \
    $${LIB_TERRALIB_PATH}/src/terralib/functions \
    $${LIB_TERRALIB_PATH}/src/terralib/stat \
    $${LIB_TERRALIB_PATH}/src/terralib/utils \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/shapelib \
    $${LIB_TERRALIB_PATH}/src/shapelib \
    $${LIB_TERRALIB_PATH}/src/zlib \
    $${LIB_TERRALIB_PATH}/src/tiff \
    $${LIB_LUA_PATH}/include \
    $${LIB_QWT_PATH}/src

LIBS += \
    -L$${LIB_TERRALIB_PATH}/Release/linux-g++ \
    -L$${LIB_QWT_PATH}/lib \
    -L$${LIB_LUA_PATH}/lib \
    #-lmysqlclient \
    #-lz \
    -lqwt \
    -llua5.1 \
    -ljpeg \
    -lshapelib \
    -lterralibtiff \
    -lterralib \
    -lte_mysql

DEPENDPATH= . \
    ../../src/lua \
    ../../src/core \
    ../../src/observer \
    ../../src/observer/types \
    ../../src/observer/types/legendWindow \
    ../../src/observer/types/painterWidget \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/MySQL \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/MySQL/include \
    $${LIB_TERRALIB_PATH}/src/terralib/kernel \
    $${LIB_TERRALIB_PATH}/src/terralib/functions \
    $${LIB_TERRALIB_PATH}/src/terralib/stat \
    $${LIB_TERRALIB_PATH}/src/terralib/utils \
    $${LIB_TERRALIB_PATH}/src/terralib/drivers/shapelib \
    $${LIB_TERRALIB_PATH}/src/shapelib \
    $${LIB_TERRALIB_PATH}/src/zlib \
    $${LIB_TERRALIB_PATH}/src/tiff \
    $${LIB_LUA_PATH}/include \
    $${LIB_QWT_PATH}/src

# Input
HEADERS += \
    ../../src/core/agent.h \
   ../../src/core/bridge.h \
   ../../src/core/cell.h \
   ../../src/core/cellularSpace.h \
   ../../src/core/composite.h \
   ../../src/core/controlMode.h \
   ../../src/core/environment.h \
   ../../src/core/event.h \
   ../../src/core/loadNeighborhood.h \
   ../../src/core/luaAgent.h \
   ../../src/core/luaCell.h \
   ../../src/core/luaCellIndex.h \
   ../../src/core/luaCellularSpace.h \
   ../../src/core/luaControlMode.h \
   ../../src/core/luaEnvironment.h \
   ../../src/core/luaEvent.h \
   ../../src/core/luaFlowCondition.h \
   ../../src/core/luaGlobalAgent.h \
   ../../src/core/luaJumpCondition.h \
   ../../src/core/luaLocalAgent.h \
   ../../src/core/luaMessage.h \
   ../../src/core/luaNeighborhood.h \
   ../../src/core/luaRegion.h \
   ../../src/core/luaRule.h \
   ../../src/core/luaTimer.h \
   ../../src/core/luaTrajectory.h \
   ../../src/core/luaUtils.h \
   ../../src/core/luna.h \
   ../../src/core/message.h \
   ../../src/core/model.h \
   ../../src/core/neighborhood.h \
   ../../src/core/process.h \
   ../../src/core/region.h \
   ../../src/core/rule.h \
   ../../src/core/scheduler.h \
   ../../src/core/strategy.h \
   ../../src/core/terrameLua.h \
   ../../src/core/terrameLua5_0.h \
   ../../src/core/terrameLua5_1.h \
   ../../src/observer/cellSpaceSubjectInterf.h \
   ../../src/observer/cellSubjectInterf.h \
   ../../src/observer/environmentSubjectInterf.h \
   ../../src/observer/eventSubjectInterf.h \
   ../../src/observer/globalAgentSubjectInterf.h \
   ../../src/observer/localAgentSubjectInterf.h \
   ../../src/observer/schedulerSubjectInterf.h \
   ../../src/observer/observer.h \
   ../../src/observer/observerImpl.h \
   ../../src/observer/observerInterf.h \
   ../../src/observer/terrameIncludes.h \
   $${LIB_LUA_PATH}/include/lua.h \
           
    ../../src/observer/types/observerStateMachine.h \
    ../../src/observer/types/observerPlayer.h \
    ../../src/observer/types/agentObserverMap.h \
    ../../src/observer/types/agentObserverImage.h \
    ../../src/observer/types/stateMachine/node.h \
    ../../src/observer/types/stateMachine/edge.h \
    ../../src/observer/protocol/decoder/decoder.h \
    ../../src/observer/components/canvas.h \
    ../../src/observer/components/painter/painterWidget.h \
    ../../src/observer/components/painter/painterThread.h \
    ../../src/observer/components/legend/legendWindow.h \
    ../../src/observer/components/legend/legendColorUtils.h \
    ../../src/observer/components/legend/legendColorBar.h \
    ../../src/observer/components/legend/legendAttributes.h \
    ../../src/observer/components/console/modelConsole.h \
    ../../src/observer/types/player/observerPlayerGUI.h \
    ../../src/observer/types/image/observerImageGUI.h \
    ../../src/observer/types/observerUDPSender.h \
    ../../src/observer/types/observerTextScreen.h \
    ../../src/observer/types/observerTable.h \
    ../../src/observer/types/observerScheduler.h \
    ../../src/observer/types/observerMap.h \
    ../../src/observer/types/observerLogFile.h \
    ../../src/observer/types/observerImage.h \
    ../../src/observer/types/observerGraphic.h \
    ../../tests/observers/udpSender/receiver.h

SOURCES += \
    ../../src/core/environment.cpp \
    ../../src/core/event.cpp \
    ../../src/core/luaCell.cpp \
    ../../src/core/luaCellularSpace.cpp \
    ../../src/core/luaEvent.cpp \
    ../../src/core/luaLocalAgent.cpp \
    ../../src/core/luaNeighborhood.cpp \
    ../../src/core/luaTimer.cpp \
    ../../src/core/luaUtils.cpp \
    ../../src/core/model.cpp \
    ../../src/core/terrameLua.cpp \
    ../../src/core/luaGlobalAgent.cpp \
    ../../src/core/luaTrajectory.cpp \
    ../../src/core/luaEnvironment.cpp \
    ../../src/observer/cellSpaceSubjectInterf.cpp \
    ../../src/observer/cellSubjectInterf.cpp \
    ../../src/observer/environmentSubjectInterf.cpp \
    ../../src/observer/eventSubjectInterf.cpp \
    ../../src/observer/globalAgentSubjectInterf.cpp \
    ../../src/observer/localAgentSubjectInterf.cpp \
    ../../src/observer/schedulerSubjectInterf.cpp \
    ../../src/observer/observerImpl.cpp \
    ../../src/observer/observerInterf.cpp \
    ../../src/observer/types/observerStateMachine.cpp \
    ../../src/observer/types/observerScheduler.cpp \
    ../../src/observer/types/observerPlayer.cpp \
    ../../src/observer/types/observerImage.cpp \
    ../../src/observer/types/agentObserverImage.cpp \
    ../../src/observer/types/observerGraphic.cpp \
    ../../src/observer/components/console/modelConsole.cpp \
    ../../src/observer/protocol/decoder/decoder.cpp \
    ../../src/observer/components/painter/painterWidget.cpp \
    ../../src/observer/components/painter/painterThread.cpp \
    ../../src/observer/types/stateMachine/node.cpp \
    ../../src/observer/types/stateMachine/edge.cpp \
    ../../src/observer/components/canvas.cpp \
    ../../src/observer/components/legend/legendWindow.cpp \
    ../../src/observer/components/legend/legendColorUtils.cpp \
    ../../src/observer/components/legend/legendColorBar.cpp \
    ../../src/observer/components/legend/legendAttributes.cpp \
    ../../src/observer/types/player/observerPlayerGUI.cpp \
    ../../src/observer/types/image/observerImageGUI.cpp \
    ../../src/observer/types/observerUDPSender.cpp \
    ../../src/observer/types/observerTextScreen.cpp \
    ../../src/observer/types/observerTable.cpp \
    ../../src/observer/types/observerMap.cpp \
    ../../src/observer/types/observerLogFile.cpp \
    ../../src/observer/types/agentObserverMap.cpp \
    ../../tests/observers/udpSender/receiver.cpp \
    ../../tests/observers/udpSender/main.cpp

RESOURCES += ../../src/observer/observerResource.qrc

OTHER_FILES += \
    ../../src/lua/TerraME.lua \
    ../../src/lua/abm.lua \
    ../../tests/observers/test_observers_cell.lua \
    ../../tests/observers/test_observers_cellular_space.lua \
    ../../tests/observers/test_observer_timer.lua \
    ../../tests/observers/test_observers_trajectory.lua \
    ../../tests/observers/test_observers_timer.lua \
    ../../tests/observers/test_observers_neighborhood.lua \
    ../../tests/observers/test_observers_event.lua \
    ../../tests/observers/test_observers_environment.lua \
    ../../tests/observers/test_observers_automaton.lua \
    ../../tests/observers/test_observers_agent.lua \
    ../../tests/basics/test_trajectory.lua \
    ../../tests/basics/test_timer.lua \
    ../../tests/basics/test_neighborhood_create.lua \
    ../../tests/basics/test_neighborhood.lua \
    ../../tests/basics/test_miscelaneous.lua \
    ../../tests/basics/test_group.lua \
    ../../tests/basics/test_gis_integration.lua \
    ../../tests/basics/test_environment.lua \
    ../../tests/basics/test_coord.lua \
    ../../tests/basics/test_cellularspace.lua \
    ../../tests/basics/test_cell.lua \
    ../../tests/basics/test_agent_automaton.lua \
    ../../tests/lunatest/test_luna_test.lua \
    ../../tests/observers/udpSender/udpReceiver.pro \
    ../../src/lua/observers.lua

FORMS += \
    ../../src/observer/types/image/observerImageGUI.ui \
    ../../src/observer/types/player/observerPlayerGUI.ui \
    ../../src/observer/components/console/modelConsoleGUI.ui