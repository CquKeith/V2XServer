#include "server.h"
#include "tcpclientsocket.h"
//#include <QMessageBox>
#include <QDebug>
#include <iostream>

using namespace std;

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    port = 12345;

    if(!this->listen(QHostAddress::AnyIPv4,port))
    {
//        QMessageBox::critical(0,tr("服务器"),tr("无法启动服务器：%1.").arg(this->errorString()));
        qDebug() << "无法启动服务器：" << this->errorString();

    }else{
        qDebug()<<"成功监听:"<<port;
    }
}
/*
 * 当有新的用户连接进来的时候 触发这个函数，参数是socket的描述符
 *  此函数 重构了父类QTcpServer类中的虚函数，当newConnection信号发射时，自动进入此函数
*/
void Server::incomingConnection(qintptr socketDescriptor)
{

    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);
    connect(tcpclientsocket,&TcpClientSocket::dataToServer,this,&Server::sendToAllClient);
//    connect(ui_counter,SIGNAL(signal_DeskNotValid(Server *,int)),tcpclientsocket,SLOT(slotDeskNotValid(Server *,int)));
//    connect(tcpclientsocket,&TcpClientSocket::aboutToClose,this,&Server::)


    connect(tcpclientsocket,SIGNAL(signal_discon(int)),this,SLOT(slotDisconnected(int)));

    tcpclientsocket->setSocketDescriptor(socketDescriptor);
   // 在服务器端中记录刚刚登陆进来的客户端
    TcpClientSocketList.append(tcpclientsocket);
    qDebug()<<socketDescriptor<<" online";

    qDebug()<<"online number:"<<TcpClientSocketList.size();


}
void Server::slotDisconnected(int descriptor)
{
    for(int i=0;i<TcpClientSocketList.count();i++)
    {
        TcpClientSocket *item = TcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            qDebug()<<descriptor<<" offline";
            TcpClientSocketList.removeAt(i);
            item->deleteLater();

            qDebug()<<"online number:"<<TcpClientSocketList.size();
            return;
        }
    }

}

void Server::sendToAllClient(QByteArray ba,int socketDesc)
{
    for(int i=0;i<TcpClientSocketList.count();i++)
    {
        TcpClientSocket *item = TcpClientSocketList.at(i);
        if(item->socketDescriptor()!=socketDesc)
        {
            item->resetHeatBeatTime();
            item->write(ba);
            item->flush();
//            item->waitForBytesWritten();
        }
    }
}

