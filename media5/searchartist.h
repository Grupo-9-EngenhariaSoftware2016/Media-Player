#ifndef SEARCHARTIST_H
#define SEARCHARTIST_H

#include <QDialog>
#include "classes.h"
namespace Ui {
class searchArtist;
}

class searchArtist : public QDialog
{
    Q_OBJECT

public:
    QList<Autor*> _artists;
    QList<Autor*> _newartists;
    explicit searchArtist(QWidget *parent = 0);
    void getArtists(QList<Autor*> listaAutores,QList<Autor*> AutoresExistentes);
    QList<Autor*> getNewArtist();
    ~searchArtist();

private slots:
    void on_lineEdit_textChanged(const QString &text);

    void on_bt_add_clicked();

    void on_bt_rmv_clicked();

private:

    Ui::searchArtist *ui;
};

#endif // SEARCHARTIST_H