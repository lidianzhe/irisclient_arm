#include "eventthread.h"
#include "mainwindow.h"
#include <QDebug>

EventThread::EventThread(QObject *parent, CMI_HANDLE cmiHandle) :
		QThread(parent)
{
	 m_cmiHandle = cmiHandle;
	 m_abort = false;
}

EventThread::~EventThread()
{
	if (isRunning()) {
		abort();
		wait();
	}
}

void EventThread::abort()
{
	m_abort = true;
	cmi_clearEventQueue(m_cmiHandle);
	cmi_cancelPendingReadEvent(m_cmiHandle);
}

void EventThread::run() {
	if (!m_cmiHandle) return;

	m_abort = false;
	DWORD ret;
	CMI_EVENT event;
	event.cbSize = sizeof(CMI_EVENT);
	event.imageInfo.cbSize = sizeof(CMI_IMAGE_INFO);
	CMI_IMAGE_INFO *cmiImageInfo;

	while (1) { 
		if (m_abort) {
			qDebug() << "EventThread run() aborted";
			return;
		}

		ret = cmi_readEvent(m_cmiHandle, &event, 100);

		if (ret == CMI_ERROR_INVALID_HANDLE) return;
		else if (ret == CMI_ERROR_READ_EVENT_CANCELLED) continue;
		else if (ret == CMI_ERROR_WAIT_TIMEOUT) continue;
		else if (ret == CMI_SUCCESS) {

			switch(event.event) {

			case CMI_EVENT_PREVIEW_FRAME_INFO:
				cmiImageInfo = new CMI_IMAGE_INFO;
				cmiImageInfo->cbSize = sizeof(CMI_IMAGE_INFO);
				*cmiImageInfo = event.imageInfo;
				emit preview(cmiImageInfo);
				break;

			case CMI_EVENT_IRIS_IMAGE_UNSELECTED:
				cmiImageInfo = new CMI_IMAGE_INFO;
				cmiImageInfo->cbSize = sizeof(CMI_IMAGE_INFO);
				*cmiImageInfo = event.imageInfo;
				emit unselectedImage(cmiImageInfo);
				break;

			case CMI_EVENT_IRIS_IMAGES_SELECTED:
				cmiImageInfo = new CMI_IMAGE_INFO;
				cmiImageInfo->cbSize = sizeof(CMI_IMAGE_INFO);
				*cmiImageInfo = event.imageInfo;
				emit selectedImages(cmiImageInfo);
				break;

			case CMI_EVENT_CAPTURE_DONE: // for capture sound
				emit eyeCaptureDone();
				break;

            case CMI_EVENT_INDICATOR: // for indicator changes and voice cues
                emit indicator(event.imageInfo.leftPosition);
                break;
			}
		}

	}
}
