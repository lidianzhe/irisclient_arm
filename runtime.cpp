#include "runtime.h"

Runtime dzrun;
QUdpSocket *udpClient;

Runtime::Runtime()
{
    this->enrollMode=false;
    this->enrollPerson = new AzIrisInfo();
    //this->configSettings.readConfig();
}




