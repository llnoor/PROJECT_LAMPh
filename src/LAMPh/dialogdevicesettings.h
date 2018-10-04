#ifndef DIALOGDEVICESETTINGS_H
#define DIALOGDEVICESETTINGS_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#define BUTTONNUM 5


QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class DialogDeviceSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeviceSettings(QString dllFile, int number, QWidget *parent = 0);
    ~DialogDeviceSettings();

Q_SIGNALS:
    void signalReady();

private Q_SLOTS:
    void save_data();
    void delete_data();
    bool setParameterButton(int row, int column);
    bool setParameterLine(int row);


private:
    QString dllFileQString;
    int number_of_device=0;

    QGroupBox                   *groupTableSettings();
	
    QLabel   *labelName;
	QLabel   *labelInfo; //text?
	
	QLabel   *labelRowName[BUTTONNUM];	
	QPushButton 	*pushButtonButtonName[BUTTONNUM][BUTTONNUM];
    QLineEdit   *lineEditRowData[BUTTONNUM];
	
	QLabel   *labelLineName[BUTTONNUM];
	QLineEdit  *lineEditParameterLine[BUTTONNUM];
	QPushButton	*pushButtonSend[BUTTONNUM];
	
private:
    void accept();

};

#endif // DIALOGDEVICESETTINGS_H
