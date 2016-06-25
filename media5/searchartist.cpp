#include "searchartist.h"
#include "ui_searchartist.h"
#include <QDebug>

searchArtist::searchArtist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchArtist)
{
    ui->setupUi(this);

}

searchArtist::~searchArtist()
{
    delete ui;
}
void searchArtist::getArtists(QList<Autor*> listaAutores,QList<Autor*> AutoresExistentes)
{
    _artists = listaAutores;
    _newartists = AutoresExistentes;

    //Lista de Autores a adicionar e Remover

    for (int i = 0; i < _newartists.size(); ++i)
    {
        // Adicionar e remover items
        QListWidgetItem* item = new QListWidgetItem(_newartists[i]->getNome(), ui->List_NEW);
        item->setData(Qt::UserRole,_newartists[i]->getIdBD());
        item->setText(_newartists[i]->getNome());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    for (int i = 0; i < _artists.size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(_artists[i]->getNome(), ui->List_ALL);
        item->setData(Qt::UserRole,_artists[i]->getIdBD()); //Associar ID do Artista
        item->setText(_artists[i]->getNome());
    }

}
void searchArtist::on_lineEdit_textChanged(const QString &text)
{
    //Alterar a tabela de Artistas consoate a edicao de texto

    ui->List_ALL->clear();

    if(text.isEmpty())
    {
        for (int i = 0; i < _artists.size(); ++i)
        {
            QListWidgetItem* item = new QListWidgetItem(_artists[i]->getNome(), ui->List_ALL);
            item->setData(Qt::UserRole,_artists[i]->getIdBD()); //Associar ID do Artista
            item->setText(_artists[i]->getNome());
        }
    }
    else
    {
        for (int i = 0; i < _artists.size(); ++i)
        {
            if(_artists[i]->procurar(text))
            {
                QListWidgetItem* item = new QListWidgetItem(_artists[i]->getNome(), ui->List_ALL);
                item->setData(Qt::UserRole,_artists[i]->getIdBD()); //Associar ID do Artista
                item->setText(_artists[i]->getNome());
            }
        }
    }

}
void searchArtist::on_bt_add_clicked()
{
   //Verificar se existe algum artista já adicionado igual
    QList<QListWidgetItem*> items = ui->List_ALL->selectedItems();

    bool exist =false;
    int ID;
    QString Nome;

    for (int i = 0; i < items.count(); ++i)
    {
        exist = false;
        ID=items[i]->data(Qt::UserRole).toInt();
        Nome=items[i]->data(Qt::DisplayRole).toString();

        for (int j = 0; j < ui->List_NEW->count(); ++j)
        {
            if(ID == ui->List_NEW->item(j)->data(Qt::UserRole).toInt())
            {
                exist=true;
            }
        }

        if(!exist)
        {
            QListWidgetItem* item = new QListWidgetItem(Nome, ui->List_NEW);
            item->setData(Qt::UserRole,ID);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);

            qDebug() << "ID"<< ID << "|Nome" << Nome << endl;
        }
    }
}
void searchArtist::on_bt_rmv_clicked()
{
    int ID;
    QVector <int> list;

    //Verificar quais a checkboxes assinaladas
    for (int idx = 0; idx < ui->List_NEW->count(); idx++)
    {
        ID = ui->List_NEW->item(idx)->data(Qt::UserRole).toInt();
        qDebug() << ui->List_NEW->item(idx)->checkState() << endl;

        if (ui->List_NEW->item(idx)->checkState() == Qt::CheckState(Qt::Checked))
        {
            list.push_back(ID);
        }
    }

    //Remover items correspondentes
    for (int i = 0; i < list.size(); ++i)
    {
        for (int j = 0; j <ui->List_NEW->count(); ++j)
        {
            ID = ui->List_NEW->item(j)->data(Qt::UserRole).toInt();

            if(list[i]==ID)
            {
               ui->List_NEW->takeItem(j);
            }
        }
    }

    qDebug() <<"===========================" <<endl;

}
