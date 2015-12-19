#include "runtime.h"

Runtime dzrun;

Runtime::Runtime()
{
    this->enrollMode=false;
    this->configSettings.readConfig();
}




