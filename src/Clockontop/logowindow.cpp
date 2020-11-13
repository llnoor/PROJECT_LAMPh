#include <QtWidgets>
#include "logowindow.h"

Logo::Logo(QStringList argumentsQStringList): QWidget(0, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    argX = "50";
    argY = "100";
    argFont = "36";
    argColor = "gray";

    for (int i=0; i<argumentsQStringList.size(); i++){
        if (argumentsQStringList.at(i).contains("-x", Qt::CaseInsensitive)){
            argX = argumentsQStringList.at(i+1);
        }
        if (argumentsQStringList.at(i).contains("-y", Qt::CaseInsensitive)){
            argY = argumentsQStringList.at(i+1);
        }
        if (argumentsQStringList.at(i).contains("-font", Qt::CaseInsensitive)){
            argFont = argumentsQStringList.at(i+1);
        }
        if (argumentsQStringList.at(i).contains("-color", Qt::CaseInsensitive)){
            argColor = argumentsQStringList.at(i+1);
        }
    }

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

    this->setGeometry(argX.toInt(),argY.toInt(),this->size().width(),this->size().height());
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

    label_1->setFont(QFont("Russo One",argFont.toInt()));

    label_1->setStyleSheet(QString("color: %1;").arg(argColor));


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


