#include <QtWidgets>
#include "logowindow.h"

Logo::Logo(): QWidget(0, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupLogo(), 0, 0);
    setLayout(grid);

    setWindowTitle(tr("Logo"));
    resize(50, 50);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background: rgba(255, 255, 255, 100);");

    /*timerLogo = new QTimer();
    connect(timerLogo, SIGNAL(timeout()), this, SIGNAL(slotTimerAlarm()) );
    timerLogo->start(1000);*/


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000);

    this->setGeometry(50,100,this->size().width(),this->size().height());
}


void Logo::slotTimerAlarm()
{
    /* Ежесекундно обновляем данные по текущему времени
     * Перезапускать таймер не требуется
     * */
    QString timeQString =QTime::currentTime().toString("  hh:mm  ");

    label_1->setText(timeQString);
}

QGroupBox *Logo::groupLogo()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
    groupBox->setStyleSheet("border: 0px solid white");

    label_1 = new QLabel(tr("LAMPh"));
    label_1->setFont(QFont("Russo One",36));
    label_1->setStyleSheet("color: gray;");



    label_2 = new QLabel(tr("        Laboratory for Advanced Materials Physics\n "));
    //label_2->setFont(QFont("Russo One",15));
    //label_2->setStyleSheet("color: gray;");


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(label_1);
    //vbox->addWidget(label_2);
    vbox->setSpacing(0);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setMargin(0);
    groupBox->setLayout(vbox);

    return groupBox;
}


