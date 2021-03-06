#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H
#include <QObject>
#include <QTcpSocket>
#include <server.h>

#include <QTimer>

//2min没有活动 则自动关闭连接
//#ifndef HEART_BEAT_NUMBER
//#define HEART_BEAT_NUMBER 120
//#endif

class Server;
class TcpClientSocket : public QTcpSocket
{
     Q_OBJECT  //添加这个宏  用于实现信号和槽的通信
public:
    TcpClientSocket(int heartBeatTime,QObject *parent = 0);
    void resetHeatBeatTime(int value){heart_beat_current = value;}

protected slots:
    //从客户端收到消息的处理
    void dataReceived();
    //用户下线的时候
    void slotDisconnected();

signals:
    void dataToServer(QByteArray,int);
    void signal_discon(int);
public :
//    int socketDescriptor;

private:
    QTimer *timer;
    qint16 heart_beat_current;//当前还剩多少秒 无活动之后 便自动关闭
    quint16 HEART_BEAT_NUMBER;
};

#endif // TCPCLIENTSOCKET_H
