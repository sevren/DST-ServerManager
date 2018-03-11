#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QWidget>
#include "ui_newdialog.h"
#include "dstservermanager.h"
#include "dstman_customdatatypes.h"

class NewDialog : public QWidget
{
	Q_OBJECT

private slots:
	void createNewServerConfig();
	void openFileChooser();


public:
	NewDialog(dstServerManager *dstWindow, GlobalDSTManSettings *dstManSettings , QWidget *parent = 0);
	~NewDialog();

signals:
	void sendData(QString,QString,QString);

private:
	Ui::NewDialog ui;
	dstServerManager *dstWindow;
	QString getTextFromInputBox(QLineEdit*);
	pair<bool,QString> validate();
	GlobalDSTManSettings *dstManSettings;
};

#endif // NEWDIALOG_H
