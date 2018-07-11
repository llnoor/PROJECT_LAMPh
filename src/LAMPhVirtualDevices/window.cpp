#include "window.h"
#include "logowindow.h"
#include "lamphdevices.h"

Window::Window() : QWidget(0, Qt::Window | Qt::FramelessWindowHint)
{
    logoWindow = new Logo();
    //logoWindow->show();

    lamphDevices = new LAMPhDevices("VirtualDevices");
    lamphDevices->show();

    setAttribute(Qt::WA_TranslucentBackground);
}




