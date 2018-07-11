#include <QtWidgets>
#include <QSql>

#include "authorization.h"
#include "logowindow.h"

Auth::Auth() : QWidget(0, Qt::Window | Qt::FramelessWindowHint)
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupAuth(), 0, 0);
    setLayout(grid);

    setWindowTitle(tr("Authorization"));
    resize(380, 120);

    this->setFont(QFont("Ubuntu",14));
}

void Auth::openWindow()
{
    this->setFont(QFont("Ubuntu",15));
    //this->close();
    //authWindow->show();
    //logoWindow->show();
    //timer->stop();
}


QGroupBox *Auth::groupAuth()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
        QGridLayout * gridLayout = new QGridLayout();
        label_login = new QLabel(tr("Login"));
        line_login = new QLineEdit(tr("Login"));
        label_pass = new QLabel(tr("Password"));
        line_pass = new QLineEdit(tr("Pass"));
        line_pass->setEchoMode(QLineEdit::Password);
        button_Submit = new QPushButton(tr("Submit"));
        gridLayout->addWidget(label_login,0,0);
        gridLayout->addWidget(line_login,0,1);
        gridLayout->addWidget(label_pass,1,0);
        gridLayout->addWidget(line_pass,1,1);
        gridLayout->addWidget(button_Submit,2,1);
        groupBox->setLayout(gridLayout);
    return groupBox;
}