#include "database.h"
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>

Database::Database()
{

}
// ===================================================================
// Conexao
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

    qDebug() <<"=================================";
    qDebug() << "PROCESS:Add Album ";

    // Adicionar Info à DB - Adicionar tudo menos Diretoria e Imagem
    QSqlQuery add_album;
    add_album.prepare("INSERT INTO Album (Nome,Ano,Genero,Descricao,DataAdicao)"
                      "VALUES (:Nome,:Ano,:Genero,:Descricao,:DataAdicao)");
    add_album.bindValue(":Nome",        newAlbum->getNome());
    add_album.bindValue(":Ano",         newAlbum->getAno());
    add_album.bindValue(":Genero",      newAlbum->getGenero());
    add_album.bindValue(":Descricao",   newAlbum->getDescricao());
    add_album.bindValue(":DataAdicao",  newAlbum->getDataAdicao());

    if(add_album.exec())
    {
        qDebug() << "conteudos: Adicionado";
    }
    else
    {
        qDebug() << "conteudos: Nao Adicionado";
    }

    // Aquisicao do ID
    QSqlQuery getID;
    getID.exec("Select last_insert_rowid();");
    getID.next();

    int id =  getID.value(0).toInt();


    newAlbum->setIdBD(id);
    newAlbum->setDiretoria(newAlbum->getDiretoria()+ QString::number(id)+ " - "+ newAlbum->getNome());

    if(newAlbum->getImagem()!= NULL)
    {
        QFileInfo imgfile(newAlbum->getImagem());
        newAlbum->setImagem(newAlbum->getDiretoria() + "/artwork." + imgfile.suffix());
    }

    qDebug() <<newAlbum->getIdBD();
    qDebug() <<newAlbum->getDiretoria();
    qDebug() <<newAlbum->getImagem();

    //Adicionar Dirs para o album
    QSqlQuery update_album;
    update_album.prepare("UPDATE Album SET Diretoria= :Diretoria, Imagem=:Imagem "
                         "WHERE ID_Album in(Select last_insert_rowid());");
    update_album.bindValue(":Diretoria",   newAlbum->getDiretoria());
    update_album.bindValue(":Imagem",      newAlbum->getImagem());

    if(update_album.exec())
    {
        qDebug() << "PROCESS END:Album Adicionado";
        qDebug() <<"=================================";
        return true;
    }
    else
    {
        qDebug() << "PROCESS END:Album Nao Adicionado";
        qDebug() <<"=================================";
        return false;
    }

    return false;

}
bool Database::updateAlbum(Album *Album)
{ 
    //Se a imagem alterar - Fazer Algo
    QSqlQuery update_img_album;
    update_img_album.prepare(" UPDATE Album SET Imagem=:Imagem,WHERE ID_Album=:ID_Album;");
    update_img_album.bindValue(":Imagem",Album->getImagem());


    QSqlQuery update_album;
    update_album.prepare("UPDATE Album SET Nome=:Nome, Ano=:Ano, Genero=:Genero, Descricao:Descricao, DataAdicao:DataAdicao "
                         "WHERE ID_Album=:ID_Album;");

    update_album.bindValue(":ID_Album",    Album->getIdBD());
    update_album.bindValue(":Nome",        Album->getNome());
    update_album.bindValue(":Ano",         Album->getAno());
    update_album.bindValue(":Genero",      Album->getGenero());
    update_album.bindValue(":Descricao",   Album->getDescricao());
    update_album.bindValue(":DataAdicao",  Album->getDataAdicao());

    if(update_album.exec())
    {
        qDebug()<< " PROCESS END: Album" << Album->getIdBD() << "Actualizado";
        qDebug() << "=================================";
        return true;
    }

    qDebug() << " PROCESS END: Album" << Album->getIdBD() << "NAO Actualizado";
    qDebug() <<"=================================";

    return false;
}
bool Database::removeAlbum(Album *Album)
{
    QString id = QString::number(Album->getIdBD());

    qDebug() << "PROCESS: Remove Album " << id;
    //Apagar conteudos dos Autores
    QSqlQuery deleteArtists;
    deleteArtists.prepare("DELETE FROM Tem WHERE ID_Musica IN("
                          "select ID_Musica FROM Musica WHERE ID_Album=:ID_Album);");
    deleteArtists.bindValue(":ID_Album", id);

    if(deleteArtists.exec())
    {
       qDebug() << "SUCESS:Apagar musicas dos autores";
    }
    else
    {
       qDebug() << "ERROR:Apagar musicas dos autores";
    }

    //Apagar conteudos das playlists
    QSqlQuery deletePlaylists;
    deletePlaylists.prepare("DELETE FROM Pertence WHERE ID_Musica IN("
                          "select ID_Musica FROM Musica WHERE ID_Album=:ID_Album);");
    deletePlaylists.bindValue(":ID_Album", id);

    if(deletePlaylists.exec())
    {
       qDebug() << "SUCESS:Apagar musicas das playlist";
    }
    else
    {
       qDebug() << "ERROR:Apagar musicas das playlist";
    }

    //Apagar Musicas
    QSqlQuery deleteSongs;
    deleteSongs.prepare("DELETE FROM Musica WHERE ID_Album=:ID_Album;");
    deleteSongs.bindValue(":ID_Album",id);

    if(deleteSongs.exec())
    {
        qDebug() << "SUCESS:Apagar musicas";
        qDebug() <<"=================================";
    }
    else
    {
       qDebug() << "ERROR:Apagar musicas";
       qDebug() <<"=================================";
    }


    //Apagar Album
    QSqlQuery deleteAlbum;
    deleteAlbum.prepare("DELETE FROM Album WHERE ID_Album=:ID_Album;");
    deleteAlbum.bindValue(":ID_Album",id);

    if(deleteAlbum.exec())
    {
        qDebug() << "PROCESS END: Album " << id << "Deleted";
        qDebug() <<"=================================";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Album " << id << "NOT Deleted";
        qDebug() <<"=================================";
        return false;
    }
}
// ===================================================================
// Musicas
bool Database::addSong(Musica *newSong, int album_id)
{
    qDebug() << "PROCESS: Add Music to Album " << album_id;

    //Guardar informacao da musica - uma musica por enquanto
    //http://doc.qt.io/qt-5/sql-sqlstatements.html
    QSqlQuery add_music;
    add_music.prepare("INSERT INTO Musica (Nome,Diretoria,Faixa,ID_Album)"
                      "VALUES (:Nome,:Diretoria,:Faixa,:ID_Album)");
    add_music.bindValue(":Nome",        newSong->getNome());
    add_music.bindValue(":Diretoria",   newSong->getDiretoria());
    add_music.bindValue(":Faixa",       newSong->getFaixa());
    add_music.bindValue(":ID_Album",    album_id);

    if(add_music.exec())
    {
        qDebug() << "Musica-Adicionada";

        //Atribuir Indice ao objecto da classe criado recentemente
        QSqlQuery getId;
        getId.exec("select last_insert_rowid();");
        getId.next();
        newSong->setIdBD(getId.value(0).toInt());

        //Atribuir Autores à música
        QList<Autor*> artistList;
        newSong->getAutor(&artistList);

        if(!artistList.empty())
        {
            QSqlQuery add_artist;
            for(int i =0; i < artistList.size();i++)
            {
                add_artist.prepare("INSERT INTO Tem (ID_Musica,ID_Autor)"
                                   "VALUES (:ID_Musica,:ID_Autor)");
                add_artist.bindValue(":ID_Musica",newSong->getIdBD());
                add_artist.bindValue(":ID_Autor" ,artistList[i]->getIdBD());

                if(add_artist.exec())
                {
                    qDebug() << "PROCESS END: Song" << newSong->getNome() <<" Added " << artistList[i]->getNome();
                }

            }
            qDebug() <<"=================================";
        }
        else
        {
            qDebug() << "->Nao existem autores para a Musica " << newSong->getNome();
            qDebug() <<"=================================";
        }

        return true;
    }

    qDebug() << "PROCESS END: Musica-Nao Adicionada";
    qDebug() <<"=================================";
    return false;

}
bool Database::updateSong(Musica *Song)
{
    qDebug() <<"=================================";
    qDebug() <<"PROCESS: Actualizar Musica" << Song->getNome();
    //Remover Autores
    QSqlQuery deleteArtists;
    deleteArtists.prepare("DELETE FROM Tem WHERE ID_Musica=:ID_Musica;");
    deleteArtists.bindValue(":ID_Musica", Song->getIdBD());

    //Adicionar Autores
    QList<Autor*> artistList;
    Song->getAutor(&artistList);

    if(!artistList.empty())
    {
        QSqlQuery add_artist;
        for(int i =0; i < artistList.size();i++)
        {
            add_artist.prepare("INSERT INTO Tem (ID_Musica,ID_Autor)"
                               "VALUES (:ID_Musica,:ID_Autor)");
            add_artist.bindValue(":ID_Musica",Song->getIdBD());
            add_artist.bindValue(":ID_Autor" ,artistList[i]->getIdBD());

            if(add_artist.exec())
            {
                qDebug() << "SUCESS: Song" << Song->getNome() <<" Added " << artistList[i]->getNome();
            }
            else
            {
                 qDebug() << "WARNING: Song" << Song->getNome() <<" Added " << artistList[i]->getNome();
            }
        }
    }
    else
    {
        qDebug() << "SUCESS: Nao existem autores para a Musica ";
    }

    //Alterar tags
    QSqlQuery update_song;
    update_song.prepare("UPDATE Musica SET Nome=:Nome,Faixa:=Faixa WHERE ID_Musica=:ID_Musica;");
    update_song.bindValue(":ID_Musica",     Song->getIdBD());
    update_song.bindValue(":Nome",          Song->getNome());
    update_song.bindValue(":Faixa",         Song->getFaixa());

    if(update_song.exec())
    {
         qDebug() <<"PROCESS END: Musica " << Song->getNome() << "Actualizada";
         qDebug() <<"=================================";
         return true;
    }


    qDebug() <<"=================================";
    qDebug() <<"PROCESS END: Musica " << Song->getNome() << " NAO Actualizada";

    return false;
}
bool Database::removeSong(Musica *Song)
{
    //Apagar conteudos dos Autores
    QSqlQuery deleteArtists;
    deleteArtists.prepare("DELETE FROM Tem WHERE ID_Musica=:ID_Musica;");
    deleteArtists.bindValue(":ID_Musica", Song->getIdBD());

    if(deleteArtists.exec())
    {
       qDebug() << "SUCESS:Apagar autor da Musica";
    }
    else
    {
       qDebug() << "ERROR:Apagar autor da Musica";
    }

    //Apagar conteudos das playlists
    QSqlQuery deletePlaylists;
    deletePlaylists.prepare("DELETE FROM Pertence WHERE ID_Musica=:ID_Musica;");
    deletePlaylists.bindValue(":ID_Musica", Song->getIdBD());

    if(deletePlaylists.exec())
    {
       qDebug() << "SUCESS:Apagar musica das playlist";
    }
    else
    {
       qDebug() << "ERROR: Apagar musica das playlist";
    }

    //Apagar Musicas
    QSqlQuery deleteSongs;
    deleteSongs.prepare("DELETE FROM Musica WHERE ID_Musica=:ID_Musica;");
    deleteSongs.bindValue(":ID_Musica", Song->getIdBD());

    if(deleteSongs.exec())
    {
        qDebug() << "SUCESS:Apagar musica";
        qDebug() <<"=================================";
    }

    qDebug() << "ERROR:Apagar musica";
    qDebug() <<"=================================";

    return false;
}
// ===================================================================
// Autores
bool Database::addArtist(Autor *newArtist)
{
    //Guardar Autor na DB
    //http://doc.qt.io/qt-5/sql-sqlstatements.html
    QSqlQuery add_autor;
    add_autor.prepare("INSERT INTO Autor (Nome,Nacionalidade,DataNascimento,DataAdicao)"
                      "VALUES (:Nome,:Nacionalidade,:DataNascimento,:DataAdicao)");
    add_autor.bindValue(":Nome",            newArtist->getNome());
    add_autor.bindValue(":Nacionalidade",   newArtist->getNacionalidade());
    add_autor.bindValue(":DataNascimento",  newArtist->getDataNascimento());
    add_autor.bindValue(":DataAdicao",      newArtist->getDataAdicao());

    if(add_autor.exec())
    {
        // Aquisicao do ID
        QSqlQuery getID;
        getID.exec("Select last_insert_rowid();");
        getID.next();

        int id =  getID.value(0).toInt();
        newArtist->setIdBD(id);

        //Caminho para a imagem
        QFileInfo imgfile(newArtist->getImagem());
        newArtist->setImagem(QDir::currentPath()+"/debug/autor/id_"
                             + QString::number(id)+"_"
                             + newArtist->getNome() +"."
                             + imgfile.suffix());

        //Adicionar Imagem para o autor
        QSqlQuery update_autor;
        update_autor.prepare("UPDATE Autor SET Imagem=:Imagem "
                             "WHERE  ID_Autor in(Select last_insert_rowid());");
        update_autor.bindValue(":Imagem",   newArtist->getImagem());

        if(update_autor.exec())
        {
            qDebug() << "SUCESS: Img Adicionada";
        }
        else
        {
            qDebug() << "ERROR: Img Não Adicionada";
        }

        qDebug() << "PROCESS END:Autor-Adicionado";

        return true;
    }

    qDebug() << "PROCESS END:Autor-Nao Adicionado";
    return false;
}
bool Database::updateArtist(Autor *Artist)
{
    qDebug() <<"=================================";
    qDebug() << "PROCESS: Actualizar o Autor"  << Artist->getNome();

    //Actualizar a Imagem do Autor
    QSqlQuery update_image;
    update_image.prepare("UPDATE Autor SET Imagem:=Imagem");
    update_image.bindValue(":Imagem",   Artist->getImagem());

    if(update_image.exec())
    {
        qDebug() << "SUCESS: Imagem";
    }
    else
    {
        qDebug() << "WARNING: Imagem";
    }

    //Actualiza Info do Autor
    QSqlQuery update_autor;
    update_autor.prepare("UPDATE Autor SET Nome=:Nome,Nacionalidade=:Nacionalidade,DataNascimento=:DataNascimento,Imagem:=Imagem, DataAdicao:=DataAdicao "
                         "WHERE ID_Autor=:ID_Autor");
    update_autor.bindValue(":ID_Autor",         Artist->getIdBD());
    update_autor.bindValue(":Nome",             Artist->getNome());
    update_autor.bindValue(":Nacionalidade",    Artist->getNacionalidade());
    update_autor.bindValue(":DataNascimento",   Artist->getDataNascimento());
    update_autor.bindValue(":DataAdicao",       Artist->getDataAdicao());
    if(update_autor.exec())
    {
        qDebug() << "PROCESS END: Autor Actualizado";
        qDebug() <<"=================================";
        return true;
    }

    qDebug() << "PROCESS END: Autor NAO Actualizado";
    qDebug() <<"=================================";

    return false;
}
bool Database::removeArtist(Autor *Artist)
{
    int id = Artist->getIdBD();
    qDebug() <<"=================================";
    qDebug() << "PROCESS: Eliminar Autor" << id;

    //Remove Associacao dos Autores com as Musicas
    QSqlQuery deleteSongs;
    deleteSongs.prepare("Delete from Tem where ID_Autor=:ID_Autor;");
    deleteSongs.bindValue(":ID_Autor",id);
    if(deleteSongs.exec())
    {
        qDebug() << "Apagou Associacao dos Autores com as Musicas";
    }
    else
    {
        qDebug() << "Nao foi possivel apagar Associacao dos Autores com as Musicas";
    }

    //Remove Autor
    QSqlQuery deleteAutor;
    deleteAutor.prepare("Delete from Autor where ID_Autor=:ID_Autor;");
    deleteAutor.bindValue(":ID_Autor",id);
    if(deleteAutor.exec())
    {
        qDebug() << "PROCESS END:Autor"<< id <<"Deleted";
        qDebug() <<"=================================";
        return true;
    }
    else
    {
        qDebug() << "PROCESS END:Autor" << id << "NOT Deleted";
        qDebug() <<"=================================";
        return false;
    }
}
// ===================================================================
// Playlists
bool Database::addPlaylist(Playlist *newPlaylist)
{
    //http://doc.qt.io/qt-5/sql-sqlstatements.html

    QSqlQuery add_playlist;
    add_playlist.prepare("INSERT INTO Playlist (Nome,Descricao,DataAdicao)"
                         "VALUES (:Nome,:Descricao,:DataAdicao)");
    add_playlist.bindValue(":Nome",         newPlaylist->getNome());
    add_playlist.bindValue(":Descricao,",   newPlaylist->getDescricao());
    add_playlist.bindValue(":DataAdicao",   newPlaylist->getDataAdicao());

    if(add_playlist.exec())
    {
        qDebug() << "Playlist-Adicionada";

        QSqlQuery last_id;
        if(last_id.exec("select ID_Playlist from Playlist order by ID_Playlist DESC Limit 1;"))
        {
            last_id.next();
            int id = last_id.value(0).toInt();

            //Adicionar musicas à Playlist
            QList <Musica*> songlist;
            newPlaylist->getMusicas(&songlist);

            if(!songlist.empty())
            {
                QSqlQuery addSongToPlayList;
                for(int i =0; i< songlist.size();i++)
                {
                    addSongToPlayList.prepare("INSERT INTO Pertence (ID_Musica,ID_Playlist)"
                                              "VALUES (:ID_Musica,:ID_Playlist)");
                    addSongToPlayList.bindValue(":ID_Musica",songlist[i]->getIdBD());
                    addSongToPlayList.bindValue(":ID_Playlist",id);

                    if(addSongToPlayList.exec())
                    {
                        qDebug() << "Song" << songlist[i]->getNome() <<"Added to playlist" << newPlaylist->getNome();
                    }
                    else
                    {
                        qDebug() << "Failed to Add Song"<< songlist[i]->getIdBD()<< "to Playlist" << id;
                        return false;
                    }
                }
            }
            else
            {
                //Não existem musicas para adicionar -> temos uma playlist vazia com sucesso
                return true;
            }
        }

    }
    else
    {
        qDebug() << "Playlist-Nao Adicionada";
        return false;
    }

    return false;
}
bool Database::updatePlaylist(Playlist *Playlist)
{
    qDebug() <<"=================================";
    qDebug() << "PROCESS: Actualizar a Playlist";

    //Update dos dados da Playlist
    QSqlQuery update_playlist;
    update_playlist.prepare("UPDATE Playlist SET Nome=:Nome, Descricao=:Descricao ,DataAdicao=:DataAdicao "
                            "WHERE ID_Playlist=:ID_Playlist;");

    update_playlist.bindValue(":ID_Playlist", Playlist->getIdBD());
    update_playlist.bindValue(":Nome"       , Playlist->getNome());
    update_playlist.bindValue(":Descricao," , Playlist->getDescricao());
    update_playlist.bindValue(":DataAdicao" , Playlist->getDataAdicao());

    if(update_playlist.exec())
    {
        qDebug() << "SUCESS:Dados da Playlist Actualizados";
    }
    else
    {
        qDebug() << "ERROR:Dados da Playlist Actualizados";
    }

    //Remover Musicas Existentes
    QSqlQuery delete_songs;
    delete_songs.prepare("DELETE FROM Pertence WHERE IN ID_Playlist=:ID_Playlist");
    delete_songs.bindValue(":ID_Playlist", Playlist->getIdBD());

    if(delete_songs.exec())
    {
        qDebug() << "SUCESS:Lista de Musicas Apagada";
    }
    else
    {
        qDebug() << "ERROR:Lista de Musicas Apagada";
    }

    //Adicionar Musicas
    QList<Musica *> songlist;
    Playlist->getMusicas(&songlist);

    if(!songlist.empty())
    {
        QSqlQuery addSongToPlayList;
        for(int i =0; i< songlist.size();i++)
        {
            addSongToPlayList.prepare("INSERT INTO Pertence (ID_Musica,ID_Playlist)"
                                      "VALUES (:ID_Musica,:ID_Playlist)");

            addSongToPlayList.bindValue(":ID_Musica",songlist[i]->getIdBD());
            addSongToPlayList.bindValue(":ID_Playlist",Playlist->getIdBD());

            if(addSongToPlayList.exec())
            {
                qDebug() << "Song" << songlist[i]->getNome() <<"Added to playlist" << Playlist->getNome();
            }
            else
            {
                qDebug() << "Failed to Add Song"<< songlist[i]->getIdBD()<< "to Playlist" << Playlist->getIdBD();
                return false;
            }
        }

        qDebug() << "PROCESS END: Actualizada da Playlist";
        qDebug() <<"=================================";
        return true;
    }
    else
    {
        qDebug() << "PROCESS END: Nao existem Musicas para adicionar";
        qDebug() <<"=================================";
        return true;
    }

    return false;
}
bool Database::addSongToPlaylist(Playlist *Playlist, int music_id)
{
    qDebug() <<"=================================";
    qDebug() << "PROCESS: Add Song to Playlist"  << music_id;

    QSqlQuery addSongToPlayList;
    addSongToPlayList.prepare("INSERT INTO Pertence (ID_Musica,ID_Playlist)"
                              "VALUES (:ID_Musica,:ID_Playlist)");
    addSongToPlayList.bindValue(":ID_Musica",   music_id);
    addSongToPlayList.bindValue(":ID_Playlist", Playlist->getIdBD());

    if(addSongToPlayList.exec())
    {
        qDebug() << "PROCESS END: Song " << music_id  << "Added to playlist" << Playlist->getNome();
        qDebug() <<"=================================";
        return  true;
    }

    qDebug() << "PROCESS END: Failed to Add Song"<< music_id  << "Added to playlist" << Playlist->getNome();
    qDebug() <<"=================================";
    return false;
}
bool Database::removeSongFromPlaylist(Playlist *Playlist, int music_id)
{
    qDebug() <<"=================================";
    qDebug() << "PROCESS: Remove Song from Playlist"  << music_id;

    QSqlQuery removeSongFromPlaylist;
    removeSongFromPlaylist.prepare("DELETE FROM Pertence WHERE ID_Musica=:ID_Musica and ID_Playlist=:ID_Playlist;");
    removeSongFromPlaylist.bindValue(":ID_Musica",   music_id);
    removeSongFromPlaylist.bindValue(":ID_Playlist", Playlist->getIdBD());

    if(removeSongFromPlaylist.exec())
    {
        qDebug() << "PROCESS END: Song " << music_id  << "Removed from playlist" << Playlist->getNome();
        qDebug() <<"=================================";
        return  true;
    }

    qDebug() << "PROCESS END: Failed to remove Song"<< music_id  << "Removed from playlist ." << Playlist->getNome();
    qDebug() <<"=================================";
    return false;
}

bool Database::removePlaylist(Playlist *Playlist)
{
    //Remover Musicas Associadas
    QSqlQuery deleteSonglist;
    deleteSonglist.prepare("DELETE from Pertence where ID_Playlist=:ID_Playlist; ");
    deleteSonglist.bindValue(":ID_Playlist",Playlist->getIdBD());
    if(deleteSonglist.exec())
    {
        qDebug() << "Musicas Retiradas da Playlist" << Playlist->getNome();
    }
    else
    {
        qDebug() << "Nao foi possivel Retirar as musicas da Playlist" << Playlist->getNome();
    }

    //Remover Playlist
    QSqlQuery deletePlaylist;
    deletePlaylist.prepare("DELETE from Playlist where ID_Playlist=:ID_Playlist;");
    deletePlaylist.bindValue(":ID_Playlist",Playlist->getIdBD());

    if(deletePlaylist.exec())
    {
        qDebug() << "Playlist" << Playlist->getNome() << " Apagada";
        return true;
    }

    qDebug() << "Playlist" << Playlist->getNome() << " Não foi Apagada";

    return false;
}
