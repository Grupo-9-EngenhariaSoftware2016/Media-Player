#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>
#include "classes.h"

class Database
{

public:

    QSqlDatabase mydb;

    Database();

    void CreateTables(); //NEW//
    bool connOpen();
    void connClose();

    bool addAlbum(Album *newAlbum);
    bool updateAlbum(Album *Album);
    bool removeAlbum(Album *Album);

    bool addSong(Musica *newSong,int album_id);
    bool updateSong(Musica *Song);
    bool removeSong(Musica *Song);

    bool addArtist(Autor *newArtist);
    bool updateArtist(Autor *Artist);
    bool removeArtist(Autor *Artist);

    bool addPlaylist(Playlist *newPlaylist);
    bool updatePlaylist(Playlist *Playlist);
    bool addSongToPlaylist(Playlist *newPlaylist,int music_id);
    bool removeSongFromPlaylist(Playlist *newPlaylist, int music_id);
    bool removePlaylist(Playlist *newPlaylist);

};

#endif // DATABASE_H
