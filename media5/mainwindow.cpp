#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QModelIndex>

#define NO_DB

MainWindow::~MainWindow()
{
   QMainWindow::centralWidget();
    mydb.close();
    delete ui;
}
