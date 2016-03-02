#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include "ui_aboutdialog.h"

class aboutDialog : public QWidget
{
	Q_OBJECT

public:
	aboutDialog(QWidget *parent = 0);
	~aboutDialog();

private:
	Ui::aboutDialog ui;
};

#endif // ABOUTDIALOG_H
