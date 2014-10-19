#if defined(_EMALIB)

#ifndef EMAEVENTTHREAD_H
#define EMAEVENTTHREAD_H

#include "emalib.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class EmaEventThread : public QThread
{
    Q_OBJECT

public:
    explicit EmaEventThread(QObject *parent, EMA_HANDLE cmiHandle);
    ~EmaEventThread();

    void abort();

signals:
    void dataReceived(EMA_EVENT *event);

protected:
    void run();

private:
    EMA_HANDLE m_emaHandle;
    bool m_abort;
    EMA_EVENT *m_emaEvent;
};

#endif // EMAEVENTTHREAD_H

#endif
