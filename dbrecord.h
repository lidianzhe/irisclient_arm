#ifndef DBRECORD_H
#define DBRECORD_H

#include <QString>

class DBRecord
{
public:
    DBRecord();
	DBRecord(DBRecord &record);

	const int id() {return m_id;}
	const QString &name() {return m_name;}
	const QByteArray &leftIrisTemplate() {return m_leftIrisTemplate;}
	const QByteArray &rightIrisTemplate() {return m_rightIrisTemplate;}
    const QByteArray &faceFeatureTemplate() {return m_faceFeatureTemplate;}
    const QString &leftIrisPath() {return m_leftIrisPath;}
    const QString &rightIrisPath() {return m_rightIrisPath;}
    const QString &faceImagePath() {return m_faceImagePath;}

	void setId(int id) {m_id = id;}
	void setName(QString &name) {m_name = name;}
	void setLeftIrisTemplate(QByteArray &leftIrisTemplate) {m_leftIrisTemplate = leftIrisTemplate;}
    void setRightIrisTemplate(QByteArray &rightIrisTemplate) {m_rightIrisTemplate = rightIrisTemplate;}
    void setFaceFeatureTemplate(QByteArray &faceFeatureTemplate) {m_faceFeatureTemplate = faceFeatureTemplate;}
    void setLeftIrisPath(QString &leftIrisPath) {m_leftIrisPath = leftIrisPath;}
    void setRightIrisPath(QString &rightIrisPath) {m_rightIrisPath = rightIrisPath;}
    void setFaceImagePath(QString &faceImagePath) {m_faceImagePath = faceImagePath;}

private:
	int m_id;
	QString m_name;
	QByteArray m_leftIrisTemplate;
	QByteArray m_rightIrisTemplate;
    QByteArray m_faceFeatureTemplate;
    QString m_leftIrisPath;
    QString m_rightIrisPath;
    QString m_faceImagePath;
};

#endif // DBRECORD_H
