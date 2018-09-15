#include "authorization.h"
#include <QtWidgets>
#include <QMainWindow>

AuthorizationWindow::AuthorizationWindow(QWidget *parent):
    //QWidget(parent)
    QWidget(0, Qt::Window | Qt::FramelessWindowHint)

{
    /*tableView = new QTableView();

    //emit

    // Первым делом необходимо создать объект, который будет использоваться для работы с данными нашей БД
    //  и инициализировать подключение к базе данных

    db = new DataBase();
    db->connectToDataBase();

    //После чего производим наполнение таблицы базы данных
    //контентом, который будет отображаться в TableView
    // for(int i = 0; i < 20; i++){
    //    QVariantList data;
    //    int random = qrand(); // Получаем случайные целые числа для вставки а базу данных
    //    data.append(QDate::currentDate()); // Получаем текущую дату для вставки в БД
    //    data.append(QTime::currentTime()); // Получаем текущее время для вставки в БД
    //    // Подготавливаем полученное случайное число для вставки в БД
    //    data.append(random);
    //    // Подготавливаем сообщение для вставки в базу данных
    //    data.append("Получено сообщение от " + QString::number(random));
    //    // Вставляем данные в БД
    //    db->inserIntoTable(data);
    //}

    //Инициализируем модель для представления данных
     //с заданием названий колонок

    this->setupModel(SCIENTISTS,
                        QStringList()
                        << trUtf8("ID")
                        << trUtf8("Name")
                        << trUtf8("Surname")
                        << trUtf8("Login")
                        << trUtf8("Date")
                        << trUtf8("Parent")
                        << trUtf8("Pass")
                        << trUtf8("Email")
                        << trUtf8("Telegram")
                        << trUtf8("Phone")
                        << trUtf8("Experiments")
                        << trUtf8("Selected_experiments")
                        << trUtf8("BirthDay")
                        << trUtf8("Position")
                        << trUtf8("Theme")
               );

    //Инициализируем внешний вид таблицы с данными
    this->createUI();

    //newuserButton = new QPushButton(tr("New"));
    newuserButton = new QPushButton(tr("New user"));
    //edituserButton = new QPushButton(tr("Edit"));


    QGridLayout *grid = new QGridLayout;
    //grid->addWidget(groupTable());
    grid->addWidget(tableView, 0, 0, model->rowCount(), model->columnCount());
    grid->addWidget(newuserButton);
    //grid->addWidget(edituserButton);
    //grid->addWidget(groupTable(),  model->rowCount()+1, 0);
    setLayout(grid);

    //tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::);
    tableView->setColumnWidth(1,120);
    tableView->setColumnWidth(2,120);
    tableView->setColumnWidth(3,120);
    tableView->setColumnWidth(13,100);

*/

    setWindowTitle(tr("Authorization"));
        resize(550, 420);
        //this->setFont(QFont("Ubuntu"));//Roboto
        //this->setFont(QFont("Roboto",16,0,1));

    //connect(newuserButton, SIGNAL(released()), this, SLOT(slotNewUser()));
    //connect(edituserButton, SIGNAL(released()), this, SLOT(slotEditUser()));

}



/*AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}*/

/* Метод для инициализации модеи представления данных
 * */
void AuthorizationWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
}

void AuthorizationWindow::createUI()
{
    tableView->setModel(model);     // Устанавливаем модель на TableView
    tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    tableView->setColumnHidden(1, false);
    tableView->setColumnHidden(2, false);
    tableView->setColumnHidden(3, false);
    tableView->setColumnHidden(4, true);
    tableView->setColumnHidden(5, true);
    tableView->setColumnHidden(6, true);
    tableView->setColumnHidden(7, true);
    tableView->setColumnHidden(8, true);
    tableView->setColumnHidden(9, true);
    tableView->setColumnHidden(10, true);
    tableView->setColumnHidden(11, true);
    tableView->setColumnHidden(12, true);
    tableView->setColumnHidden(13, false);
    tableView->setColumnHidden(14, true);


    // Разрешаем выделение строк
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    tableView->resizeColumnsToContents();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setStretchLastSection(true);

    model->select(); // Делаем выборку данных из таблицы

    connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLogin(QModelIndex)));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditUser(QModelIndex)));

}

/* Метод для активации диалога добавления записей
 * */
/*void AuthorizationWindow::on_addDeviceButton_clicked()
{

    DialogAuth *addDialogAuth = new DialogAuth();
    connect(addDialogAuth, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    addDialogAuth->setWindowTitle(trUtf8("Добавить"));
    addDialogAuth->exec();
}*/

void AuthorizationWindow::slotNewUser()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */
    DialogReg *addDialogReg = new DialogReg();
    connect(addDialogReg, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDialogReg->setWindowTitle(trUtf8("Add User"));
    addDialogReg->exec();
    //showlogoWindow();
}


void AuthorizationWindow::slotEditUser(QModelIndex index)
{


    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */


    DialogReg *addDialogReg = new DialogReg(index.row());
    connect(addDialogReg, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDialogReg->setWindowTitle(trUtf8("Edit"));
    addDialogReg->exec();
}


void AuthorizationWindow::slotUpdateModels()
{
    model->select();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void AuthorizationWindow::slotLogin(QModelIndex index)
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogAuth *addDialogAuth = new DialogAuth(index.row());
    connect(addDialogAuth, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    connect(addDialogAuth, SIGNAL(accepted()),this, SIGNAL(showlogoWindow()));
    connect(addDialogAuth, SIGNAL(accepted()),this, SLOT(close()));
    connect(addDialogAuth, SIGNAL(sendLoginToAuth(QString)),this, SIGNAL(sendLogin(QString)));
    //emit openExperiments(index.row());

    /* Выполняем запуск диалогового окна
     * */
    addDialogAuth->setWindowTitle(trUtf8("Authorization"));
    addDialogAuth->exec();

    /*if(addDialogAuth->exec()){
        QMessageBox::information(this, trUtf8("Check"),
                                 trUtf8("True"));

    } else {
        QMessageBox::information(this, trUtf8("Check"),
                                 trUtf8("False"));
    }*/

}


QGroupBox *AuthorizationWindow::groupTable()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));

    //newuserButton = new QPushButton(tr("New user"));
    //edituserButton = new QPushButton(tr("Edit"));

    QVBoxLayout *vbox = new QVBoxLayout;
    //vbox->addWidget(newuserButton,0);
    //vbox->addWidget(edituserButton,1);
    vbox->setSpacing(0);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setMargin(0);
    groupBox->setLayout(vbox);

    return groupBox;
}
