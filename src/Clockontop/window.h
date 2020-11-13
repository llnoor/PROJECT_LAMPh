#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <logowindow.h>


class Window : public QWidget
{
    Q_OBJECT
public:
    Window(QStringList);

private:
    QTimer *timer;
    Logo *logoWindow;
};

#endif


