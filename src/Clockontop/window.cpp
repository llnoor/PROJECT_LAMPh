#include "window.h"
#include "logowindow.h"

Window::Window(QStringList argumentsQStringList) : QWidget(0, Qt::Window | Qt::FramelessWindowHint)
{
    logoWindow = new Logo(argumentsQStringList);
    logoWindow->show();

    setAttribute(Qt::WA_TranslucentBackground);
}




