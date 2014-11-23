#ifndef INOUTCONTROLLER_H
#define INOUTCONTROLLER_H
#include <QSqlDatabase>
#include <QList>
class InoutController
{
public:
    InoutController();
    InoutController(QSqlDatabase &db);

    bool addInout(InoutInfo &inout);
    QList<InoutInfo *> m_inoutList;
private:
    QSqlDatabase m_db;
};

#endif // INOUTCONTROLLER_H
