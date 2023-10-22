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
    , currentMidiChannel(0)
    , currentSerialPort(new QSerialPort())
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
}

DriverWindow::~DriverWindow()
{
    if (midiPort) {
        virtualMIDIClosePort(midiPort);
    }
    if (currentSerialPort->isOpen()) {
        currentSerialPort->close();
    }
    delete ui;
}

void DriverWindow::availableUsbPortComboBox_onActivated(int index) {
    // If port not changed, do nothing and return
    // 如果串口未变，直接返回
    if (currentSerialPort->portName() == ui->availableUsbPortComboBox->currentText()) {
        return;
    }

    // Close serial port if it's opened
    // 若串口已打开，则关闭
    if (currentSerialPort->portName() != ui->availableUsbPortComboBox->currentText()) {
        if (currentSerialPort->isOpen()) {
            currentSerialPort->close();
            currentSerialPort->setPortName(DEFAULT_PORT_NAME);
        }
    }

    // Open serial port only when selected item isn't "(Not Connected)"
    // 只有选中项不为 "(Not Connected)" 时才打开端口
    if (index != 0) {
        // Set port name and open port
        // 设置端口名并打开端口
        currentSerialPort->setPortName(ui->availableUsbPortComboBox->currentText());

        if (currentSerialPort->open(QIODevice::ReadWrite)) {
            currentSerialPort->setBaudRate(QSerialPort::Baud2400);
            return;
        }
        else {
            QMessageBox::information(this, "Error", "串口打开失败", QMessageBox::Ok);
            ui->availableUsbPortComboBox->setCurrentIndex(0);
            currentSerialPort->setPortName(DEFAULT_PORT_NAME);
        }
    }
}

void DriverWindow::midiChannelComboBox_onActivated(int index) {
    currentMidiChannel = index;
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

