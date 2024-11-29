#include "mainwindow.h"

Q_DECLARE_METATYPE(QSerialPortInfo)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), serial(new QSerialPort(this))
{
    setupUi(this); 
    connect(btn_refresh, SIGNAL(clicked()), this, SLOT(updatePorts()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    fillBaudCBList();
    updatePorts();
}

MainWindow::~MainWindow() {}

void MainWindow::fillPortCBList(QSerialPortInfo port)
{
    cb_port->addItem(port.portName());
}

void MainWindow::fillBaudCBList()
{
    QList<qint32> list = QSerialPortInfo::standardBaudRates();
    for (int i = 0; i < list.count(); i++) cb_baud->addItem(QString("%1Baud").arg(list[i]));
}

void MainWindow::openSerialPort()
{
    serial->setPortName(cb_port->currentText());
    serial->setBaudRate(cb_baud->currentText().toInt());
    serial->open(QIODevice::ReadWrite);
    changeOpenBtn();
}

void MainWindow::changeOpenBtn()
{
    if (serial->isOpen()){
        btn_openPort->setText("Port schließen");
        cb_port->setEnabled(false);
        cb_baud->setEnabled(false);
        btn_refresh->setEnabled(false);
    }
    else {
        btn_openPort->setText("Port öffnen");
        cb_port->setEnabled(true);
        cb_baud->setEnabled(true);
        btn_refresh->setEnabled(true);
    }
}

void MainWindow::updateTextField(QString text)
{
    textList = QString("%1%2").arg(text).arg(textList);
    plainTextEdit->setPlainText(textList);
}

void MainWindow::updatePorts()
{
    cb_port->clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for (int i = 0; i < list.count(); i++) fillPortCBList(list[i]);
}

void MainWindow::on_btn_openPort_clicked()
{
    if (serial->isOpen()) {
        serial->close();
        changeOpenBtn();
        textList.clear();
        return;
    }
    if (cb_port->count() <= 0) return;
    openSerialPort();
    plainTextEdit->clear();
}


void MainWindow::on_btn_send_clicked()
{
    serial->write(ln_text->text().toUtf8());
    ln_text->clear();
}

void MainWindow::readData()
{
    updateTextField(QString::fromUtf8(serial->readAll()));
}


void MainWindow::on_btn_clearLog_clicked()
{
    textList.clear();
    plainTextEdit->clear();
}

