#include "udpserverthread.h"
#include <QByteArray>
#include <QDebug>
UdpServerThread::UdpServerThread(QObject *parent) :
    QThread(parent)
{
    //m_udpServer.bind(QHostAddress::LocalHost,1234);
    m_udpServer.bind(1234);
    connect(&m_udpServer,SIGNAL(readyRead()),this,SLOT(processPendingDatarams()));
}

UdpServerThread::~UdpServerThread()
{
    abort();
}

void UdpServerThread::abort()
{
    m_abort=true;
    m_udpServer.close();
}

void UdpServerThread::run()
{
    m_abort=false;
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

    in>>cmdHead;
    QT_TRY{
    if(QString::number(cmdHead,16)=="ccff")
    {
        in>>irisInfo.command;
        switch(irisInfo.command)
        {
            case 0x02://personinfo
                in>>irisInfo.dataSize>>irisInfo.personId>>irisInfo.if_UserNo>>irisInfo.leftIrisTemplate
                    >>irisInfo.rightIrisTemplate;
                irisInfo.commandHead = QString::number(cmdHead,16).toUpper();
                qDebug()<<irisInfo.commandHead<<quint8(0x02);
                emit readingDatagrams(irisInfo);
                break;
            case 0x04://delete
                in>>irisInfo.dataSize>>irisInfo.personId;
                qDebug()<<"CC-FF-04";
                emit deletePerson(irisInfo.personId);
                break;
            default:
            break;
        }
    }
    }QT_CATCH(...){

    }
    in.device()->close();
}

