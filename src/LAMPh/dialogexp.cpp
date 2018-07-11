#include "dialogexp.h"
#include <QtWidgets>
#include <QMainWindow>


DialogExp::DialogExp(int row, QWidget *parent) :
  QDialog(parent)
  //QDialog(0, Qt::Window | Qt::FramelessWindowHint)
{
  //ui->setupUi(this);

  QGridLayout *grid = new QGridLayout;
  grid->addWidget(groupTableExp(), 0, 0);
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

DialogExp::~DialogExp()
{
   //delete ui;
}

/* Метод настройки модели данных и mapper
* */
void DialogExp::setupModel()
{
   /* Инициализируем модель и делаем выборку из неё
    * */
   model = new QSqlTableModel(this);
   model->setTable(EXPERIMENTS);
   model->setEditStrategy(QSqlTableModel::OnManualSubmit);

   model->setFilter("1=1 ORDER BY CASE "EXPERIMENTS_AUTHOR" WHEN 'user2' THEN '1' ELSE '0' END DESC");

   model->select();

   /* Инициализируем mapper и привязываем
    * поля данных к объектам LineEdit
    * */
   mapper = new QDataWidgetMapper();
   mapper->setModel(model);


   mapper->addMapping(experimentName, 1);
   mapper->addMapping(experimentDescription, 2);
   mapper->addMapping(experimentAuthor, 3);
   mapper->addMapping(experimentDate, 4);
   mapper->addMapping(experimentDevices, 5);
   mapper->addMapping(experimentParent, 6);
   mapper->addMapping(experimentLink, 7);
   mapper->addMapping(experimentConf, 8);


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
   connect(startExpButton,SIGNAL(clicked()),this, SLOT(startExp()));
}

void DialogExp::startExp()
{
   if (1 /*scientistPass->text() == checkScientistPass->text()*/)
   {
    //QMessageBox::information(this, trUtf8("Pass"),trUtf8("Success"));
    Q_EMIT accepted();
    this->close();
   }else
   {
    QMessageBox::information(this, trUtf8("Pass"),trUtf8("Try again"));
   }

}


void DialogExp::save_data()
{
   /* SQL-запрос для проверки существования записи
    * с такими же учетными данными.
    * Если запись не существует или находится лишь индекс
    * редактируемой в данный момент записи,
    * то диалог позволяет вставку записи в таблицу данных
    * */
   QSqlQuery query;
   QString str = QString("SELECT EXISTS (SELECT " EXPERIMENTS_NAME " FROM " EXPERIMENTS
                         " WHERE ( " EXPERIMENTS_NAME " = '%1' )"
                         " AND id NOT LIKE '%2' )")
           .arg(experimentName->text(),
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


void DialogExp::delete_data()
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
       QString str = QString("DELETE FROM " EXPERIMENTS
                             " WHERE ( " EXPERIMENTS_NAME " = '%1' )")
               .arg(experimentName->text());

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

void DialogExp::accept()
{

}

/* Метод изменения состояния активности кнопок пролистывания
* */
void DialogExp::updateButtons(int row)
{
   /* В том случае, если мы достигаем одного из крайних (самый первый или
    * самый последний) из индексов в таблице данных,
    * то мы изменяем состояние соответствующей кнопки на
    * состояние неактивна
    * */
   previousButton->setEnabled(row > 0);
   nextButton->setEnabled(row < model->rowCount() - 1);
}


QGroupBox *DialogExp::groupTableExp()
{
   QGroupBox *groupBox = new QGroupBox(tr(""));
   groupBox->setStyleSheet("border: 0px solid white");

   previousButton = new QPushButton(tr("Previous"));
   nextButton = new QPushButton(tr("Next"));
   saveButton = new QPushButton(tr("Save"));
   newButton = new QPushButton(tr("New"));
   deleteButton = new QPushButton(tr("Delete"));
   startExpButton = new QPushButton(tr("Start Experiment"));


   labelExperimentName = new QLabel(tr("Name"));
   labelExperimentDescription = new QLabel(tr("Description"));
   labelExperimentAuthor = new QLabel(tr("Author"));
   labelExperimentDate = new QLabel(tr("Date"));
   labelExperimentDevices = new QLabel(tr("Devices"));
   labelExperimentParent = new QLabel(tr("Parent"));
   labelExperimentLink = new QLabel(tr("Link"));
   labelExperimentConf = new QLabel(tr("Conf"));

  labelEmpty  = new QLabel(tr(""));

  experimentName = new QLineEdit();
  experimentDescription = new QLineEdit();
  experimentAuthor = new QLineEdit();
  experimentDate = new QLineEdit();
  experimentDevices = new QLineEdit();
  experimentParent = new QLineEdit();
  experimentLink = new QLineEdit();
  experimentConf = new QLineEdit();


   /*label_1 = new QLabel(tr(" LAMP "));
   label_1->setFont(QFont("Russo One",100));
   label_1->setStyleSheet("color: gray;");

   label_2 = new QLabel(tr("         Laboratory for Advanced Materials Physics\n "));
   label_2->setFont(QFont("Russo One",13));
   label_2->setStyleSheet("color: gray;");*/

  QGridLayout * gridLayout = new QGridLayout();

  gridLayout->addWidget(labelExperimentName,0,0);
  gridLayout->addWidget(labelExperimentDescription,1,0);
  gridLayout->addWidget(labelExperimentAuthor,2,0);
  gridLayout->addWidget(labelExperimentDate,3,0);
  gridLayout->addWidget(labelExperimentDevices,4,0);
  gridLayout->addWidget(labelExperimentParent,5,0);
  gridLayout->addWidget(labelExperimentLink,6,0);
  gridLayout->addWidget(labelExperimentConf,7,0);

  gridLayout->addWidget(labelEmpty,8,0);

  gridLayout->addWidget(experimentName,0,1);
  gridLayout->addWidget(experimentDescription,1,1);
  gridLayout->addWidget(experimentAuthor,2,1);
  gridLayout->addWidget(experimentDate,3,1);
  gridLayout->addWidget(experimentDevices,4,1);
  gridLayout->addWidget(experimentParent,5,1);
  gridLayout->addWidget(experimentLink,6,1);
  gridLayout->addWidget(experimentConf,7,1);

  gridLayout->addWidget(labelEmpty,9,0);


  gridLayout->addWidget(saveButton,11,0);
  gridLayout->addWidget(deleteButton,12,0);
  gridLayout->addWidget(startExpButton,13,0);

  groupBox->setLayout(gridLayout);

   return groupBox;
}
