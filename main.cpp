#include <QApplication>

#include "game.h"
int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    Game *game = new Game(QSize(16,12));
    game->showMaximized();

    return app.exec();
}
