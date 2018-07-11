#ifndef SQLAMP_H
#define SQLAMP_H

#include <QtWidgets>
#include <QtSql>

class SQLamp: public QMainWindow
{
    Q_OBJECT
public:
    SQLamp();

private:
    void showError(const QSqlError &err);
    QSqlRelationalTableModel *model;
    int authorIdx, genreIdx;
};

#endif // SQLAMP_H
