#ifndef INITDB
#define INITDB

#include <QtSql>

void addScientist(QSqlQuery &q,
                  const QString &fullname,
                  const QString &login,
                  const QString &pass,
                  const QDate &date,
                  const QString &parent,
                  const QString &email,
                  const QString &telegram,
                  const QString &phone,
                  const QString &experiments,
                  const QString &theme,
                  const QDate &birthdate)
{
    q.addBindValue(fullname);
    q.addBindValue(login);
    q.addBindValue(pass);
    q.addBindValue(date);
    q.addBindValue(parent);
    q.addBindValue(email);
    q.addBindValue(telegram);
    q.addBindValue(phone);
    q.addBindValue(experiments);
    q.addBindValue(theme);
    q.addBindValue(birthdate);
    q.exec();
}

void addExperiment(QSqlQuery &q,
                  const QString &name,
                  const QString &description,
                  const QString &author,
                  const QDate &date,
                  const QString &parent,
                  const QString &pass,
                  const QString &devices,
                  const QString &docs,
                  int protection)
{
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(author);
    q.addBindValue(date);
    q.addBindValue(parent);
    q.addBindValue(pass);
    q.addBindValue(devices);
    q.addBindValue(docs);
    q.addBindValue(protection);
    q.exec();
}

void addDevice(QSqlQuery &q,
                  const QString &name,
                  const QString &description,
                  const QString &author,
                  const QDate &date,
                  const QString &parent,
                  const QString &pass,
                  const QString &docs,
                  int type)
{
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(author);
    q.addBindValue(date);
    q.addBindValue(parent);
    q.addBindValue(pass);
    q.addBindValue(docs);
    q.addBindValue(type);
    q.exec();
}

void addTheme(QSqlQuery &q,
                  const QString &name,
                  const QString &description,
                  const QString &author,
                  const QDate &date,
                  const QString &background,
                  const QString &translucent,
                  const QString &border, // ! it can be int
                  const QString &border_color,
                  const QString &font,
                  int font_size,
                  const QString &font_color,
                  const QString &mergin,
                  const QString &img)
{
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(author);
    q.addBindValue(date);
    q.addBindValue(background);
    q.addBindValue(translucent);
    q.addBindValue(border);
    q.addBindValue(border_color);
    q.addBindValue(font);
    q.addBindValue(font_size);
    q.addBindValue(font_color);
    q.addBindValue(mergin);
    q.addBindValue(img);
    q.exec();
}

