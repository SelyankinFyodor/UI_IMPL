QT -= gui

TEMPLATE = lib
DEFINES += IPROBLEM_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    IProblem_Impl.cpp

HEADERS += \
    IBrocker.h \
    ICompact.h \
    IProblem_global.h \
    IProblem.h \
    IVector.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -llibvector.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -llibvector.dlld
else:unix: LIBS += -L$$PWD/../../bin/Selyankin/ -llibvector.dll

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -lliblogger.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -lliblogger.dlld
else:unix: LIBS += -L$$PWD/../../bin/Selyankin/ -lliblogger.dll

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -llibcompact.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/Selyankin/ -llibcompact.dlld
else:unix: LIBS += -L$$PWD/../../bin/Selyankin/ -llibcompact.dll

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
