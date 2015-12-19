#ifndef UDPSERVERTHREAD_H
#define UDPSERVERTHREAD_H

#include <QThread>
#include <QUdpSocket>
#include "azsocketinfo.h"
#include "azirisinfo.h"
#include "configsettings.h"
#include "runtime.h"
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
    void deletePerson(int personId);
    void deleteRecord(int nums);
    void updateSettings(ConfigSettings *settings);
    void enrollPerson(AzIrisInfo &irisInfo);
public slots:
    void processPendingDatarams();

private:
    QUdpSocket m_udpServer;
    bool m_abort;
};

#endif // UDPSERVERTHREAD_H
