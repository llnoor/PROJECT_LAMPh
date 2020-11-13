#include <QtWidgets>
#include <QSql>

#include "table.h"

Table::Table(QWidget *parent) :
    QMainWindow(parent)
{

    tableView  = new QTableView();



    QGridLayout *grid = new QGridLayout;
    //grid->addWidget(groupTable(), 0, 0);
    grid->addWidget(tableView, 0, 0);

    setLayout(grid);

    setWindowTitle(tr("Table"));
    resize(580, 420);

    this->setFont(QFont("Ubuntu",14));


    //ui->setupUi(this);

    /* Первым делом необходимо создать объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();

    /* После чего производим наполнение таблицы базы данных
     * контентом, который будет отображаться в TableView
     * */
    for(int i = 0; i < 4; i++){
        QVariantList data;
        int random = qrand(); // Получаем случайные целые числа для вставки а базу данных
        data.append(QDate::currentDate()); // Получаем текущую дату для вставки в БД
        data.append(QTime::currentTime()); // Получаем текущее время для вставки в БД
        // Подготавливаем полученное случайное число для вставки в БД
        data.append(random);
        // Подготавливаем сообщение для вставки в базу данных
        data.append("Получено сообщение от " + QString::number(random));
        // Вставляем данные в БД
        db->inserIntoTable(data);
    }

    /* Инициализируем модель для представления данных
     * с заданием названий колонок
     * */
    this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Дата")
                                   << trUtf8("Время")
                                   << trUtf8("Рандомное число")
                                   << trUtf8("Сообщение")
               );

    /* Инициализируем внешний вид таблицы с данными
     * */
    this->createUI();








}

Table::~Table()
{
    //delete ui;
}

/* Метод для инициализации модеи представления данных
 * */
void Table::setupModel(const QString &tableName, const QStringList &headers)
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

void Table::createUI()
{
    tableView->setModel(model);     // Устанавливаем модель на TableView
    tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    tableView->resizeColumnsToContents();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setStretchLastSection(true);

    model->select(); // Делаем выборку данных из таблицы
}


QGroupBox *Table::groupTable()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
        QGridLayout * gridLayout = new QGridLayout();
        //tableView  = new QTableView();

        //gridLayout->addWidget(tableView,0,0);

        groupBox->setLayout(gridLayout);
    return groupBox;
}



