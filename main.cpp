#include <QCoreApplication>
#include <QMap>
#include "server.h"
class Server;

#define INCLUDE_SENDER "include_sender"
#define HEART_BEAT "heart_beat"
#define PORT "port"
#define HELP "help"

QMap<QString,int> argMap;

void init(){
    // 是否包括发送者
    argMap.insert(INCLUDE_SENDER,0);

    // 最长多少秒没有活动（发送或者接受），服务器就主动关闭连接
    argMap.insert(HEART_BEAT,120);

    // 端口
    argMap.insert(PORT,12345);

    //help
//    argMap.insert(HELP,0);

}

int main(int argc, char *argv[])
{
    init();

    QCoreApplication a(argc, argv);

//    qDebug()<<argc;
    for(int i=1;i<argc;i+=2){
        argMap[QString(argv[i]).replace("-","")] = QString(argv[i+1]).toInt();
    }

    if(argMap.find(HELP) != argMap.end()){

        qDebug()<<"accep parameter table:"<<endl
                <<"-"<<INCLUDE_SENDER<<"1:server will brocast all user include sender,0:will exclude sender"<<endl
                <<"-"<<HEART_BEAT<<"server will disconnect this unactivity link after seconds, unit:s"<<endl
                <<"-"<<PORT<<"server will listen on this port"<<endl;

        return 0;
    }


    foreach (QString key, argMap.keys()) {
        qDebug()<<key<<"  "<<argMap[key];
    }


    new Server(0,argMap[PORT],argMap[INCLUDE_SENDER],argMap[HEART_BEAT]);

    return a.exec();
}
