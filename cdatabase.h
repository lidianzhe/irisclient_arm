#ifndef CDATABASE_H
#define CDATABASE_H

#include "dbrecord.h"
#include <QtSql/QSqlDatabase>
#include <QList>
#if defined(_SENDTOSERVER)
#include "inoutinfo.h"
#endif
class QSqlTableModel;

class CDataBase
{
public:
    CDataBase();
	~CDataBase();

    int open();
	void close();
	bool insert(DBRecord &record);
	int recordListSize() {return m_recordList.size();}


	QList<DBRecord *> m_recordList;
#if defined(_SENDTOSERVER)
    //
    QSqlDatabase db() {return m_db;}
    bool addInout(InoutInfo &inout);
    QList<InoutInfo *> m_inoutList;
#endif
private:
    QSqlDatabase m_db;
	QSqlTableModel *m_sqlModel;
	int m_maxID;
};

#endif // CDATABASE_H
