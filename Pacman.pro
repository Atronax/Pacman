TEMPLATE = app

QT += core gui widgets
CONFIG += c++11

TARGET = Pacman

SOURCES += \
    Graph/edge.cpp \
    Graph/graph.cpp \
    Graph/node.cpp \
    Graph/tree.cpp \
    main.cpp \
    Path/grid.cpp \
    Path/map.cpp \
    player.cpp \
    game.cpp \
    enemy.cpp \
    animgraphicsitem.cpp \
    food.cpp \
    button.cpp

HEADERS += \
    Graph/edge.h \
    Graph/graph.h \
    Graph/node.h \
    Graph/tree.h \
    Path/grid.h \
    Path/map.h \
    player.h \
    game.h \
    enemy.h \
    animgraphicsitem.h \
    food.h \
    button.h

RESOURCES += \
    res.qrc

