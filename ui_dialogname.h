/********************************************************************************
** Form generated from reading UI file 'dialogname.ui'
**
** Created: Wed Oct 15 23:41:35 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGNAME_H
#define UI_DIALOGNAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogName
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_33;
    QLineEdit *lineEdit_Name;

    void setupUi(QDialog *DialogName)
    {
        if (DialogName->objectName().isEmpty())
            DialogName->setObjectName(QString::fromUtf8("DialogName"));
        DialogName->resize(400, 301);
        buttonBox = new QDialogButtonBox(DialogName);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_33 = new QLabel(DialogName);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(30, 80, 71, 31));
        QPalette palette;
        QBrush brush(QColor(0, 85, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_33->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Calibri"));
        font.setPointSize(18);
        label_33->setFont(font);
        lineEdit_Name = new QLineEdit(DialogName);
        lineEdit_Name->setObjectName(QString::fromUtf8("lineEdit_Name"));
        lineEdit_Name->setGeometry(QRect(110, 80, 261, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("HY\352\262\254\352\263\240\353\224\225"));
        font1.setPointSize(26);
        lineEdit_Name->setFont(font1);

        retranslateUi(DialogName);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogName, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogName, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogName);
    } // setupUi

    void retranslateUi(QDialog *DialogName)
    {
        DialogName->setWindowTitle(QApplication::translate("DialogName", "Input Name", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("DialogName", "Name:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogName: public Ui_DialogName {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGNAME_H
