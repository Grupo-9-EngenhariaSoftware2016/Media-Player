#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "classes.h"
#include <database.h>




MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->statusBar->showMessage(QDir::currentPath());

    // NEW //================================
    // DIR for musics and images
    QString autor_path = QDir::currentPath() +"/debug/autor";
    if(!QDir(autor_path).exists())
    {
        qDebug() << "Criar " << autor_path << "diretoria.";
        QDir().mkpath(autor_path);
    }

    QString albuns_path = QDir::currentPath() +"/debug/album";
    if(!QDir(albuns_path).exists())
    {
        qDebug() << "Criar " << albuns_path << "diretoria.";
        QDir().mkpath(albuns_path);
    }
    // END NEW //================================

    QString sPath = "C:/ ";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);

    // Set buttons checkables
    ui->menu_small_button_album->setCheckable(true);
    ui->menu_small_button_artist->setCheckable(true);
    ui->menu_small_button_list->setCheckable(true);
    ui->menu_small_button_play->setCheckable(true);
    ui->menu_small_button_song->setCheckable(true);

    ui->menu_full_button_album->setCheckable(true);
    ui->menu_full_button_artist->setCheckable(true);
    ui->menu_full_button_list->setCheckable(true);
    ui->menu_full_button_play->setCheckable(true);
    ui->menu_full_button_song->setCheckable(true);

    ui->page_categories_button_select->setCheckable(true);
    ui->page_album_info_button_select->setCheckable(true);
    ui->page_artist_button_select->setCheckable(true);

    ui->player_button_play->setCheckable(true);
    ui->player_button_shuffle->setCheckable(true);
    ui->player_button_repeat->setCheckable(true);
    ui->player_button_mute->setCheckable(true);

    ui->options_button_edit->setCheckable(true);

    // Hide tab for add album
    ui->page_add_album_tabs->tabBar()->hide();

    connect(_player._mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(on_player_positionChanged(qint64)));
    connect(_player._mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(on_player_durationChanged(qint64)));
    connect(_player._mediaPlayer, SIGNAL(mutedChanged(bool)), this, SLOT(on_player_mutedChanged(bool)));
    connect(_player._mediaPlayer, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode mode)), this, SLOT(on_player_playbackModeChanged(QMediaPlaylist::PlaybackMode mode)));
    connect(_player._mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_player_stateChanged(QMediaPlayer::State)));

    connect(ui->page_categories_tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(on_categories_cell_changed(int,int)));
    connect(ui->page_album_info_tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(on_album_info_cell_changed(int,int)));
    connect(ui->page_artist_tableWidget_albuns, SIGNAL(cellChanged(int,int)), this, SLOT(on_artist_cell_changed(int,int)));
    connect(ui->page_playlist_tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(on_playlist_cell_changed(int,int)));

    Autor *newArtist;
    Musica *newSong;
    Album *newAlbum;
    Playlist *newPlaylist;
    QList<Musica*> music_list;
    QList<Autor*> autor_list;
    QDate date;

    //============================================================================
    // Carregar dados Da BD
    Database BD;
    BD.connOpen();qDebug() << "Abriu Base de Dados";

    //===================================
    // LoadGeneros
    QSqlQuery loadStyle;
    if(loadStyle.exec("select Nome_Genero from Genero;"))
    {
        qDebug() << "load Genero";
        while(loadStyle.next())
        {
            Generos <<loadStyle.value(0).toString();
        }
    }
    else
    {
        qDebug() << "Cannot Load Genero";
    }

    //Debug Generos
    foreach (QString v,Generos)
    {
        qDebug()<<v ; //returns "Ten" , "Two" , "..." , ".....";
    }

    //===================================
    QSqlQuery loadAutor;
    if(loadAutor.exec("select * from Autor;"))
    {
        qDebug() << "load Autor";
        while(loadAutor.next())
        {
            newArtist   = new Autor;
            newArtist->setIdBD(loadAutor.value(0).toInt());
            newArtist->setNome(loadAutor.value(1).toString());
            newArtist->setNacionalidade(loadAutor.value(2).toString());
            newArtist->setDataNascimento(loadAutor.value(3).toDate());
            newArtist->setImagem(loadAutor.value(4).toString());
            _artists.append(newArtist);
        }
    }
    else
    {
        qDebug() << "Cannot Load Autor";
    }

    //===================================
    // LoadAlbuns
    QSqlQuery loadAlbum;
    int id_album;
    if(loadAlbum.exec("select * from Album;"))
    {
        qDebug() << "load Album";
        while(loadAlbum.next())
        {
            newAlbum = new Album;
            id_album=loadAlbum.value(0).toInt();
            newAlbum->setIdBD(id_album);
            newAlbum->setNome(loadAlbum.value(1).toString());
            newAlbum->setDescricao(loadAlbum.value(2).toString());
            newAlbum->setDiretoria(loadAlbum.value(3).toString());
            newAlbum->setImagem(loadAlbum.value(4).toString());
            newAlbum->setAno(loadAlbum.value(5).toInt());
            newAlbum->setGenero(loadAlbum.value(6).toString());

            //============================================================================
            // Load Songs
            QSqlQuery loadSong,loadArtistToSong;
            if (loadSong.exec("select * from Musica where ID_Album='"+QString::number(id_album)+"';"))
            {
                qDebug() << "load Song from Album" << id_album;
                while(loadSong.next())
                {
                    newSong = new Musica;
                    newSong->setIdBD(loadSong.value(0).toInt());
                    newSong->setNome(loadSong.value(1).toString());
                    newSong->setDiretoria(loadSong.value(2).toString());
                    newSong->setFaixa(loadSong.value(3).toInt());

                    //============================================================================
                    // Load Artists to Songs
                    loadArtistToSong.prepare("select Autor.ID_Autor from Autor "
                                             "join Tem on Tem.ID_Autor=Autor.ID_Autor "
                                             "join Musica on Tem.ID_Musica=Musica.ID_Musica "
                                             "where Tem.ID_Musica=:ID_Musica;");
                    loadArtistToSong.bindValue(":ID_Musica", newSong->getIdBD());

                    if(loadArtistToSong.exec())
                    {
                        while(loadArtistToSong.next())
                        {
                            for(int i=0; i < _artists.size();i++)
                            {
                                if(_artists[i]->getIdBD()==loadArtistToSong.value(0).toInt())
                                {
                                       newSong->addAutor(_artists[i]);
                                }
                            }
                        }
                    }

                    _songs.append(newSong);
                    music_list.append(newSong);

                }
                newAlbum->setMusicas(&music_list);
                music_list.clear();
            }
            else
            {
                qDebug() << "Cannot Load Song";
            }

            _albuns.append(newAlbum);
        }
    }
    else
    {
        qDebug() << "Cannot Load Album";
    }

    //===================================
    //LoadPlaylist
    QSqlQuery loadPlaylist;
    QSqlQuery loadPlaylistSongs;
    if (loadPlaylist.exec("select * from Playlist;"))
    {
        qDebug() << "load Playlist";
        while(loadPlaylist.next())
        {
            newPlaylist = new Playlist;
            newPlaylist->setIdBD(loadPlaylist.value(0).toInt());
            newPlaylist->setNome(loadPlaylist.value(1).toString());
            newPlaylist->setDescricao(loadPlaylist.value(2).toString());

            //============================================================================
            //LoadSongsToPlaylist
            loadPlaylistSongs.prepare(
                        "select Musica.ID_Musica from Musica "
                        "join Pertence on Musica.ID_Musica=Pertence.ID_Musica "
                        "join Playlist on Playlist.ID_Playlist=Pertence.ID_Playlist "
                        "where Playlist.ID_Playlist=:ID_Playlist;");
            loadPlaylistSongs.bindValue(":ID_Playlist",newPlaylist->getIdBD());

            if(loadPlaylistSongs.exec())
            {
                while(loadPlaylistSongs.next())
                {
                    //qDebug() << "Playlist" << newPlaylist->getIdBD();
                    for(int i=0; i< _songs.size();i++)
                    {
                        if(_songs[i]->getIdBD()==loadPlaylistSongs.value(0).toInt())
                        {
                           music_list.append(_songs[i]);
                           //qDebug() <<_songs[i]->getNome();
                        }
                    }
                }
            }
            else
            {
                qDebug() << "Cannot Load Songs to Playlist";
            }

            newPlaylist->setMusicas(&music_list);
            _playlist.append(newPlaylist);
            music_list.clear();
        }
    }
    else
    {
        qDebug() << "Cannot Playlist";
    }

    BD.connClose(); qDebug() << "Fechou Base de Dados";
    //============================================================================

    ExpandMenu(false);
    MovePageToAlbuns();
}
MainWindow::~MainWindow()
{
    QMainWindow::centralWidget();

    while (!_albuns.isEmpty())
        _albuns.removeLast();
    while (!_playlist.isEmpty())
        _playlist.removeLast();

    while (!_artists.isEmpty())
        _artists.removeLast();

    while (!_songs.isEmpty())
        _songs.removeLast();

    while (!_newSongList.isEmpty())
        _newSongList.removeLast();

    delete ui;
}

//==============================================================
// Metodos de simplificação
// Para tornar o código mais legivel e de fácil manutenção
QList<Autor*> MainWindow::getArtistsFrom(Album *album)
{
    QList <Musica*> songs;
    QList <Autor*> artists, temp;

    album->getMusicas(&songs);

    for(int i = 0; i < songs.size(); i++)
    {
        songs[i]->getAutor(&temp);
    }

    for(int i = 0; i < temp.size(); i++)
    {
        if(!artists.contains(temp[i]))
        {
            artists.append(temp[i]);
        }
    }

    return artists;
}
QList<Autor*> MainWindow::getArtistsFrom(Playlist *playlist)
{
    QList <Musica*> songs;
    QList <Autor*> artists, temp;

    playlist->getMusicas(&songs);

    for(int i = 0; i < songs.size(); i++)
    {
        songs[i]->getAutor(&temp);
    }

    for(int i = 0; i < temp.size(); i++)
    {
        if(!artists.contains(temp[i]))
        {
            artists.append(temp[i]);
        }
    }

    return artists;
}
QList<Autor*> MainWindow::getArtistsFrom(Musica *song)
{
    QList<Autor*> artists;
    song->getAutor(&artists);

    return artists;
}
QString MainWindow::printArtistList(QList<Autor*> artists)
{
    QString text = "";
    int displayed = 0;

    if(artists.isEmpty())
    {
        return NULL;
    }else{

        for(int i = 0; i < artists.size(); i++)
        {
            if(displayed != 0)
            {
                QTextStream(&text) << ", ";
            }

            if(displayed == 3)
            {
                QTextStream(&text) << "...";
                break;
            }
            QTextStream(&text) << artists[i]->getNome();
            displayed++;
        }
        return text;
    }
}
QList <Album*> MainWindow::getAlbunsFromArtist(Autor *artist)
{
    QList <Album*> albuns;

    for (int i = 0; i < _albuns.size(); i++)
    {
        if(_albuns[i]->hasAutor(artist))
            albuns.append(_albuns[i]);
    }

    return albuns;
}
QList <Musica*> MainWindow::getSongsFromArtist(Autor *artist)
{
    QList <Musica*> songs;

    for (int i = 0; i < _songs.size(); i++)
    {
        if(_songs[i]->hasAutor(artist))
            songs.append(_songs[i]);
    }

    return songs;
}
Album* MainWindow::getAlbumWith(Musica* song)
{
    QList <Musica*> tempList;
    for(int i = 0; i < _albuns.size(); i++)
    {
        tempList.clear();
       _albuns[i]->getMusicas(&tempList);
        if(tempList.contains(song))
            return _albuns[i];
    }
    return NULL;
}

