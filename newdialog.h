#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QWidget>
#include "ui_newdialog.h"

class NewDialog : public QWidget
{
	Q_OBJECT

private slots:
	void createNewServerConfig();


public:
	NewDialog(QWidget *parent = 0);
	~NewDialog();
signals:
	void sendData(QString);

private:
	Ui::NewDialog ui;
	QString getTextFromInputBox();
	bool validate();
};

#endif // NEWDIALOG_H
