TARGET = pixedrawing

QMAKE_CXX = clang++

CONFIG += c++17

QT += core widgets

SOURCES += \
    main.C \
    drawingpanel.C \
    mainwindow.C \
    customredimdialog.C \
    wheelfilter.C \
    drawingpanelwrapper.C \
    helpers.C \
    undocommands.C \
    docklayers.C \
    layerset.C

HEADERS += \
    drawingpanel.H \
    mainwindow.H \
    customredimdialog.H \
    wheelfilter.H \
    drawingpanelwrapper.H \
    helpers.H \
    undocommands.H \
    docklayers.H \
    layerset.H

FORMS += \
    customredimdialog.ui

RESOURCES += \
    images.qrc