void MainWindow::CheckMenuButton(QString button)
{
    QStringList options;
    /*
     * Albuns   - 0
     * Artists  - 1
     * Player   - 2
     * Playlist - 3
     * Songs    - 4
     * Search   - 5
     * */
    options << "Albuns" << "Artists" << "Player" << "Playlist" << "Songs" << "Search";

    switch(options.indexOf(button))
    {
    case 0:
        //Set album as checked
        ui->menu_small_button_album->setChecked(true);
        ui->menu_full_button_album->setChecked(true);
        ui->menu_small_button_artist->setChecked(false);
        ui->menu_full_button_artist->setChecked(false);
        ui->menu_small_button_list->setChecked(false);
        ui->menu_full_button_list->setChecked(false);
        ui->menu_full_button_play->setChecked(false);
        ui->menu_small_button_play->setChecked(false);
        ui->menu_small_button_song->setChecked(false);
        ui->menu_full_button_song->setChecked(false);

        break;
    case 1:
        //Set artist as checked
        ui->menu_small_button_album->setChecked(false);
        ui->menu_full_button_album->setChecked(false);
        ui->menu_small_button_artist->setChecked(true);
        ui->menu_full_button_artist->setChecked(true);
        ui->menu_small_button_list->setChecked(false);
        ui->menu_full_button_list->setChecked(false);
        ui->menu_full_button_play->setChecked(false);
        ui->menu_small_button_play->setChecked(false);
        ui->menu_small_button_song->setChecked(false);
        ui->menu_full_button_song->setChecked(false);

        break;
    case 2:
        //Set player as checked
        ui->menu_small_button_album->setChecked(false);
        ui->menu_full_button_album->setChecked(false);
        ui->menu_small_button_artist->setChecked(false);
        ui->menu_full_button_artist->setChecked(false);
        ui->menu_small_button_list->setChecked(false);
        ui->menu_full_button_list->setChecked(false);
        ui->menu_full_button_play->setChecked(true);
        ui->menu_small_button_play->setChecked(true);
        ui->menu_small_button_song->setChecked(false);
        ui->menu_full_button_song->setChecked(false);

        break;
    case 3:
        //Set playlist as checked
        ui->menu_small_button_album->setChecked(false);
        ui->menu_full_button_album->setChecked(false);
        ui->menu_small_button_artist->setChecked(false);
        ui->menu_full_button_artist->setChecked(false);
        ui->menu_small_button_list->setChecked(true);
        ui->menu_full_button_list->setChecked(true);
        ui->menu_full_button_play->setChecked(false);
        ui->menu_small_button_play->setChecked(false);
        ui->menu_small_button_song->setChecked(false);
        ui->menu_full_button_song->setChecked(false);

        break;
    case 4:
        //Set Songs as checked
        ui->menu_small_button_album->setChecked(false);
        ui->menu_full_button_album->setChecked(false);
        ui->menu_small_button_artist->setChecked(false);
        ui->menu_full_button_artist->setChecked(false);
        ui->menu_small_button_list->setChecked(false);
        ui->menu_full_button_list->setChecked(false);
        ui->menu_full_button_play->setChecked(false);
        ui->menu_small_button_play->setChecked(false);
        ui->menu_small_button_song->setChecked(true);
        ui->menu_full_button_song->setChecked(true);

        break;
    case 5:
        //Set search as checked
        ui->menu_small_button_album->setChecked(false);
        ui->menu_full_button_album->setChecked(false);
        ui->menu_small_button_artist->setChecked(false);
        ui->menu_full_button_artist->setChecked(false);
        ui->menu_small_button_list->setChecked(false);
        ui->menu_full_button_list->setChecked(false);
        ui->menu_full_button_play->setChecked(false);
        ui->menu_small_button_play->setChecked(false);
        ui->menu_small_button_song->setChecked(false);
        ui->menu_full_button_song->setChecked(false);

        break;
    }
}
void MainWindow::ExpandMenu(bool expand)
{
    if(expand)
    {
        ui->menuTab->setCurrentIndex(0);
        ui->menuTab->setFixedWidth(200);
    }else{
        ui->menuTab->setCurrentIndex(1);
        ui->menuTab->setFixedWidth(41);
    }
}
void MainWindow::ShowOptionsTab(bool show)
{
    if(show)
    {
        // Set Options tab
        ui->Player_tab->setCurrentIndex(1);
    }else{
        // Set Player tab
        ui->Player_tab->setCurrentIndex(0);
    }
}
void MainWindow::ShowProgressTab(bool show)
{
    if(show)
    {
        // Set Progress tab
        ui->Player_tab->setCurrentIndex(2);
    }else{
        // Set Player tab
        ui->Player_tab->setCurrentIndex(0);
    }
}

void MainWindow::FormatTableFor(QTableWidget *table, QString format)
{
    QStringList options;
    QStringList artistsNames;
    artistsNames << "Desconhecido";
    QStringList tableLabels;
    /*
     * Albuns         - 0
     * Artists        - 1
     * Playlist       - 2
     * Songs          - 3
     * NewSong        - 4
     * SimplifiedSong - 5
     * ArtistName     - 6
     * */
    options << "Albuns" << "Artists" << "Playlist" << "Songs" << "NewSong" << "SimplifiedSong" << "ArtistName";

    switch(options.indexOf(format))
    {
    case 0:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Capa") << tr("Nome") << tr("Género") << tr("Artistas");
        table->setColumnWidth(1,50);
        table->verticalHeader()->setDefaultSectionSize(50);
        // Gender Delegate
        genderDelegate = new MyDelegate(Generos,this);
        table->setItemDelegateForColumn(3,genderDelegate);
        // Artist Delegate
        for(int i = 0; i < _artists.size(); i++)
        {
            artistsNames << _artists[i]->getNome();
        }
        artistDelegate = new MyDelegate(artistsNames, this);
        table->setItemDelegateForColumn(4, artistDelegate);
        break;

    case 1:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Imagem") << tr("Nome") << tr("Nº de Álbuns") << tr("Nº de Musicas");
        table->setColumnWidth(1,50);
        table->verticalHeader()->setDefaultSectionSize(50);
        break;

    case 2:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Nome") << tr("Tamanho") << tr("Tempo de Reprodução");
        table->verticalHeader()->setDefaultSectionSize(25);
        break;

    case 3:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Nome") << tr("Genero") << tr("Album") << tr("Artistas");
        table->verticalHeader()->setDefaultSectionSize(25);
        genderDelegate = new MyDelegate(Generos,this);
        table->setItemDelegateForColumn(2,genderDelegate);
        // Artist Delegate
        for(int i = 0; i < _artists.size(); i++)
        {
            artistsNames << _artists[i]->getNome();
        }
        artistDelegate = new MyDelegate(artistsNames, this);
        table->setItemDelegateForColumn(4, artistDelegate);
        break;

    case 4:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Faixa") << tr("Nome") << tr("Genero") << tr("Diretoria");
        table->verticalHeader()->setDefaultSectionSize(25);
        genderDelegate = new MyDelegate(Generos,this);
        table->setItemDelegateForColumn(3,genderDelegate);
        break;

    case 5:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Nome") << tr("Artists");
        table->verticalHeader()->setDefaultSectionSize(25);
        // Artist Delegate
        for(int i = 0; i < _artists.size(); i++)
        {
            artistsNames << _artists[i]->getNome();
        }
        artistDelegate = new MyDelegate(artistsNames, this);
        table->setItemDelegateForColumn(2, artistDelegate);
        break;

    case 6:
        // Set labels para a tabela de elementos
        tableLabels << tr(" ") << tr("Imagem") << tr("Nome");
        table->setColumnWidth(1,50);
        table->verticalHeader()->setDefaultSectionSize(50);
        break;
    }

    table->clear();
    table->setRowCount(0);
    table->setColumnCount(0);
    table->setColumnCount(tableLabels.size());
    table->setHorizontalHeaderLabels(tableLabels);
    table->horizontalHeader()->setHidden(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setHidden(true);
    table->setColumnHidden(0,true);
    table->setColumnWidth(0,25);
    table->setFrameShape(QFrame::NoFrame);
    table->setFrameShadow(QFrame::Sunken);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setTextElideMode(Qt::ElideRight);
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    QObject::connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_table_checkBox_pressed(QTableWidgetItem*)));

}
void MainWindow::AddAlbumLineToTable(QTableWidget *table, Album *album)
{
    table->blockSignals(true);

    int newRow = table->rowCount();
    QTableWidgetItem *item;
    QLabel *artwork;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_albuns.indexOf(album));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Artwork
    artwork = new QLabel;
    QPixmap pxmap = QPixmap(album->getImagem());
    if(!pxmap.isNull())
        artwork->setPixmap(QPixmap(album->getImagem()).scaled(100,100));
    else
        artwork->setPixmap(QPixmap(":/album_art.png"));

    artwork->setAlignment(Qt::AlignCenter);
    artwork->setStyleSheet("background-color: rgba(255, 255, 255, 10);");
    table->setCellWidget(newRow, 1, artwork);
    table->setColumnWidth(1,100);
    table->setRowHeight(newRow,100);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_albuns.indexOf(album));
    item->setData(Qt::DisplayRole,album->getNome());
    table->setItem(newRow, 2, item);

    // Gender
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_albuns.indexOf(album));
    item->setData(Qt::DisplayRole,album->getGenero());
    table->setItem(newRow, 3, item);

    // Artists
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_albuns.indexOf(album));
    item->setData(Qt::DisplayRole,printArtistList(getArtistsFrom(album)));
    table->setItem(newRow, 4, item);

    table->blockSignals(false);

}
void MainWindow::AddSongLineToTable(QTableWidget *table, Musica *song)
{
    table->blockSignals(true);

    int newRow = table->rowCount();
    QTableWidgetItem *item;
    Album *fromAlbum;
    table->insertRow(newRow);
    table->setRowHeight(newRow,50);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_songs.indexOf(song));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_songs.indexOf(song));
    item->setData(Qt::DisplayRole,song->getNome());
    table->setItem(newRow, 1, item);
    table->setColumnWidth(1,200);
    // Gender
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_songs.indexOf(song));
    item->setData(Qt::DisplayRole,song->getGenero());
    table->setItem(newRow, 2, item);

    // Album
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_songs.indexOf(song));
    fromAlbum = getAlbumWith(song);
    item->setData(Qt::DisplayRole,fromAlbum != NULL ? fromAlbum->getNome() : "Nenhum");
    table->setItem(newRow, 3, item);

    // Artist
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_songs.indexOf(song));
    item->setData(Qt::DisplayRole,printArtistList(getArtistsFrom(song)));
    table->setItem(newRow, 4, item);

    table->blockSignals(false);

}
void MainWindow::AddNewSongLineToTable(QTableWidget *table, Musica *newSong)
{
    table->blockSignals(true);

    int newRow = table->rowCount();
    QTableWidgetItem *item;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_newSongList.indexOf(newSong));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Faixa
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_newSongList.indexOf(newSong));
    item->setData(Qt::DisplayRole,newSong->getFaixa());
    table->setItem(newRow, 1, item);

    // Nome
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_newSongList.indexOf(newSong));
    item->setData(Qt::DisplayRole,newSong->getNome());
    table->setItem(newRow, 2, item);

    // Gender
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_newSongList.indexOf(newSong));
    item->setData(Qt::DisplayRole,newSong->getGenero());
    table->setItem(newRow, 3, item);

    // Diretoria
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_newSongList.indexOf(newSong));
    item->setData(Qt::DisplayRole,newSong->getDiretoria());
    table->setItem(newRow, 4, item);

    table->blockSignals(false);

}
void MainWindow::AddSimplifiedSongLineToTable(QTableWidget *table, Musica *song, QList<Musica*> *list)
{
    table->blockSignals(true);

    int newRow = table->rowCount();
    QTableWidgetItem *item;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,list->indexOf(song));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,list->indexOf(song));
    item->setData(Qt::DisplayRole,song->getNome());
    table->setItem(newRow, 1, item);

    // Artists
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,list->indexOf(song));
    item->setData(Qt::DisplayRole,printArtistList(getArtistsFrom(song)));
    table->setItem(newRow, 2, item);
    table->blockSignals(false);

}
void MainWindow::AddArtistNameLineToTable(QTableWidget *table, Autor *artist)
{
    table->blockSignals(true);

    int newRow = table->rowCount();
    QTableWidgetItem *item;
    QLabel *photo;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Photo
    photo = new QLabel;
    QPixmap pxmap = QPixmap(artist->getImagem());
    if(!pxmap.isNull())
        photo->setPixmap(QPixmap(artist->getImagem()).scaled(50,50));
    else
        photo->setPixmap(QPixmap(":/artist_art.png").scaled(50,50));
    photo->setAlignment(Qt::AlignCenter);
    photo->setStyleSheet("background-color: rgba(255, 255, 255, 10);");
    table->setCellWidget(newRow, 1, photo);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    item->setData(Qt::DisplayRole,artist->getNome());
    table->setItem(newRow, 2, item);
    table->blockSignals(false);

}
void MainWindow::AddArtistLineToTable(QTableWidget *table, Autor *artist)
{
    table->blockSignals(true);

    QString text;
    int newRow = table->rowCount();
    QTableWidgetItem *item;
    QLabel *photo;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Photo
    photo = new QLabel;
    QPixmap pxmap = QPixmap(artist->getImagem());
    if(!pxmap.isNull())
        photo->setPixmap(QPixmap(artist->getImagem()).scaled(100,100));
    else
        photo->setPixmap(QPixmap(":/social.png"));
    photo->setAlignment(Qt::AlignCenter);
    photo->setStyleSheet("background-color: rgba(255, 255, 255, 10);");
    table->setCellWidget(newRow, 1, photo);
    table->setColumnWidth(1,100);
    table->setRowHeight(newRow,100);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    item->setData(Qt::DisplayRole,artist->getNome());
    table->setItem(newRow, 2, item);

    // nº Albuns
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    text = "";
    QTextStream(&text) << getAlbunsFromArtist(artist).size() << " albuns";
    item->setData(Qt::DisplayRole,text);
    table->setItem(newRow, 3, item);

    // nº Songs
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_artists.indexOf(artist));
    text = "";
    QTextStream(&text) << getSongsFromArtist(artist).size() << " musicas";
    item->setData(Qt::DisplayRole,text);
    table->setItem(newRow, 4, item);
    table->blockSignals(false);

}
void MainWindow::AddPlaylistLineToTable(QTableWidget *table, Playlist *playlist)
{
    table->blockSignals(true);

    QString text = "";
    QList<Musica*> songList;
    QMediaPlayer mediaSong;
    qint64 totalDuration = 0;
    int newRow = table->rowCount();
    QTableWidgetItem *item;
    table->insertRow(newRow);

    // CheckBox
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_playlist.indexOf(playlist));
    item->setCheckState(Qt::Unchecked);
    table->setItem(newRow, 0, item);

    // Name
    item = new QTableWidgetItem;
    item->setData(Qt::WhatsThisRole,_playlist.indexOf(playlist));
    item->setData(Qt::DisplayRole,playlist->getNome());
    table->setItem(newRow, 1, item);

    // Size
    item = new QTableWidgetItem;
    QTextStream(&text) << playlist->getSize() << " musicas";
    item->setData(Qt::WhatsThisRole,_playlist.indexOf(playlist));
    item->setData(Qt::DisplayRole,text);
    table->setItem(newRow, 2, item);

    // Play Time
    item = new QTableWidgetItem;
    playlist->getMusicas(&songList);
    for(int i = 0; i < songList.size(); i++)
    {
        mediaSong.setMedia(QUrl::fromLocalFile(songList[i]->getDiretoria()));
        totalDuration += mediaSong.duration();
    }
    QTime duration(0, totalDuration / 60000, qRound((totalDuration % 60000) / 1000.0));
    item->setData(Qt::WhatsThisRole,_playlist.indexOf(playlist));
    item->setData(Qt::DisplayRole,duration.toString("hh:mm:ss"));
    table->setItem(newRow, 3, item);
    table->blockSignals(false);

}

