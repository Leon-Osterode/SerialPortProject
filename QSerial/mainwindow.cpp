#include "mainwindow.h"

Q_DECLARE_METATYPE(QSerialPortInfo)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), serial(new QSerialPort(this))
{
    setupUi(this);

    sp_char_timer->setValue(ROW_UPLOUD_TIMER);
    sp_row_timer->setValue(CHAR_UPLOUD_TIMER);

    connect(btn_refresh, SIGNAL(clicked()), this, SLOT(updatePorts()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    updateGui();
}

MainWindow::~MainWindow() {}

void MainWindow::updatePorts()
{
    cb_port->clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for (int i = 0; i < list.count(); i++) fillPortCBList(list[i]);
}

void MainWindow::fillPortCBList(QSerialPortInfo port)
{
    cb_port->addItem(port.portName());
}

void MainWindow::fillBaudCBList()
{
    QList<qint32> list = QSerialPortInfo::standardBaudRates();
    for (int i = 0; i < list.count(); i++) cb_baud->addItem(QString("%1Baud").arg(list[i]), list[i]);
}

void MainWindow::updateGui()
{
    bool state = serial->isOpen();
    cb_port->setEnabled(!state);
    cb_baud->setEnabled(!state);
    btn_refresh->setEnabled(!state);
    btn_openPort->setVisible(!state);
    btn_closePort->setVisible(state);

    if (!state){
        fillBaudCBList(); // Füllt die Baudratenliste
        updatePorts(); // Füllt die Portliste
    }
}

void MainWindow::on_btn_closePort_clicked()
{
    serial->close();
    updateGui();
}

void MainWindow::on_btn_openPort_clicked()
{
    if (cb_port->count() <= 0) return;
    openSerialPort();
    plainTextEdit->clear();
    updateGui();
}

void MainWindow::openSerialPort()
{
    serial->setPortName(cb_port->currentText());
    serial->setBaudRate(cb_baud->itemData(cb_baud->currentIndex()).toInt());
    if(!serial->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Fehler", QString("Fehler beim öffnen!!!\r\n%1").arg(serial->error()));
        serial->close();
    }
}

void MainWindow::showData()
{
    plainTextEdit->setPlainText(QString::fromUtf8(actionHex->isChecked() ? getHexDump() : data));
}

void MainWindow::on_actionHex_triggered()
{
    showData();
}

QByteArray MainWindow::getHexDump()
{
    QByteArray hexdump;
    for (int i = 0; i < data.size(); i += HEXDUMP_COL_COUNT) {
        hexdump.append(QString("%1: ").arg(i, MIN_ADDR_WIDTH, 16, ADDR_FILL_CHAR).toUtf8());
        hexdump.append(data.mid(i, HEXDUMP_COL_COUNT).toHex(' ').toUpper());
        hexdump.append("\n");
    }
    return hexdump;
}

void MainWindow::on_btn_send_clicked()
{
    if (!serial->isOpen()) return;
    serial->write(ln_text->text().toUtf8());
}

void MainWindow::readData()
{
    if (!serial->isOpen()) return;
    data.append(serial->readAll());
    showData();
}

void MainWindow::on_btn_clearLog_clicked()
{
    data.clear();
    showData();
}

// 'ed' zum setzen der Daten
// STRG + j = 0a \n | minicom

void MainWindow::on_btn_searchFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, "Datei Wählen", ln_filePath->text());
    if (!fn.isEmpty()) ln_filePath->setText(fn);
    else return;

    dataFile.clear();

    QFile f(fn);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        dataFile = f.readAll();
        f.close();
    }
}

void MainWindow::on_btn_UploafFile_clicked()
{
    if (serial->isOpen()) sendChar();
}

void MainWindow::sendChar()
{
    if (dataFile.isEmpty()) return;
    serial->write(dataFile.mid(0, 1));
    dataFile.remove(0, 1);
    int interval = (dataFile.mid(0, 1) == "\n") ? sp_row_timer->value() : sp_char_timer->value();
    QTimer::singleShot(interval, this, SLOT(sendChar()));
}
