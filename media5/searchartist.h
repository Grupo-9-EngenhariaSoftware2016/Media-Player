#ifndef SEARCHARTIST_H
#define SEARCHARTIST_H

#include <QDialog>
#include "classes.h"
#include "database.h"

namespace Ui {
class searchArtist;
}

class searchArtist : public QDialog
{
    Q_OBJECT

public:
    QList<Autor*> _artists;
    QList<Autor*> _newartists;
    QList<Musica*> _songs;
    Musica *_song;


    explicit searchArtist(QString name, QList<Autor *> listaAutores, QList<Musica*> songs, QWidget *parent = 0);
    void getArtists(int song);
    QList<Autor*> getNewArtist();
    ~searchArtist();

private slots:
    void on_lineEdit_textChanged(const QString &text);

    void on_bt_add_clicked();

    void on_bt_rmv_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:

    Ui::searchArtist *ui;
};

#endif // SEARCHARTIST_H
