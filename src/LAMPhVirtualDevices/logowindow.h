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
    Logo();

Q_SIGNALS:
    void firstWindow();

public Q_SLOTS:
    void showlogoWindow();

private:
    QGroupBox *groupLogo();
    QTimer *timerLogo;
    QLabel *label_1;
    QLabel *label_2;
};

#endif // LOGO_H
