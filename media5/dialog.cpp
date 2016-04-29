#include "dialog.h"
#include "ui_dialog.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QString sPath = "C:/ ";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    //ui->treeView->setModel(dirmodel);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    // Add Songs
    // Adicao por botao
    // Abertura de uma browser de ficheiros e adicionar 1 ou mais musicas em mp3
    QStringList filename = QFileDialog::getOpenFileNames(this,tr("Open a File"),"","MP3 files (*.mp3)");
    // Recolha do nomes dos ficheiros
    QString filenames = filename.join(", ");
    QMessageBox::information(this,tr("File Name"),filenames);
   //  Tendo Todos os Paths do Ficheiro prosseguir para a mostrar o conteudo

    //Add Directory
//    QFileDialog* _f_dlg = new QFileDialog(this);
//      _f_dlg->setFileMode(QFileDialog::Directory);
//      _f_dlg->setOption(QFileDialog::DontUseNativeDialog, true);
//      _f_dlg->setNameFilter("All MP3 files (*.mp3)");

      // Try to select multiple files and directories at the same time in QFileDialog
//      QListView *l = _f_dlg->findChild<QListView*>("listView");
//      if (l) {
//        l->setSelectionMode(QAbstractItemView::MultiSelection);
//       }
//      QTreeView *t = _f_dlg->findChild<QTreeView*>();
//       if (t) {
//         t->setSelectionMode(QAbstractItemView::MultiSelection);
//        }

//      QDir _fnames = _f_dlg->selectedFiles();
//      QStringList fileList = _fnames.entryList();
//      QString filenames = fileList.join(", ");
//      QMessageBox::information(this,tr("File Name"),filenames);
}


void Dialog::on_bt_cancel_clicked()
{
    this->close();
}

void Dialog::on_bt_add_img_artwork_diag_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open a File"),"","Image files (*.jpg)");
    ui->img_artwork_diag->setPixmap(QPixmap(filename));
}
