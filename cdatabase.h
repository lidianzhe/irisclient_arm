#ifndef CDATABASE_H
#define CDATABASE_H

#include "dbrecord.h"
#include <QtSql/QSqlDatabase>
#include <QList>

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

private:
	QSqlDatabase m_db;
	QSqlTableModel *m_sqlModel;
	int m_maxID;
};

#endif // CDATABASE_H
