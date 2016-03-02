/********************************************************************************
** Form generated from reading UI file 'aboutDialoggq9068.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ABOUTDIALOGGQ9068_H
#define ABOUTDIALOGGQ9068_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_aboutDialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *aboutDialog)
    {
        if (aboutDialog->objectName().isEmpty())
            aboutDialog->setObjectName(QStringLiteral("aboutDialog"));
        aboutDialog->setWindowModality(Qt::NonModal);
        aboutDialog->resize(400, 300);
        pushButton = new QPushButton(aboutDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(160, 240, 75, 23));

        retranslateUi(aboutDialog);

        QMetaObject::connectSlotsByName(aboutDialog);
    } // setupUi

    void retranslateUi(QDialog *aboutDialog)
    {
        aboutDialog->setWindowTitle(QApplication::translate("aboutDialog", "Dialog", 0));
        pushButton->setText(QApplication::translate("aboutDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class aboutDialog: public Ui_aboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ABOUTDIALOGGQ9068_H
