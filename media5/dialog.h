#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QFileDialog>
#include "classes.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Autor * getNewArtist();

private slots:

    void on_dialog_button_add_artwork_clicked();
    void on_dialog_button_cancel_clicked();
    void on_dialog_button_save_clicked();

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QString _photo;
    Autor *_newArtist;
};

#endif // DIALOG_H
