#include <QCoreApplication>

#include "server.h"
class Server;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *server = new Server(0);

    return a.exec();
}
