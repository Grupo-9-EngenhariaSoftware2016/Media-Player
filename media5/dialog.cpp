#include "dialog.h"
#include "ui_dialog.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>
#include <database.h>
#include <classes.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QString sPath = "C:/ ";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    _photo = "";
   this->setFixedSize(this->width(),this->height());

}

Dialog::~Dialog()
{
    delete ui;
}

Autor* Dialog::getNewArtist()
{
    return _newArtist;
}

void Dialog::on_dialog_button_add_artwork_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","Image files (*.jpg , *.png)");

    if(!filename.isNull())
    {
        _photo = filename;
        ui->dialog_label_artwork->setPixmap(QPixmap(filename));
    }
}

void Dialog::on_dialog_button_cancel_clicked()
{
    this->reject();
}

void Dialog::on_dialog_button_save_clicked()
{
    if(ui->dialog_lineEdit_name->text().isEmpty())
    {
         QMessageBox::warning(this,"Adicionar Autor",tr("Falta preencher o nome do Autor."));
    }
    else
    {
        Database db;
        _newArtist = new Autor;
        _newArtist->setDataNascimento(ui->dialog_dateEdit_birth->date());
        _newArtist->setNome(ui->dialog_lineEdit_name->text());
        _newArtist->setNacionalidade(ui->dialog_lineEdit_nacionality->text());
        _newArtist->setImagem(_photo);
        _newArtist->criar();

        this->accept();
    }
}
