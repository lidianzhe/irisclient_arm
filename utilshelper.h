#ifndef UTILSHELPER_H
#define UTILSHELPER_H
#include <stdio.h>

class utilshelper
{
public:
    utilshelper();
    void setLocalhostIf(const char* ifname,const char* ipaddr,const char* netmask,const char* gateway,const char* dns);
};

#endif // UTILSHELPER_H
