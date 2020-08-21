#ifndef ARMSYNCWORKER_H
#define ARMSYNCWORKER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <iostream>

struct rawData
{
    double R[3][3];
    double p[3];
    float clawAction;
    float MOTOR6;
};

class ArmSyncWorker : public QObject
{
    Q_OBJECT
public:
    explicit ArmSyncWorker(QObject *parent = nullptr);
    ~ArmSyncWorker();
    void convertData(rawData*);


private:
    QTcpServer *tcpServer; //定义监听套接字tcpServer
    QList<QTcpSocket*> tcpClient;
    QTcpSocket *tcpSocket; //定义通信套接字tcpSocket
    rawData recv1; //TCP接受缓存
    QSerialPort *serial;
    QTime t3;
    bool isRunning;
    QTimer *timer;

private slots:
    void ServerNewConnection();
    void ServerNewConnection1();
signals:
    void recvData(rawData);
};



#endif // ARMSYNCWORKER_H