void MainWindow::MovePageToAlbuns()
{
    CheckMenuButton("Albuns");

    ui->pages->setCurrentIndex(0);

    Refresh();
}
void MainWindow::MovePageToAlbumInfo(int index)
{
    _showingAlbum = index;
    CheckMenuButton("Albuns");

    ui->pages->setCurrentIndex(1);

    Refresh();
}
void MainWindow::MovePageToArtists()
{
    CheckMenuButton("Artists");

    ui->pages->setCurrentIndex(0);

    Refresh();
}
void MainWindow::MovePageToArtistInfo(int index)
{
    _showingArtist = index;
    CheckMenuButton("Artists");

    ui->pages->setCurrentIndex(5);

    Refresh();
}
void MainWindow::MovePageToPlayer()
{
    CheckMenuButton("Player");

    ui->pages->setCurrentIndex(6);

    Refresh();
}
void MainWindow::MovePageToPlaylists()
{
    CheckMenuButton("Playlist");

    ui->pages->setCurrentIndex(0);

    Refresh();
}
void MainWindow::MovePageToPlaylistInfo(int index)
{
    _showingPlaylist = index;
    CheckMenuButton("Playlist");

    ui->pages->setCurrentIndex(6);

    Refresh();
}
void MainWindow::MovePageToSongs()
{
    CheckMenuButton("Songs");

    ui->pages->setCurrentIndex(0);

    Refresh();
}
void MainWindow::MovePageToSearch()
{
    CheckMenuButton("Search");

    ui->pages->setCurrentIndex(4);

    Refresh();
}
void MainWindow::MovePageToAddAlbuns()
{
	CheckMenuButton("Albuns");

    ui->page_add_album_label_artwork->setPixmap(QPixmap(":/album_art.png"));
    ui->page_add_album_lineEdit_name->setText("");
    ui->page_add_album_comboBox_gender->addItems(Generos);
    ui->page_add_album_comboBox_gender->setCurrentIndex(0);
    ui->page_add_album_lineEdit_year->setText("");
    _imageURL = "";
    ui->page_add_album_textEdit_description->clear();

    while(!_newSongList.isEmpty())
        _newSongList.removeLast();

    ui->pages->setCurrentIndex(2);
    ui->page_add_album_tabs->setCurrentIndex(0);

    Refresh();
}
void MainWindow::MovePageToAddPlaylist()
{
	CheckMenuButton("Playlist");

    ui->pages->setCurrentIndex(7);

    if(_editMode)
    {
        ui->page_add_playlist_lineEdit_name->setText(_playlist[_showingPlaylist]->getNome());
        ui->page_add_playlist_plainText_description->appendPlainText(_playlist[_showingPlaylist]->getDescricao());
        _playlist[_showingPlaylist]->getMusicas(&_newSongList);
    }

    Refresh();
}
void MainWindow::MovePageToAddSongs()
{
	CheckMenuButton("Songs");

    ui->pages->setCurrentIndex(3);

    Refresh();
}

