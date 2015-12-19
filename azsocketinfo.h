#ifndef AZSOCKETINFO_H
#define AZSOCKETINFO_H
#include <Qt>

class AzSocketInfo
{
public:
    AzSocketInfo();

    quint16 commandHead;
    qint8 command;
    qint16 dataSize;
    qint32 nums;

    unsigned char *pData;
};

#endif // AZSOCKETINFO_H
