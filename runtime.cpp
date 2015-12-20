#include "runtime.h"

Runtime dzrun;

Runtime::Runtime()
{
    this->enrollMode=false;
    this->enrollPerson = new AzIrisInfo();
    //this->configSettings.readConfig();
}




