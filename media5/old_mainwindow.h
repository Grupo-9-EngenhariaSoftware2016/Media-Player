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

private slots:
    void on_menu_small_button_search_clicked();
    void on_menu_full_button_menu_clicked();
    void on_menu_small_button_menu_clicked();
    void on_menu_small_button_album_clicked();
    void on_menu_small_button_artist_clicked();
    void on_menu_small_button_song_clicked();
    void on_menu_small_button_list_clicked();
    void on_menu_full_button_album_clicked();
    void on_menu_full_button_artist_clicked();
    void on_menu_full_button_list_clicked();
    void on_menu_full_button_play_clicked();
    void on_menu_full_button_search_clicked();
    void on_menu_full_button_song_clicked();
    void on_menu_full_button_add_album_clicked();
    void on_menu_full_button_play_clicked();
    void on_bt_tab_add_clicked();
    void on_bt_add_songs_clicked();
    void on_bt_album_artwork_clicked();
    void on_bt_cancel_clicked();
    void on_bt_next_clicked();
    void on_bt_prev_clicked();
    void on_bt_save_clicked();
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_bt_nwalbum_nwartist_clicked();
    void on_bt_tab_select_toggled(bool checked);
    void on_tableWidget_doubleClicked(const QModelIndex &index);

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
