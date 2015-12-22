#ifndef RUNTIME_H
#define RUNTIME_H
#include "configsettings.h"
#include "azirisinfo.h"
#include <QUdpSocket>

class Runtime
{
public:
    Runtime();
    ConfigSettings configSettings;

    bool enrollMode;
    AzIrisInfo *enrollPerson;

};

extern Runtime dzrun;
extern QUdpSocket *udpClient;
#endif // RUNTIME_H
