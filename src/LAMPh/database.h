#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QCryptographicHash>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "DataBase"
#define DATABASE_NAME       "DataBase.db"

#define DEVICE                  "DeviceTable"
#define DEVICE_HOSTNAME         "Hostname"
#define DEVICE_IP               "IP"
#define DEVICE_MAC              "MAC"

#define TABLE                   "TableExample"
#define TABLE_DATE              "Date"
#define TABLE_TIME              "Time"
#define TABLE_MESSAGE           "Message"
#define TABLE_RANDOM            "Random"

#define SCIENTISTS                          "TableScientists"
#define SCIENTISTS_NAME                     "Name"
#define SCIENTISTS_SURNAME                  "Surname"
#define SCIENTISTS_LOGIN                    "Login"
#define SCIENTISTS_DATE                     "Date"  //DATE OF REGISTRATION
#define SCIENTISTS_PARENT                   "Parent" //ADMIN
#define SCIENTISTS_PASS                     "Pass" //SHA1
#define SCIENTISTS_EMAIL                    "Email"
#define SCIENTISTS_TELEGRAM                 "Telegram" //USERNAME
#define SCIENTISTS_PHONE                    "Phone"
#define SCIENTISTS_EXPERIMENTS              "Experiments"  // => TableExperimentsOf(login of user)
#define SCIENTISTS_SELECTED_EXPERIMENTS     "Selected_experiments"  //FAVORITES =>TableSelectedExperimentsOf(login of user)
#define SCIENTISTS_BIRTHDAY                 "BirthDay"
#define SCIENTISTS_POSITION                 "Position"
#define SCIENTISTS_THEME                    "Theme"

#define EXPERIMENTS                         "TableExperiments"
#define EXPERIMENTS_NAME                    "Name"   // like LOGIN!!!
#define EXPERIMENTS_DESCRIPTION             "Description"
#define EXPERIMENTS_AUTHOR                  "Author" //admin
#define EXPERIMENTS_DATE                    "Date"    //date of creating
#define EXPERIMENTS_DEVICES                 "Devices" //TableDevicesOf(name of this Exp)
#define EXPERIMENTS_PARENT                  "Parent"  //PARENT-EXP OF THIS EXP
#define EXPERIMENTS_LINK                    "Link"   //LINK TO DOCS
#define EXPERIMENTS_CONF                    "Conf"   //CONF

#define DEVICES                             "TableDevices"
#define DEVICES_NAME                        "Name"
#define DEVICES_TYPE                        "Type"
#define DEVICES_DATE                        "Date"
#define DEVICES_DESCRIPTION                 "Description"
#define DEVICES_LINK                        "Link"   //LINK TO DOCS
#define DEVICES_CONF                        "Conf"

#define THEMES                              "TableThemes"
#define THEMES_NAME                         "Name"
#define THEMES_DESCRIPTION                  "Description"
#define THEMES_AUTHOR                       "Author" //admin
#define THEMES_CONF                         "Conf"

#define LOGS                                "TableLogs"
#define LOGS_NAME                           "Name"
#define LOGS_DATE                           "Date"
#define LOGS_TIME                           "Time"
#define LOGS_TYPE                           "Type"
#define LOGS_DESCRIPTION                    "Description"
#define LOGS_PROCESS                        "Process"
#define LOGS_PROBLEM                        "Problem"

#define EXPERIMENT                         "TableExperiment"
#define EXPERIMENT_NAME                    "Name"
#define EXPERIMENT_DESCRIPTION             "Description"
#define EXPERIMENT_AUTHOR                  "Author"
#define EXPERIMENT_DATE                    "Date"
#define EXPERIMENT_TIME                    "Time"
#define EXPERIMENT_DATA                    "Data"
#define EXPERIMENT_TABLE                   "NameTable"
#define EXPERIMENT_CONF                    "Conf"



class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();
    bool inserIntoTable(const QVariantList &data); //data = data
    bool inserIntoScientists(const QVariantList &data);
    bool inserIntoExperiments(const QVariantList &data);
    bool inserIntoDevices(const QVariantList &data);
    bool inserIntoThemes(const QVariantList &data);
    bool inserIntoLogs(const QVariantList &data);
    bool inserIntoDeviceTable(const QVariantList &data);
    bool inserIntoExperiment(const QVariantList &data);

    /* Creat tables
     * TableExperimentsOf
     * TableSelectedExperimentsOf
     * TableDevicesOf
     * TableExperimentNumber
    */

    bool createNewTableExperimentsOf(const QVariantList &table); //table = nameOfTable
    bool createNewTableSelectedExperimentsOf(const QVariantList &table); //table = nameOfTable
    bool createNewTableDevicesOf(const QVariantList &table); //table = nameOfTable
    bool createNewTableExperimentNumber(const QVariantList &table); //table = nameOfTable

    bool inserIntoNewTableExperimentsOf(const QVariantList &ndata); //ndata = nameOfTable+data
    bool inserIntoNewTableSelectedExperimentsOf(const QVariantList &ndata); //ndata = nameOfTable+data
    bool inserIntoNewTableDevicesOf(const QVariantList &ndata); //ndata = nameOfTable+data
    bool inserIntoNewTableExperimentNumber(const QVariantList &ndata); //ndata = nameOfTable+data

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();
    bool createAdmin();
};

#endif // DATABASE_H
