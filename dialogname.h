#ifndef DIALOGNAME_H
#define DIALOGNAME_H

#include <QDialog>
#include <QString>

namespace Ui {
	class DialogName;
}

class DialogName : public QDialog
{
    Q_OBJECT
public:
    explicit DialogName(QWidget *parent = 0);

	QString name() {return m_name;}

signals:

public slots:
	void readName(QString name);

private:
	Ui::DialogName *ui;
	QString m_name;
	bool m_isAccepted;

};

#endif // DIALOGNAME_H
