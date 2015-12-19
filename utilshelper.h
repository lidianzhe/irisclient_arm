#ifndef UTILSHELPER_H
#define UTILSHELPER_H
#include <stdio.h>
#include <azirisinfo.h>
#include "runtime.h"
class UtilsHelper
{
public:
    UtilsHelper();
    void setLocalhostIf(const char* ifname,const char* ipaddr,const char* netmask,const char* gateway,const char* dns);
    void sendEnrollPerson(AzIrisInfo *personInfo);
};

#endif // UTILSHELPER_H
