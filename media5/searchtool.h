#ifndef SEARCHTOOL_H
#define SEARCHTOOL_H

#include <QWidget>

namespace Ui {
class Searchtool;
}

class Searchtool : public QWidget
{
    Q_OBJECT

public:
    explicit Searchtool(QWidget *parent = 0);
    ~Searchtool();

private:
    Ui::Searchtool *ui;
};

#endif // SEARCHTOOL_H