void MainWindow::NewArtist()
{
    mdialog = new Dialog(this);
    Autor *artist;
    mdialog->setWindowTitle("Criar novo artista");
    mdialog->exec();

    if(mdialog->result() == 1)
    {
        artist = mdialog->getNewArtist();
        artist->setDataAdicao(QDate::currentDate());
        _artists.append(artist);
    }
}
void MainWindow::Refresh()
{
    if(ui->pages->currentIndex() == 0) // categories
    {
        if(ui->menu_small_button_album->isChecked()) // Albuns
        {
            ShowOptionsTab(false);
            ShowProgressTab(false);

            //Reset botão "Selecionar"
            ui->page_categories_button_select->setChecked(false);

            // Order Options
            ui->page_categories_comboBox_order->clear();
            ui->page_categories_comboBox_order->setHidden(false);
            ui->page_categories_label_order->setHidden(false);
            ui->page_categories_comboBox_order->insertItem(0,"Data de Adição");
            ui->page_categories_comboBox_order->insertItem(1,"A a Z");
            ui->page_categories_comboBox_order->insertItem(2,"Género");
            ui->page_categories_comboBox_order->insertItem(3,"Autor");

            //Reset de orderBox
            ui->page_categories_comboBox_order->setCurrentIndex(1);
            ui->page_categories_comboBox_order->setDisabled(false);

            // Set main page Albuns
            ui->page_categories_label_category->setText("Álbuns");

            FormatTableFor(ui->page_categories_tableWidget, "Albuns");
            for(int i = 0; i < _albuns.size(); i++)
            {
                AddAlbumLineToTable(ui->page_categories_tableWidget, _albuns[i]);
            }

        }else if(ui->menu_small_button_artist->isChecked()) // Artists
        {
            ShowOptionsTab(false);
            ShowProgressTab(false);

            //Reset botão "Selecionar"
            ui->page_categories_button_select->setChecked(false);

            // Order Options
            ui->page_categories_comboBox_order->clear();
            ui->page_categories_comboBox_order->setHidden(false);
            ui->page_categories_label_order->setHidden(false);
            ui->page_categories_comboBox_order->insertItem(0,"Data de Adição");
            ui->page_categories_comboBox_order->insertItem(1,"A a Z");
            ui->page_categories_comboBox_order->insertItem(2,"Nº de Albuns");
            ui->page_categories_comboBox_order->insertItem(3,"Nº de Musicas");

            //Reset de orderBox
            ui->page_categories_comboBox_order->setCurrentIndex(1);
            ui->page_categories_comboBox_order->setDisabled(false);

            // Set main page Autores
            ui->page_categories_label_category->setText("Autores");

            FormatTableFor(ui->page_categories_tableWidget,"Artists");

            //Dados da Tabela
            for (int i = 0; i < _artists.size(); i++)
            {
                AddArtistLineToTable(ui->page_categories_tableWidget,_artists[i]);
            }

        }else if(ui->menu_small_button_list->isChecked()) // Playlists
        {
            ShowOptionsTab(false);
            ShowProgressTab(false);

            //Reset botão "Selecionar"
            ui->page_categories_button_select->setChecked(false);

            // Order Options
            ui->page_categories_comboBox_order->setHidden(true);
            ui->page_categories_label_order->setHidden(true);

            //Reset de orderBox
            ui->page_categories_comboBox_order->setCurrentIndex(1);
            ui->page_categories_comboBox_order->setDisabled(false);

            // Set Main Page Playlists
            ui->page_categories_label_category->setText("Playlists");

            FormatTableFor(ui->page_categories_tableWidget, "Playlist");
            for(int i = 0; i < _playlist.size(); i++)
            {
                AddPlaylistLineToTable(ui->page_categories_tableWidget,_playlist[i]);
            }
        }
        else if(ui->menu_small_button_song->isChecked()) // Songs
        {
            ShowOptionsTab(false);
            ShowProgressTab(false);

            //Reset botão "Selecionar" e "orderBox"
            ui->page_categories_button_select->setChecked(false);

            // Order Options
            ui->page_categories_comboBox_order->clear();
            ui->page_categories_comboBox_order->setHidden(false);
            ui->page_categories_label_order->setHidden(false);
            ui->page_categories_comboBox_order->insertItem(0,"Data de Adição");
            ui->page_categories_comboBox_order->insertItem(1,"A a Z");
            ui->page_categories_comboBox_order->insertItem(2,"Género");
            ui->page_categories_comboBox_order->insertItem(3,"Autor");
            ui->page_categories_comboBox_order->insertItem(4,"Album");

            //Reset de orderBox
            ui->page_categories_comboBox_order->setCurrentIndex(1);
            ui->page_categories_comboBox_order->setDisabled(false);

            // Set main page Musicas
            ui->page_categories_label_category->setText("Músicas");

            FormatTableFor(ui->page_categories_tableWidget,"Songs");
            // tabela de dados
            for(int i = 0; i < _songs.size(); i++)
            {
                AddSongLineToTable(ui->page_categories_tableWidget,_songs[i]);
            }

        }
    }
    else if(ui->pages->currentIndex() == 1) // Album Info
    {
        ShowOptionsTab(false);
        ShowProgressTab(false);

        // reset de botão select
        ui->page_album_info_button_select->setChecked(false);

        ui->page_album_info_label_name->setText(_albuns[_showingAlbum]->getNome());

        if(!_albuns[_showingAlbum]->getImagem().isEmpty())
            ui->page_album_info_label_artwork->setPixmap(_albuns[_showingAlbum]->getImagem());
        else
            ui->page_album_info_label_artwork->setPixmap(QString(":/album_art.png"));

        ui->page_album_info_label_gender->setText(_albuns[_showingAlbum]->getGenero());
        ui->page_album_info_label_year->setText(QString::number(_albuns[_showingAlbum]->getAno()));

        QMenu *menu = new QMenu;
        MyAction *action;

        for(int i = 0; i < _playlist.size(); i++)
        {
            action = new MyAction(this);
            action->setText(_playlist[i]->getNome());
            QObject::connect(action, SIGNAL(myTrigger(MyAction*)), this, SLOT(on_playlist_selected(MyAction*)));

            menu->addAction(action);
        }

        action = new MyAction(this);
        action->setText("Criar nova playlist");
        QObject::connect(action, SIGNAL(myTrigger(MyAction*)), this, SLOT(on_menu_full_button_add_list_clicked()));
        menu->addAction(action);

        ui->page_album_info_button_addTo->setMenu(menu);

        QList <Musica*> songs;
        _albuns[_showingAlbum]->getMusicas(&songs);

        FormatTableFor(ui->page_album_info_tableWidget,"Songs");
        for(int i = 0; i < songs.size(); i++)
        {
            AddSongLineToTable(ui->page_album_info_tableWidget, songs[i]);
        }

    }else if(ui->pages->currentIndex() == 2) // Add Album
    {
        if(ui->page_add_album_tabs->currentIndex() == 0) // Add Album Info
        {
            ShowOptionsTab(false);
            ShowProgressTab(true);

            ui->progress_button_previous->setDisabled(true);
            ui->progress_button_next->setDisabled(false);

        }else if(ui->page_add_album_tabs->currentIndex() == 1) // Add Album Songs
        {
            ShowOptionsTab(false);
            ShowProgressTab(true);

            ui->progress_button_previous->setDisabled(false);
            ui->progress_button_next->setDisabled(false);

            FormatTableFor(ui->page_add_album_tableWidget_toAddMusics,"NewSong");
            ui->page_add_album_tableWidget_toAddMusics->setColumnHidden(0,false);
            ui->page_add_album_tableWidget_toAddMusics->setFrameShape(QFrame::StyledPanel);

            ui->page_add_album_tableWidget_toAddMusics->clear();
            ui->page_add_album_tableWidget_toAddMusics->setRowCount(0);

            // Fill tables
            for(int i = 0; i < _newSongList.size(); i++)
            {
                AddNewSongLineToTable(ui->page_add_album_tableWidget_toAddMusics,_newSongList[i]);
            }

        }else if(ui->page_add_album_tabs->currentIndex() == 2) // Add Album Artists
        {
            ShowOptionsTab(false);
            ShowProgressTab(true);

            ui->progress_button_previous->setDisabled(false);
            ui->progress_button_next->setDisabled(true);

            FormatTableFor(ui->page_add_album_tableWidget_musics,"SimplifiedSong");
            ui->page_add_album_tableWidget_musics->setColumnHidden(0,false);
            ui->page_add_album_tableWidget_musics->setFrameShape(QFrame::StyledPanel);
            FormatTableFor(ui->page_add_album_tableWidget_artists,"ArtistName");
            ui->page_add_album_tableWidget_artists->setColumnHidden(0,false);
            ui->page_add_album_tableWidget_artists->setFrameShape(QFrame::StyledPanel);

            ui->page_add_album_tableWidget_artists->clear();
            ui->page_add_album_tableWidget_artists->setRowCount(0);
            ui->page_add_album_tableWidget_musics->clear();
            ui->page_add_album_tableWidget_musics->setRowCount(0);

            QString search = ui->page_add_album_lineEdit_searchArtists->text();

            // Fill tables
            for(int i = 0; i < _artists.size(); i++)
            {
                if(search.isEmpty())
                {
                    AddArtistLineToTable(ui->page_add_album_tableWidget_artists, _artists[i]);

                }else if(_artists[i]->procurar(search))
                {
                    AddArtistLineToTable(ui->page_add_album_tableWidget_artists, _artists[i]);
                }
            }
            for(int i = 0; i < _newSongList.size(); i++)
            {
                AddSimplifiedSongLineToTable(ui->page_add_album_tableWidget_musics,_newSongList[i],&_newSongList);
            }
        }

    }else if(ui->pages->currentIndex() == 3) // Add Song
    {
        ShowOptionsTab(false);
        ShowProgressTab(true);

        ui->progress_button_previous->setDisabled(true);
        ui->progress_button_next->setDisabled(true);

        // Order Options
        ui->page_add_music_comboBox_albuns->clear();
        ui->page_add_music_comboBox_albuns->setHidden(false);
        for(int i = 0; i < _albuns.size(); i++)
        {
            ui->page_add_music_comboBox_albuns->insertItem(i,_albuns[i]->getNome());
        }

//        FormatTableFor(ui->page_add_music_tableWidget_artists,"ArtistName");
//        ui->page_add_music_tableWidget_artists->setColumnHidden(0,false);
//        ui->page_add_music_tableWidget_artists->setFrameShape(QFrame::StyledPanel);
//        for(int i = 0; i < _artists.size(); i++)
//        {
//            AddArtistNameLineToTable(ui->page_add_music_tableWidget_artists,_artists[i]);
//        }
        FormatTableFor(ui->page_add_music_tableWidget_musics,"NewSong");
        ui->page_add_music_tableWidget_musics->setColumnHidden(0,false);
        ui->page_add_music_tableWidget_musics->setFrameShape(QFrame::StyledPanel);
        for(int i = 0; i < _newSongList.size(); i++)
        {
            AddNewSongLineToTable(ui->page_add_music_tableWidget_musics,_newSongList[i]);
        }

    }else if(ui->pages->currentIndex() == 4) // Search
    {
        ShowOptionsTab(false);
        ShowProgressTab(false);
        QString search = ui->menu_full_lineEdit_search->text();

        ui->page_search_label_main->setText("Resultados para: \"" + search + "\"");

        FormatTableFor(ui->page_search_tableWidget_albuns,"Albuns");
        ui->page_search_tableWidget_albuns->setFrameShape(QFrame::StyledPanel);
        ui->page_search_tableWidget_albuns->verticalHeader()->setDefaultSectionSize(25);

        FormatTableFor(ui->page_search_tableWidget_artists,"Artists");
        ui->page_search_tableWidget_artists->setFrameShape(QFrame::StyledPanel);
        ui->page_search_tableWidget_artists->verticalHeader()->setDefaultSectionSize(25);

        FormatTableFor(ui->page_search_tableWidget_musics,"Songs");
        ui->page_search_tableWidget_musics->setFrameShape(QFrame::StyledPanel);

        for(int i = 0; i < _songs.size(); i++)
        {
            if(search.isEmpty())
            {
                AddSongLineToTable(ui->page_search_tableWidget_musics, _songs[i]);

            }else{
                if(_songs[i]->procurar(search))
                    AddSongLineToTable(ui->page_search_tableWidget_musics, _songs[i]);
            }
        }
        for(int i = 0; i < _artists.size(); i++)
        {
            if(search.isEmpty())
            {
                AddArtistLineToTable(ui->page_search_tableWidget_artists, _artists[i]);

            }else{
                if(_artists[i]->procurar(search))
                    AddArtistLineToTable(ui->page_search_tableWidget_artists, _artists[i]);
            }
        }
        for(int i = 0; i < _albuns.size(); i++)
        {
            if(search.isEmpty())
            {
                AddAlbumLineToTable(ui->page_search_tableWidget_albuns, _albuns[i]);

            }else{
                if(_albuns[i]->procurar(search))
                    AddAlbumLineToTable(ui->page_search_tableWidget_albuns, _albuns[i]);
            }
        }

    }else if(ui->pages->currentIndex() == 5) // Artist
    {
        ShowOptionsTab(false);
        ShowProgressTab(false);

        //Reset botão "Selecionar"
        ui->page_artist_button_select->setChecked(false);

        ui->page_artist_label_name->setText(_artists[_showingArtist]->getNome());

        QMenu *menu = new QMenu;
        MyAction *action;

        for(int i = 0; i < _playlist.size(); i++)
        {
            action = new MyAction(this);
            action->setText(_playlist[i]->getNome());
            QObject::connect(action, SIGNAL(myTrigger(MyAction*)), this, SLOT(on_playlist_selected(MyAction*)));

            menu->addAction(action);
        }

        action = new MyAction(this);
        action->setText("Criar nova playlist");
        QObject::connect(action, SIGNAL(myTrigger(MyAction*)), this, SLOT(on_menu_full_button_add_list_clicked()));
        menu->addAction(action);

        ui->page_album_info_button_addTo->setMenu(menu);

        QList <Album*> albuns = getAlbunsFromArtist(_artists[_showingArtist]);

        FormatTableFor(ui->page_artist_tableWidget_albuns,"Albuns");
        for(int i = 0; i < albuns.size(); i++)
        {
            AddAlbumLineToTable(ui->page_artist_tableWidget_albuns,albuns[i]);
        }
    }else if(ui->pages->currentIndex() == 6) // Playlist
    {
        if(ui->menu_small_button_play->isChecked()) // Player
        {
            ShowOptionsTab(false);
            ShowProgressTab(false);

            ui->page_playlist_label_name->setText("Em Execução");
            ui->page_playlist_button_play->setHidden(true);
            ui->page_playlist_label_description->setHidden(true);
            ui->page_playlist_textEdit_description->setHidden(true);
            ui->page_playlist_button_addMusic->setHidden(true);
            ui->page_playlist_button_remove->setHidden(true);

            QList <Musica*> songs;
            _player.getMusicas(&songs);

            FormatTableFor(ui->page_playlist_tableWidget,"Songs");
            for (int i = 0; i < songs.size(); i++)
            {
                AddSongLineToTable(ui->page_playlist_tableWidget,songs[i]);
            }

        }else{ // Playlist
            ShowOptionsTab(false);
            ShowProgressTab(false);

            ui->page_playlist_label_name->setText(_playlist[_showingPlaylist]->getNome());
            ui->page_playlist_button_play->setHidden(false);
            ui->page_playlist_label_description->setHidden(false);
            ui->page_playlist_textEdit_description->setHidden(false);
            ui->page_playlist_button_addMusic->setHidden(false);
            ui->page_playlist_button_remove->setHidden(false);

            FormatTableFor(ui->page_playlist_tableWidget,"Songs");
            QList <Musica*> songs;
            _playlist[_showingPlaylist]->getMusicas(&songs);

            for (int i = 0; i < songs.size(); i++)
            {
                AddSongLineToTable(ui->page_playlist_tableWidget,songs[i]);
            }

        }
    }else if(ui->pages->currentIndex() == 7) // Add Playlist
    {
        ShowOptionsTab(false);
        ShowProgressTab(true);

        ui->progress_button_previous->setDisabled(true);
        ui->progress_button_next->setDisabled(true);

        FormatTableFor(ui->page_add_playlist_tableWidget_toAdd,"SimplifiedSong");
        ui->page_add_playlist_tableWidget_toAdd->setColumnHidden(0,false);
        ui->page_add_playlist_tableWidget_toAdd->setFrameShape(QFrame::StyledPanel);
        FormatTableFor(ui->page_add_playlist_tableWidget_Added,"SimplifiedSong");
        ui->page_add_playlist_tableWidget_Added->setColumnHidden(0,false);
        ui->page_add_playlist_tableWidget_Added->setFrameShape(QFrame::StyledPanel);

        QString search = ui->page_add_playlist_lineEdit_search->text();

        for(int i = 0; i < _songs.size(); i++)
        {
            if(search.isEmpty())
            {
                AddSimplifiedSongLineToTable(ui->page_add_playlist_tableWidget_toAdd, _songs[i], &_songs);

            }else if(_songs[i]->procurar(search))
            {
                AddSimplifiedSongLineToTable(ui->page_add_playlist_tableWidget_toAdd, _songs[i], &_songs);
            }
        }

        for(int i = 0; i < _newSongList.size(); i++)
        {
            AddSimplifiedSongLineToTable(ui->page_add_playlist_tableWidget_Added, _newSongList[i], &_newSongList);
        }
    }
}
//==============================================================
// Handlers para actualizações de barra de player
void MainWindow::on_player_positionChanged(qint64 position)
{
    ui->player_slider->setValue(position);

    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    ui->player_label_timer->setText(duration.toString(tr("mm:ss")));
}
void MainWindow::on_player_durationChanged(qint64 duration)
{
    ui->player_slider->setRange(0, duration);
    ui->player_slider->setEnabled(duration > 0);
    ui->player_slider->setPageStep(duration / 10);
}
void MainWindow::on_player_mutedChanged(bool muted)
{
    ui->player_button_mute->setChecked(muted);
}
void MainWindow::on_player_playbackModeChanged(QMediaPlaylist::PlaybackMode mode)
{
    switch(mode)
    {
    case QMediaPlaylist::Sequential:
        ui->player_button_repeat->setChecked(false);
        ui->player_button_shuffle->setChecked(false);
        break;

    case QMediaPlaylist::Loop:
        ui->player_button_repeat->setChecked(true);
        ui->player_button_shuffle->setChecked(false);
        break;

    case QMediaPlaylist::Random:
        ui->player_button_repeat->setChecked(false);
        ui->player_button_shuffle->setChecked(true);
        break;

    }
}
void MainWindow::on_player_stateChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::PlayingState:
        ui->player_button_play->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        ui->player_button_play->setChecked(false);
        break;
    case QMediaPlayer::StoppedState:
        ui->player_button_play->setChecked(false);
        break;
    }
}
void MainWindow::on_playlist_selected(MyAction *action)
{
    QString playlistName = action->text();

    for(int i = 0; i < _playlist.size(); i++)
    {
        if(_playlist[i]->getNome() == playlistName)
        {
            if(ui->menu_small_button_album->isChecked())
            {
                QList<Musica*> songs;
                _albuns[_showingAlbum]->getMusicas(&songs);
                _playlist[i]->adicionar(&songs);

            }else if(ui->menu_small_button_artist->isChecked())
            {
                QList<Musica*> artist;
                artist=getSongsFromArtist(_artists[_showingArtist]);
                _playlist[i]->adicionar(&artist);
            }
        }
    }
}
void MainWindow::on_categories_cell_changed(int row, int column)
{
    Database db;
    if(ui->menu_small_button_album->isChecked()) // Albuns
    {
        int albumIndex = ui->page_categories_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
        /*
         * column   -   Property
         *  0       -   check
         *  1       -   capa
         *  2       -   nome
         *  3       -   genero
         *  4       -   artistas
        */
        switch(column){
        case 2:
            _albuns[albumIndex]->setNome(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updateAlbum(_albuns[albumIndex]);
            db.connClose();
            break;
        case 3:
            _albuns[albumIndex]->setGenero(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updateAlbum(_albuns[albumIndex]);
            db.connClose();
            break;
        case 4:
            QList<Autor*> *artist = new QList<Autor*>;
            for(int i = 0; i < _artists.size(); i++)
            {
                if(_artists[i]->procurar(ui->page_categories_tableWidget->item(row,column)->text()))
                {
                    artist->append(_artists[i]);
                }
            }

            if(!artist->isEmpty())
            {
                _albuns[albumIndex]->setAutores(artist);
            }
            db.connOpen();
            db.updateAlbum(_albuns[albumIndex]);
            db.connClose();
            break;
        }
    }else if(ui->menu_small_button_artist->isChecked())
    {
        int artistIndex = ui->page_categories_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
        /*
         * column   -   Property
         *  0       -   check
         *  1       -   imagem
         *  2       -   nome
         *  3       -   nº albuns
         *  4       -   nº musicas
        */
        switch(column){
        case 2:
            _artists[artistIndex]->setNome(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updateArtist(_artists[artistIndex]);
            db.connClose();
            break;
        }
    }else if(ui->menu_small_button_list->isChecked())
    {
        int playlistIndex = ui->page_categories_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
        /*
         * column   -   Property
         *  0       -   check
         *  1       -   nome
         *  2       -   tamanho
         *  3       -   tempo
        */
        switch(column){
        case 1:
            _playlist[playlistIndex]->setNome(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updatePlaylist(_playlist[playlistIndex]);
            db.connClose();
            break;
        }
    }else if(ui->menu_small_button_song->isChecked())
    {
        int songIndex = ui->page_categories_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
        /*
         * column   -   Property
         *  0       -   check
         *  1       -   nome
         *  2       -   genero
         *  3       -   album
         *  4       -   artistas
        */
        switch(column){
        case 1:
            _songs[songIndex]->setNome(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updateSong(_songs[songIndex]);
            db.connClose();
            break;
        case 2:
            _songs[songIndex]->setGenero(ui->page_categories_tableWidget->item(row,column)->text());
            db.connOpen();
            db.updateSong(_songs[songIndex]);
            db.connClose();
            break;
        }
    }
}
void MainWindow::on_album_info_cell_changed(int row, int column)
{
    int songIndex = ui->page_album_info_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
    /*
     * column   -   Property
     *  0       -   nome
     *  1       -   genero
     *  2       -   album
     *  3       -   artistas
    */
    Database db;
    switch(column){
    case 1:
        _songs[songIndex]->setNome(ui->page_album_info_tableWidget->item(row,column)->text());
        db.connOpen();
        db.updateSong(_songs[songIndex]);
        db.connClose();
        break;
    case 2:
        _songs[songIndex]->setGenero(ui->page_album_info_tableWidget->item(row,column)->text());
        db.connOpen();
        db.updateSong(_songs[songIndex]);
        db.connClose();
        break;
    }
}
void MainWindow::on_artist_cell_changed(int row, int column)
{
    int albumIndex = ui->page_artist_tableWidget_albuns->item(row,2)->data(Qt::WhatsThisRole).toInt();
    /*
     * column   -   Property
     *  0       -   check
     *  1       -   capa
     *  2       -   nome
     *  3       -   genero
     *  4       -   artistas
    */

    Database db;
    switch(column){
    case 2:
        _albuns[albumIndex]->setNome(ui->page_artist_tableWidget_albuns->item(row,column)->text());
        db.connOpen();
        db.updateAlbum(_albuns[albumIndex]);
        db.connClose();
        break;
    case 3:
        _albuns[albumIndex]->setGenero(ui->page_artist_tableWidget_albuns->item(row,column)->text());
        db.connOpen();
        db.updateAlbum(_albuns[albumIndex]);
        db.connClose();
        break;
    }
}
void MainWindow::on_playlist_cell_changed(int row, int column)
{
    int songIndex = ui->page_playlist_tableWidget->item(row,2)->data(Qt::WhatsThisRole).toInt();
    /*
     * column   -   Property
     *  0       -   nome
     *  1       -   genero
     *  2       -   album
     *  3       -   artistas
    */
    Database db;
    switch(column){
    case 1:
        _songs[songIndex]->setNome(ui->page_playlist_tableWidget->item(row,column)->text());
        db.connOpen();
        db.updateSong(_songs[songIndex]);
        db.connClose();
        break;
    case 2:
        _songs[songIndex]->setGenero(ui->page_playlist_tableWidget->item(row,column)->text());
        db.connOpen();
        db.updateSong(_songs[songIndex]);
        db.connClose();
        break;
    }
}
void MainWindow::on_table_checkBox_pressed(QTableWidgetItem* item)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());

    if(item->column() == 0)
    {
        table->selectionModel()->clearSelection();
        for(int i = 0; i < table->rowCount(); i++)
        {
            if(table->item(i, 0)->checkState() == Qt::Checked)
            {
                table->selectRow(i);
            }
        }
    }
}
void MainWindow::on_table_cell_activated(int row, int column)
{
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());

    if(table->item(row,0)->checkState() == Qt::Unchecked)
    {
        table->item(row, 0)->setCheckState(Qt::Checked);
    }else{
        table->item(row, 0)->setCheckState(Qt::Unchecked);
    }
}

//==============================================================
// Handlers para ações de utilizador (Menu)
void MainWindow::on_menu_small_button_search_clicked()
{
    ExpandMenu(true);
}
void MainWindow::on_menu_small_button_album_clicked()
{
    MovePageToAlbuns();
}
void MainWindow::on_menu_small_button_artist_clicked()
{
    MovePageToArtists();
}
void MainWindow::on_menu_small_button_list_clicked()
{
    MovePageToPlaylists();
}
void MainWindow::on_menu_small_button_menu_clicked()
{
    ExpandMenu(true);
}
void MainWindow::on_menu_small_button_play_clicked()
{
    MovePageToPlayer();
}
void MainWindow::on_menu_small_button_song_clicked()
{
    MovePageToSongs();
}
void MainWindow::on_menu_full_button_add_album_clicked()
{
    MovePageToAddAlbuns();
}
void MainWindow::on_menu_full_button_add_artist_clicked()
{
    NewArtist();
    Refresh();
}
void MainWindow::on_menu_full_button_add_list_clicked()
{
    MovePageToAddPlaylist();
}
void MainWindow::on_menu_full_button_add_song_clicked()
{
    MovePageToAddSongs();
}
void MainWindow::on_menu_full_button_album_clicked()
{
    MovePageToAlbuns();
}
void MainWindow::on_menu_full_button_artist_clicked()
{
    MovePageToArtists();
}
void MainWindow::on_menu_full_button_list_clicked()
{
    MovePageToPlaylists();
}
void MainWindow::on_menu_full_button_menu_clicked()
{
    ExpandMenu(false);
}
void MainWindow::on_menu_full_button_play_clicked()
{
    MovePageToPlayer();
}
void MainWindow::on_menu_full_button_search_clicked()
{
    MovePageToSearch();
}
void MainWindow::on_menu_full_button_song_clicked()
{
    MovePageToSongs();
}
void MainWindow::on_menu_full_lineEdit_search_returnPressed()
{
    MovePageToSearch();
    Refresh();
}

//==============================================================
// Handlers para ações de utilizador (Page_Categories)
void MainWindow::on_page_categories_button_add_clicked()
{
    if(ui->menu_small_button_album->isChecked() || ui->menu_full_button_album->isChecked())
        {
            MovePageToAddAlbuns();
        }
        //Artista
        if(ui->menu_small_button_artist->isChecked() || ui->menu_full_button_artist->isChecked())
        {
            NewArtist();
            Refresh();
        }

        if( ui->menu_small_button_song->isChecked() ||  ui->menu_full_button_song->isChecked())
        {
             MovePageToAddSongs();
        }

        if( ui->menu_small_button_list->isChecked() || ui->menu_full_button_list->isChecked())
        {
             MovePageToAddPlaylist();
        }
}
void MainWindow::on_page_categories_button_select_toggled(bool checked)
{

    if(checked)
    {
        ShowOptionsTab(true);
        ui->page_categories_tableWidget->setColumnHidden(0,false);
        ui->page_categories_tableWidget->clearSelection();
    }else{
        ShowOptionsTab(false);
        ui->options_button_edit->setChecked(false);
        ui->page_categories_tableWidget->setColumnHidden(0,true);
        ui->page_categories_tableWidget->clearSelection();
    }
}
void MainWindow::on_page_categories_button_random_clicked()
{
    if(ui->menu_small_button_album->isChecked())
    {
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();

        QList <Musica*> newList;
        _albuns[rand() % ui->page_categories_tableWidget->rowCount()]->getMusicas(&newList);

        _player.adicionar(&newList);
        if(!ui->player_button_shuffle->isChecked())
            ui->player_button_shuffle->setChecked(true);
        _player.play();
        ui->player_button_play->setChecked(true);
        MovePageToPlayer();

    }else if(ui->menu_small_button_song->isChecked())
    {
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();

        _player.adicionar(&_songs);
        if(!ui->player_button_shuffle->isChecked())
            ui->player_button_shuffle->setChecked(true);
        _player.play();
        ui->player_button_play->setChecked(true);
        MovePageToPlayer();

    }else if(ui->menu_small_button_artist->isChecked())
    {
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();

        QList <Musica*> newList;
        newList = getSongsFromArtist(_artists[rand() % ui->page_categories_tableWidget->rowCount()]);

        _player.adicionar(&newList);
        if(!ui->player_button_shuffle->isChecked())
            ui->player_button_shuffle->setChecked(true);
        _player.play();
        ui->player_button_play->setChecked(true);
        MovePageToPlayer();

    }else if(ui->menu_small_button_list->isChecked())
    {
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();

        QList <Musica*> newList;
        _playlist[rand() % ui->page_categories_tableWidget->rowCount()]->getMusicas(&newList);

        _player.adicionar(&newList);
        if(!ui->player_button_shuffle->isChecked())
            ui->player_button_shuffle->setChecked(true);
        _player.play();
        ui->player_button_play->setChecked(true);
        MovePageToPlayer();
    }
}
void MainWindow::on_page_categories_comboBox_order_currentIndexChanged(int index)
{
    if(ui->menu_small_button_album->isChecked())
    {
        switch(index)
        {
        case 0:
            // data de adição (ainda não faz nada)
            break;
        case 1:
            // A a Z
            ui->page_categories_tableWidget->sortByColumn(2, Qt::AscendingOrder);
            break;
        case 2:
            // Genero
            ui->page_categories_tableWidget->sortByColumn(3, Qt::AscendingOrder);
            break;
        case 3:
            // Autor
            ui->page_categories_tableWidget->sortByColumn(4, Qt::AscendingOrder);
            break;
        }
    }else if(ui->menu_small_button_song->isChecked())
    {
        switch(index)
        {
        case 0:
            // data de adição (ainda não faz nada)
            break;
        case 1:
            // A a Z
            ui->page_categories_tableWidget->sortByColumn(1, Qt::AscendingOrder);
            break;
        case 2:
            // Genero
            ui->page_categories_tableWidget->sortByColumn(2, Qt::AscendingOrder);
            break;
        case 3:
            // Autor
            ui->page_categories_tableWidget->sortByColumn(4, Qt::AscendingOrder);
            break;
        case 4:
            // Album
            ui->page_categories_tableWidget->sortByColumn(3, Qt::AscendingOrder);
            break;
        }
    }else if (ui->menu_small_button_artist->isChecked())
    {

    }
}
void MainWindow::on_page_categories_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (ui->menu_full_button_album->isChecked()){
        MovePageToAlbumInfo(index.sibling(index.row(),2).data(Qt::WhatsThisRole).toInt());
    }else if (ui->menu_full_button_artist->isChecked()){
        MovePageToArtistInfo(index.sibling(index.row(),2).data(Qt::WhatsThisRole).toInt());
    }else if (ui->menu_full_button_list->isChecked()){
        MovePageToPlaylistInfo(index.data(Qt::WhatsThisRole).toInt());
    }else if (ui->menu_full_button_song->isChecked()){
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();
        _player.adicionar(_songs[index.data(Qt::WhatsThisRole).toInt()]);
        ui->player_button_shuffle->setChecked(false);
        _player.play();
        ui->player_button_play->setChecked(true);
    }
}

//==============================================================
// Handlers para ações de utilizador (Page Album Info)

void MainWindow::on_page_album_info_button_play_clicked()
{
    _player.parar();
    ui->player_button_play->setChecked(false);
    _player.removerTodas();

    QList <Musica*> newList;
    int musicToAdd;

    for(int i = 0; i < ui->page_album_info_tableWidget->rowCount(); i++)
    {
        musicToAdd = ui->page_album_info_tableWidget->item(i,0)->data(Qt::WhatsThisRole).toInt();
        newList.append(_songs[musicToAdd]);
    }

    _player.adicionar(&newList);

    if(ui->player_button_shuffle->isChecked())
        ui->player_button_shuffle->setChecked(false);

    _player.play();
    ui->player_button_play->setChecked(true);
    MovePageToPlayer();
}
void MainWindow::on_page_album_info_button_remove_clicked()
{
    QMessageBox msgBox;
    QPushButton *confirm, *cancel;
    confirm = new QPushButton;
    cancel = new QPushButton;
    msgBox.setText("Atenção, está prestes a remover o album: " + _albuns[_showingAlbum]->getNome());
    confirm->setText("Confirmar");
    cancel->setText("Cancelar");
    msgBox.addButton(confirm,QMessageBox::AcceptRole);
    msgBox.addButton(cancel,QMessageBox::RejectRole);
    msgBox.setWindowTitle("Atenção");
    msgBox.exec();

    if(msgBox.result() == QMessageBox::AcceptRole)
    {
        QList<Musica*> toRemoveSongs;
        _albuns[_showingAlbum]->getMusicas(&toRemoveSongs);

        for(int i = 0; i < toRemoveSongs.size(); i++)
        {
            _songs.removeOne(toRemoveSongs[i]);
        }

        _albuns[_showingAlbum]->apagar();

        _albuns.removeAt(_showingAlbum);
        MovePageToAlbuns();
    }
}
void MainWindow::on_page_album_info_button_select_toggled(bool checked)
{
    if(checked)
    {
        ShowOptionsTab(true);
        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_album_info_tableWidget->setColumnHidden(0,false);
        ui->page_album_info_tableWidget->clearSelection();
    }else{
        ShowOptionsTab(false);
        ui->options_button_edit->setChecked(false);
        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->page_album_info_tableWidget->setColumnHidden(0,true);
        ui->page_album_info_tableWidget->clearSelection();
    }
}
void MainWindow::on_page_album_info_tableWidget_doubleClicked(const QModelIndex &index)
{
    _player.parar();
    ui->player_button_play->setChecked(false);
    _player.removerTodas();
    _player.adicionar(_songs[index.data(Qt::WhatsThisRole).toInt()]);
    if(ui->player_button_shuffle->isChecked())
        ui->player_button_shuffle->setChecked(false);
    _player.play();
    ui->player_button_play->setChecked(true);
}

//==============================================================
// Handlers para ações de utilizador (Page Add Album)

void MainWindow::on_page_add_album_button_addArtwork_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","Image files (*.jpg , *.png)");

    if(!filename.isEmpty())
    {
        _imageURL = filename;
        ui->page_add_album_label_artwork->setPixmap(QPixmap(filename));
    }

    Refresh();
}
void MainWindow::on_page_remove_album_artwork_clicked()
{
    _imageURL = "";
    ui->page_add_album_label_artwork->setPixmap(QPixmap(":/album_art.png"));
}
void MainWindow::on_page_add_album_button_addMusic_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","MP3 files (*.mp3)");

    if(!filename.isNull())
    {
        Musica *newSong;

        newSong = new Musica;
        newSong->setDiretoria(filename);
        newSong->retrieveInfo(newSong->getDiretoria());
        _newSongList.append(newSong);

    }

    Refresh();
}
void MainWindow::on_page_add_album_button_addFolder_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QDir dir(folderName);
    QStringList filters;
    filters << "*.mp3";
    QStringList filenames = dir.entryList(filters,QDir::Files,QDir::Name);

    Musica *newSong;
    for(int i = 0; i < filenames.size(); i++)
    {
        if(!filenames[i].isEmpty())
        {
            newSong = new Musica;
            newSong->setDiretoria(folderName + "/" + filenames[i]);
            newSong->retrieveInfo(newSong->getDiretoria());
            _newSongList.append(newSong);
        }
    }

    Refresh();
}
void MainWindow::on_page_add_album_button_remove_clicked()
{
    QList<QTableWidgetItem*> songsIndex;
    int indexToRemove;

    ui->page_add_album_tableWidget_toAddMusics->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_toAddMusics->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_toAddMusics->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_toAddMusics->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_toAddMusics->selectRow(i);
        }
    }
    songsIndex = ui->page_add_album_tableWidget_toAddMusics->selectedItems();
    ui->page_add_album_tableWidget_toAddMusics->setSelectionMode(QAbstractItemView::SingleSelection);

    for(int i = 0; i < songsIndex.size(); i++)
    {
        indexToRemove = songsIndex[i]->data(Qt::WhatsThisRole).toInt();
        _newSongList.removeAt(indexToRemove);
    }

    Refresh();
}
void MainWindow::on_page_add_album_button_addArtistTo_clicked()
{
    QList<QTableWidgetItem*> songsIndex;

    ui->page_add_album_tableWidget_musics->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_musics->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_musics->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_musics->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_musics->selectRow(i);
        }
    }
    songsIndex = ui->page_add_album_tableWidget_musics->selectedItems();
    ui->page_add_album_tableWidget_musics->setSelectionMode(QAbstractItemView::SingleSelection);

    QList<QTableWidgetItem*> artistsIndex;

    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_artists->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_artists->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_artists->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_artists->selectRow(i);
        }
    }
    artistsIndex = ui->page_add_album_tableWidget_artists->selectedItems();
    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::SingleSelection);

    if(!songsIndex.isEmpty() && !artistsIndex.isEmpty())
    {
        int song, artist;
        for(int i = 0; i < songsIndex.size(); i++)
        {
            song = songsIndex[i]->data(Qt::WhatsThisRole).toInt();
            for(int j = 0; j < artistsIndex.size(); j++)
            {
                artist = artistsIndex[j]->data(Qt::WhatsThisRole).toInt();
                _newSongList[song]->addAutor(_artists[artist]);
            }
        }
    }else{
        // Mensagem de erro
    }

    Refresh();
}
void MainWindow::on_page_add_album_button_addArtistToAll_clicked()
{
    QList<QTableWidgetItem*> artistsIndex;

    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_artists->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_artists->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_artists->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_artists->selectRow(i);
        }
    }
    artistsIndex = ui->page_add_album_tableWidget_artists->selectedItems();
    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::SingleSelection);

    if(!artistsIndex.isEmpty())
    {
        int artist;
        for(int i = 0; i < _newSongList.size(); i++)
        {
            for(int j = 0; j < artistsIndex.size(); j++)
            {
                artist = artistsIndex[j]->data(Qt::WhatsThisRole).toInt();
                _newSongList[i]->addAutor(_artists[artist]);
            }
        }
    }

    Refresh();
}
void MainWindow::on_page_add_album_button_removeArtistFrom_clicked()
{
    QList<QTableWidgetItem*> artistsIndex;

    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_artists->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_artists->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_artists->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_artists->selectRow(i);
        }
    }
    artistsIndex = ui->page_add_album_tableWidget_artists->selectedItems();
    ui->page_add_album_tableWidget_artists->setSelectionMode(QAbstractItemView::SingleSelection);

    QList<QTableWidgetItem*> songsIndex;

    ui->page_add_album_tableWidget_musics->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_album_tableWidget_musics->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_album_tableWidget_musics->rowCount(); i++)
    {
        if(ui->page_add_album_tableWidget_musics->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_album_tableWidget_musics->selectRow(i);
        }
    }
    songsIndex = ui->page_add_album_tableWidget_musics->selectedItems();
    ui->page_add_album_tableWidget_musics->setSelectionMode(QAbstractItemView::SingleSelection);

    if(!songsIndex.isEmpty() &&
            !artistsIndex.isEmpty())
    {
        int song, artist;
        for(int i = 0; i < songsIndex.size(); i++)
        {
            song = songsIndex[i]->data(Qt::WhatsThisRole).toInt();
            for(int j = 0; j < artistsIndex.size(); j++)
            {
                artist = artistsIndex[j]->data(Qt::WhatsThisRole).toInt();
                if(_newSongList[song]->hasAutor(_artists[artist]))
                {
                    _newSongList[song]->removeAutor(_artists[artist]);
                }
            }
        }
    }

    Refresh();
}
void MainWindow::on_page_add_album_button_newArtist_clicked()
{
    NewArtist();

    Refresh();
}
void MainWindow::on_page_add_album_lineEdit_searchArtists_returnPressed()
{
    Refresh();
}
void MainWindow::on_page_add_album_lineEdit_searchArtists_textChanged(const QString &arg1)
{
    Refresh();
}

