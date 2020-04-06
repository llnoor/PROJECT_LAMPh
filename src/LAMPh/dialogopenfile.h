#ifndef DIALOGOPENFILE_H
#define DIALOGOPENFILE_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class DialogOpenFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpenFile(QString txtFile, int number, QWidget *parent = 0);

Q_SIGNALS:
    //void send_qMap(int, QMap <int, QStringList>);
    void send_qVectorData(int, QVector <QStringList>);
    void send_x_result(float);
    void send_all_results(float,int);
    void appendPointXY(int);
    void sendAll(float,float,int);


private Q_SLOTS:
    void save_data();
    void delete_data();
    void hide_empty(int);
    void open_file();
    void setCheckBox(int);

private:
    QString txtFileQString;
    int number_of_device=0;

    QGroupBox                   *groupTableSettings();
	
    QLabel   *labelName;
    QLabel   *labelInfo;

    QLabel   *labelFileStr;


    QLabel  *labelD;
    QLabel  *labelX;
    QLabel  *labelY;

    //QString *textData[20];
    QLabel  *labelData[30];
    QCheckBox *checkBox_F_X[30];
    QCheckBox *checkBox_F_Y[30];

    QLabel  *labelImportTo;
    QPushButton *button_plot_F;
    QComboBox *comboBox_Device;
    QProgressBar *progressBar;

    QString filePath;
    int numberOfX;
    int numberOfY;
    //QMap <int, QStringList> qMapDataOpenFile;
    QVector <QStringList> qVectorData;


};

#endif // DIALOGOPENFILE_H
