#ifndef LOGO_H
#define LOGO_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class Logo : public QWidget
{
    Q_OBJECT
public:
    Logo(QStringList argumentsQStringList);

private slots:
    void slotTimerAlarm();

private:
    QGroupBox *groupLogo();
    QTimer *timer;
    QLabel *label_1;
    QLabel *label_2;

    QString argX;
    QString argY;
    QString argFont;
    QString argColor;
};

#endif // LOGO_H
