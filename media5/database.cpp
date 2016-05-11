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

    if(!mydb.open()){
        qDebug() << ("Falha a abrir a DB");
        return false;
    }else{
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
    int new_id;
    QString text;

    // obter indice a adicionar
    QSqlQuery last_id;
    last_id.prepare("select ID_Album from Album order by ID_Album DESC Limit 1;");

    if(last_id.exec())
    {
        last_id.next();
        new_id = last_id.value(0).toInt();
        qDebug() << "Ultimo ID=" << new_id;
        new_id++; // ID do proximo elemento a adicionar
        newAlbum->setIdBD(new_id);
    }else{
        return false;
    }

    // Adicionar Info à DB
    QSqlQuery add_album;
    text = "";
    QTextStream(&text) << "insert into Album (Nome,Ano,Genero,Diretoria,Descricao) values ('";
    QTextStream(&text) << newAlbum->getNome() << "','";
    QTextStream(&text) << newAlbum->getAno() << "','";
    QTextStream(&text) << newAlbum->getGenero() << "','";
    QTextStream(&text) << newAlbum->getDiretoria() << "','";
    QTextStream(&text) << newAlbum->getDescricao() << "') ;";
    add_album.prepare(text);

    if(!add_album.exec())
    {
        qDebug() << "Album Nao Adicionado";
        return false;
    }

    return true;
}

bool Database::addSong(Musica *newSong)
{
    int new_id;

    // obter indice a adicionar
    QSqlQuery last_id;
    last_id.prepare("select ID_Album from Album order by ID_Album DESC Limit 1;");

    if(last_id.exec())
    {
        last_id.next();
        new_id = last_id.value(0).toInt();
        qDebug() << "Ultimo ID=" << new_id;
        new_id++; // ID do proximo elemento a adicionar
        newSong->setIdBD(new_id);
    }else{
        return false;
    }

    // Guardar informacao da musica - uma musica por enquanto
    QSqlQuery add_music;

    //http://doc.qt.io/qt-5/sql-sqlstatements.html
    add_music.prepare("INSERT INTO Musica (Nome,Diretoria,ID_Album)"
                      "VALUES (:Nome, :Diretoria, :ID_Album)");

    add_music.bindValue(":Nome", newSong->getNome());
    add_music.bindValue(":Diretoria", "Diretoria_teste");
    add_music.bindValue(":ID_Album", new_id);

    if(!add_music.exec())
    {
        qDebug() << "Musica-Nao adiciona";
        return false;
    }

    return true;
}
