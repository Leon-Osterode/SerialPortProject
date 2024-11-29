#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qserialportinfo.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include "QSerialPort"
#include "QSerialPortInfo"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSerialPort* serial;
    void fillPortCBList(QSerialPortInfo port);
    QString textList;
    void fillBaudCBList();
    void openSerialPort();
    void changeOpenBtn();
    void updateTextField(QString text);

private slots:
    void updatePorts();
    void on_btn_openPort_clicked();
    void on_btn_send_clicked();
    void readData();
    void on_btn_clearLog_clicked();
};

#endif // MAINWINDOW_H
