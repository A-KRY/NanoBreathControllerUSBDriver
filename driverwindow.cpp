#include "driverwindow.h"
#include "./ui_driverwindow.h"

#define MAX_SYSEX_BUFFER 65535
#define DEFAULT_PORT_NAME "(Not Connected)"
#define INFO(TITLE, TEXT) QMessageBox::information(this, (TITLE), (TEXT), QMessageBox::Ok);

DriverWindow::DriverWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DriverWindow)
    , midiPort(virtualMIDICreatePortEx2(L"Nano Breath Controller", nullptr, 0, MAX_SYSEX_BUFFER, TE_VM_FLAGS_INSTANTIATE_BOTH))
    , usbPortUpdateTimer(new QTimer(this))
    , midiChannel(0)
    , serialPort(new QSerialPort())
    , EMA(new ExponentialMovingAverage(0.5))
{
    ui->setupUi(this);

    // Available USB port update
    // 可用 USB 端口更新
    usbPortUpdateTimer->setInterval(5000);
    connect(usbPortUpdateTimer, &QTimer::timeout, this, &DriverWindow::updateAvailableUsbPort);
    this->updateAvailableUsbPort();
    usbPortUpdateTimer->start();

    // midiChannelComboBox initialization
    // 初始化 midiChannelComboBox
    for (auto i = 1; i <= 16; ++i) {
        ui->midiChannelComboBox->addItem(QString::number(i));
    }

    // availableUsbPortComboBox Callbacks
    // availableUsbPortComboBox 回调函数
    connect(ui->availableUsbPortComboBox, &QComboBox::activated,
            this, &DriverWindow::availableUsbPortComboBox_onActivated);

    // midiChannelComboBox Callbacks
    // midiChannelComboBox 回调函数
    connect(ui->midiChannelComboBox, &QComboBox::activated,
            this, &DriverWindow::midiChannelComboBox_onActivated);

    // serialPort 回调函数
    // serialPort callbacks
    connect(serialPort, &QSerialPort::readyRead, this, &DriverWindow::serialPort_onDataReceived);
}

DriverWindow::~DriverWindow()
{
    if (midiPort) {
        virtualMIDIClosePort(midiPort);
    }
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;
}

void DriverWindow::serialPort_onDataReceived() {
    auto data = serialPort->read(1);
    if (!data.isEmpty()) {
        BYTE midiMessage[3] = {static_cast<BYTE>(0xB0|midiChannel), 11, static_cast<BYTE>(EMA->filter(data[0]))};
        if (midiPort) {
            virtualMIDISendData(midiPort, midiMessage, sizeof(midiMessage));
        }
    }
}

void DriverWindow::availableUsbPortComboBox_onActivated(int index) {
    // If port not changed, do nothing and return
    // 如果串口未变，直接返回
    if (serialPort->portName() == ui->availableUsbPortComboBox->currentText()) {
        return;
    }

    // Close serial port if it's opened
    // 若串口已打开，则关闭
    if (serialPort->portName() != ui->availableUsbPortComboBox->currentText()) {
        if (serialPort->isOpen()) {
            serialPort->close();
            serialPort->setPortName(DEFAULT_PORT_NAME);
        }
    }

    // Open serial port only when selected item isn't "(Not Connected)"
    // 只有选中项不为 "(Not Connected)" 时才打开端口
    if (index != 0) {
        // Set port name and open port
        // 设置端口名并打开端口
        serialPort->setPortName(ui->availableUsbPortComboBox->currentText());

        if (serialPort->open(QIODevice::ReadWrite)) {
            serialPort->setBaudRate(QSerialPort::Baud2400);
            return;
        }
        else {
            QMessageBox::information(this, "Error", "串口打开失败", QMessageBox::Ok);
            ui->availableUsbPortComboBox->setCurrentIndex(0);
            serialPort->setPortName(DEFAULT_PORT_NAME);
        }
    }
}

void DriverWindow::midiChannelComboBox_onActivated(int index) {
    midiChannel = index;
}

void DriverWindow::updateAvailableUsbPort() {
    QString previousText = ui->availableUsbPortComboBox->currentText();
//    QMessageBox::information(this, "info", previousText, QMessageBox::Ok);

    ui->availableUsbPortComboBox->clear();
    ui->availableUsbPortComboBox->addItem(DEFAULT_PORT_NAME);
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for (auto& portInfo : availablePorts) {
        ui->availableUsbPortComboBox->addItem(portInfo.portName());
    }

    // If previous port still exists, set selected item to it,
    // otherwise set to 0 which corresponding "(Not Connected)"
    // 若之前的端口还存在，将其设为当前选项。否则设为 0 即 "(Not Connected)"
    auto index = ui->availableUsbPortComboBox->findText(previousText);
    ui->availableUsbPortComboBox->setCurrentIndex(
            index == -1 ? 0 : index
            );
}

