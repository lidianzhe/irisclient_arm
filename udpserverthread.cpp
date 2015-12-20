#include "udpserverthread.h"
#include <QByteArray>
#include <QDebug>
#include "runtime.h"
UdpServerThread::UdpServerThread(QObject *parent) :
    QThread(parent)
{
    //m_udpServer.bind(QHostAddress::LocalHost,1234);
    m_udpServer.bind(1234,QUdpSocket::ShareAddress);
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

    AzSocketInfo sockInfo;

    AzIrisInfo irisInfo;
    //quint16 cmdHead;
    //qint8 command;
    //quint16 datasize;

    ConfigSettings *settings;
    QByteArray qbaHostAddress;
    QByteArray qbaDeviceSN;

    in>>sockInfo.commandHead>>sockInfo.command>>sockInfo.dataSize;

    QT_TRY{
    if(QString::number(sockInfo.commandHead,16)=="ccff")
    {
        switch(sockInfo.command)
        {
            case 0x01://setting
               settings= new ConfigSettings();
                in>>settings->pid>>qbaDeviceSN>>qbaHostAddress>>settings->port>>settings->seriesId>>settings->mode>>settings->allowSwitchMode
                >>settings->allowEnroll;
                settings->deviceSN=QString::fromAscii(qbaDeviceSN);
                settings->hostAddress=QString::fromAscii(qbaHostAddress);
                emit updateSettings(settings);
                qDebug()<<"CC-FF-01";
                break;
            case 0x02://personinfo
                irisInfo.command = sockInfo.command;
                irisInfo.dataSize = sockInfo.dataSize;
                in>>irisInfo.personId>>irisInfo.if_UserNo>>irisInfo.leftIrisTemplate
                    >>irisInfo.rightIrisTemplate;
                irisInfo.commandHead = QString::number(sockInfo.commandHead,16).toUpper();
                qDebug()<<irisInfo.commandHead<<quint8(0x02);
                emit readingDatagrams(irisInfo);
                break;
            case 0x03: //Enroll 注册人员
                dzrun.enrollMode = true;
                in>>irisInfo.pid>>irisInfo.personId>>irisInfo.if_UserNo;

                dzrun.enrollPerson->personId = irisInfo.personId;
                dzrun.enrollPerson->if_UserNo = irisInfo.if_UserNo;
                qDebug()<<"Enroll person "<<irisInfo.personId;
                emit enrollPerson(irisInfo);
                break;
            case 0x04://delete
                in>>irisInfo.personId;
                qDebug()<<"CC-FF-04";
                emit deletePerson(irisInfo.personId);
                break;
            default:
            break;
        }
    }
    if(QString::number(sockInfo.commandHead,16)=="ccaa") //answer
    {
        switch(sockInfo.command)
        {
            case 0x01:
                qint32 nums;
                in>>nums;
                emit deleteRecord(nums);

                break;
            default:
            break;
        }

    }
    }QT_CATCH(...){

    }
    in.device()->close();
}

