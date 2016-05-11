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
    bool addSong(Musica *newSong);
    bool addArtist(Autor *newArtist);
    bool addPlaylist(Playlist *newPlaylist);
};

#endif // DATABASE_H
