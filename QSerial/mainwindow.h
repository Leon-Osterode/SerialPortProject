#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qserialportinfo.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QtWidgets>
#include "QSerialPort"
#include "QSerialPortInfo"
#include <QTimer>

#define HEXDUMP_COL_COUNT 16
#define MIN_ADDR_WIDTH 4
#define ADDR_FILL_CHAR QChar('0')
#define ROW_UPLOUD_TIMER 1000
#define CHAR_UPLOUD_TIMER 500

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSerialPort* serial;
    QByteArray data;
    QByteArray dataFile;

    void fillPortCBList(QSerialPortInfo port);
    void fillBaudCBList();

    void updateGui();

    void openSerialPort();
    void showData();
    QByteArray getHexDump();

private slots:
    void updatePorts();
    void readData();

    void sendChar();

    void on_btn_openPort_clicked();
    void on_btn_send_clicked();
    void on_btn_clearLog_clicked();
    void on_btn_closePort_clicked();
    void on_actionHex_triggered();
    void on_btn_UploafFile_clicked();
    void on_btn_searchFile_clicked();
};

#endif // MAINWINDOW_H
