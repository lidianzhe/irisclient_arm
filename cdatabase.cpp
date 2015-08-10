#include "cdatabase.h"

#include <QCoreApplication>
#include <QtDebug>
#include <QSqlError>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QStringList>

CDataBase::CDataBase()
{
	m_sqlModel = NULL;
}

CDataBase::~CDataBase()
{
	if (m_sqlModel) {
		delete m_sqlModel;
		m_sqlModel = NULL;
	}
	if (m_recordList.size() != 0) {
		for (int i = 0; i < m_recordList.size(); i++) {
			delete m_recordList.at(i);
		}
		m_recordList.clear();
	}
}

int CDataBase::open() {

    qDebug() << "CDataBase::open() called";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
#if defined(WIN32)
    //m_db.setDatabaseName("C:/cmimirdemo_WIN/2.0.3/Enroll.db");
    m_db.setDatabaseName("./Enroll.db");
#else
    //m_db.setDatabaseName("Enroll.db");
    m_db.setDatabaseName("/usr/local/share/aizhetech/Enroll.db");
#endif

	if(m_db.open()) {
		m_sqlModel = new QSqlTableModel(NULL, m_db);
		//m_sqlModel->setTable("EnrollTable");

                QSqlQuery query("CREATE TABLE IF NOT EXISTS EnrollTable"
                                "(Id INTEGER, Name TEXT, "
                                "LeftIrisTemplate VARCHAR(1024), "
                                "RightIrisTemplate VARCHAR(1024), "
                                "FaceFeatureTemplate VARCHAR(5120), "
                                "LeftIrisPath TEXT, "
                                "RightIrisPath TEXT, "
                                "FaceImagePath TEXT,"
                                "if_UserNo integer)");
                if (!query.exec()) return false;

                query.setForwardOnly(true);
                if (!query.exec("SELECT * FROM EnrollTable")) return false;

		if (m_recordList.size() != 0) {
			for (int i = 0; i < m_recordList.size(); i++) {
				delete m_recordList.at(i);
			}
			m_recordList.clear();
		}

		while (query.next()) {
			DBRecord *record = new DBRecord();

			record->setId(query.value(0).toInt());
            QString qstr;
            qstr = query.value(1).toString();
            record->setName(qstr);
            QByteArray qba;
            qba = query.value(2).toByteArray();
            record->setLeftIrisTemplate(qba);
            qba = query.value(3).toByteArray();
            record->setRightIrisTemplate(qba);
            qba = query.value(4).toByteArray();
            record->setFaceFeatureTemplate(qba);
            qstr = query.value(5).toString();
            record->setLeftIrisPath(qstr);
            qstr = query.value(6).toString();
            record->setRightIrisPath(qstr);
            qstr = query.value(7).toString();
            record->setFaceImagePath(qstr);

            //record->setif_UserNo(query.value(8).toInt());


			qDebug() << "Loaded Template Size left right" << record->leftIrisTemplate().size() <<
						record->rightIrisTemplate().size();

			m_recordList << record;
		}
        return 0;
	}
	else {
        return m_db.lastError().number();
	}
}

void CDataBase::close() {

    m_db.close();

    QStringList names;
    names = QSqlDatabase::connectionNames();
    if (names.size() > 0)
        QSqlDatabase::removeDatabase(names.at(0));

    if (m_sqlModel) {
        delete m_sqlModel;
        m_sqlModel = NULL;
    }

	if (m_recordList.size() != 0) {
		for (int i = 0; i < m_recordList.size(); i++) {
			delete m_recordList.at(i);
		}
		m_recordList.clear();
	}
}

