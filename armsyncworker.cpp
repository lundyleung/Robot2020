/*
 * @Descripttion:
 * @version:
 * @Author: Qingmao Wei
 * @Date: 2020-06-30 15:00:42
 * @LastEditors: Qingmao Wei
 * @LastEditTime: 2020-07-03 08:39:06
 */
#include "armsyncworker.h"
#include "kinemetics.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include <QDebug>
#include <QDataStream>
#include <stdio.h>
#include <string.h>
//#include <Windows.h>
#include <ctime>

using namespace std;
typedef union
{

    float fdata;

    quint8 data[4];

}byte4;

float raw[16];
ArmSyncWorker::ArmSyncWorker(QObject *parent) : QObject(parent)
{

    //tcpServer = new QTcpServer(this);
    serial = new QSerialPort(this);
    timer = new QTimer(this);
    //设置串口名
    serial->setPortName("/dev/ttyUSB0");
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud115200);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
//    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
//    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //打开串口
    serial->open(QIODevice::ReadWrite);
    //断开连接消息
//    if(!tcpServer->listen(QHostAddress::Any, 22345))
//    {
//        qDebug()<<"TCP listen failed.";
//        return;
//    }
    //创建串口对象

    isRunning = 0;
    connect(serial, SIGNAL(readyRead()), this, SLOT(ServerNewConnection1()));
    connect(timer, SIGNAL(timeout()), this, SLOT(ServerNewConnection()));
}

ArmSyncWorker::~ArmSyncWorker()
{
}


void ArmSyncWorker::ServerNewConnection1()
{
    timer->start(10);

}
void ArmSyncWorker::ServerNewConnection()
{
    timer->stop();
    isRunning = 1;
    //float newf[15];
    QByteArray newa = serial->readAll();
    QString newh,str1[4];
    quint8 num[4];
    byte4 newd;
    bool ok;
    newh = newa.toHex();
    qDebug()<<newh<<endl;
    for (int i = 0; i < 16; ++i) {
        for (int var = 0; var < 4; ++var) {
                str1[0+var]=newh.mid(i*8+2*var,2);
                newd.data[var] = str1[var].toInt(&ok, 16);}
                raw[i]=newd.fdata;
    }

    if(raw[0]==8808&&raw[15]==8888) {
        recv1.R[0][0]=raw[1];
        recv1.R[0][1]=raw[2];
        recv1.R[0][2]=raw[3];
        recv1.R[1][0]=raw[4];
        recv1.R[1][1]=raw[5];
        recv1.R[1][2]=raw[6];
        recv1.R[2][0]=raw[7];
        recv1.R[2][1]=raw[8];
        recv1.R[2][2]=raw[9];
        recv1.p[0]=raw[10];
        recv1.p[1]=raw[11];
        recv1.p[2]=raw[12];
        recv1.clawAction=raw[13];
        recv1.MOTOR6=raw[14];
        qDebug()<<"xray: "<< recv1.R[0][0]<< recv1.R[0][1]<< recv1.R[0][2]
                << recv1.R[1][0]<< recv1.R[1][1]<< recv1.R[1][2]
                << recv1.R[2][0]<< recv1.R[2][1]<< recv1.R[2][2]
                <<"/"<<recv1.p[0]<<recv1.p[1]<<recv1.p[2]
                <<"/"<<recv1.clawAction<<recv1.MOTOR6<<endl;
        isRunning = 0;
        if (isRunning == 0){
            emit recvData(recv1);
        }
    }

}
void ArmSyncWorker::convertData(rawData* data)
{

}
