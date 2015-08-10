#ifndef AZSOCKETINFO_H
#define AZSOCKETINFO_H
#include <QByteArray>
#include <QDateTime>
#include <Qt>
#include "azirisinfo.h"
class AzSocketInfo
{
public:
    AzSocketInfo();
    QString commandHead;
    qint8 command;
    qint16 dataSize;

    AzIrisInfo azirisInfo;
};
#endif // AZSOCKETINFO_H
