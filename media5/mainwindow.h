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
#include "dialog.h"
#include "classes.h"

namespace Ui {
class MainWindow;
}

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
    void on_page_album_info_button_addTo_clicked();
    void on_page_album_info_button_remove_clicked();
    void on_page_album_info_button_exploreArtist_clicked();
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
    void on_page_artist_button_addTo_clicked();
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
    void on_options_button_edit_clicked();
    void on_options_button_remove_clicked();
    void on_options_button_information_clicked();

private:
    Ui::MainWindow *ui;
    Dialog * mdialog;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;

    QString getArtistsFrom(Album *album);
    QString getArtistsFrom(Playlist *playlist);
    QString getArtistsFrom(int *displayed, Musica *song);
    QList <Album*> getAlbunsFromArtist(Autor *artist);
    QList <Musica*> getSongsFromArtist(Autor *artist);
    Album* getAlbumWith(Musica* song);

    void CheckMenuButton(QString button);
    void ExpandMenu(bool expand);
    void ShowOptionsTab(bool show);
    void ShowProgressTab(bool show);

    void FormatTableFor(QTableWidget *table, QString format);
    void AddAlbumLineToTable(QTableWidget *table, Album *album);
    void AddSongLineToTable(QTableWidget *table, Musica *song);
    void AddNewSongLineToTable(QTableWidget *table, Musica *newSong);
    void AddSimplifiedSongLineToTable(QTableWidget *table, Musica *song);
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
