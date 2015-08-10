#ifndef INOUTINFO_H
#define INOUTINFO_H
#include <QString>
#include <QDateTime>
class InoutInfo
{
public:
    InoutInfo();
    InoutInfo(InoutInfo &inout);
    const int pid() {return m_pid;}
    const QString &deviceNo() {return m_deviceNo;}
    const int personId() {return m_personId;}
    const QDateTime cardTime() {return m_cardTime;}
    const qint8 flag() {return m_flag;}
    const qint8 seriesId() {return m_seriesId;}
    const int if_UserNo(){return m_if_UserNo;}
    const int nums(){return m_nums;}

    void setPid(int pid){m_pid=pid;}
    void setDeviceNo(QString &deviceNo){m_deviceNo=deviceNo;}
    void setPersonId(int personId) {m_personId=personId;}
    void setCardTime(QDateTime cardTime){m_cardTime=cardTime;}
    void setFlag(qint8 flag){m_flag=flag;}
    void setSeriesId(qint8 seriesId){m_seriesId=seriesId;}
    void setif_UserNo(int userNo){m_if_UserNo=userNo;}
    void setNums(int _nums){m_nums = _nums;}
private:
    int m_pid;
    QString m_deviceNo;
    int m_personId;
    QDateTime m_cardTime;
    qint8 m_flag;
    qint8 m_seriesId;
    int m_if_UserNo;
    int m_nums;
};

#endif // INOUTINFO_H