//==============================================================
// Handlers para ações de utilizador (Page Add Music)

void MainWindow::on_page_add_music_button_addFolder_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QDir dir(folderName);
    QStringList filters;
    filters << "*.mp3";
    QStringList filenames = dir.entryList(filters,QDir::Files,QDir::Name);

    Musica *newSong;
    for(int i = 0; i < filenames.size(); i++)
    {
        if(!filenames[i].isEmpty())
        {
            newSong = new Musica;
            newSong->setDiretoria(folderName + "/" + filenames[i]);
            newSong->retrieveInfo(newSong->getDiretoria());
            _newSongList.append(newSong);
        }
    }

    Refresh();
}
void MainWindow::on_page_add_music_button_addMusic_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","MP3 files (*.mp3)");

    if(!filename.isEmpty())
    {
        Musica *newSong;

        newSong = new Musica;
        newSong->setDiretoria(filename);
        newSong->retrieveInfo(newSong->getDiretoria());
        _newSongList.append(newSong);
    }

    Refresh();
}
void MainWindow::on_page_add_music_comboBox_albuns_currentIndexChanged(int index)
{
    ui->page_add_music_comboBox_albuns->setCurrentIndex(index);
}

//==============================================================
// Handlers para ações de utilizador (Page Search)
void MainWindow::on_page_search_tableWidget_artists_doubleClicked(const QModelIndex &index)
{
    MovePageToArtistInfo(index.sibling(index.row(),2).data(Qt::WhatsThisRole).toInt());
}
void MainWindow::on_page_search_tableWidget_albuns_doubleClicked(const QModelIndex &index)
{
    MovePageToAlbumInfo(index.sibling(index.row(),2).data(Qt::WhatsThisRole).toInt());
}
void MainWindow::on_page_search_tableWidget_musics_doubleClicked(const QModelIndex &index)
{
    _player.parar();
    ui->player_button_play->setChecked(false);
    _player.removerTodas();
    _player.adicionar(_songs[index.data(Qt::WhatsThisRole).toInt()]);
    if(ui->player_button_shuffle->isChecked())
        ui->player_button_shuffle->setChecked(false);
    _player.play();
    ui->player_button_play->setChecked(true);
}

