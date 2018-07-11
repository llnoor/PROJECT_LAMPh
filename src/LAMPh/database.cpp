#include "database.h"
#include <QDateTime>
#include <QTime>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("C:/Git/git/PROJECT/src/LAMPh/db/" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createTable()){
            return false;
        } else {
            createAdmin();
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("C:/Git/git/PROJECT/src/LAMPh/db/" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы в базе данных
 * */
bool DataBase::createTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;

    //Table delete after tests
    if(query.exec( "CREATE TABLE " TABLE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_DATE      " DATE            NOT NULL,"
                            TABLE_TIME      " TIME            NOT NULL,"
                            TABLE_RANDOM    " INTEGER         NOT NULL,"
                            TABLE_MESSAGE   " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    }


    //SCIENTISTS
    if(query.exec( "CREATE TABLE " SCIENTISTS " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            SCIENTISTS_NAME                 " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_SURNAME              " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_LOGIN                " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_DATE                 " DATE            NOT NULL,"
                            SCIENTISTS_PARENT               " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_PASS                 " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_EMAIL                " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_TELEGRAM             " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_PHONE                " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_EXPERIMENTS          " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_SELECTED_EXPERIMENTS " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_BIRTHDAY             " DATE            NOT NULL,"
                            SCIENTISTS_POSITION             " VARCHAR(255)    NOT NULL,"
                            SCIENTISTS_THEME                " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << SCIENTISTS;
        qDebug() << query.lastError().text();
        return false;
    }

    //EXPERIMENTS
    if(query.exec( "CREATE TABLE " EXPERIMENTS " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            EXPERIMENTS_NAME                    " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_DESCRIPTION             " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_AUTHOR                  " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_DATE                    " DATE            NOT NULL,"
                            EXPERIMENTS_DEVICES                 " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_PARENT                  " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_LINK                    " VARCHAR(255)    NOT NULL,"
                            EXPERIMENTS_CONF                    " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << EXPERIMENTS;
        qDebug() << query.lastError().text();
        return false;
    }

    //DEVICES
    if(query.exec( "CREATE TABLE " DEVICES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            DEVICES_NAME                        " VARCHAR(255)    NOT NULL,"
                            DEVICES_TYPE                        " VARCHAR(255)    NOT NULL,"
                            DEVICES_DATE                        " DATE            NOT NULL,"
                            DEVICES_DESCRIPTION                 " VARCHAR(255)    NOT NULL,"
                            DEVICES_LINK                        " VARCHAR(255)    NOT NULL,"
                            DEVICES_CONF                        " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << DEVICES;
        qDebug() << query.lastError().text();
        return false;
    }

    //THEMES
    if(query.exec( "CREATE TABLE " THEMES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            THEMES_NAME                         " VARCHAR(255)    NOT NULL,"
                            THEMES_DESCRIPTION                  " VARCHAR(255)    NOT NULL,"
                            THEMES_AUTHOR                       " VARCHAR(255)    NOT NULL,"
                            THEMES_CONF                         " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << THEMES;
        qDebug() << query.lastError().text();
        return false;
    }

    //LOGS
    if(query.exec( "CREATE TABLE " LOGS " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            LOGS_NAME                       " VARCHAR(255)    NOT NULL,"
                            LOGS_DATE                       " DATE            NOT NULL,"
                            LOGS_TIME                       " TIME            NOT NULL,"
                            LOGS_TYPE                       " VARCHAR(255)    NOT NULL,"
                            LOGS_DESCRIPTION                " VARCHAR(255)    NOT NULL,"
                            LOGS_PROCESS                    " VARCHAR(255)    NOT NULL,"
                            LOGS_PROBLEM                    " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << LOGS;
        qDebug() << query.lastError().text();
        return false;
    }

    //DEVICE
    if(query.exec( "CREATE TABLE " DEVICE " ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                DEVICE_HOSTNAME  " VARCHAR(255)    NOT NULL,"
                                DEVICE_IP        " VARCHAR(16)     NOT NULL,"
                                DEVICE_MAC       " VARCHAR(18)     NOT NULL"
                            " )"
                        ))
    {
        //return true;
    } else {
        qDebug() << "DataBase: error of create " << DEVICE ;
        qDebug() << query.lastError().text();
        return false;
    }

    //EXPERIMENT
    if(query.exec( "CREATE TABLE " EXPERIMENT " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            EXPERIMENT_NAME                     " VARCHAR(255)    NOT NULL,"
                            EXPERIMENT_DESCRIPTION              " VARCHAR(255)    NOT NULL,"
                            EXPERIMENT_AUTHOR                   " VARCHAR(255)    NOT NULL,"
                            EXPERIMENT_DATE                     " DATE            NOT NULL,"
                            EXPERIMENT_TIME                     " TIME            NOT NULL,"
                            EXPERIMENT_DATA                     " VARCHAR(255)    NOT NULL,"
                            EXPERIMENT_TABLE                    " VARCHAR(255)    NOT NULL,"
                            EXPERIMENT_CONF                     " VARCHAR(255)    NOT NULL"
                        " )"
                    ))
    {
        return true;
    } else {
        qDebug() << "DataBase: error of create " << EXPERIMENT;
        qDebug() << query.lastError().text();
        return false;
    }

    return false;
}


bool DataBase::createAdmin()
{

    QByteArray pass_byte;
    pass_byte.append("lamp");
    QString pass_admin = QString(QCryptographicHash::hash(pass_byte,QCryptographicHash::Sha1).toHex());

    QVariantList dataScientists;
    dataScientists.append("Admin");
    dataScientists.append("Administrator");
    dataScientists.append("admin"); //write with the small letters
    dataScientists.append(QDate::currentDate());
    dataScientists.append("Noor");
    dataScientists.append(pass_admin); //sha1 "lamp"
    dataScientists.append("a@m.in");
    dataScientists.append("admin" + pass_admin);//username
    dataScientists.append("+79876543210");
    dataScientists.append("TableExperimentsOfadmin");
    dataScientists.append("TableSelectedExperimentsOfadmin");
    dataScientists.append(QDate::currentDate());
    dataScientists.append("admin");
    dataScientists.append("default");

    QVariantList dataExperiments;
    dataExperiments.append("First");
    dataExperiments.append("First Experiment (delete after first start)");
    dataExperiments.append("admin");
    dataExperiments.append(QDate::currentDate());
    dataExperiments.append("TableDevicesOfFirst");
    dataExperiments.append("Zero");
    dataExperiments.append("first");
    dataExperiments.append("default");

    QVariantList dataDevices;
    dataDevices.append("Universally");
    dataDevices.append("default");
    dataDevices.append(QDate::currentDate());
    dataDevices.append("Testing");
    dataDevices.append("Test");
    dataDevices.append("Test");

    QVariantList dataThemes;
    dataThemes.append("Default");
    dataThemes.append("Default");
    dataThemes.append("Admin");
    dataThemes.append("Default");

    QVariantList dataLogs;
    dataLogs.append("FirstConnection");
    dataLogs.append(QDate::currentDate());
    dataLogs.append(QTime::currentTime());
    dataLogs.append("0");
    dataLogs.append("SQL created");
    dataLogs.append("SQL");
    dataLogs.append("No problem");

    QVariantList dataExperiment;
    dataExperiment.append("First");
    dataExperiment.append("First Experiment (delete after first start)");
    dataExperiment.append("admin");
    dataExperiment.append(QDate::currentDate());
    dataExperiment.append(QTime::currentTime());
    dataExperiment.append("Test");
    dataExperiment.append("ExperimentNumberFirst");
    dataExperiment.append("default");

    /*if (!inserIntoScientists(dataScientists)
            || !inserIntoExperiments(dataExperiments)
            || !inserIntoLogs(dataLogs)
            ) // !!! check out !!!
    {
        return false;
    }else
    {
       return true;
    }*/

    inserIntoScientists(dataScientists);
    inserIntoExperiments(dataExperiments);
    inserIntoLogs(dataLogs);
    inserIntoDevices(dataDevices);
    inserIntoThemes(dataThemes);
    inserIntoExperiment(dataExperiment);

    if ( 1 and 1 and 1 )
    {
        return true;
    }else
    {
        return false;
    }
}



/* Creat tables in time of experiments
 * vsOf
 * TableSelectedExperimentsOf
 * TableDevicesOf
 * TableExperimentNumber
*/

bool DataBase::createNewTableExperimentsOf(const QVariantList &table)
{
    QSqlQuery query;

    query.prepare("CREATE TABLE :Table ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "Experiment_name VARCHAR(255) NOT NULL )");

    query.bindValue(":Table", table[0].toString());




    if(query.exec())
    {
        return true;
    } else {
        qDebug() << "DataBase: error of create " << table[0].toString();
        qDebug() << query.lastError().text();
        return false;
    }

}

bool DataBase::createNewTableSelectedExperimentsOf(const QVariantList &table)
{
    QSqlQuery query;

    query.prepare("CREATE TABLE :Table ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "Experiment_name VARCHAR(255) NOT NULL )");

    query.bindValue(":Table", table[0].toString());

    if(query.exec())
    {
        return true;
    } else {
        qDebug() << "DataBase: error of create " << table[0].toString();
        qDebug() << query.lastError().text();
        return false;
    }

}

bool DataBase::createNewTableDevicesOf(const QVariantList &table)
{
    QSqlQuery query;

    query.prepare("CREATE TABLE :Table ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "Device_name VARCHAR(255) NOT NULL )");

    query.bindValue(":Table", table[0].toString());

    if(query.exec())
    {
        return true;
    } else {
        qDebug() << "DataBase: error of create " << table[0].toString();
        qDebug() << query.lastError().text();
        return false;
    }

}

bool DataBase::createNewTableExperimentNumber(const QVariantList &table)
{
    QSqlQuery query;

    query.prepare("CREATE TABLE :Table ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "ExperimentNumber VARCHAR(255) NOT NULL )");

    query.bindValue(":Table", table[0].toString());

    if(query.exec())
    {
        return true;
    } else {
        qDebug() << "DataBase: error of create " << table[0].toString();
        qDebug() << query.lastError().text();
        return false;
    }

}

/* Метод для вставки записи в базу данных
 * */
bool DataBase::inserIntoTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " TABLE " ( " TABLE_DATE ", "
                                             TABLE_TIME ", "
                                             TABLE_RANDOM ", "
                                             TABLE_MESSAGE " ) "
                  "VALUES (:Date, :Time, :Random, :Message )");
    query.bindValue(":Date",        data[0].toDate());
    query.bindValue(":Time",        data[1].toTime());
    query.bindValue(":Random",      data[2].toInt());
    query.bindValue(":Message",     data[3].toString());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::inserIntoScientists(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " SCIENTISTS " ( " SCIENTISTS_NAME ", "
                                              SCIENTISTS_SURNAME ", "
                                              SCIENTISTS_LOGIN ", "
                                              SCIENTISTS_DATE ", "
                                              SCIENTISTS_PARENT ", "
                                              SCIENTISTS_PASS ", "
                                              SCIENTISTS_EMAIL ", "
                                              SCIENTISTS_TELEGRAM ", "
                                              SCIENTISTS_PHONE ", "
                                              SCIENTISTS_EXPERIMENTS ", "
                                              SCIENTISTS_SELECTED_EXPERIMENTS ", "
                                              SCIENTISTS_BIRTHDAY ", "
                                              SCIENTISTS_POSITION ", "
                                              SCIENTISTS_THEME " ) "
                  "VALUES (:Name, :Surname, :Login, :Date, :Parent, :Pass, :Email, :Telegram, :Phone, :Experiments, :Selected_experiments, :Birthday, :Position, :Theme )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Surname",                     data[1].toString());
    query.bindValue(":Login",                       data[2].toString());
    query.bindValue(":Date",                        data[3].toDate());
    query.bindValue(":Parent",                      data[4].toString());
    query.bindValue(":Pass",                        data[5].toString());
    query.bindValue(":Email",                       data[6].toString());
    query.bindValue(":Telegram",                    data[7].toString());
    query.bindValue(":Phone",                       data[8].toString());
    query.bindValue(":Experiments",                 data[9].toString());
    query.bindValue(":Selected_experiments",        data[10].toString());
    query.bindValue(":Birthday",                    data[11].toDate());
    query.bindValue(":Position",                    data[12].toString());
    query.bindValue(":Theme",                       data[13].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << SCIENTISTS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    //return false;
}


bool DataBase::inserIntoExperiments(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " EXPERIMENTS " ( " EXPERIMENTS_NAME ", "
                                              EXPERIMENTS_DESCRIPTION ", "
                                              EXPERIMENTS_AUTHOR ", "
                                              EXPERIMENTS_DATE ", "
                                              EXPERIMENTS_DEVICES ", "
                                              EXPERIMENTS_PARENT ", "
                                              EXPERIMENTS_LINK ", "
                                              EXPERIMENTS_CONF " ) "
                  "VALUES (:Name, :Description, :Author, :Date, :Devices, :Parent, :Link, :Conf )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Description",                 data[1].toString());
    query.bindValue(":Author",                      data[2].toString());
    query.bindValue(":Date",                        data[3].toDate());
    query.bindValue(":Devices",                     data[4].toString());
    query.bindValue(":Parent",                      data[5].toString());
    query.bindValue(":Link",                        data[6].toString());
    query.bindValue(":Conf",                        data[7].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << EXPERIMENTS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoDevices(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " DEVICES " ( " DEVICES_NAME ", "
                                              DEVICES_TYPE ", "
                                              DEVICES_DATE ", "
                                              DEVICES_DESCRIPTION ", "
                                              DEVICES_LINK ", "
                                              DEVICES_CONF " ) "
                  "VALUES (:Name, :Type, :Date, :Description, :Link, :Conf )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Type",                        data[1].toString());
    query.bindValue(":Date",                        data[2].toDate());
    query.bindValue(":Description",                 data[3].toString());
    query.bindValue(":Link",                        data[4].toString());
    query.bindValue(":Conf",                        data[5].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << DEVICES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoThemes(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " THEMES " ( " THEMES_NAME ", "
                                              THEMES_DESCRIPTION ", "
                                              THEMES_AUTHOR ", "
                                              THEMES_CONF " ) "
                  "VALUES (:Name, :Description, :Author, :Conf )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Description",                 data[1].toString());
    query.bindValue(":Author",                      data[2].toString());
    query.bindValue(":Conf",                        data[3].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << THEMES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoLogs(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " LOGS " ( " LOGS_NAME ", "
                                              LOGS_DATE ", "
                                              LOGS_TIME ", "
                                              LOGS_TYPE ", "
                                              LOGS_DESCRIPTION ", "
                                              LOGS_PROCESS ", "
                                              LOGS_PROBLEM " ) "
                  "VALUES (:Name, :Date, :Time, :Type, :Description, :Process, :Problem )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Date",                        data[1].toDate());
    query.bindValue(":Time",                        data[2].toTime());
    query.bindValue(":Type",                        data[3].toString());
    query.bindValue(":Description",                 data[4].toString());
    query.bindValue(":Process",                     data[5].toString());
    query.bindValue(":Problem",                     data[6].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << LOGS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoDeviceTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " DEVICE " ( " DEVICE_HOSTNAME ", "
                                              DEVICE_IP ", "
                                              DEVICE_MAC " ) "
                  "VALUES (:Hostname, :IP, :MAC )");
    query.bindValue(":Hostname",    data[0].toString());
    query.bindValue(":IP",          data[1].toString());
    query.bindValue(":MAC",         data[2].toString());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << DEVICE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoExperiment(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " EXPERIMENT " ( " EXPERIMENT_NAME ", "
                                              EXPERIMENT_DESCRIPTION ", "
                                              EXPERIMENT_AUTHOR ", "
                                              EXPERIMENT_DATE ", "
                                              EXPERIMENT_TIME ", "
                                              EXPERIMENT_DATA ", "
                                              EXPERIMENT_TABLE ", "
                                              EXPERIMENT_CONF " ) "
                  "VALUES (:Name, :Description, :Author, :Date, :Time, :Data, :Table, :Conf )");
    query.bindValue(":Name",                        data[0].toString());
    query.bindValue(":Description",                 data[1].toString());
    query.bindValue(":Author",                      data[2].toString());
    query.bindValue(":Date",                        data[3].toDate());
    query.bindValue(":Time",                        data[4].toTime());
    query.bindValue(":Data",                        data[5].toString());
    query.bindValue(":Table",                       data[6].toString());
    query.bindValue(":Conf",                        data[7].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << EXPERIMENT;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::inserIntoNewTableExperimentsOf(const QVariantList &ndata)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO :Table ( Experiment_name ) "
                  "VALUES (:Data )");

    query.bindValue(":Table", ndata[0].toString());
    query.bindValue(":Data", ndata[1].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << ndata[0].toString();
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoNewTableSelectedExperimentsOf(const QVariantList &ndata)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO :Table ( Experiment_name ) "
                  "VALUES (:Data )");

    query.bindValue(":Table", ndata[0].toString());
    query.bindValue(":Data", ndata[1].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << ndata[0].toString();
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoNewTableDevicesOf(const QVariantList &ndata)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO :Table ( Device_name ) "
                  "VALUES (:Data )");

    query.bindValue(":Table", ndata[0].toString());
    query.bindValue(":Data", ndata[1].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << ndata[0].toString();
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoNewTableExperimentNumber(const QVariantList &ndata)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO :Table ( ExperimentNumber ) "
                  "VALUES (:Data )");

    query.bindValue(":Table", ndata[0].toString());
    query.bindValue(":Data", ndata[1].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << ndata[0].toString();
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}










