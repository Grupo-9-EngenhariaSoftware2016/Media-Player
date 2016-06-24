#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QModelIndex>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QList>
#include <QTableWidget>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QItemDelegate>
#include <QStandardItemModel>
#include "dialog.h"
#include "searchartist.h"
#include "classes.h"
#include "mydelegate.h"

namespace Ui {
class MainWindow;
}

class MyAction : public QAction
{
    Q_OBJECT

public:
    explicit MyAction(QObject *parent = 0) : QAction(parent){
        QObject::connect(this,SIGNAL(triggered(bool)),this,SLOT(sendTrigger()));
    }

private slots:
    void sendTrigger(){
        emit myTrigger(this);
    }

signals:
    void myTrigger(MyAction *action);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QSqlDatabase mydb;
    QList <Album*> _albuns;
    QList <Playlist*> _playlist;
    QList <Autor*> _artists;
    QList <Musica*> _songs;
    Player _player;

    QString _imageURL;
    QList <Musica*> _newSongList;
    Playlist *_newPlaylist;
    int _showingArtist, _showingAlbum, _showingPlaylist;
    bool _editMode = false;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Refresh Handlers
    void on_player_positionChanged(qint64 position);
    void on_player_durationChanged(qint64 duration);
    void on_player_mutedChanged(bool muted);
    void on_player_playbackModeChanged(QMediaPlaylist::PlaybackMode mode);
    void on_player_stateChanged(QMediaPlayer::State state);
    void on_playlist_selected(MyAction *action);
    void on_categories_cell_changed(int row, int column);
    void on_album_info_cell_changed(int row, int column);
    void on_artist_cell_changed(int row, int column);
    void on_playlist_cell_changed(int row, int column);
    void on_table_checkBox_pressed(QTableWidgetItem* item);
    void on_table_cell_activated(int row, int column);


    // Menu Handlers
    void on_menu_small_button_search_clicked();
    void on_menu_small_button_album_clicked();
    void on_menu_small_button_artist_clicked();
    void on_menu_small_button_list_clicked();
    void on_menu_small_button_menu_clicked();
    void on_menu_small_button_play_clicked();
    void on_menu_small_button_song_clicked();
    void on_menu_full_button_add_album_clicked();
    void on_menu_full_button_add_artist_clicked();
    void on_menu_full_button_add_list_clicked();
    void on_menu_full_button_add_song_clicked();
    void on_menu_full_button_album_clicked();
    void on_menu_full_button_artist_clicked();
    void on_menu_full_button_list_clicked();
    void on_menu_full_button_menu_clicked();
    void on_menu_full_button_play_clicked();
    void on_menu_full_button_search_clicked();
    void on_menu_full_button_song_clicked();
    void on_menu_full_lineEdit_search_returnPressed();

    // Page Categories Handlers
    void on_page_categories_button_add_clicked();
    void on_page_categories_button_select_toggled(bool checked);
    void on_page_categories_button_random_clicked();
    void on_page_categories_comboBox_order_currentIndexChanged(int index);
    void on_page_categories_tableWidget_doubleClicked(const QModelIndex &index);

    // Page Album Info Handlers
    void on_page_album_info_button_play_clicked();
    void on_page_album_info_button_remove_clicked();
    //void on_page_album_info_button_exploreArtist_clicked();
    void on_page_album_info_button_select_toggled(bool checked);
    void on_page_album_info_tableWidget_doubleClicked(const QModelIndex &index);

    // Page Add Album Handlers
    void on_page_add_album_button_addArtwork_clicked();
    void on_page_add_album_button_addMusic_clicked();
    void on_page_add_album_button_addFolder_clicked();
    void on_page_add_album_button_remove_clicked();
    void on_page_add_album_button_addArtistTo_clicked();
    void on_page_add_album_button_addArtistToAll_clicked();
    void on_page_add_album_button_removeArtistFrom_clicked();
    void on_page_add_album_button_newArtist_clicked();
    void on_page_add_album_lineEdit_searchArtists_returnPressed();
    void on_page_add_album_lineEdit_searchArtists_textChanged(const QString &arg1);

    // Page Add Music Handlers
    void on_page_add_music_button_addFolder_clicked();
    void on_page_add_music_button_addMusic_clicked();
    void on_page_add_music_comboBox_albuns_currentIndexChanged(int index);

