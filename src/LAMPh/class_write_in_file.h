#ifndef CLASS_WRITE_IN_FILE_H
#define CLASS_WRITE_IN_FILE_H
#define CurvCnt 20
#define CurvCounter 5
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

class class_write_in_file
{
public:
    class_write_in_file();
    void write_in_file();
    void get_bool(bool , int );
    void create_new_file();
    void get_all_results(float , int );
    void get_Name(QString,int);
    void get_Unit(QString,int);
    void get_x_result(float );
    void get_numberofdeviceInt(int);

private:
    bool number_of_device[CurvCnt+CurvCounter];
    float all_results[CurvCnt+CurvCounter];
    QString all_names[CurvCnt+CurvCounter];
    QString all_units[CurvCnt+CurvCounter];
    float x_result;
    QString name;
    QString str;
    QString str1;
    QString dir;
    QDir dirconf;

};

#endif // CLASS_WRITE_IN_FILE_H
