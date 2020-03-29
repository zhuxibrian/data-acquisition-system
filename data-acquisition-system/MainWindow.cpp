#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void MainWindow::closeEvent(QCloseEvent * closeevent)
{
    emit closeSignal();
}
