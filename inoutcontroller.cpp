#include "inoutcontroller.h"
#include "inoutinfo.h"

#include <QSqlDatabase>

//#include <QCoreApplication>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

InoutController::InoutController()
{

}

InoutController::~InoutController()
{
    if (m_inoutList.size() != 0) {
        for (int i = 0; i < m_inoutList.size(); i++) {
            delete m_inoutList.at(i);
        }
        m_inoutList.clear();
    }
}
InoutController::InoutController(QSqlDatabase &db)
{
    m_db = db;
}

bool InoutController::addInout(InoutInfo &inout)
{

    QSqlQuery query(m_db);
    query.prepare("insert into InoutDetails("
                  "DeviceNo,PersonId,CardTime,Flag,SeriesId) values ("
                  ":DeviceNo,:PersonId,:CardTime,:Flag,:SeriesId)"
                  );
    query.bindValue(":DeviceNo",QVariant(inout.deviceNo()));
    query.bindValue(":CardTime",QVariant(inout.cardTime()));
    query.bindValue(":Flag",QVariant(inout.flag()));
    query.bindValue(":SeriesId",QVariant(inout.seriesId()));
    query.bindValue(":PersonId",QVariant(inout.personId()));
    if(query.exec()){
        //InoutInfo *newInout = new InoutInfo(inout);
        //m_inoutList << newInout;
    }else{
        qDebug()<<query.lastError();
        return false;
    }
}
