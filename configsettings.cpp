#include "configsettings.h"
#include <QSqlDatabase>

//#include <QCoreApplication>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include "mainwindow.h"
ConfigSettings::ConfigSettings()
{
}
void ConfigSettings::readConfig()
{

    QSqlQuery query(m_db);
    query.setForwardOnly(true);
    query.exec("select DeviceNo,SeriesId,Mode,HostAddress,HostPort,flag from ConfigSettings limit 1;");
    if(query.next()){
        deviceSN = query.value(0).toString();
        seriesId = query.value(1).toInt();
        mode =query.value(2).toInt();
        flag = (mode==0) ? 0 : 1;
        hostAddress = query.value(3).toString();
        port = query.value(4).toInt();
        allowSwitchMode=query.value(5).toInt();
        allowEnroll=query.value(6).toInt();
    }else
    {
        //deviceSN=m_deviceSN;
        mode=0;
        flag=0;
        seriesId=2;       
        hostAddress="192.168.0.200";
        port=1234;
        allowSwitchMode=0;
        allowEnroll=0;
    }
}
