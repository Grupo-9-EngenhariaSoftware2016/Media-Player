#include "database.h"
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>

Database::Database()
{

}
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
    }

    // Adicionar Info à DB
    QSqlQuery add_album;

    add_album.prepare("INSERT INTO Album (Nome,Ano,Genero,Diretoria,Descricao,DataAdicao)"
                      "VALUES (:Nome,:Ano,:Genero,:Diretoria,:Descricao,:DataAdicao)");

    add_album.bindValue(":Nome",        newAlbum->getNome());
    add_album.bindValue(":Ano",         newAlbum->getAno());
    add_album.bindValue(":Genero",      newAlbum->getGenero());
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
bool Database::removeAlbum(Album *album)
{
    QString id = QString::number(album->getIdBD());

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
bool Database::addSong(Musica *newSong)
{
    int new_id;

    // obter indice a adicionar
    QSqlQuery last_id;
    if(last_id.exec("select ID_Album from Album order by ID_Album DESC Limit 1;"))
    {
        last_id.next();
        new_id = last_id.value(0).toInt();
        qDebug() << "Ultimo ID=" << new_id;
        new_id++; // ID do proximo elemento a adicionar
        newSong->setIdBD(new_id);
    }
    else
    {
        return false;
    }

    // Guardar informacao da musica - uma musica por enquanto
    QSqlQuery add_music;

    //http://doc.qt.io/qt-5/sql-sqlstatements.html
    add_music.prepare("INSERT INTO Musica (Nome,Diretoria,Faixa,ID_Album)"
                      "VALUES (:Nome, :Diretoria,:Faixa,:ID_Album)");

    add_music.bindValue(":Nome",        newSong->getNome());
    add_music.bindValue(":Diretoria",   newSong->getDiretoria());
    add_music.bindValue(":Faixa",       newSong->getFaixa());
    add_music.bindValue(":ID_Album",    new_id);

    if(add_music.exec())
    {
        qDebug() << "Musica-Adicionada";
        return true;
    }
    else
    {
        qDebug() << "Musica-Nao Adicionada";
        return false;
    }


}
bool Database::addArtist(Autor *newArtist)
{
    QSqlQuery add_autor;

    //http://doc.qt.io/qt-5/sql-sqlstatements.html
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
bool Database::addPlaylist(Playlist *newPlaylist)
{

    return true;
}
