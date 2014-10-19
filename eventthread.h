#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H

#include "cmirislib2.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class EventThread : public QThread
{
	Q_OBJECT

public:
	explicit EventThread(QObject *parent, CMI_HANDLE cmiHandle);
	~EventThread();

	void abort();

signals:
	void preview(CMI_IMAGE_INFO *imageInfo);
	void unselectedImage(CMI_IMAGE_INFO *imageInfo);
	void selectedImages(CMI_IMAGE_INFO *imageInfo);
	void eyeCaptureDone();
    void indicator(int indicatorValue);
    void matchedFaceImages(CMI_IMAGE_INFO *cmiImageInfo);

protected:
    void run();

private:
	CMI_HANDLE m_cmiHandle;
	bool m_abort;
};

#endif // EVENTTHREAD_H