void addLog(QSqlQuery &q,
                  const QString &name,
                  const QString &description,
                  const QString &process,
                  const QDate &date,
                  int type,
                  const QString &problem)
{
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(process);
    q.addBindValue(date);
    q.addBindValue(type);
    q.addBindValue(problem);
    q.exec();
}

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("lamp_db.sqlite");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("scientists", Qt::CaseInsensitive)
        && tables.contains("experiments", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;

    if (!q.exec(QLatin1String("create table scientists"
                              "(id integer primary key, "
                              "fullname varchar, "
                              "login varchar, "
                              "pass varchar, " //sha1
                              "date date, "
                              "parent varchar, " //who created this user
                              "email varchar, "
                              "telegram varchar, "
                              "phone varchar, "
                              "experiments varchar, "  //name of table "exp_of_admin"
                              "theme varchar, "
                              "birthdate date"
                              ")")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into scientists("
                                 "fullname, "
                                 "login, "
                                 "pass, "
                                 "date, "
                                 "parent, "
                                 "email, "
                                 "telegram, "
                                 "phone, "
                                 "experiments, "
                                 "theme, "
                                 "birthdate"
                                 ") values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table experiments"
                              "(id integer primary key, "
                              "name varchar, " //name of experiment
                              "description varchar, "  //varchar -> text (affinity)
                              "author varchar, " //login
                              "date date, "
                              "parent varchar, "    //if this exp was duplicated
                              "pass varchar, "  //for a private experiments //sha1
                              "devices varchar, " //name of table "devices_of_(this_exp)"
                              "docs varchar, " //link to docs
                              "protection int " //there are related child experiments (int because of protection level)
                              ")")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into experiments("
                                 "name, "
                                 "description, "
                                 "author, " //login
                                 "date , "
                                 "parent, "
                                 "pass, " //sha1
                                 "devices, "
                                 "docs, "
                                 "protection "
                                 ") values(?, ?, ?, ?, ?, ?, ?, ?, ?)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table devices"
                              "(id integer primary key, "
                              "name varchar, " //name of experiment
                              "description varchar, "  //varchar -> text (affinity)
                              "author varchar, "
                              "date date, "
                              "parent varchar, "    //if this device was duplicated
                              "pass varchar, "  //for a private device //sha1
                              "docs varchar, " //link to docs
                              "type int "
                              ")")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into devices("
                                 "name, "
                                 "description, "
                                 "author, "
                                 "date , "
                                 "parent, "
                                 "pass, "
                                 "docs, "
                                 "type "
                                 ") values(?, ?, ?, ?, ?, ?, ?, ?)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table themes"
                              "(id integer primary key, "
                              "name varchar, " //name of theme
                              "description varchar, "  //varchar -> text (affinity)
                              "author varchar, "
                              "date date, "
                              "background varchar, "
                              "translucent varchar, "
                              "border varchar, " //varchar = int (size) + varchar (type(solid)) ?
                              "border_color varchar, "
                              "font varchar, "
                              "font_size int, "
                              "font_color varchar, "
                              "mergin varchar, " //type?
                              "img varchar "
                              ")")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into themes("
                                 "name, "
                                 "description, "
                                 "author, "
                                 "date, "
                                 "background, "
                                 "translucent, "
                                 "border, "
                                 "border_color, "
                                 "font, "
                                 "font_size, "
                                 "font_color, "
                                 "mergin, "
                                 "img "
                                 ") values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table logs"
                              "(id integer primary key, "
                              "name varchar, " //name of log (or type)
                              "description varchar, "  //varchar -> text (affinity)
                              "process varchar, "
                              "date date, "
                              "type int, "
                              "problem varchar " //same as description
                              ")")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into logs("
                                 "name, "
                                 "description, "
                                 "process, "
                                 "date, "
                                 "type, "
                                 "problem "
                                 ") values(?, ?, ?, ?, ?, ?)")))
        return q.lastError();


    addScientist(q,
                 QLatin1String("Admin Admin"),
                 QLatin1String("admin"),
                 QLatin1String("pass54_admin"),
                 QDate(2017, 9, 23),
                 QLatin1String("admin"),
                 QLatin1String("admin@gmail.com"),
                 QLatin1String("admin_telegram"),
                 QLatin1String("+79271234567"),
                 QLatin1String("exp_of_admin"),
                 QLatin1String("default"),
                 QDate(2017, 10, 10));

    addExperiment(q,
                      QLatin1String("First exp"),
                      QLatin1String("measurement of susceptibility"),
                      QLatin1String("admin"),
                      QDate(2017, 9 ,23),
                      QLatin1String(""),
                      QLatin1String(""),
                      QLatin1String("devices_of_First_exp"), //use the ground (_) instead of a space ( )
                      QLatin1String("First exp.doc"),
                      0);

    addDevice(q,
                      QLatin1String("APPA"),
                      QLatin1String("APPA1"),
                      QLatin1String("admin"),
                      QDate(2017, 9, 23),
                      QLatin1String(""),
                      QLatin1String(""),
                      QLatin1String("APPA.doc"),
                      0); //add enum type {appa,keithley,lock-in}

    addTheme(q,
                      QLatin1String("default"),
                      QLatin1String("default theme"),
                      QLatin1String("admin"),
                      QDate(2017, 9, 23),
                      QLatin1String("default"), // background,
                      QLatin1String("default"), // translucent,
                      QLatin1String("default"), // border, // ! it can be int
                      QLatin1String("default"), // border_color,
                      QLatin1String("default"), // font,
                      0, // int font_size,
                      QLatin1String("default"), // font_color,
                      QLatin1String("default"),// mergin,
                      QLatin1String("default"));


    addLog(q,
                      QLatin1String("main"),
                      QLatin1String("main log"),
                      QLatin1String("default"),
                      QDate(2017, 9, 23),
                      0,
                      QLatin1String("default"));

            //addTheme
    //addLog


    return QSqlError();
}


#endif // INITDB
