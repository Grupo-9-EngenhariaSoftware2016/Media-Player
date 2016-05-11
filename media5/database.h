#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>

class Database
{

public:

    QSqlDatabase mydb;

    Database();

    bool connOpen()
    {
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName(QDir::currentPath() +"/debug/database.db");

        if(!mydb.open()){
            qDebug() << ("Falha a abrir a DB");
            return false;
        }else{
            qDebug() << ("Abriu a DB");
            return true;
        }
    }

    void connClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }


};

#endif // DATABASE_H
