#include "dialogname.h"
#include "ui_dialogname.h"

DialogName::DialogName(QWidget *parent) :
	QDialog(parent), ui(new Ui::DialogName)
{
	ui->setupUi(this);

	connect(ui->lineEdit_Name, SIGNAL(textEdited(QString)), this, SLOT(readName(QString)));
 }

void DialogName::readName(QString name) {
	m_name = name;
}
