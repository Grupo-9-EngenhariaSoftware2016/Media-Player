#include "mydelegate.h"

MyDelegate::MyDelegate(QStringList input, QObject *parent) :
    QItemDelegate(parent)
{
    _values << input;
}

QWidget* MyDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    return editor;

}
void MyDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    QComboBox *cBox = static_cast<QComboBox*>(editor);
    int selectedIndex = _values.indexOf(value);

    cBox->addItems(_values);

    if(selectedIndex != -1)
    {
        cBox->setCurrentIndex(selectedIndex);
    }

}
void MyDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *cBox = static_cast<QComboBox*>(editor);

    int selectedIndex = cBox->currentIndex();
    QString selectedValue = _values.at(selectedIndex);

    model->setData(index, selectedValue, Qt::DisplayRole);
}
void MyDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
void MyDelegate::addValues(QString value)
{
    _values << value;
}
void MyDelegate::clearValues()
{
    _values.clear();
}

/*
 *
 * MyAlbumDelegate
 *
 * */
MyAlbumDelegate::MyAlbumDelegate(QStringList valuesInput, QList<int> idsInput, QObject *parent) :
    QItemDelegate(parent)
{
    _values << valuesInput;
    _IDs = idsInput;
}

QWidget* MyAlbumDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    return editor;

}
void MyAlbumDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    int id = index.model()->data(index, Qt::UserRole).toInt();

    QComboBox *cBox = static_cast<QComboBox*>(editor);
    int selectedIndex = _values.indexOf(value);

    cBox->addItems(_values);

    if(selectedIndex != -1)
    {
        cBox->setCurrentIndex(selectedIndex);
    }

}
void MyAlbumDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *cBox = static_cast<QComboBox*>(editor);

    int selectedIndex = cBox->currentIndex();
    QString selectedValue = _values.at(selectedIndex);
    int selectedID = _IDs.at(selectedIndex);

    model->blockSignals(true);
    model->setData(index, selectedID, Qt::UserRole);
    model->blockSignals(false);
    model->setData(index, selectedValue, Qt::DisplayRole);
}
void MyAlbumDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

/*
 *
 * MyArtistDelegate
 *
 * */
MyArtistDelegate::MyArtistDelegate(QList<Autor*> listaAutores, Musica *song, QObject *parent) :
    QItemDelegate(parent)
{
    _listaAutores = listaAutores;
    _song = song;
}

QWidget* MyArtistDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
//    _diagArtist = new searchArtist(this);
//    _diagArtist->setWindowTitle("Criar novo artista");
//    _diagArtist->getArtists(_listaAutores,_song);
//    _diagArtist->exec();

    return NULL;

}
void MyArtistDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{

}
void MyArtistDelegate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{

}
void MyArtistDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
