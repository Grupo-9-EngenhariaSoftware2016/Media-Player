#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
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

    // Order Options
    ui->page_categories_comboBox_order->insertItem(0,"Data de Adição");
    ui->page_categories_comboBox_order->insertItem(1,"A a Z");
    ui->page_categories_comboBox_order->insertItem(2,"Género");
    ui->page_categories_comboBox_order->insertItem(3,"Autor");

    // Hide tab for add album
    ui->page_add_album_tabs->tabBar()->hide();

    //set menu tab
    ui->menuTab->setCurrentIndex(1);
    ui->menuTab->setFixedWidth(41);

    MovePageToAlbuns();
}

MainWindow::~MainWindow()
{
   QMainWindow::centralWidget();
    mydb.close();
    delete ui;
}

//==============================================================
// Metodos
void MainWindow::MovePageToAlbuns()
{
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

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set main page Albuns
    ui->page_categories_label_category->setText("Álbuns");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

#ifdef NO_DB
    // Tabela de dados
    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setRowCount(3);
    ui->page_categories_tableWidget->setColumnCount(3);
    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Imagem da capa"));
    ui->page_categories_tableWidget->setItem(0, 1, new QTableWidgetItem("Nome do Album"));
    ui->page_categories_tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem da capa1"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nome do Album1"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem da capa2"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nome do Album2"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));

    ui->statusBar->showMessage( QString::number(ui->menu_small_button_album->isChecked()));
#endif

//    QSqlQuery query;        // Preparar queries à base de dados
//    query.prepare("select * from album;");

//    if(query.exec()){
//        connClose();
//    }

//    //Login conn;
//    //QSqlQueryModel *modal=new query;

}

void MainWindow::MovePageToArtists()
{
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

    //Reset botão "Selecionar"
    ui->page_categories_button_select->setChecked(false);

    // Set main page Autores
    ui->page_categories_label_category->setText("Autores");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

#ifdef NO_DB
    //Dados da Tabela
    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setRowCount(5);
    ui->page_categories_tableWidget->setColumnCount(3);

    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de albuns"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Nº de albuns"));

    ui->page_categories_tableWidget->setItem(3, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->page_categories_tableWidget->setItem(4, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->page_categories_tableWidget->setItem(4, 1, new QTableWidgetItem("Nome do Artista"));
    ui->page_categories_tableWidget->setItem(4, 2, new QTableWidgetItem("Nº de albuns"));
#endif
}

void MainWindow::MovePageToPlayer()
{
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

    ui->page_playlist_label_name->setText("Em Execução");
    ui->pages->setCurrentIndex(6);

#ifdef NO_DB
    // tabela de dados
    ui->page_playlist_tableWidget->clear();
    ui->page_playlist_tableWidget->setRowCount(3);
    ui->page_playlist_tableWidget->setColumnCount(3);
    ui->page_playlist_tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Musica"));
    ui->page_playlist_tableWidget->setItem(0, 1, new QTableWidgetItem("Album"));
    ui->page_playlist_tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->page_playlist_tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Musica1"));
    ui->page_playlist_tableWidget->setItem(1, 1, new QTableWidgetItem("Album1"));
    ui->page_playlist_tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));


    ui->page_playlist_tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Musica2"));
    ui->page_playlist_tableWidget->setItem(2, 1, new QTableWidgetItem("Album2"));
    ui->page_playlist_tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));
#endif
}

void MainWindow::MovePageToPlaylists()
{
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

    // Set Main Page Playlists
    ui->page_categories_label_category->setText("Playlists");
    ui->pages->setCurrentIndex(0);

    // Set Player Tab
    ui->Player_tab->setCurrentIndex(0);

#ifdef NO_DB
    //Dados da Tabela
    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setRowCount(3);
    ui->page_categories_tableWidget->setColumnCount(3);
    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Playlist"));
    ui->page_categories_tableWidget->setItem(0, 1, new QTableWidgetItem("Nº de Musicas"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Nº de Musicas1"));

    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Nº de Musicas1"));
#endif
}

void MainWindow::MovePageToSongs()
{
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

    // Set main page Musicas
    ui->page_categories_label_category->setText("Músicas");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

#ifdef NO_DB
    // tabela de dados
    ui->page_categories_tableWidget->clear();
    ui->page_categories_tableWidget->setRowCount(3);
    ui->page_categories_tableWidget->setColumnCount(3);
    ui->page_categories_tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Musica"));
    ui->page_categories_tableWidget->setItem(0, 1, new QTableWidgetItem("Album"));
    ui->page_categories_tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->page_categories_tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Musica1"));
    ui->page_categories_tableWidget->setItem(1, 1, new QTableWidgetItem("Album1"));
    ui->page_categories_tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));


    ui->page_categories_tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Musica2"));
    ui->page_categories_tableWidget->setItem(2, 1, new QTableWidgetItem("Album2"));
    ui->page_categories_tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));
#endif
}

void MainWindow::MovePageToSearch()
{
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

    ui->pages->setCurrentIndex(4);
}

void MainWindow::on_menu_small_button_search_clicked()
{
    ui->menuTab->setCurrentIndex(0);
    ui->menuTab->setFixedWidth(200);
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
    ui->menuTab->setCurrentIndex(0);
    ui->menuTab->setFixedWidth(200);
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
    ui->pages->setCurrentIndex(2);
    ui->Player_tab->setCurrentIndex(2);
}

void MainWindow::on_menu_full_button_add_artist_clicked()
{

}

void MainWindow::on_menu_full_button_add_list_clicked()
{

}

void MainWindow::on_menu_full_button_add_song_clicked()
{

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
    ui->menuTab->setCurrentIndex(1);
    ui->menuTab->setFixedWidth(41);
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
