#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "classes.h"
#include <QModelIndex>

#define NO_DB

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!connOpen())
    {
         ui->statusBar->showMessage(" - Coneccao a DB - INVALIDA");
    }
    else
    {
         ui->statusBar->showMessage(" - Coneccao a DB - Valida");
    }

    // DIR for musics and images

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

    // Order Options
    ui->page_categories_comboBox_order->insertItem(0,"Data de Adição");
    ui->page_categories_comboBox_order->insertItem(1,"A a Z");
    ui->page_categories_comboBox_order->insertItem(2,"Género");
    ui->page_categories_comboBox_order->insertItem(3,"Autor");

    // Hide tab for add album
    ui->page_add_album_tabs->tabBar()->hide();

#ifdef NO_DB

    Album *newAlbum;
    //Playlist *newPlaylist;
    //Autor *newArtist;
    //Musica *newSong;
    //_player = new Player;


    for(int i = 0; i < 5; i++){
        newAlbum = new Album;
        QString text;
        //newPlaylist = new Playlist;
        //newArtist = new Autor;
        //newSong = new Musica;

        text = "";
        QTextStream(&text) << "Imagem das boas " << i;
        newAlbum->setImagem(text);
        text = "";
        QTextStream(&text) << "Vamos la! " << i;
        newAlbum->setNome(text);
        text = "";
        QTextStream(&text) << "mesmo como eu gosto " << i;
        newAlbum->setGenero(text);
        text = "";
        QTextStream(&text) << "nada a comentar " << i;
        newAlbum->setDescricao(text);

        _albuns.append(newAlbum);
        //_playlist.append(newPlaylist);
        //_artists.append(newArtist);
        //_songs.append(newSong);
    }

#endif // NO_DB

    ExpandMenu(false);
    MovePageToAlbuns();
}

MainWindow::~MainWindow()
{
    QMainWindow::centralWidget();
    mydb.close();

    while (!_albuns.isEmpty())
        _albuns.removeLast();
    /*
    while (!_playlist.isEmpty())
        _playlist.removeLast();

    while (!_artists.isEmpty())
        _artists.removeLast();

    while (!_songs.isEmpty())
        _songs.removeLast();

    delete _player;
    */
    delete ui;
}

//==============================================================
// Metodos de simplificação
// Para tornar o código mais legivel e de fácil manutenção
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

void MainWindow::MovePageToAlbuns()
{
    CheckMenuButton("Albuns");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set main page Albuns
    ui->page_categories_label_category->setText("Álbuns");
    ui->pages->setCurrentIndex(0);

    // Set labels para a tabela de elementos
    QStringList tableLabels;
    tableLabels << tr("Capa") << tr("Nome") << tr("Género") << tr("Artistas");

    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setColumnCount(tableLabels.size());
    ui->page_categories_tableWidget->setHorizontalHeaderLabels(tableLabels);

#ifdef NO_DB

    /*
     * Aceder a base de dados e preencher _albuns
     * */

    ui->page_categories_tableWidget->clear();

    for(int i = 0; i < _albuns.size(); i++)
    {
        ui->page_categories_tableWidget->insertRow(i);
        ui->page_categories_tableWidget->setItem(i, 0, new QTableWidgetItem(_albuns[i]->getImagem()));
        ui->page_categories_tableWidget->setItem(i, 1, new QTableWidgetItem(_albuns[i]->getNome()));
        ui->page_categories_tableWidget->setItem(i, 2, new QTableWidgetItem(_albuns[i]->getGenero()));
        ui->page_categories_tableWidget->setItem(i, 3, new QTableWidgetItem("album[i].artista"));
    }

#endif
}

void MainWindow::MovePageToAlbumInfo(int index)
{
    ShowOptionsTab(false);
    ShowProgressTab(false);

    // reset de botão select
    ui->page_album_info_button_select->setChecked(false);

    ui->page_album_info_label_name->setText(_albuns[index]->getNome());
    ui->page_album_info_label_artwork->setText(_albuns[index]->getImagem());
    ui->page_album_info_label_gender->setText(_albuns[index]->getGenero());
    ui->page_album_info_label_year->setText("Não estamos a guardar a data!!!");


    ui->pages->setCurrentIndex(1);
}