//==============================================================
// Handlers para ações de utilizador (Page Artist)

void MainWindow::on_page_artist_button_play_clicked()
{
    _player.parar();
    ui->player_button_play->setChecked(false);
    _player.removerTodas();

    QList <Musica*> newList;

    for(int i = 0; i < _songs.size(); i++)
    {
        if(_songs[i]->hasAutor(_artists[_showingArtist]))
            newList.append(_songs[i]);
    }

    _player.adicionar(&newList);

    if(ui->player_button_shuffle->isChecked())
        ui->player_button_shuffle->setChecked(false);

    _player.play();
    ui->player_button_play->setChecked(true);
}
void MainWindow::on_page_artist_button_remove_clicked()
{
    QMessageBox msgBox;
    QPushButton *confirm, *cancel;
    confirm = new QPushButton;
    cancel = new QPushButton;
    msgBox.setText("Atenção, está prestes a remover o artista: " + _artists[_showingArtist]->getNome());
    confirm->setText("Confirmar");
    cancel->setText("Cancelar");
    msgBox.addButton(confirm,QMessageBox::AcceptRole);
    msgBox.addButton(cancel,QMessageBox::RejectRole);
    msgBox.setWindowTitle("Atenção");
    msgBox.exec();

    if(msgBox.result() == QMessageBox::AcceptRole)
    {
        for(int j = 0; j < _songs.size(); j++)
        {
            _songs[j]->removeAutor(_artists[_showingArtist]);

        }
        _artists[_showingArtist]->apagar();
        _artists.removeOne(_artists[_showingArtist]);
    }

    MovePageToArtists();
}
void MainWindow::on_page_artist_button_select_toggled(bool checked)
{
    if(checked)
    {
        ShowOptionsTab(true);
        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_artist_tableWidget_albuns->setColumnHidden(0,false);
        ui->page_artist_tableWidget_albuns->clearSelection();
    }else{
        ShowOptionsTab(false);
        ui->options_button_edit->setChecked(false);
        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->page_artist_tableWidget_albuns->setColumnHidden(0,true);
        ui->page_artist_tableWidget_albuns->clearSelection();
    }
}
void MainWindow::on_page_artist_tableWidget_albuns_doubleClicked(const QModelIndex &index)
{
    MovePageToAlbumInfo(index.sibling(index.row(),2).data(Qt::WhatsThisRole).toInt());
}

//==============================================================
// Handlers para ações de utilizador (Page Playlist)

void MainWindow::on_page_playlist_button_play_clicked()
{
    _player.parar();
    ui->player_button_play->setChecked(false);
    _player.removerTodas();

    QList <Musica*> newList;
    int musicToAdd;

    for(int i = 0; i < ui->page_playlist_tableWidget->rowCount(); i++)
    {
        musicToAdd = ui->page_playlist_tableWidget->item(i,0)->data(Qt::WhatsThisRole).toInt();
        newList.append(_songs[musicToAdd]);
    }

    _player.adicionar(&newList);
    if(ui->player_button_shuffle->isChecked())
        ui->player_button_shuffle->setChecked(false);
    _player.play();
    ui->player_button_play->setChecked(true);
}
void MainWindow::on_page_playlist_button_addMusic_clicked()
{
    _editMode = true;

    if(ui->page_playlist_button_play->isVisible())
    {
        MovePageToAddPlaylist();

    }
}
void MainWindow::on_page_playlist_button_remove_clicked()
{
    QMessageBox msgBox;
    QPushButton *confirm, *cancel;
    confirm = new QPushButton;
    cancel = new QPushButton;
    msgBox.setText("Atenção, está prestes a remover a playlist: " + _playlist[_showingPlaylist]->getNome());
    confirm->setText("Confirmar");
    cancel->setText("Cancelar");
    msgBox.addButton(confirm,QMessageBox::AcceptRole);
    msgBox.addButton(cancel,QMessageBox::RejectRole);
    msgBox.setWindowTitle("Atenção");
    msgBox.exec();

    if(msgBox.result() == QMessageBox::AcceptRole)
    {

        _playlist[_showingPlaylist]->apagar();
        _playlist.removeOne(_playlist[_showingPlaylist]);


        MovePageToPlaylists();
    }
}
void MainWindow::on_page_playlist_tableWidget_doubleClicked(const QModelIndex &index)
{
    if(ui->page_playlist_button_play->isVisible())
    {
        _player.parar();
        ui->player_button_play->setChecked(false);
        _player.removerTodas();
        _player.adicionar(_songs[index.data(Qt::WhatsThisRole).toInt()]);
        if(ui->player_button_shuffle->isChecked())
            ui->player_button_shuffle->setChecked(false);
        _player.play();
        ui->player_button_play->setChecked(true);

    }else{
        _player._mediaPlayer->playlist()->setCurrentIndex(index.row());
    }
}

