#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>


class MyDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    QStringList values;

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

#endif // MYDELEGATE_H
