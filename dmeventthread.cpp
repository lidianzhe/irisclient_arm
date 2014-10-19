#include "dmeventthread.h"
#include "mainwindow.h"
#include <QDebug>

DMEventThread::DMEventThread(QObject *parent, CMI_HANDLE cmiHandle) :
		QThread(parent)
{
	m_cmiHandle = cmiHandle;
	m_abort = false;
}

DMEventThread::~DMEventThread()
{
	if (isRunning()) {
		abort();
		wait();
	}
}

void DMEventThread::abort()
{
	m_abort = true;
	cmi_clearDMEventQueue(m_cmiHandle);
	cmi_cancelPendingReadDMEvent(m_cmiHandle);
}

void DMEventThread::run() {
	if (!m_cmiHandle) return;

	m_abort = false;
	DWORD ret;
	CMI_DM_EVENT dmEvent;
	dmEvent.cbSize = sizeof(CMI_DM_EVENT);
	dmEvent.deviceInfo.cbSize = sizeof(CMI_DEVICE_INFO);
	CMI_DEVICE_INFO *cmiDeviceInfo;

	while (1) {
		if (m_abort) {
			qDebug() << "DM EventThread run() aborted";
			return;
		}

		ret = cmi_readDMEvent(m_cmiHandle, &dmEvent, 100);

		if (ret == CMI_ERROR_INVALID_HANDLE) return;
		else if (ret == CMI_ERROR_READ_EVENT_CANCELLED) continue;
		else if (ret == CMI_ERROR_WAIT_TIMEOUT) continue;
		else if (ret == CMI_SUCCESS) {
			switch(dmEvent.event) {

			case CMI_EVENT_DEVICE_REMOVAL:
				cmiDeviceInfo = new CMI_DEVICE_INFO;
				cmiDeviceInfo->cbSize = sizeof(CMI_DEVICE_INFO);
				*cmiDeviceInfo = dmEvent.deviceInfo;
				emit deviceRemoved(cmiDeviceInfo);
				break;

			case CMI_EVENT_DEVICE_ARRIVAL:
				cmiDeviceInfo = new CMI_DEVICE_INFO;
				cmiDeviceInfo->cbSize = sizeof(CMI_DEVICE_INFO);
				*cmiDeviceInfo = dmEvent.deviceInfo;
				emit deviceArrived(cmiDeviceInfo);
				break;

			case CMI_EVENT_POWER_SUSPENDED:
				emit suspended();
				break;

			case CMI_EVENT_POWER_RESUMED:
				emit resumed();
				break;

            ///////////////////////////////////////////
            // EMA Events handle
            case CMI_EVENT_TAMPER_SWITCH_OPEN:
            case CMI_EVENT_TAMPER_SWITCH_CLOSED:
            case CMI_EVENT_MODE_KEY_PRESSED:
            case CMI_EVENT_MODE_KEY_RELEASED:
            case CMI_EVENT_FUNC_KEY_1_PRESSED:
            case CMI_EVENT_FUNC_KEY_1_RELEASED:
            case CMI_EVENT_FUNC_KEY_2_PRESSED:
            case CMI_EVENT_FUNC_KEY_2_RELEASED:
            case CMI_EVENT_FUNC_KEY_3_PRESSED:
            case CMI_EVENT_FUNC_KEY_3_RELEASED:
            case CMI_EVENT_FUNC_KEY_4_PRESSED:
            case CMI_EVENT_FUNC_KEY_4_RELEASED:

            case CMI_EVENT_MOVE_UP_PRESSED:
            case CMI_EVENT_MOVE_UP_RELEASED:
            case CMI_EVENT_MOVE_DOWN_PRESSED:
            case CMI_EVENT_MOVE_DOWN_RELEASED:
                emit switchChanged(dmEvent.event);
                qDebug() << "emit switchChanged" << dmEvent.event;
                break;
            }
		}
	}
}
