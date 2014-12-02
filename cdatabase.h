#ifndef CDATABASE_H
#define CDATABASE_H

#include "dbrecord.h"
#include <QtSql/QSqlDatabase>
#include <QList>
#if defined(_SENDTOSERVER)
#include "inoutinfo.h"
#include "azirisinfo.h"
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
    //for inoutcontroll
    QSqlDatabase db() {return m_db;}
    bool downloadIrisTemplate(AzIrisInfo &irisInfo);
#endif
private:
    QSqlDatabase m_db;
	QSqlTableModel *m_sqlModel;
	int m_maxID;
};

#endif // CDATABASE_H
