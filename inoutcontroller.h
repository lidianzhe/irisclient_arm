#ifndef INOUTCONTROLLER_H
#define INOUTCONTROLLER_H
#include <QSqlDatabase>
#include <QList>
#include "inoutinfo.h"
class InoutController
{
public:
    InoutController();
    InoutController(QSqlDatabase &db);

    bool addInout(InoutInfo &inout);
    QList<InoutInfo *> m_inoutList;
    void setDatabase(QSqlDatabase &db){m_db=db;}
private:
    QSqlDatabase m_db;
};

#endif // INOUTCONTROLLER_H
