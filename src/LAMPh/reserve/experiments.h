#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <QWidget>
#include <QtSql>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>


QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class Experiments : public QWidget
{
    Q_OBJECT
public:
    Experiments();
public slots:
    void openWindowF();

private:
    QGroupBox *groupExperiments();
    QTimer *timerAuth;
    QLabel *label_login;
    QLabel *label_pass;
    QLineEdit *line_login;
    QLineEdit *line_pass;
    QPushButton *button_Submit;

    QFont *font;
};

#endif // EXPERIMENTS_H