void MainWindow::MovePageToArtists()
{
    CheckMenuButton("Artists");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set main page Autores
    ui->page_categories_label_category->setText("Autores");
    ui->pages->setCurrentIndex(0);

    // Set labels para a tabela de elementos
    QStringList tableLabels;
    tableLabels << tr("Imagem") << tr("Nome") << tr("Nº de Álbuns") << tr("Nº de Musicas");

    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setColumnCount(tableLabels.size());
    ui->page_categories_tableWidget->setHorizontalHeaderLabels(tableLabels);

#ifdef NO_DB
    //Dados da Tabela
    ui->page_categories_tableWidget->setRowCount(5);

    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de albuns"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de musicas"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Nº de albuns"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de musicas"));

    ui->page_categories_tableWidget->setItem(3, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->page_categories_tableWidget->setItem(4, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(4, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(4, 2, new QTableWidgetItem("Nº de albuns"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de musicas"));
#endif
}

void MainWindow::MovePageToArtistInfo(int index)
{

}

void MainWindow::MovePageToPlayer()
{
    CheckMenuButton("Player");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    ui->page_playlist_label_name->setText("Em Execução");
    ui->pages->setCurrentIndex(6);

    // Set labels para a tabela de elementos
    QStringList tableLabels;
    tableLabels << tr("Capa") << tr("Nome") << tr("Album") << tr("Artistas");

    ui->page_playlist_tableWidget->clear();
    ui->page_playlist_tableWidget->setColumnCount(tableLabels.size());
    ui->page_playlist_tableWidget->setHorizontalHeaderLabels(tableLabels);

#ifdef NO_DB
    // tabela de dados
    ui->page_playlist_tableWidget->setRowCount(3);
    ui->page_playlist_tableWidget->setItem(0, 0, new QTableWidgetItem("Imagem de album"));
    ui->page_playlist_tableWidget->setItem(0, 1, new QTableWidgetItem("Nome da Musica"));
    ui->page_playlist_tableWidget->setItem(0, 2, new QTableWidgetItem("Album"));
    ui->page_playlist_tableWidget->setItem(0, 3, new QTableWidgetItem("Artista"));

    ui->page_playlist_tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem de album1"));
    ui->page_playlist_tableWidget->setItem(1, 1, new QTableWidgetItem("Nome da Musica1"));
    ui->page_playlist_tableWidget->setItem(1, 2, new QTableWidgetItem("Album1"));
    ui->page_playlist_tableWidget->setItem(1, 3, new QTableWidgetItem("Artista1"));

    ui->page_playlist_tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem de album2"));
    ui->page_playlist_tableWidget->setItem(2, 1, new QTableWidgetItem("Nome da Musica2"));
    ui->page_playlist_tableWidget->setItem(2, 2, new QTableWidgetItem("Album2"));
    ui->page_playlist_tableWidget->setItem(2, 3, new QTableWidgetItem("Artista2"));
#endif
}

void MainWindow::MovePageToPlaylists()
{
    CheckMenuButton("Playlist");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set Main Page Playlists
    ui->page_categories_label_category->setText("Playlists");
    ui->pages->setCurrentIndex(0);

    // Set labels para a tabela de elementos
    QStringList tableLabels;
    tableLabels << tr("Nome") << tr("Tamanho") << tr("Artistas");

    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setColumnCount(tableLabels.size());
    ui->page_categories_tableWidget->setHorizontalHeaderLabels(tableLabels);

#ifdef NO_DB
    //Dados da Tabela
    ui->page_categories_tableWidget->setRowCount(3);
    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Playlist"));
    ui->page_categories_tableWidget->setItem(0, 1, new QTableWidgetItem("Nº de Musicas"));
    ui->page_categories_tableWidget->setItem(0, 2, new QTableWidgetItem("Artistas"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nº de Musicas1"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Artistas1"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nº de Musicas1"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Artistas2"));
#endif
}

void MainWindow::MovePageToPlaylistInfo(int index)
{

}

void MainWindow::MovePageToSongs()
{
    CheckMenuButton("Songs");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set main page Musicas
    ui->page_categories_label_category->setText("Músicas");
    ui->pages->setCurrentIndex(0);

    // Set labels para a tabela de elementos
    QStringList tableLabels;
    tableLabels << tr("Capa") << tr("Nome") << tr("Album") << tr("Artistas");

    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setColumnCount(tableLabels.size());
    ui->page_categories_tableWidget->setHorizontalHeaderLabels(tableLabels);

#ifdef NO_DB
    // tabela de dados
    ui->page_categories_tableWidget->setRowCount(3);
    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Imagem de album"));
    ui->page_categories_tableWidget->setItem(0, 1, new QTableWidgetItem("Nome da Musica"));
    ui->page_categories_tableWidget->setItem(0, 2, new QTableWidgetItem("Album"));
    ui->page_categories_tableWidget->setItem(0, 3, new QTableWidgetItem("Artista"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem de album1"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nome da Musica1"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Album1"));
    ui->page_categories_tableWidget->setItem(1, 3, new QTableWidgetItem("Artista1"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem de album2"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nome da Musica2"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Album2"));
    ui->page_categories_tableWidget->setItem(2, 3, new QTableWidgetItem("Artista2"));
#endif
}

void MainWindow::MovePageToSongInfo(int index)
{

}

void MainWindow::MovePageToSearch()
{
    CheckMenuButton("Search");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    ui->pages->setCurrentIndex(4);
}

void MainWindow::MovePageToAddAlbuns()
{
	CheckMenuButton("Albuns");
    ShowOptionsTab(false);
    ShowProgressTab(true);

    ui->pages->setCurrentIndex(2);
}

void MainWindow::MovePageToAddPlaylist()
{
	CheckMenuButton("Playlist");
    ShowOptionsTab(false);
    ShowProgressTab(false);

    ui->pages->setCurrentIndex(7);
}

void MainWindow::MovePageToAddSongs()
{
	CheckMenuButton("Songs");
    ShowOptionsTab(false);
    ShowProgressTab(true);

    ui->pages->setCurrentIndex(3);
}

void MainWindow::NewArtist()
{
    mdialog = new Dialog(this);
    mdialog->show();
}

//==============================================================
// Handlers para ações de utilizador (Menu)
void MainWindow::on_menu_small_button_search_clicked()
{
    ExpandMenu(true);

    MovePageToSearch();
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

//==============================================================
// Handlers para ações de utilizador (Page_Categories)
void MainWindow::on_page_categories_button_add_clicked()
{
    if(ui->menu_small_button_album->isChecked() || ui->menu_full_button_album->isChecked())
        {
            MovePageToAddAlbuns();
        }

        if(ui->menu_small_button_artist->isChecked() || ui->menu_full_button_artist->isChecked())
        {
            NewArtist();
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
    }else{
        ShowOptionsTab(false);
    }
}

//==============================================================
//==============================================================
//==============================================================
//                POINT OF NOT DONE YET
//==============================================================
//==============================================================
//==============================================================
void MainWindow::on_page_categories_button_random_clicked()
{
    // Add all to player on random?
}

void MainWindow::on_page_categories_comboBox_order_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        // data de adição
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
        ui->page_categories_tableWidget->sortByColumn(3, Qt::AscendingOrder);
        break;
    }
}

void MainWindow::on_page_categories_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (ui->menu_full_button_album->isChecked()){
        MovePageToAlbumInfo(index.row());
    }else if (ui->menu_full_button_artist->isChecked()){
        MovePageToArtistInfo(index.row());
    }else if (ui->menu_full_button_list->isChecked()){
        MovePageToPlaylistInfo(index.row());
    }else if (ui->menu_full_button_song->isChecked()){
        MovePageToSongInfo(index.row());
    }
}

//==============================================================
// Handlers para ações de utilizador (Page Album Info)

void MainWindow::on_page_album_info_button_play_clicked()
{

}

void MainWindow::on_page_album_info_button_addTo_clicked()
{

}

void MainWindow::on_page_album_info_button_remove_clicked()
{

}

void MainWindow::on_page_album_info_button_exploreArtist_clicked()
{

}

void MainWindow::on_page_album_info_button_select_toggled(bool checked)
{
    if(checked)
    {
        ShowOptionsTab(true);
    }else{
        ShowOptionsTab(false);
    }
}

void MainWindow::on_page_album_info_tableWidget_doubleClicked(const QModelIndex &index)
{

}
//==============================================================
// Handlers para ações de utilizador (Page Add Album)

void MainWindow::on_page_add_album_button_addArtwork_clicked()
{

}

void MainWindow::on_page_add_album_button_addMusic_clicked()
{

}

void MainWindow::on_page_add_album_button_addFolder_clicked()
{

}

void MainWindow::on_page_add_album_button_remove_clicked()
{

}

void MainWindow::on_page_add_album_button_addArtistTo_clicked()
{

}

void MainWindow::on_page_add_album_button_addArtistToAll_clicked()
{

}

void MainWindow::on_page_add_album_button_removeArtistFrom_clicked()
{

}

void MainWindow::on_page_add_album_button_newArtist_clicked()
{
    NewArtist();
}

//==============================================================
// Handlers para ações de utilizador (Page Add Music)

void MainWindow::on_page_add_music_button_addFolder_clicked()
{

}

void MainWindow::on_page_add_music_button_addMusic_clicked()
{

}

//==============================================================
// Handlers para ações de utilizador (Page Search)
void MainWindow::on_page_search_tableWidget_artists_doubleClicked(const QModelIndex &index)
{

}

void MainWindow::on_page_search_tableWidget_albuns_doubleClicked(const QModelIndex &index)
{

}

void MainWindow::on_page_search_tableWidget_musics_doubleClicked(const QModelIndex &index)
{

}

//==============================================================
// Handlers para ações de utilizador (Page Artist)

void MainWindow::on_page_artist_button_play_clicked()
{

}

void MainWindow::on_page_artist_button_addTo_clicked()
{

}

void MainWindow::on_page_artist_button_remove_clicked()
{

}

void MainWindow::on_page_artist_tableWidget_albuns_doubleClicked(const QModelIndex &index)
{

}

//==============================================================
// Handlers para ações de utilizador (Page Playlist)

void MainWindow::on_page_playlist_button_play_clicked()
{

}

void MainWindow::on_page_playlist_button_addMusic_clicked()
{

}

void MainWindow::on_page_playlist_button_remove_clicked()
{

}

void MainWindow::on_page_playlist_tableWidget_doubleClicked(const QModelIndex &index)
{

}

//==============================================================
// Handlers para ações de utilizador (Page Add Playlist)

void MainWindow::on_page_add_playlist_button_search_clicked()
{

}

void MainWindow::on_page_add_playlist_button_add_clicked()
{

}

void MainWindow::on_page_add_playlist_button_remove_clicked()
{

}

//==============================================================
// Handlers para ações de utilizador (Tab Player)

void MainWindow::on_player_slider_sliderReleased()
{

}

void MainWindow::on_player_button_previous_clicked()
{

}

void MainWindow::on_player_button_play_clicked()
{

}

void MainWindow::on_player_button_stop_clicked()
{

}

void MainWindow::on_player_button_next_clicked()
{

}

//==============================================================
// Handlers para ações de utilizador (Tab Progress)

void MainWindow::on_progress_button_previous_clicked()
{

}

void MainWindow::on_progress_button_next_clicked()
{

}

void MainWindow::on_progress_button_cancel_clicked()
{

}

void MainWindow::on_progress_button_save_clicked()
{

}

//==============================================================
// Handlers para ações de utilizador (Tab Options)

void MainWindow::on_options_button_play_clicked()
{

}

void MainWindow::on_options_button_edit_clicked()
{

}

void MainWindow::on_options_button_remove_clicked()
{

}

void MainWindow::on_options_button_information_clicked()
{

}
