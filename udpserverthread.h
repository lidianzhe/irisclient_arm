#ifndef UDPSERVERTHREAD_H
#define UDPSERVERTHREAD_H

#include <QThread>
#include <QUdpSocket>
#include "azirisinfo.h"
class UdpServerThread : public QThread
{
    Q_OBJECT
public:
    explicit UdpServerThread(QObject *parent = 0);
protected:
    void run();
signals:
    
public slots:
    void processPendingDatarams();

private:
    QUdpSocket m_udpServer;
};

#endif // UDPSERVERTHREAD_H
