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
    ~UdpServerThread();
    void abort();
protected:
    void run();

signals:
    void readingDatagrams(AzIrisInfo &irisInfo);
    void deletePerson(int personI);
public slots:
    void processPendingDatarams();

private:
    QUdpSocket m_udpServer;
    bool m_abort;
};

#endif // UDPSERVERTHREAD_H
