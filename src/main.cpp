#include "MP4player.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MP4player w;
    w.show();
    return a.exec();
}