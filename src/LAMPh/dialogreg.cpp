#include "dialogreg.h"
#include <QtWidgets>
#include <QMainWindow>

DialogReg::DialogReg(int row, QWidget *parent) :
    QDialog(parent)
    //QDialog(0, Qt::Window | Qt::FramelessWindowHint)
{
    //ui->setupUi(this);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupTableAuth(), 0, 0);
    setLayout(grid);


     /* Метода для инициализации модели,
      * из которой будут транслироваться данные
      * */
     setupModel();

     /* Если строка не задана, то есть равна -1,
      * тогда диалог работает по принципу создания новой записи.
      * А именно, в модель вставляется новая строка и работа ведётся с ней.
      * */
     if(row == -1){
         model->insertRow(model->rowCount(QModelIndex()));
         mapper->toLast();
     /* В противном случае диалог настраивается на заданную запись
      * */
     } else {
         mapper->setCurrentModelIndex(model->index(row,0));
     }

     //createUI();
 }

 DialogReg::~DialogReg()
 {
     //delete ui;
 }

 /* Метод настройки модели данных и mapper
  * */
 void DialogReg::setupModel()
 {
     /* Инициализируем модель и делаем выборку из неё
      * */
     model = new QSqlTableModel(this);
     model->setTable(SCIENTISTS);
     model->setEditStrategy(QSqlTableModel::OnManualSubmit);
     model->select();

     /* Инициализируем mapper и привязываем
      * поля данных к объектам LineEdit
      * */
     mapper = new QDataWidgetMapper();
     mapper->setModel(model);


     mapper->addMapping(scientistName, 1);
     mapper->addMapping(scientistSurname, 2);
     mapper->addMapping(scientistLogin, 3);
     mapper->addMapping(scientistDate, 4);
     mapper->addMapping(scientistParent, 5);
     mapper->addMapping(scientistPass, 6);
     mapper->addMapping(scientistEmail, 7);
     mapper->addMapping(scientistTelegram,8);
     mapper->addMapping(scientistPhone, 9);
     mapper->addMapping(scientistExperiments, 10);
     mapper->addMapping(scientistSelected_experiments, 11);
     mapper->addMapping(scientistBirthday, 12);
     mapper->addMapping(scientistPosition,13);
     mapper->addMapping(scientistTheme, 14);



                 /*scientistName
                 scientistSurname
                 scientistLogin
                 scientistDate
                 scientistParent
                 scientistPass
                 scientistEmail
                 scientistTelegram
                 scientistPhone
                 scientistExperiments
                 scientistSelected_experiments
                 scientistBirthday
                 scientistPosition
                 scientistTheme*/



     /* Ручное подтверждение изменения данных
      * через mapper
      * */
     mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

     /* Подключаем коннекты от кнопок пролистывания
      * к прилистыванию модели данных в mapper
      * */
     connect(previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
     connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
     /* При изменении индекса в mapper изменяем состояние кнопок
      * */
     connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
     connect(saveButton,SIGNAL(clicked()),this, SLOT(save_data()));
     connect(deleteButton,SIGNAL(clicked()),this, SLOT(delete_data()));
 }

 /* Метод для установки валидатора на поле ввода IP и MAC адресов
  * */
 /*void DialogReg::createUI()
 {
     QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
     QRegExp ipRegex ("^" + ipRange
                      + "\\." + ipRange
                      + "\\." + ipRange
                      + "\\." + ipRange + "$");
     QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
     //IPAddressLineEdit->setValidator(ipValidator);

     QString macRange = "(?:[0-9A-Fa-f][0-9A-Fa-f])";
     QRegExp macRegex ("^" + macRange
                       + "\\:" + macRange
                       + "\\:" + macRange
                       + "\\:" + macRange
                       + "\\:" + macRange
                       + "\\:" + macRange + "$");
     QRegExpValidator *macValidator = new QRegExpValidator(macRegex, this);
     //MACLineEdit->setValidator(macValidator);
 }*/

 void DialogReg::save_data()
 {
     /* SQL-запрос для проверки существования записи
      * с такими же учетными данными.
      * Если запись не существует или находится лишь индекс
      * редактируемой в данный момент записи,
      * то диалог позволяет вставку записи в таблицу данных
      * */
     QSqlQuery query;
     QString str = QString("SELECT EXISTS (SELECT " SCIENTISTS_LOGIN " FROM " SCIENTISTS
                           " WHERE ( " SCIENTISTS_LOGIN " = '%1' )"
                           " AND id NOT LIKE '%2' )")
             .arg(scientistLogin->text(),
                  //IPAddressLineEdit->text(),
                  model->data(model->index(mapper->currentIndex(),0), Qt::DisplayRole).toString());

     query.prepare(str);
     query.exec();
     query.next();

     /* Если запись существует, то диалог вызывает
      * предупредительное сообщение
      * */
     if(query.value(0) != 0){
         QMessageBox::information(this, trUtf8("Ошибка"),
                                  trUtf8("уже существует"));
     /* В противном случае производится вставка новых данных в таблицу
      * и диалог завершается с передачей сигнала для обновления
      * таблицы в главном окне
      * */
     } else {
         mapper->submit();
         model->submitAll();
         Q_EMIT signalReady();
         this->close();
     }
 }


 void DialogReg::delete_data()
 {


     QMessageBox* pmbx =
                        new QMessageBox("MessageBox",
                        "Do you want to delete this data?",
                        QMessageBox::Information,
                        QMessageBox::Yes,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
     int n = pmbx->exec();
     delete pmbx;
     if (n == QMessageBox::Yes)
     {
         /* SQL-запрос для проверки существования записи
          * с такими же учетными данными.
          * Если запись не существует или находится лишь индекс
          * редактируемой в данный момент записи,
          * то диалог позволяет вставку записи в таблицу данных
          * */
         QSqlQuery query;
         QString str = QString("DELETE FROM " SCIENTISTS
                               " WHERE ( " SCIENTISTS_LOGIN " = '%1' )")
                 .arg(scientistLogin->text());

         query.prepare(str);
         query.exec();
         query.next();

         mapper->submit();
         model->submitAll();
         Q_EMIT signalReady();
         this->close();
     }else
     {
         this->close();
     }



 }

 void DialogReg::accept()
 {

 }

 /* Метод изменения состояния активности кнопок пролистывания
  * */
 void DialogReg::updateButtons(int row)
 {
     /* В том случае, если мы достигаем одного из крайних (самый первый или
      * самый последний) из индексов в таблице данных,
      * то мы изменяем состояние соответствующей кнопки на
      * состояние неактивна
      * */
     previousButton->setEnabled(row > 0);
     nextButton->setEnabled(row < model->rowCount() - 1);
 }


 QGroupBox *DialogReg::groupTableAuth()
 {
     QGroupBox *groupBox = new QGroupBox(tr(""));
     groupBox->setStyleSheet("border: 0px solid white");

     previousButton = new QPushButton(tr("Previous"));
     nextButton = new QPushButton(tr("Next"));
     saveButton = new QPushButton(tr("Save"));
     newButton = new QPushButton(tr("New"));
     deleteButton = new QPushButton(tr("Delete"));


    labelScientistName = new QLabel(tr("Name"));
    labelScientistSurname = new QLabel(tr("Surname"));
    labelScientistLogin = new QLabel(tr("Login"));
    labelScientistDate = new QLabel(tr("Date"));
    labelScientistParent = new QLabel(tr("Parent"));
    labelScientistPass = new QLabel(tr("Pass"));
    labelScientistEmail = new QLabel(tr("Email"));
    labelScientistTelegram = new QLabel(tr("Telegram"));
    labelScientistPhone = new QLabel(tr("Phone"));
    labelScientistExperiments = new QLabel(tr("Experiments"));
    labelScientistSelected_experiments = new QLabel(tr("Selected_experiments"));
    labelScientistBirthday = new QLabel(tr("Birthday"));
    labelScientistPosition = new QLabel(tr("Position"));
    labelScientistTheme = new QLabel(tr("Theme"));
    labelEmpty  = new QLabel(tr(""));

    scientistName = new QLineEdit();
    scientistSurname = new QLineEdit();
    scientistLogin = new QLineEdit();
    scientistDate = new QLineEdit();
    scientistParent = new QLineEdit();
    scientistPass = new QLineEdit();
    scientistEmail = new QLineEdit();
    scientistTelegram = new QLineEdit();
    scientistPhone = new QLineEdit();
    scientistExperiments = new QLineEdit();
    scientistSelected_experiments = new QLineEdit();
    scientistBirthday = new QLineEdit();
    scientistPosition = new QLineEdit();
    scientistTheme = new QLineEdit();

    scientistPass->setEchoMode(QLineEdit::Password);


     /*label_1 = new QLabel(tr(" LAMP "));
     label_1->setFont(QFont("Russo One",100));
     label_1->setStyleSheet("color: gray;");

     label_2 = new QLabel(tr("         Laboratory for Advanced Materials Physics\n "));
     label_2->setFont(QFont("Russo One",13));
     label_2->setStyleSheet("color: gray;");*/

    QGridLayout * gridLayout = new QGridLayout();

    gridLayout->addWidget(labelScientistName,0,0);
    gridLayout->addWidget(labelScientistSurname,1,0);
    gridLayout->addWidget(labelScientistLogin,2,0);
    gridLayout->addWidget(labelScientistDate,3,0);
    gridLayout->addWidget(labelScientistParent,4,0);
    gridLayout->addWidget(labelScientistPass,5,0);
    gridLayout->addWidget(labelScientistEmail,6,0);
    gridLayout->addWidget(labelScientistTelegram,7,0);
    gridLayout->addWidget(labelScientistPhone,8,0);
    //gridLayout->addWidget(labelScientistExperiments,11,0);
    //gridLayout->addWidget(labelScientistSelected_experiments,12,0);
    gridLayout->addWidget(labelScientistBirthday,9,0);
    gridLayout->addWidget(labelScientistPosition,10,0);
    //gridLayout->addWidget(labelScientistTheme,13,0);

    gridLayout->addWidget(scientistName,0,1);
    gridLayout->addWidget(scientistSurname,1,1);
    gridLayout->addWidget(scientistLogin,2,1);
    gridLayout->addWidget(scientistDate,3,1);
    gridLayout->addWidget(scientistParent,4,1);
    gridLayout->addWidget(scientistPass,5,1);
    gridLayout->addWidget(scientistEmail,6,1);
    gridLayout->addWidget(scientistTelegram,7,1);
    gridLayout->addWidget(scientistPhone,8,1);
    //gridLayout->addWidget(scientistExperiments,11,1);
    //gridLayout->addWidget(scientistSelected_experiments,12,1);
    gridLayout->addWidget(scientistBirthday,9,1);
    gridLayout->addWidget(scientistPosition,10,1);
    //gridLayout->addWidget(scientistTheme,13,1);

    gridLayout->addWidget(labelEmpty,14,0);

    gridLayout->addWidget(saveButton,15,0);
    gridLayout->addWidget(deleteButton,15,1);

    groupBox->setLayout(gridLayout);

     return groupBox;
 }
