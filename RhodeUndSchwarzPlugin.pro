#/**********************************************************************************
#**                                                                               **
#**  Rhode&Schwarz Plugin for LabAnlyser, control&visualize Rhode&Schwarz data.   **
#**  Copyright (C) 2015-2021 Andreas Hoffmann                                     **
#**                                                                               **
#**  Rhode&Schwarz Plugin is free software: you can redistribute it and/or modify **
#**  it under the terms of the GNU General Public License as published by         **
#**  the Free Software Foundation, either version 3 of the License, or            **
#**  (at your option) any later version.                                          **
#**                                                                               **
#**  This program is distributed in the hope that it will be useful,              **
#**  but WITHOUT ANY WARRANTY; without even the implied warranty of               **
#**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                **
#**  GNU General Public License for more details.                                 **
#**                                                                               **
#**  You should have received a copy of the GNU General Public License            **
#**  along with this program.  If not, see http://www.gnu.org/licenses/.          **
#**                                                                               **
#***********************************************************************************
#***********************************************************************************/

QT       -= gui

TARGET = RhodeUndSchwarzPluginPlugin
TEMPLATE = lib

CONFIG         += plugin
QT             += widgets

OBJECTS_DIR=$$builddir
DESTDIR=$$builddir

INCLUDEPATH += C:/libraries/boost_1_59_0
INCLUDEPATH += $$PWD/../Rhode-SchwarzPlugin/include

DEFINES +=  TektronixPlugin_LIBRARY
DEFINES +=  WIN32_LEAN_AND_MEAN

SOURCES +=\
            ../LabAnalyser/plugins/InterfaceDataType.cpp \
            DataType/datastorage.cpp \
            LoadSave/XmlReader.cpp \
            Messenger/messengerclass.cpp \
            Osci/RhodeSchwarzClass.cpp \
            Osci/readoscichannel.cpp \
            RhodeUndSchwarzPlugin.cpp \
            Symbols/createsymbols.cpp \
            worker.cpp

HEADERS += \
            ../LabAnalyser/plugins/platforminterface.h\
            ../LabAnalyser/plugins/InterfaceDataType.h \
            DataType/datastorage.h \
            LoadSave/XmlReader.h \
            Messenger/messengerclass.h \
            Osci/RhodeSchwarzClass.h \
            Osci/readoscichannel.h \
            RhodeUndSchwarzPlugin.h \
            Symbols/createsymbols.h \
            worker.h


LIBS += -LC:/cpp/GitHub/Rhode-SchwarzPlugin/Lib_x64/msc -lvisa64

LIBS += -LC:/libraries/boost_1_59_0/boost-lib/lib -llibboost_system-mgw73-mt-1_59
LIBS += -LC:/libraries/boost_1_59_0/boost-lib/lib -llibboost_thread-mgw73-mt-1_59
LIBS += -lws2_32



TARGET          = $$qtLibraryTarget(RhodeUndSchwarzPlugin)
Debug:DESTDIR         = ../../plugins
Release:DESTDIR         = C:/LabAnalyser

#QMAKE_CXXFLAGS_RELEASE += -O1
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE += -O3