bool CDataBase::insert(DBRecord &record) {

	QSqlQuery query(m_db);

    query.prepare("INSERT INTO EnrollTable (Id, Name, LeftIrisTemplate, RightIrisTemplate, FaceFeatureTemplate, LeftIrisPath, RightIrisPath, FaceImagePath,if_UserNo)"
                      "VALUES (:ID, :NAME, :LEFT_IRIS_TEMPLATE, :RIGHT_IRIS_TEMPLATE, :FACE_FEATURE_TEMPLATE, :LEFT_IRIS_PATH, :RIGHT_IRIS_PATH, :FACE_IMAGE_PATH,:if_UserNo)");
	query.bindValue(":ID", QVariant(record.id()));
	query.bindValue(":NAME", QVariant(record.name()));
	query.bindValue(":LEFT_IRIS_TEMPLATE", QVariant(record.leftIrisTemplate()));
	query.bindValue(":RIGHT_IRIS_TEMPLATE", QVariant(record.rightIrisTemplate()));
    query.bindValue(":FACE_FEATURE_TEMPLATE", QVariant(record.faceFeatureTemplate()));
    query.bindValue(":LEFT_IRIS_PATH", QVariant(record.leftIrisPath()));
    query.bindValue(":RIGHT_IRIS_PATH", QVariant(record.rightIrisPath()));
    query.bindValue(":FACE_IMAGE_PATH", QVariant(record.faceImagePath()));
    query.bindValue(":if_UserNo",QVariant(record.name()));
	if (query.exec()) {
		DBRecord *newRecord = new DBRecord(record);
		m_recordList << newRecord;
		return true;
	}
	else {
		qDebug() << query.lastError();
		return false;
    }
}

bool CDataBase::downloadIrisTemplate(AzIrisInfo &irisInfo)
{
    QSqlQuery query("select * from enrolltable where id="+QString::number(irisInfo.personId));
    if(query.next())
    {
        query.prepare("update enrolltable set Id=?,Name=?, LeftIrisTemplate=?,"
                      "RightIrisTemplate=? where id="+
                      QString::number(irisInfo.personId));
        query.addBindValue(QVariant(irisInfo.personId));
        query.addBindValue(QVariant(irisInfo.personId));
        query.addBindValue(QVariant(irisInfo.leftIrisTemplate));
        query.addBindValue(QVariant(irisInfo.rightIrisTemplate));
        if(query.exec()){
            for(int i=0;i<m_recordList.size();i++){
                if(m_recordList.at(i)->id()==irisInfo.personId){
                    m_recordList.at(i)->setLeftIrisTemplate(irisInfo.leftIrisTemplate);
                    m_recordList.at(i)->setRightIrisTemplate(irisInfo.rightIrisTemplate);
                    m_recordList.at(i)->setId(irisInfo.personId);
                    i=m_recordList.size();
                }
            }
            return true;
        }else{
            qDebug()<<"update error"<<query.lastError();
            return false;
        }
    }
    else{
        query.prepare("INSERT INTO EnrollTable (Id, Name, LeftIrisTemplate, RightIrisTemplate, FaceFeatureTemplate, LeftIrisPath, RightIrisPath, FaceImagePath,if_UserNo)"
                      "values(?,?,?,?,?,?,?,?,?)");
        query.addBindValue(QVariant(irisInfo.personId));
        query.addBindValue(QVariant(irisInfo.personId));
        query.addBindValue(QVariant(irisInfo.leftIrisTemplate));
        query.addBindValue(QVariant(irisInfo.rightIrisTemplate));

        query.addBindValue(QVariant(NULL));
        query.addBindValue(QVariant(NULL));
        query.addBindValue(QVariant(NULL));
        query.addBindValue(QVariant(NULL));
        query.addBindValue(QVariant(irisInfo.personId));

        if(query.exec()){
            DBRecord *newRecord = new DBRecord();
            newRecord->setId(irisInfo.personId);
            QString name=QString::number(irisInfo.personId);
            newRecord->setName(name);
            //newRecord->setif_UserNo(irisInfo.if_UserNo);
            newRecord->setLeftIrisTemplate(irisInfo.leftIrisTemplate);
            newRecord->setRightIrisTemplate(irisInfo.rightIrisTemplate);
            m_recordList << newRecord;
            return true;
        }else{
            qDebug()<<"insert error"<<query.lastError();
            return false;
        }
    }
}
//----------------------------------------------------------------------------
bool CDataBase::deletePerson(int personId)
{
    qDebug()<<"delete personid="<<personId;
    QSqlQuery query(m_db);
    if(personId==0)
    {
        QSqlQuery query("delete from enrolltable");
        if (m_recordList.size() != 0) {
            for (int i = 0; i < m_recordList.size(); i++) {
                delete m_recordList.at(i);
            }
            m_recordList.clear();
        }
    }
    else
    {
        QSqlQuery query("delete from enrolltable where id="+QString::number(personId));
        for (int i = 0; i < m_recordList.size(); i++) {
            if(m_recordList.at(i)->id()==personId){
                delete m_recordList.at(i);
                m_recordList.removeAt(i);
                i=m_recordList.size();
            }
        }
    }
    if(query.exec())
    {
        return true;
    }else
        return false;
}






