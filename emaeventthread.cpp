#include "emaeventthread.h"

#include <QDebug>

EmaEventThread::EmaEventThread(QObject *parent, EMA_HANDLE emaHandle) :
        QThread(parent)
{
     m_emaHandle = emaHandle;
     m_abort = false;
     m_emaEvent = NULL;
}

EmaEventThread::~EmaEventThread()
{
    if (isRunning()) {
        abort();
        wait();
    }
}

void EmaEventThread::abort()
{
    m_abort = true;
    ema_clearEventQueue(m_emaHandle);
    ema_cancelPendingReadEvent(m_emaHandle);
}

void EmaEventThread::run() {
    if (!m_emaHandle) return;

    m_abort = false;
    int ret;
    if (m_emaEvent) delete m_emaEvent;
    m_emaEvent = NULL;

    m_emaEvent = new EMA_EVENT;
    m_emaEvent->cbSize = sizeof(EMA_EVENT);

    while (1) {
        if (m_abort) {
            qDebug() << "EmaEventThread run() aborted";
            if (m_emaEvent) delete m_emaEvent;
            return;
        }

        ret = ema_readEvent(m_emaHandle, m_emaEvent, 10);

        if (ret == EMA_ERROR_INVALID_HANDLE) {
            if (m_emaEvent) delete m_emaEvent;
            return;
        }
        else if (ret == EMA_ERROR_READ_EVENT_CANCELLED) continue;
        else if (ret == EMA_ERROR_WAIT_TIMEOUT) continue;
        else if (ret == EMA_SUCCESS) {
            emit dataReceived(m_emaEvent);
            m_emaEvent = new EMA_EVENT;
            m_emaEvent->cbSize = sizeof(EMA_EVENT);
        }
    }
}
