#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QWidget>
#include "ui_newdialog.h"
#include "dstservermanager.h"

class NewDialog : public QWidget
{
	Q_OBJECT

private slots:
	void createNewServerConfig();


public:
	NewDialog(dstServerManager *dstWindow, QWidget *parent = 0);
	~NewDialog();
signals:
	void sendData(QString,QString);

private:
	Ui::NewDialog ui;
	dstServerManager *dstWindow;
	QString getTextFromInputBox();
	pair<bool,QString> validate();
};

#endif // NEWDIALOG_H
