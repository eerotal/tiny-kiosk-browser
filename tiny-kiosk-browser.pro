VERSION = 0.1.0

TEMPLATE = app
TARGET = tiny-kiosk-browser
INCLUDEPATH += .

HEADERS += Core/Inc/browser.hpp
SOURCES += Core/Src/browser.cpp Core/Src/main.cpp

QT += widgets webengine webenginewidgets

DEFINES += VERSION=\\\"$$VERSION\\\"
