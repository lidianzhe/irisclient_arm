#ifndef AZSOCKETINFO_H
#define AZSOCKETINFO_H


class AzSocketInfo
{
public:
    AzSocketInfo();

    QString commandHead;
    qint8 command;
    qint16 dataSize;

    unsigned char *pData;
};

#endif // AZSOCKETINFO_H
