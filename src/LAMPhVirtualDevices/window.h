#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <logowindow.h>
#include <lamphdevices.h>


class Window : public QWidget
{
    Q_OBJECT
public:
    Window();

private Q_SLOTS:
    //void openWindow();
    //void login(QString loginQString);

private:
    QTimer *timer;
    Logo *logoWindow;
    LAMPhDevices *lamphDevices;



};

#endif