    // Page Search Handlers
    void on_page_search_tableWidget_artists_doubleClicked(const QModelIndex &index);
    void on_page_search_tableWidget_albuns_doubleClicked(const QModelIndex &index);
    void on_page_search_tableWidget_musics_doubleClicked(const QModelIndex &index);

    // Page Artist Handlers
    void on_page_artist_button_play_clicked();
    void on_page_artist_button_remove_clicked();
    void on_page_artist_button_select_toggled(bool checked);
    void on_page_artist_tableWidget_albuns_doubleClicked(const QModelIndex &index);

    // Page Playlist Handlers
    void on_page_playlist_button_play_clicked();
    void on_page_playlist_button_addMusic_clicked();
    void on_page_playlist_button_remove_clicked();
    void on_page_playlist_tableWidget_doubleClicked(const QModelIndex &index);

    // Page Add Playlist Handlers
    void on_page_add_playlist_button_search_clicked();
    void on_page_add_playlist_button_add_clicked();
    void on_page_add_playlist_button_remove_clicked();
    void on_page_add_playlist_lineEdit_search_returnPressed();
    void on_page_add_playlist_lineEdit_search_textChanged(const QString &arg1);
    void on_page_add_playlist_tableWidget_toAdd_doubleClicked(const QModelIndex &index);
    void on_page_add_playlist_tableWidget_Added_doubleClicked(const QModelIndex &index);

    // Tab Player Handlers
    void on_player_slider_sliderReleased();
    void on_player_slider_valueChanged(int value);
    void on_player_button_previous_clicked();
    void on_player_button_play_toggled(bool checked);
    void on_player_button_stop_clicked();
    void on_player_button_next_clicked();
    void on_player_button_shuffle_toggled(bool checked);
    void on_player_button_repeat_clicked();
    void on_player_button_mute_clicked();

    // Tab Progress Handlers
    void on_progress_button_previous_clicked();
    void on_progress_button_next_clicked();
    void on_progress_button_cancel_clicked();
    void on_progress_button_save_clicked();

    // Tab Options Handlers
    void on_options_button_play_clicked();
    void on_options_button_remove_clicked();

    void on_options_button_edit_toggled(bool checked);


    void on_page_remove_album_artwork_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog * mdialog;
    searchArtist * diagArtist;

    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    MyDelegate *genderDelegate, *artistDelegate;

    QList<Autor*> getArtistsFrom(Album *album);
    QList<Autor*> getArtistsFrom(Playlist *playlist);
    QList<Autor*> getArtistsFrom(Musica *song);
    QString printArtistList(QList<Autor*> artists);
    QList <Album*> getAlbunsFromArtist(Autor *artist);
    QList <Musica*> getSongsFromArtist(Autor *artist);
    Album* getAlbumWith(Musica* song);
    QStringList Generos;

    void CheckMenuButton(QString button);
    void ExpandMenu(bool expand);
    void ShowOptionsTab(bool show);
    void ShowProgressTab(bool show);

    void FormatTableFor(QTableWidget *table, QString format);
    void AddAlbumLineToTable(QTableWidget *table, Album *album);
    void AddSongLineToTable(QTableWidget *table, Musica *song);
    void AddNewSongLineToTable(QTableWidget *table, Musica *newSong);
    void AddSimplifiedSongLineToTable(QTableWidget *table, Musica *song, QList<Musica *> *list);
    void AddArtistNameLineToTable(QTableWidget *table, Autor *artist);
    void AddArtistLineToTable(QTableWidget *table, Autor *artist);
    void AddPlaylistLineToTable(QTableWidget *table, Playlist *playlist);

    void MovePageToAlbuns();
    void MovePageToAlbumInfo(int index);
    void MovePageToArtists();
    void MovePageToArtistInfo(int index);
    void MovePageToPlayer();
    void MovePageToSongs();
    void MovePageToPlaylists();
    void MovePageToPlaylistInfo(int index);
    void MovePageToSearch();
    void MovePageToAddAlbuns();
    void MovePageToAddPlaylist();
    void MovePageToAddSongs();
    void NewArtist();

    void Refresh();
};

#endif // MAINWINDOW_H
