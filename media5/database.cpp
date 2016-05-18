#include "database.h"
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>

Database::Database()
{

}
// ===================================================================
// Conecao
bool Database::connOpen()
{
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(QDir::currentPath() +"/debug/database.db");

    if(!mydb.open())
    {
        qDebug() << ("Falha a abrir a DB");
        return false;

    }
    else
    {
        qDebug() << ("Abriu a DB");
        return true;
    }
}
void Database::connClose()
{
    mydb.close();
    mydb.removeDatabase(QSqlDatabase::defaultConnection);
}
// ===================================================================
// Albuns
bool Database::addAlbum(Album *newAlbum)
{

    // Get Album Index
    QSqlQuery last_id;
    if(last_id.exec("select ID_Album from Album order by ID_Album DESC Limit 1;"))
    {
        last_id.next();
        int id = last_id.value(0).toInt()+1;
        newAlbum->setIdBD(id);
        newAlbum->setDiretoria(newAlbum->getDiretoria()+QString::number(id)+" - "+ newAlbum->getNome());
        QFileInfo imgfile(newAlbum->getImagem());
        newAlbum->setImagem(newAlbum->getDiretoria() + "/artwork." + imgfile.suffix());

        // Adicionar Info à DB
        QSqlQuery add_album;
        add_album.prepare("INSERT INTO Album (Nome,Ano,Genero,Diretoria,Imagem,Descricao,DataAdicao)"
                          "VALUES (:Nome,:Ano,:Genero,:Diretoria,:Imagem,:Descricao,:DataAdicao)");
        add_album.bindValue(":Nome",        newAlbum->getNome());
        add_album.bindValue(":Ano",         newAlbum->getAno());
        add_album.bindValue(":Genero",      newAlbum->getGenero());
        add_album.bindValue(":Imagem",      newAlbum->getImagem());
        add_album.bindValue(":Diretoria",   newAlbum->getDiretoria());
        add_album.bindValue(":Descricao",   newAlbum->getDescricao());
        add_album.bindValue(":DataAdicao",  newAlbum->getDataAdicao());

        if(add_album.exec())
        {
            qDebug() << "Album Adicionado";
            return true;
        }
        else
        {
            qDebug() << "Album Nao Adicionado";
            return false;
        }
    }

    return false;

}
bool Database::UpdateAlbum(Album *newAlbum)
{
    QSqlQuery update_album;
//    update_album.prepare("UPDATE Album"
//                         "SET Nome=:Nome, City='Hamburg'"
//                         WHERE CustomerName='Alfreds Futterkiste';")

    return false;
}
bool Database::removeAlbum(Album *album)
{
    QString id = QString::number(album->getIdBD());

    QSqlQuery deleteSongs;
    if(deleteSongs.exec("Delete from Musica where ID_Album='"+id+"';"))
    {
        qDebug() << "Apagar musicas";
    }

    QSqlQuery deleteAlbum;
    if(deleteAlbum.exec("Delete from Album where ID_Album='"+id+"';"))
    {
        qDebug() << "Album " << id << "Deleted";
        return true;
    }
    else
    {
        qDebug() << "Album " << id << "NOT Deleted";
        return false;
    }
}
// ===================================================================
// Musicas
bool Database::addSong(Musica *newSong)
{

    // obter indice a adicionar
    QSqlQuery last_id;
    if(last_id.exec("select ID_Album from Album order by ID_Album DESC Limit 1;"))
    {
        last_id.next();
        int id = last_id.value(0).toInt();
        qDebug() << id;

        // Guardar informacao da musica - uma musica por enquanto
        QSqlQuery add_music;

        //http://doc.qt.io/qt-5/sql-sqlstatements.html
        add_music.prepare("INSERT INTO Musica (Nome,Diretoria,Faixa,ID_Album)"
                          "VALUES (:Nome,:Diretoria,:Faixa,:ID_Album)");
        add_music.bindValue(":Nome",        newSong->getNome());
        add_music.bindValue(":Diretoria",   newSong->getDiretoria());
        add_music.bindValue(":Faixa",       newSong->getFaixa());
        add_music.bindValue(":ID_Album",    id);

        if(add_music.exec())
        {
            last_id.exec("select ID_Musica from Musica order by ID_Musica DESC Limit 1;");
            last_id.next();
            newSong->setIdBD(last_id.value(0).toInt());
            qDebug() << "Musica-Adicionada";
            return true;
        }
        else
        {
            qDebug() << "Musica-Nao Adicionada";
            return false;
        }
    }
    else
    {
        return false;
    }

}
// ===================================================================
// Autores
bool Database::addArtist(Autor *newArtist)
{
    QSqlQuery last_id;
    if(last_id.exec("select ID_Autor from Autor order by ID_Autor DESC Limit 1;"))
    {
        last_id.next();
        int id = last_id.value(0).toInt()+1;

        newArtist->setIdBD(id);
        QFileInfo imgfile(newArtist->getImagem());
        newArtist->setImagem(QDir::currentPath()+"/debug/autor/id_"+QString::number(id)+"_"+ newArtist->getNome() +"." + imgfile.suffix());

        //http://doc.qt.io/qt-5/sql-sqlstatements.html
        QSqlQuery add_autor;
        add_autor.prepare("INSERT INTO Autor (Nome,Nacionalidade,DataNascimento,Imagem,DataAdicao)"
                          "VALUES (:Nome,:Nacionalidade,:DataNascimento,:Imagem,:DataAdicao)");
        add_autor.bindValue(":Nome",            newArtist->getNome());
        add_autor.bindValue(":Nacionalidade",   newArtist->getNacionalidade());
        add_autor.bindValue(":DataNascimento",  newArtist->getDataNascimento());
        add_autor.bindValue(":Imagem",          newArtist->getImagem());
        add_autor.bindValue(":DataAdicao",      newArtist->getDataAdicao());

        if(add_autor.exec())
        {
            qDebug() << "Autor-Adicionado";
            return true;
        }
        else
        {
            qDebug() << "Autor-Nao Adicionado";
            return false;
        }
    }
    return false;
}

// ===================================================================
// Playlists
bool Database::addPlaylist(Playlist *newPlaylist)
{
    QSqlQuery add_playlist;

    //http://doc.qt.io/qt-5/sql-sqlstatements.html
    add_playlist.prepare("INSERT INTO PLaylist (Nome,Descricao)"
                         "VALUES (:Nome,:Descricao)");
    add_playlist.bindValue(":Nome",        newPlaylist->getNome());
    add_playlist.bindValue(":Descricao",   newPlaylist->getDescricao());

    if(add_playlist.exec())
    {
        qDebug() << "Playlist-Adicionada";
        return true;
    }
    else
    {
        qDebug() << "Playlist-Nao Adicionada";
        return false;
    }
    return true;
}
