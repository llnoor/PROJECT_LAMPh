#ifndef DIALOGAUTH_H
#define DIALOGAUTH_H

#include <QMainWindow>
#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include "database.h"
#include <QTableView>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class DialogAuth : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuth(int row = -1, QWidget *parent = 0);
    ~DialogAuth();

Q_SIGNALS:
    void signalReady();
    void sendLoginToAuth(QString loginQString);

private Q_SLOTS:
    void login();
    void save_data();
    void delete_data();
    void updateButtons(int row);

private:
    QGroupBox                   *groupTableAuth();
    QTableView                  *tableViewEditAuth;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    QPushButton                 *previousButton;
    QPushButton                 *nextButton;
    QPushButton                 *saveButton;
    QPushButton                 *newButton;
    QPushButton                 *deleteButton;
    QPushButton                 *loginButton;

    QLabel   *labelScientistName;
    QLabel   *labelScientistSurname;
    QLabel   *labelScientistLogin;
    QLabel   *labelScientistDate;
    QLabel   *labelScientistParent;
    QLabel   *labelScientistPass;
    QLabel   *labelScientistEmail;
    QLabel   *labelScientistTelegram;
    QLabel   *labelScientistPhone;
    QLabel   *labelScientistExperiments;
    QLabel   *labelScientistSelected_experiments;
    QLabel   *labelScientistBirthday;
    QLabel   *labelScientistPosition;
    QLabel   *labelScientistTheme;

    QLabel   *labelEmpty;

    QLineEdit   *scientistName;
    QLineEdit   *scientistSurname;
    QLineEdit   *scientistLogin;
    QLineEdit   *scientistDate;
    QLineEdit   *scientistParent;
    QLineEdit   *scientistPass;
    QLineEdit   *scientistEmail;
    QLineEdit   *scientistTelegram;
    QLineEdit   *scientistPhone;
    QLineEdit   *scientistExperiments;
    QLineEdit   *scientistSelected_experiments;
    QLineEdit   *scientistBirthday;
    QLineEdit   *scientistPosition;
    QLineEdit   *scientistTheme;

    QLineEdit   *checkScientistPass;



private:
    void setupModel();
    //void createUI();
    void accept();

};

#endif // DIALOGAUTH_H
