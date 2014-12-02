#ifndef AZIRISINFO_H
#define AZIRISINFO_H
#include <QByteArray>
#include <QDateTime>
#include <Qt>
class AzIrisInfo
{
public:
    AzIrisInfo();
    QString commandHead;
    qint8 command;
    qint16 dataSize;
    qint32 pid;

    QString deviceSN;
    qint32 personId;
    QByteArray leftIrisTemplate;
    QByteArray rightIrisTemplate;

    qint32 if_UserNo;
};

#endif // AZIRISINFO_H
