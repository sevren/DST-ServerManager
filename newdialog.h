#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QWidget>
#include "ui_newdialog.h"

class NewDialog : public QWidget
{
	Q_OBJECT

public:
	NewDialog(QWidget *parent = 0);
	~NewDialog();

private:
	Ui::NewDialog ui;
};

#endif // NEWDIALOG_H
