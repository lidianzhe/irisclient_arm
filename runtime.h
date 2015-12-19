#ifndef RUNTIME_H
#define RUNTIME_H
#include "configsettings.h"
#include "azirisinfo.h"


class Runtime
{
public:
    Runtime();
    ConfigSettings configSettings;

    bool enrollMode;
    AzIrisInfo *enrollPerson;

};

extern Runtime dzrun;

#endif // RUNTIME_H
