#include "inoutcontroller.h"
#include "inoutinfo.h"
#include <QSqlQuery>
#include <QSqlDatabase>
InoutController::InoutController()
{

}
InoutController::InoutController(QSqlDatabase &db)
{
    m_db = db;
}

bool InoutController::addInout(InoutInfo &inout)
{
    QSqlQuery cmd(m_db);
    cmd.prepare("insert into InoutDetails("
                  "DeviceNo,CardTime,Flag,SeriesId,if_UserNo) values ("
                  ":DeviceNo,:CardTime,:Flag,:SeriesId,:if_UserNo)"
                  );
    cmd.bindValue(":DeviceNo",QVariant(inout.deviceNo()));
    cmd.bindValue(":CardTime",QVariant(inout.cardTime()));
    cmd.bindValue(":Flag",QVariant(inout.flag()));
    cmd.bindValue(":SeriesId",QVariant(inout.seriesId()));
    cmd.bindValue(":if_UserNo",QVariant(inout.if_UserNo()));
    if(cmd.exec()){
        InoutInfo *newInout = new InoutInfo(inout);
        m_inoutList << newInout;
    }else{
        qDebug()<<cmd.lastError();
        return false;
    }

}
