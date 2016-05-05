#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QModelIndex>

#define NO_DB
/*
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
    ui->bt_s_album->setCheckable(true);
    ui->bt_s_artist->setCheckable(true);
    ui->bt_s_list->setCheckable(true);
    ui->bt_s_play->setCheckable(true);
    ui->bt_s_song->setCheckable(true);

    ui->bt_f_album->setCheckable(true);
    ui->bt_f_artist->setCheckable(true);
    ui->bt_f_list->setCheckable(true);
    ui->bt_f_play->setCheckable(true);
    ui->bt_f_song->setCheckable(true);
    ui->bt_tab_select->setCheckable(true);

    // Order Options
    ui->cb_order->insertItem(0,"Data de Adição");
    ui->cb_order->insertItem(1,"A a Z");
    ui->cb_order->insertItem(2,"Género");
    ui->cb_order->insertItem(3,"Autor");

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
void MainWindow::FillForm()
{


}
void MainWindow::MovePageToAlbuns()
{
    //Set album as checked
    ui->bt_s_album->setChecked(true);
    ui->bt_f_album->setChecked(true);
    ui->bt_s_artist->setChecked(false);
    ui->bt_f_artist->setChecked(false);
    ui->bt_s_list->setChecked(false);
    ui->bt_f_list->setChecked(false);
    ui->bt_f_play->setChecked(false);
    ui->bt_s_play->setChecked(false);
    ui->bt_s_song->setChecked(false);
    ui->bt_f_song->setChecked(false);
    ui->bt_tab_select->setChecked(false);

    // Set main page Albuns
    ui->lb_category->setText("Álbuns");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

    // Hide tab for add album
    ui->tab_new_album->tabBar()->hide();

#ifdef NO_DB
    // Tabela de dados
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Imagem da capa"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Nome do Album"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem da capa1"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("Nome do Album1"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));

    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem da capa2"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("Nome do Album2"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));

    ui->statusBar->showMessage( QString::number(ui->bt_s_album->isChecked()));
    FillForm();
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
    // Set main page Autores
    ui->lb_category->setText("Autores");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

    //checked atributes
    ui->bt_s_album->setChecked(false);
    ui->bt_f_album->setChecked(false);
    ui->bt_s_artist->setChecked(true);
    ui->bt_f_artist->setChecked(true);
    ui->bt_s_list->setChecked(false);
    ui->bt_f_list->setChecked(false);
    ui->bt_f_play->setChecked(false);
    ui->bt_s_play->setChecked(false);
    ui->bt_s_song->setChecked(false);
    ui->bt_f_song->setChecked(false);

#ifdef NO_DB
    //Dados da Tabela
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(3);

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("Nome do Artista"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("Nº de albuns"));

    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("Nome do Artista"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("Nº de albuns"));

    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("Letra do Alfabeto"));

    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("Imagem do Artista"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("Nome do Artista"));
    ui->tableWidget->setItem(4, 2, new QTableWidgetItem("Nº de albuns"));
#endif

}
void MainWindow::MovePageToPlayer()
{
    ui->lb_category->setText("Em Execução");
    ui->label_31->setText("Em Execução");
    ui->pages->setCurrentIndex(6);

    ui->bt_s_album->setChecked(false);
    ui->bt_f_album->setChecked(false);
    ui->bt_s_artist->setChecked(false);
    ui->bt_f_artist->setChecked(false);
    ui->bt_s_list->setChecked(false);
    ui->bt_f_list->setChecked(false);
    ui->bt_f_play->setChecked(true);
    ui->bt_s_play->setChecked(true);
    ui->bt_s_song->setChecked(false);
    ui->bt_f_song->setChecked(false);

#ifdef NO_DB
    // tabela de dados
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Musica"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Album"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Musica1"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("Album1"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));


    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Musica2"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("Album2"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));
#endif
}
void MainWindow::MovePageToPlaylists()
{
    // Set Main Page Playlists
    ui->lb_category->setText("Playlists");
    ui->pages->setCurrentIndex(0);

    // Set Player Tab
    ui->Player_tab->setCurrentIndex(0);

    //checked atributes
    ui->bt_s_album->setChecked(false);
    ui->bt_f_album->setChecked(false);
    ui->bt_s_artist->setChecked(false);
    ui->bt_f_artist->setChecked(false);
    ui->bt_s_list->setChecked(true);
    ui->bt_f_list->setChecked(true);
    ui->bt_f_play->setChecked(false);
    ui->bt_s_play->setChecked(false);
    ui->bt_s_song->setChecked(false);
    ui->bt_f_song->setChecked(false);

#ifdef NO_DB
    //Dados da Tabela
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Playlist"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Nº de Musicas"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("Nº de Musicas1"));

    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Playlist1"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("Nº de Musicas1"));
#endif
}
void MainWindow::MovePageToSongs()
{
    // Set main page Musicas
    ui->lb_category->setText("Músicas");
    ui->pages->setCurrentIndex(0);

    // Set Player tab
    ui->Player_tab->setCurrentIndex(0);

    //checked atributes
    ui->bt_s_album->setChecked(false);
    ui->bt_f_album->setChecked(false);
    ui->bt_s_artist->setChecked(false);
    ui->bt_f_artist->setChecked(false);
    ui->bt_s_list->setChecked(false);
    ui->bt_f_list->setChecked(false);
    ui->bt_f_play->setChecked(false);
    ui->bt_s_play->setChecked(false);
    ui->bt_s_song->setChecked(true);
    ui->bt_f_song->setChecked(true);

#ifdef NO_DB
    // tabela de dados
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Nome da Musica"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Album"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("Artista"));

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Nome da Musica1"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("Album1"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("Artista1"));


    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Nome da Musica2"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("Album2"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("Artista2"));
#endif

}
void MainWindow::MovePageToSearch()
{
    ui->pages->setCurrentIndex(4);
    ui->bt_s_album->setChecked(false);
    ui->bt_f_album->setChecked(false);
    ui->bt_s_artist->setChecked(false);
    ui->bt_f_artist->setChecked(false);
    ui->bt_s_list->setChecked(false);
    ui->bt_f_list->setChecked(false);
    ui->bt_f_play->setChecked(false);
    ui->bt_s_play->setChecked(false);
    ui->bt_s_song->setChecked(false);
    ui->bt_f_song->setChecked(false);
}

//==============================================================
// Clicks de Butoes pequenos
void MainWindow::on_bt_s_search_clicked()
{
    ui->menuTab->setCurrentIndex(0);
    ui->menuTab->setFixedWidth(200);

}
void MainWindow::on_bt_f_menu_clicked()
{
    ui->menuTab->setCurrentIndex(1);
    ui->menuTab->setFixedWidth(41);

}
void MainWindow::on_bt_s_menu_clicked()
{
    ui->menuTab->setCurrentIndex(0);
    ui->menuTab->setFixedWidth(200);

}
void MainWindow::on_bt_s_album_clicked()
{
    MovePageToAlbuns();
}
void MainWindow::on_bt_f_album_clicked()
{
    MovePageToAlbuns();
}
void MainWindow::on_bt_f_add_album_clicked()
{
     ui->pages->setCurrentIndex(2);
     ui->Player_tab->setCurrentIndex(2);
}
void MainWindow::on_bt_s_artist_clicked()
{
    MovePageToArtists();
}
void MainWindow::on_bt_f_artist_clicked()
{
    MovePageToArtists();
}
void MainWindow::on_bt_s_song_clicked()
{
    MovePageToSongs();
}
void MainWindow::on_bt_f_song_clicked()
{
    MovePageToSongs();
}
void MainWindow::on_bt_s_list_clicked()
{
    MovePageToPlaylists();
}
void MainWindow::on_bt_f_list_clicked()
{
    MovePageToPlaylists();
}
void MainWindow::on_bt_f_play_clicked()
{
    MovePageToPlayer();
}
void MainWindow::on_bt_f_search_clicked()
{
     MovePageToSearch();
}
void MainWindow::on_bt_s_play_clicked()
{
    MovePageToPlayer();
}
void MainWindow::on_bt_tab_add_clicked()
{

    if(ui->bt_s_album->isChecked() ||ui->bt_f_album->isChecked())
    {
        ui->tab_new_album->setCurrentIndex(0);
        ui->pages->setCurrentIndex(2);
        ui->Player_tab->setCurrentIndex(2);
    }

    if(ui->bt_s_artist->isChecked() || ui->bt_f_artist->isChecked())
    {
        mdialog = new Dialog(this);
        mdialog->show();
    }

    if( ui->bt_s_song->isChecked() ||  ui->bt_f_song->isChecked())
    {
         ui->pages->setCurrentIndex(3);
    }

    if( ui->bt_s_list->isChecked() || ui->bt_f_list->isChecked())
    {
         ui->pages->setCurrentIndex(7);
         ui->Player_tab->setCurrentIndex(2);
    }

    if(  ui->bt_f_play->isChecked() || ui->bt_s_play->isChecked())
    {
         ui->pages->setCurrentIndex(3);
    }

}*/
void MainWindow::on_bt_add_songs_clicked()
{
    QStringList filename = QFileDialog::getOpenFileNames(this,tr("Open a File"),"","MP3 files (*.mp3)");
    for (int i = 0; i < filename.size(); ++i)
    {
     //         ui->listWidget->addItem(filename[i]);
    }
    //QMessageBox::information(this,tr("File Name"),filenames);
}
void MainWindow::on_bt_album_artwork_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","Image files (*.jpg)");
    ui->img_artwork->setPixmap(QPixmap(filename));
}
void MainWindow::on_bt_cancel_clicked()
{
    ui->lb_category->setText("Álbuns");
    ui->pages->setCurrentIndex(0);
    ui->Player_tab->setCurrentIndex(0);
}
void MainWindow::on_bt_next_clicked()
{
    if(ui->tab_new_album->currentIndex()<2){
        ui->tab_new_album->setCurrentIndex(ui->tab_new_album->currentIndex()+1);
    }
}
void MainWindow::on_bt_prev_clicked()
{
    if(ui->tab_new_album->currentIndex()>0){
        ui->tab_new_album->setCurrentIndex(ui->tab_new_album->currentIndex()-1);
    }
}
void MainWindow::on_bt_save_clicked()
{
    ui->lb_category->setText("Álbuns");
    ui->pages->setCurrentIndex(0);
    ui->Player_tab->setCurrentIndex(0);
}
void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    if(index.column()==0 && !ui->bt_tab_select->isChecked() &&
            ui->bt_s_album->isChecked() ||ui->bt_f_album->isChecked())
    {
        ui->pages->setCurrentIndex(1);
    }

    if(index.column()==0 && !ui->bt_tab_select->isChecked() &&
            ui->bt_s_artist->isChecked() ||ui->bt_f_artist->isChecked())
    {
        ui->pages->setCurrentIndex(5);
    }

    if(index.column()==0 && !ui->bt_tab_select->isChecked() &&
            ui->bt_s_list->isChecked() || ui->bt_f_list->isChecked())
    {
        ui->pages->setCurrentIndex(6);
    }

}
/*
void MainWindow::on_bt_nwalbum_nwartist_clicked()
{
    mdialog = new Dialog(this);
    mdialog->show();
}*/

void MainWindow::on_bt_tab_select_toggled(bool checked)
{
    if(checked){
         ui->Player_tab->setCurrentIndex(1);
         ui->bt_tab_select->setText("Cancel");
    }else{
        ui->Player_tab->setCurrentIndex(0);
        ui->bt_tab_select->setText("Seleccionar");

    }

}
void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if(!ui->bt_tab_select->isChecked() &&
            ui->bt_s_song->isChecked() ||ui->bt_f_song->isChecked())
    {
        QModelIndex nome=  ui->tableWidget->model()->index(index.row(),0 , QModelIndex());
        ui->lb_player_song->setText(ui->tableWidget->model()->data(nome).toString());

        QModelIndex artist=  ui->tableWidget->model()->index(index.row(),2 , QModelIndex());
        ui->lb_player_artist->setText(ui->tableWidget->model()->data(artist).toString());
    }

}
