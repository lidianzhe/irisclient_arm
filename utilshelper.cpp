#include <stdlib.h>
#include "utilshelper.h"

UtilsHelper::UtilsHelper()
{

}

void UtilsHelper::setLocalhostIf(const char *ifname, const char *ipaddr, const char *netmask, const char *gateway, const char *dns)
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

void UtilsHelper::sendEnrollPerson(AzIrisInfo *personInfo)
{
    /*
    qDebug("send to server");
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    m_hostAddress=QHostAddress(dzrun.configSettings.hostAddress);
    m_port=dzrun.configSettings.port;
    quint64 dt=QDateTime::currentDateTime().toString("yyyyMMddhhmm").toLongLong();
    quint32 num = QDateTime::currentDateTime().toString("ddhhmmss").toInt();

    out<<quint16(0xAAFF)<<quint8(0x01)<<quint8(0)<<quint32(num)<<m_deviceSN.toAscii()
      <<quint32(personId)<<quint64(dt)<<dzrun.configSettings.flag;
    out.device()->seek(3);
    out<<quint8(block.size()-sizeof(quint16)-sizeof(quint8)*2);

    udpClient->writeDatagram(block,m_hostAddress,m_port);
    udpClient->writeDatagram(block,QHostAddress("192.168.0.3"),1234);
    out.device()->close();
*/
}

