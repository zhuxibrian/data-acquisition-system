#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

    void closeEvent(QCloseEvent * closeevent);

signals:
    void closeSignal();

private:
	Ui::MainWindowClass ui;
};
