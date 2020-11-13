/****************************************************************************
**  LAMP (Laboratory for Advanced Materials Physics)
**  Copyright (C) 2017 Ilnur Gimazov ubvfp94@mail.ru
**
****************************************************************************/

#include <QApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QStringList argumentsQStringList;
        for (int i=0; i<argc; i++){
            argumentsQStringList.append(QString::fromLocal8Bit(argv[i]));
        }

    QApplication app(argc, argv);
    Window window(argumentsQStringList);
    return app.exec();
}

