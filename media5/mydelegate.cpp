#include "mydelegate.h"

MyDelegate::MyDelegate(QStringList input, QObject *parent) :
    QItemDelegate(parent)
{
    values << input;
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
    int selectedIndex = values.indexOf(value);

    cBox->addItems(values);

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
    QString selectedValue = values.at(selectedIndex);

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
    values << value;
}
void MyDelegate::clearValues()
{
    values.clear();
}
