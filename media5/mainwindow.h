#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileDialog>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase mydb;
    void connClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()
    {
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName(QDir::currentPath() +"/database/database.db");

        if(!mydb.open())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Dialog * mdialog;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    void FillForm();
    void MovePageToAlbuns();
    void MovePageToArtists();
    void MovePageToPlayer();
    void MovePageToSongs();
    void MovePageToPlaylists();
    void MovePageToSearch();
};

#endif // MAINWINDOW_H
