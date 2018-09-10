#include "class_write_in_file.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QDir>
#include <QDebug>


class_write_in_file::class_write_in_file()
{

    QDateTime date=QDateTime::currentDateTime();
    str1 = "DATA/";
    dir = date.toString("yyyy.MM.dd");
    dir.prepend(str1);

    qDebug() << "dir" << dir;

    dirconf.mkpath(dir);

    /*if (!dirconf.exists()){
      dirconf.mkdir(dir);
    }*/

    //dirconf.mkpath(dir);



    for (int i=0; i<CurvCnt+CurvCounter; i++)
    {
    number_of_device[i]=0;
    all_results[i]=0;
    all_names[i]="None";
    all_units[i]="None";
    }


}

void class_write_in_file::get_x_result(float result)
{
    x_result=result;
}

void class_write_in_file::get_all_results(float resultD, int new_int)
{
    all_results[new_int]=resultD;
}

void class_write_in_file::get_Name(QString nameD, int new_int)
{
    all_names[new_int]=nameD;
}

void class_write_in_file::get_Unit(QString unitD, int new_int)
{
    all_units[new_int]=unitD;
}

void class_write_in_file::get_bool(bool new_bool, int new_int)
{
    number_of_device[new_int]=new_bool;
}


void class_write_in_file::create_new_file()
{


    QDateTime date=QDateTime::currentDateTime();
    name = date.toString("yyyy.MM.dd/hh.mm.ss");
    name.prepend(str1);
    name.append(".txt");
    QFile file(name);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << "X\t";
    out << "Time\t";

    for (int r=0; r<CurvCnt+CurvCounter; r++)
    {
       out << all_names[r] << "\t";
    }

    out << "\n";

    out << "X\t";
    out << "Time\t";

    for (int r=0; r<CurvCnt+CurvCounter; r++)
    {
       out << all_units[r] << "\t";
    }

    out << "\n";

    file.flush();
    file.close();

}

void class_write_in_file::write_in_file()
{
    QFile file(name);
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);

    QDateTime date=QDateTime::currentDateTime();
    QString time = date.toString("hh.mm.ss.zzz");


    out << x_result << "\t";
    out << time << "\t";

    for (int r=0; r<CurvCnt+CurvCounter; r++)
    {
       if (1==number_of_device[r])
       out << all_results[r] << "\t";
       else out << " \t";
    }

    out << "\n";

    file.flush();
    file.close();

}

