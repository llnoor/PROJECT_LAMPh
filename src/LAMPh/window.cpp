#include "window.h"
#include "logowindow.h"
#include "authorization.h"
#include "sqlamp.h"
#include "experiments.h"
#include "lamphplot.h"

Window::Window(QStringList argumentsQStringList) : QWidget(0, Qt::Window | Qt::FramelessWindowHint)
{
    logoWindow = new Logo();
    logoWindow->show();

    //sqLamp = new SQLamp();

    //Exp = new Experiments();
    //Exp->show();


    authWindow = new AuthorizationWindow();
    //authWindow->show();

    lamphPlot = new LAMPhPlot(argumentsQStringList);
    //lamphPlot = new LAMPhPlot("user");
    lamphPlot ->show();

    //exprimentsWindow = new Experiments("user");
    //exprimentsWindow ->show();


    //MainWindow_Test = new MainWindow_T();
    //MainWindow_Test ->show();

    setAttribute(Qt::WA_TranslucentBackground);
    //timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(openWindow()));
    //timer->start(3000); //timer->setInterval(3000); timer->start();

    //connect(logoWindow, SIGNAL(firstWindow()) , authWindow, SLOT(openWindow()) );


    //connect(authWindow, SIGNAL(showlogoWindow()) , logoWindow, SLOT(showlogoWindow()) );
    //connect(authWindow, SIGNAL(openExperiments(int)) , logoWindow, SLOT(showlogoWindow()) );
    connect(authWindow, SIGNAL(sendLogin(QString)),this, SLOT(login(QString)));
    connect(logoWindow, SIGNAL(firstWindow()),this,SLOT(openWindow()));

}

void Window::openWindow()
{
    logoWindow->close();
    //logoWindow->close();
    //authWindow->show();
    //logoWindow->show();
    //timer->stop();
}



void Window::login(QString loginQString)
{
    exprimentsWindow = new Experiments(loginQString);
    exprimentsWindow ->show();


    //exprimentWindow = new Experiment(loginQString);
    //appaWindow = new Appa(loginQString);
}




