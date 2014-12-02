#include "udpserverthread.h"
#include <QByteArray>
UdpServerThread::UdpServerThread(QObject *parent) :
    QThread(parent)
{
    //m_udpServer.bind(QHostAddress::LocalHost,1234);
    m_udpServer.bind(1234);
    connect(&m_udpServer,SIGNAL(readyRead()),this,SLOT(processPendingDatarams()));
}

void UdpServerThread::run()
{


}

void UdpServerThread::processPendingDatarams()
{
    QByteArray data;
    do{
        data.resize(m_udpServer.pendingDatagramSize());
        m_udpServer.readDatagram(data.data(),data.size());
    }while(m_udpServer.hasPendingDatagrams());

    QDataStream in(&data,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    AzIrisInfo irisInfo;
    quint16 cmdHead;


    in>>cmdHead>>irisInfo.command>>irisInfo.dataSize;
    in>>irisInfo.pid>>irisInfo.PersonId>>irisInfo.leftIrisTemplate>>irisInfo.rightIrisTemplate;
    irisInfo.commandHead = QString::number(cmdHead,16).toUpper();

    qDebug()<<irisInfo.commandHead<<QString::number(irisInfo.command,16);
    in.device()->close();
}

