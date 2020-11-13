#include "window.h"
#include "logowindow.h"

Window::Window() : QWidget(0, Qt::Window | Qt::FramelessWindowHint)
{
    logoWindow = new Logo();
    logoWindow->show();

    setAttribute(Qt::WA_TranslucentBackground);
}




