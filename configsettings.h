#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H
#include <QtCore>
#include <QSqlDatabase>
class ConfigSettings
{
public:
    ConfigSettings();    
    QString deviceSN;
    QString hostAddress;
    quint16 port;
    quint8 seriesId;
    quint8 flag;
    quint8 mode;

    void readConfig();
    void setDatabase(QSqlDatabase &db){m_db=db;}
private:
    QSqlDatabase m_db;
};

#endif // CONFIGSETTINGS_H