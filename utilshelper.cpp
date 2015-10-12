#include <stdlib.h>
#include "utilshelper.h"

utilshelper::utilshelper()
{
}

void utilshelper::setLocalhostIf(const char *ifname, const char *ipaddr, const char *netmask, const char *gateway, const char *dns)
{
    const char *interfaces="/etc/network/interfaces";
    const char *dnsfile = "/etc/resolv.conf";
    const char *networking="/etc/init.d/networking restart";

    FILE *file=fopen(interfaces,"wr");

    fprintf(file,"auto lo\n");
    fprintf(file,"iface lo inet loopback\n");
    fprintf(file,"auto %s\n",ifname);
    fprintf(file,"iface %s inet static\n",ifname);
    fprintf(file,"address %s\n",ipaddr );
    fprintf(file,"netmask %s\n",netmask );
    fprintf(file,"gateway %s\n",gateway );
    fclose(file);

    file = fopen(dnsfile,"wr");
    fprintf(file,"nameserver %s\n",dns);
    fclose(file);

    system(networking);
}

