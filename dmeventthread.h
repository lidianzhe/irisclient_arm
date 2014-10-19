#ifndef DMEVENTTHREAD_H
#define DMEVENTTHREAD_H

#include "cmirislib2.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class DMEventThread : public QThread
{
	Q_OBJECT

public:
	explicit DMEventThread(QObject *parent, CMI_HANDLE cmiHandle);
	~DMEventThread();

	void abort();

signals:
	void deviceRemoved(CMI_DEVICE_INFO *deviceInfo);
	void deviceArrived(CMI_DEVICE_INFO *deviceInfo);
	void suspended();
	void resumed();
    void switchChanged(int eventID);


protected:
    void run();

private:
	CMI_HANDLE m_cmiHandle;
	bool m_abort;
};

#endif // DMEVENTTHREAD_H
