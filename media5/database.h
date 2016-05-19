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

    bool connOpen();
    void connClose();

    bool addAlbum(Album *newAlbum);
    bool UpdateAlbum(Album *Album);
    bool removeAlbum(Album *Album);

    bool addSong(Musica *newSong,int album_id);
    bool UpdateSong(Musica *Song);
    bool removeSong(Musica *Song);

    bool addArtist(Autor *newArtist);
    bool UpdateArtist(Autor *Artist);
    bool removeArtist(Autor *Artist);

    bool addPlaylist(Playlist *newPlaylist);
    bool addSongsToPlaylist(Playlist *newPlaylist);
    bool removePlaylist(Playlist *newPlaylist);
};

#endif // DATABASE_H