//==============================================================
// Handlers para ações de utilizador (Page Add Playlist)

void MainWindow::on_page_add_playlist_button_search_clicked()
{
    Refresh();
}
void MainWindow::on_page_add_playlist_button_add_clicked()
{
    QList<QTableWidgetItem*> toAddItems;

    ui->page_add_playlist_tableWidget_toAdd->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_playlist_tableWidget_toAdd->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_playlist_tableWidget_toAdd->rowCount(); i++)
    {
        if(ui->page_add_playlist_tableWidget_toAdd->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_playlist_tableWidget_toAdd->selectRow(i);
        }
    }
    toAddItems = ui->page_add_playlist_tableWidget_toAdd->selectedItems();
    ui->page_add_playlist_tableWidget_toAdd->setSelectionMode(QAbstractItemView::SingleSelection);

    if(!toAddItems.isEmpty())
    {
        int index;
        for(int i = 0; i < toAddItems.size(); i++)
        {
            if(toAddItems[i]->column() == 2){
                index = toAddItems[i]->data(Qt::WhatsThisRole).toInt();
                _newSongList.append(_songs[index]);
            }
        }
    }

    Refresh();
}
void MainWindow::on_page_add_playlist_button_remove_clicked()
{
    QList<QTableWidgetItem*> toRemoveItems;

    ui->page_add_playlist_tableWidget_Added->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->page_add_playlist_tableWidget_Added->selectionModel()->clearSelection();
    for(int i = 0; i < ui->page_add_playlist_tableWidget_Added->rowCount(); i++)
    {
        if(ui->page_add_playlist_tableWidget_Added->item(i, 0)->checkState() == Qt::Checked)
        {
            ui->page_add_playlist_tableWidget_Added->selectRow(i);
        }
    }
    toRemoveItems = ui->page_add_playlist_tableWidget_Added->selectedItems();
    ui->page_add_playlist_tableWidget_Added->setSelectionMode(QAbstractItemView::SingleSelection);

    if(!toRemoveItems.isEmpty())
    {
        int index;
        for(int i = 0; i < toRemoveItems.size(); i++)
        {
            if(toRemoveItems[i]->column() == 2){
                index = toRemoveItems[i]->data(Qt::WhatsThisRole).toInt();
                _newSongList.removeOne(_newSongList[index]);
            }
        }
    }

    Refresh();
}
void MainWindow::on_page_add_playlist_lineEdit_search_returnPressed()
{
    Refresh();
}
void MainWindow::on_page_add_playlist_lineEdit_search_textChanged(const QString &arg1)
{
    Refresh();
}
void MainWindow::on_page_add_playlist_tableWidget_toAdd_doubleClicked(const QModelIndex &index)
{
    _newSongList.append(_songs[index.data(Qt::WhatsThisRole).toInt()]);
    Refresh();
}
void MainWindow::on_page_add_playlist_tableWidget_Added_doubleClicked(const QModelIndex &index)
{
    _newSongList.removeOne(_newSongList[index.data(Qt::WhatsThisRole).toInt()]);
    Refresh();
}

//==============================================================
// Handlers para ações de utilizador (Tab Player)

void MainWindow::on_player_slider_sliderReleased()
{
    if (qAbs(_player._mediaPlayer->position() - ui->player_slider->value()) > 99)
        _player.setPosicao(ui->player_slider->value());
}
void MainWindow::on_player_slider_valueChanged(int value)
{
    if (qAbs(_player._mediaPlayer->position() - value) > 99)
        _player.setPosicao(value);
}
void MainWindow::on_player_button_previous_clicked()
{
    _player.anterior();
}
void MainWindow::on_player_button_play_toggled(bool checked)
{
    if(checked)
        _player.play();
    else
        _player.pausa();

    if(_player.isATocar())
        ui->player_button_play->setChecked(true);
    else
        ui->player_button_play->setChecked(false);
}
void MainWindow::on_player_button_stop_clicked()
{
    _player.parar();
    ui->player_button_play->setChecked(false);
}
void MainWindow::on_player_button_next_clicked()
{
    _player.seguinte();
}
void MainWindow::on_player_button_shuffle_toggled(bool checked)
{
    _player.aleatorio(checked);

    if(_player.isAleatorio())
        ui->player_button_shuffle->setChecked(true);
    else
        ui->player_button_shuffle->setChecked(false);
}
void MainWindow::on_player_button_repeat_clicked()
{
    _player.repetir(!_player.isRepeat());
}
void MainWindow::on_player_button_mute_clicked()
{
    _player.silencio(!_player.isSilencio());
}

//==============================================================
// Handlers para ações de utilizador (Tab Progress)

void MainWindow::on_progress_button_previous_clicked()
{
    if(ui->page_add_album_tabs->currentIndex() != 0)
    {
        ui->page_add_album_tabs->setCurrentIndex(ui->page_add_album_tabs->currentIndex()-1);

        Refresh();
    }
}
void MainWindow::on_progress_button_next_clicked()
{
    if(ui->page_add_album_tabs->currentIndex() != 2)
    {
        ui->page_add_album_tabs->setCurrentIndex(ui->page_add_album_tabs->currentIndex()+1);

        Refresh();
    }
}
void MainWindow::on_progress_button_cancel_clicked()
{
    if(ui->menu_small_button_album->isChecked())
    {
        MovePageToAlbuns();
        while (!_newSongList.isEmpty())
            _newSongList.removeLast();

    }else if(ui->menu_small_button_song->isChecked())
    {
        MovePageToSongs();
        while (!_newSongList.isEmpty())
            _newSongList.removeLast();

    }else if(ui->menu_small_button_list->isChecked())
    {
        MovePageToPlaylists();
        while (!_newSongList.isEmpty())
            _newSongList.removeLast();

        ui->page_add_playlist_lineEdit_name->clear();
        //ui->page_add_playlist_lineEdit_gender->clear();
        ui->page_add_playlist_plainText_description->clear();
        _editMode = false;
    }
}
void MainWindow::on_progress_button_save_clicked()
{
    if(ui->menu_small_button_album->isChecked())
    {
        if(ui->page_add_album_lineEdit_name->text() == NULL)
        {
         /*
         * Avisos de irregularidades ao criar album ou assim
         */
            QMessageBox::warning(this,"Adicionar Album",tr("Falta preencher o nome do Album."));
            ui->page_add_album_tabs->setCurrentIndex(0);
        }
        else
        {
            // Tudo certo para criar album
            Database db;
            db.connOpen();

            Album *newAlbum;
            newAlbum = new Album;
            newAlbum->setNome(ui->page_add_album_lineEdit_name->text());
            newAlbum->setGenero(Generos.at(ui->page_add_album_comboBox_gender->currentIndex()));
            newAlbum->setAno(ui->page_add_album_lineEdit_year->text().toInt());
            newAlbum->setImagem(_imageURL);
            newAlbum->setDescricao(ui->page_add_album_textEdit_description->toPlainText());
            newAlbum->setDiretoria(QDir::currentPath() +"/debug/album/ID_");

            if(newAlbum->criar() != -1)
            {
                for (int i = 0; i < _newSongList.size(); i++)
                {
                    _newSongList[i]->criar(newAlbum->getIdBD(), newAlbum->getDiretoria());
                    newAlbum->adicionar(_newSongList[i]);
                    _songs.append(_newSongList[i]);
                }

            _albuns.append(newAlbum);

            }

            db.connClose();


            while(!_newSongList.isEmpty())
                _newSongList.removeLast();

            MovePageToAlbuns();

        }
    }
    else if(ui->menu_small_button_song->isChecked())
    {

        int albumIndex = ui->page_add_music_comboBox_albuns->currentIndex();

        for (int i = 0; i < _newSongList.size(); i++)
        {
            _newSongList[i]->criar(_albuns[albumIndex]->getIdBD(), _albuns[albumIndex]->getDiretoria());
            _albuns[albumIndex]->adicionar(_newSongList[i]);
            _songs.append(_newSongList[i]);
        }

        while(!_newSongList.isEmpty())
            _newSongList.removeLast();

        MovePageToSongs();
    }
    else if(ui->menu_small_button_list->isChecked())
    {
        if(!_editMode)
        {
            //
            // Criar nova Playlist
            //
            _newPlaylist = new Playlist;
            _newPlaylist->setNome(ui->page_add_playlist_lineEdit_name->text());
            _newPlaylist->setDescricao(ui->page_add_playlist_plainText_description->toPlainText());
            _newPlaylist->setDataAdicao(QDate::currentDate());

            _newPlaylist->setMusicas(&_newSongList);

            _newPlaylist->criar();
            _playlist.append(_newPlaylist);

        }else{
            _playlist[_showingPlaylist]->setNome(ui->page_add_playlist_lineEdit_name->text());
            _playlist[_showingPlaylist]->setDescricao(ui->page_add_playlist_plainText_description->toPlainText());

            QList<Musica*> oldSongList;
            _playlist[_showingPlaylist]->getMusicas(&oldSongList);

            if(_newSongList != oldSongList)
            {
                for(int i = 0; i < oldSongList.size(); i++)
                {
                    _playlist[_showingPlaylist]->remover(oldSongList[i]);
                }

                _playlist[_showingPlaylist]->setMusicas(&_newSongList);
            }

            /* isto pode ser para fazer lá dentro
             * */
            Database db;

            db.connOpen();
            db.updatePlaylist(_playlist[_showingPlaylist]);
            db.connClose();

            _editMode = false;

        }

        ui->page_add_playlist_lineEdit_name->clear();
        //ui->page_add_playlist_lineEdit_gender->clear();
        ui->page_add_playlist_plainText_description->clear();

        while(!_newSongList.isEmpty())
            _newSongList.removeLast();

        MovePageToPlaylists();
    }
}

//==============================================================
// Handlers para ações de utilizador (Tab Options)

