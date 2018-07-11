#ifndef AUTH_H
#define AUTH_H

#include <QWidget>
#include <QtSql>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class Auth : public QWidget
{
    Q_OBJECT
public:
    Auth();

public slots:
    void openWindow();

private:
    QGroupBox *groupAuth();
    QTimer *timerAuth;
    QLabel *label_login;
    QLabel *label_pass;
    QLineEdit *line_login;
    QLineEdit *line_pass;
    QPushButton *button_Submit;

    QFont *font;
    //int fontSize;
};

#endif // AUTH_H