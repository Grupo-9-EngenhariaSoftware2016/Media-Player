#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include "searchartist.h"


class MyDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    QStringList _values;

public:
    explicit MyDelegate(QStringList input, QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void addValues(QString value);
    void clearValues();

signals:

public slots:

};

class MyAlbumDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    QStringList _values;
    QList<int> _IDs;

public:
    explicit MyAlbumDelegate(QStringList valuesInput, QList<int> idsInput, QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

class MyArtistDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    searchArtist *_diagArtist;

public:
    explicit MyArtistDelegate(searchArtist *dialog, QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};


#endif // MYDELEGATE_H