void MainWindow::on_options_button_play_clicked()
{

    QList<QTableWidgetItem*> selected;
    QList<Album*> AlbunsToPlay;
    QList<Autor*> ArtistsToPlay;
    QList<Playlist*> PlaylistsToPlay;
    QList<Musica*> songsToPlay;
    int index;

    switch(ui->pages->currentIndex())
    {
    case 0:
        // page_categories
        if(ui->menu_small_button_album->isChecked()) // Albuns
        {
            _player.parar();
            _player.removerTodas();

            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!AlbunsToPlay.contains(_albuns[index]))
                    AlbunsToPlay.append(_albuns[index]);

                selected.removeFirst();
            }

            for(int i = 0; i < AlbunsToPlay.size(); i++)
            {
                AlbunsToPlay[i]->getMusicas(&songsToPlay);
            }

            _player.adicionar(&songsToPlay);
            _player.play();

        }else if(ui->menu_small_button_artist->isChecked()) // Artists
        {
            _player.parar();
            _player.removerTodas();

            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!ArtistsToPlay.contains(_artists[index]))
                    ArtistsToPlay.append(_artists[index]);

                selected.removeFirst();
            }

            for(int i = 0; i < ArtistsToPlay.size(); i++)
            {
                songsToPlay.append(getSongsFromArtist(ArtistsToPlay[i]));
            }

            _player.adicionar(&songsToPlay);
            _player.play();
        }else if(ui->menu_small_button_song->isChecked()) // Songs
        {
            _player.parar();
            _player.removerTodas();

            QList<QTableWidgetItem*> selected;
            int index;

            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!songsToPlay.contains(_songs[index]))
                    songsToPlay.append(_songs[index]);

                selected.removeFirst();
            }

            _player.adicionar(&songsToPlay);
            _player.play();
        }else if(ui->menu_small_button_list->isChecked()) // Playlists
        {
            _player.parar();
            _player.removerTodas();

            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!PlaylistsToPlay.contains(_playlist[index]))
                    PlaylistsToPlay.append(_playlist[index]);

                selected.removeFirst();
            }

            for(int i = 0; i < PlaylistsToPlay.size(); i++)
            {
                PlaylistsToPlay[i]->getMusicas(&songsToPlay);
            }

            _player.adicionar(&songsToPlay);
            _player.play();
        }

        break;

    case 1:
        // page_album_info
        _player.parar();
        _player.removerTodas();

        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_album_info_tableWidget->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_album_info_tableWidget->rowCount(); i++)
        {
            if(ui->page_album_info_tableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_album_info_tableWidget->selectRow(i);
            }
        }
        selected = ui->page_album_info_tableWidget->selectedItems();
        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        while(!selected.isEmpty())
        {
            index = selected.first()->data(Qt::WhatsThisRole).toInt();
            if(!songsToPlay.contains(_songs[index]))
                songsToPlay.append(_songs[index]);

            selected.removeFirst();
        }

        _player.adicionar(&songsToPlay);
        _player.play();
        break;

    case 5:
        // page_artist
        _player.parar();
        _player.removerTodas();

        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_artist_tableWidget_albuns->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_artist_tableWidget_albuns->rowCount(); i++)
        {
            if(ui->page_artist_tableWidget_albuns->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_artist_tableWidget_albuns->selectRow(i);
            }
        }
        selected = ui->page_artist_tableWidget_albuns->selectedItems();
        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::SingleSelection);

        while(!selected.isEmpty())
        {
            index = selected.first()->data(Qt::WhatsThisRole).toInt();
            if(!AlbunsToPlay.contains(_albuns[index]))
                AlbunsToPlay.append(_albuns[index]);

            selected.removeFirst();
        }

        for(int i = 0; i < AlbunsToPlay.size(); i++)
        {
            AlbunsToPlay[i]->getMusicas(&songsToPlay);
        }

        _player.adicionar(&songsToPlay);
        _player.play();
        break;

    case 6:
        // page_playlist
        _player.parar();
        _player.removerTodas();

        ui->page_playlist_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_playlist_tableWidget->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_playlist_tableWidget->rowCount(); i++)
        {
            if(ui->page_playlist_tableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_playlist_tableWidget->selectRow(i);
            }
        }
        selected = ui->page_playlist_tableWidget->selectedItems();
        ui->page_playlist_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        while(!selected.isEmpty())
        {
            index = selected.first()->data(Qt::WhatsThisRole).toInt();
            if(!songsToPlay.contains(_songs[index]))
                songsToPlay.append(_songs[index]);

            selected.removeFirst();
        }

        _player.adicionar(&songsToPlay);
        _player.play();
        break;
    }
}
void MainWindow::on_options_button_remove_clicked()
{

    QList<QTableWidgetItem*> selected;
    QList<Album*> AlbunsToRemove;
    QList<Autor*> ArtistsToRemove;
    QList<Playlist*> PlaylistsToRemove;
    QList<Musica*> songsToRemove;
    int index;

    QMessageBox msgBox;
    QPushButton *confirm, *cancel;
    confirm = new QPushButton;
    cancel = new QPushButton;
    confirm->setText("Confirmar");
    cancel->setText("Cancelar");
    msgBox.addButton(confirm,QMessageBox::AcceptRole);
    msgBox.addButton(cancel,QMessageBox::RejectRole);
    msgBox.setWindowTitle("Atenção");

    switch(ui->pages->currentIndex())
    {
    case 0:
        // page_categories
        if(ui->menu_small_button_album->isChecked()) // Albuns
        {
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            if(!selected.isEmpty())
            {
                while(!selected.isEmpty())
                {
                    index = selected.first()->data(Qt::WhatsThisRole).toInt();
                    if(!AlbunsToRemove.contains(_albuns[index]))
                        AlbunsToRemove.append(_albuns[index]);

                    selected.removeFirst();
                }

                msgBox.setText("Atenção, está prestes a remover " + QString::number(AlbunsToRemove.size()) + " albuns!");
                msgBox.exec();

                if(msgBox.result() == QMessageBox::AcceptRole)
                {
                    for(int i = 0; i < AlbunsToRemove.size(); i++)
                    {
                        songsToRemove.clear();
                        AlbunsToRemove[i]->getMusicas(&songsToRemove);

                        for(int j = 0; j < songsToRemove.size(); j++)
                        {
                            for(int k = 0; k < _playlist.size(); k++)
                            {
                                _playlist[k]->remover(songsToRemove[j]);
                            }

                            _songs.removeOne(songsToRemove[j]);

                        }

                        _albuns.removeOne(AlbunsToRemove[i]);
                        AlbunsToRemove[i]->apagar();
                    }
                    MovePageToAlbuns();
                }
            }

        }else if(ui->menu_small_button_artist->isChecked()) // Artists
        {
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            if(!selected.isEmpty())
            {
                while(!selected.isEmpty())
                {
                    index = selected.first()->data(Qt::WhatsThisRole).toInt();
                    if(!ArtistsToRemove.contains(_artists[index]))
                        ArtistsToRemove.append(_artists[index]);

                    selected.removeFirst();
                }

                msgBox.setText("Atenção, está prestes a remover " + QString::number(ArtistsToRemove.size()) + " artistas!");
                msgBox.exec();

                if(msgBox.result() == QMessageBox::AcceptRole)
                {
                    for(int i = 0; i < ArtistsToRemove.size(); i++)
                    {
                        for(int j = 0; j < _songs.size(); j++)
                        {
                            _songs[j]->removeAutor(ArtistsToRemove[i]);
                        }

                        _artists.removeOne(ArtistsToRemove[i]);
                        ArtistsToRemove[i]->apagar();
                    }
                    MovePageToArtists();
                }
            }

        }if(ui->menu_small_button_song->isChecked()) // Songs
        {
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            if(!selected.isEmpty())
            {
                while(!selected.isEmpty())
                {
                    index = selected.first()->data(Qt::WhatsThisRole).toInt();
                    if(!songsToRemove.contains(_songs[index]))
                        songsToRemove.append(_songs[index]);

                    selected.removeFirst();
                }

                msgBox.setText("Atenção, está prestes a remover " + QString::number(songsToRemove.size()) + " musicas!");
                msgBox.exec();

                if(msgBox.result() == QMessageBox::AcceptRole)
                {
                    for(int i = 0; i < songsToRemove.size(); i++)
                    {
                        for(int j = 0; j < _playlist.size(); j++)
                        {
                            _playlist[j]->remover(songsToRemove[j]);
                        }
                        for(int j = 0; j < _albuns.size(); j++)
                        {
                            _albuns[j]->remover(songsToRemove[j]);
                        }

                        _songs.removeOne(songsToRemove[i]);
                        songsToRemove[i]->apagar();
                    }
                    MovePageToSongs();
                }
            }


        }else if(ui->menu_small_button_list->isChecked()) // Playlists
        {
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            ui->page_categories_tableWidget->selectionModel()->clearSelection();
            for(int i = 0; i < ui->page_categories_tableWidget->rowCount(); i++)
            {
                if(ui->page_categories_tableWidget->item(i, 0)->checkState() == Qt::Checked)
                {
                    ui->page_categories_tableWidget->selectRow(i);
                }
            }
            selected = ui->page_categories_tableWidget->selectedItems();
            ui->page_categories_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

            if(!selected.isEmpty())
            {
                while(!selected.isEmpty())
                {
                    index = selected.first()->data(Qt::WhatsThisRole).toInt();
                    if(!PlaylistsToRemove.contains(_playlist[index]))
                        PlaylistsToRemove.append(_playlist[index]);

                    selected.removeFirst();
                }

                msgBox.setText("Atenção, está prestes a remover " + QString::number(songsToRemove.size()) + " playlists!");
                msgBox.exec();

                if(msgBox.result() == QMessageBox::AcceptRole)
                {
                    for(int i = 0; i < PlaylistsToRemove.size(); i++)
                    {
                        _playlist.removeOne(PlaylistsToRemove[i]);
                        PlaylistsToRemove[i]->apagar();
                    }
                    MovePageToPlaylists();
                }
            }
        }
        break;
    case 1:
        // page_album_info
        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_categories_tableWidget->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_album_info_tableWidget->rowCount(); i++)
        {
            if(ui->page_album_info_tableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_album_info_tableWidget->selectRow(i);
            }
        }
        selected = ui->page_album_info_tableWidget->selectedItems();
        ui->page_album_info_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        if(!selected.isEmpty())
        {
            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!songsToRemove.contains(_songs[index]))
                    songsToRemove.append(_songs[index]);

                selected.removeFirst();
            }

            msgBox.setText("Atenção, está prestes a remover " + QString::number(songsToRemove.size()) + " musicas!");
            msgBox.exec();

            if(msgBox.result() == QMessageBox::AcceptRole)
            {
                for(int i = 0; i < songsToRemove.size(); i++)
                {
                    for(int j = 0; j < _playlist.size(); j++)
                    {
                        _playlist[j]->remover(songsToRemove[j]);
                    }
                    for(int j = 0; j < _albuns.size(); j++)
                    {
                        _albuns[j]->remover(songsToRemove[j]);
                    }

                    _songs.removeOne(songsToRemove[i]);
                    songsToRemove[i]->apagar();
                }
                MovePageToSongs();
            }
        }
        break;
    case 5:
        // page_artist
        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_artist_tableWidget_albuns->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_artist_tableWidget_albuns->rowCount(); i++)
        {
            if(ui->page_artist_tableWidget_albuns->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_artist_tableWidget_albuns->selectRow(i);
            }
        }
        selected = ui->page_artist_tableWidget_albuns->selectedItems();
        ui->page_artist_tableWidget_albuns->setSelectionMode(QAbstractItemView::SingleSelection);

        if(!selected.isEmpty())
        {
            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!AlbunsToRemove.contains(_albuns[index]))
                    AlbunsToRemove.append(_albuns[index]);

                selected.removeFirst();
            }

            msgBox.setText("Atenção, está prestes a remover " + QString::number(AlbunsToRemove.size()) + " albuns!");
            msgBox.exec();

            if(msgBox.result() == QMessageBox::AcceptRole)
            {
                for(int i = 0; i < AlbunsToRemove.size(); i++)
                {
                    songsToRemove.clear();
                    AlbunsToRemove[i]->getMusicas(&songsToRemove);

                    for(int j = 0; j < songsToRemove.size(); j++)
                    {
                        for(int k = 0; k < _playlist.size(); k++)
                        {
                            _playlist[k]->remover(songsToRemove[j]);
                        }
                    }

                    _albuns.removeOne(AlbunsToRemove[i]);
                    AlbunsToRemove[i]->apagar();
                }
                MovePageToAlbuns();
            }
        }
        break;
    case 6:
        // page_playlist
        ui->page_playlist_tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->page_playlist_tableWidget->selectionModel()->clearSelection();
        for(int i = 0; i < ui->page_playlist_tableWidget->rowCount(); i++)
        {
            if(ui->page_playlist_tableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                ui->page_playlist_tableWidget->selectRow(i);
            }
        }
        selected = ui->page_playlist_tableWidget->selectedItems();
        ui->page_playlist_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        if(!selected.isEmpty())
        {
            while(!selected.isEmpty())
            {
                index = selected.first()->data(Qt::WhatsThisRole).toInt();
                if(!songsToRemove.contains(_songs[index]))
                    songsToRemove.append(_songs[index]);

                selected.removeFirst();
            }

            msgBox.setText("Atenção, está prestes a remover " + QString::number(songsToRemove.size()) + " musicas!");
            msgBox.exec();

            if(msgBox.result() == QMessageBox::AcceptRole)
            {
                for(int i = 0; i < songsToRemove.size(); i++)
                {
                    _playlist[_showingPlaylist]->remover(songsToRemove[i]);
                }
                MovePageToSongs();
            }
        }
        break;
    }
}
void MainWindow::on_options_button_edit_toggled(bool checked)
{
    if(checked)
    {
        switch(ui->pages->currentIndex())
        {
        case 0:
            // page_categories
            ui->page_categories_tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
            ui->page_categories_tableWidget->clearSelection();
            break;

        case 1:
            // page_album_info
            ui->page_album_info_tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
            ui->page_album_info_tableWidget->clearSelection();
            break;

        case 5:
            // page_artist
            ui->page_artist_tableWidget_albuns->setEditTriggers(QAbstractItemView::AllEditTriggers);
            ui->page_artist_tableWidget_albuns->clearSelection();
            break;

        case 6:
            // page_playlist
            ui->page_playlist_tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
            ui->page_playlist_tableWidget->clearSelection();
            break;
        }
    }else{
        switch(ui->pages->currentIndex())
        {
        case 0:
            // page_categories
            ui->page_categories_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->page_categories_tableWidget->clearSelection();
            break;

        case 1:
            // page_album_info
            ui->page_album_info_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->page_album_info_tableWidget->clearSelection();
            break;

        case 5:
            // page_artist
            ui->page_artist_tableWidget_albuns->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->page_artist_tableWidget_albuns->clearSelection();
            break;

        case 6:
            // page_playlist
            ui->page_playlist_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->page_playlist_tableWidget->clearSelection();
            break;

        }
    }
}

