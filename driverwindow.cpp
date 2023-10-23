#include "driverwindow.h"
#include "./ui_driverwindow.h"
#include "SliderEventFilter.h"

#define DEFAULT_PORT_NAME "(Not Connected)"
#define INFO(TITLE, TEXT) QMessageBox::information(this, (TITLE), (TEXT), QMessageBox::Ok);

DriverWindow::DriverWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DriverWindow)
#define MAX_SYSEX_BUFFER 65535
    , midiPort(virtualMIDICreatePortEx2(L"Nano Breath Controller", nullptr, 0, MAX_SYSEX_BUFFER, TE_VM_FLAGS_INSTANTIATE_BOTH))
#undef MAX_SYSEX_BUFFER
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

    // usbPortComboBox Callbacks
    // usbPortComboBox 回调函数
    connect(ui->usbPortComboBox, &QComboBox::activated,
            this, &DriverWindow::usbPortComboBox_onActivated);

    // midiChannelComboBox Callbacks
    // midiChannelComboBox 回调函数
    connect(ui->midiChannelComboBox, &QComboBox::activated,
            this, &DriverWindow::midiChannelComboBox_onActivated);

    // serialPort 回调函数
    // serialPort callbacks
    connect(serialPort, &QSerialPort::readyRead, this, &DriverWindow::serialPort_onDataReceived);

    // smoothnessHorizontalSlider
    ui->smoothnessHorizontalSlider->setTickPosition(QSlider::NoTicks);
    ui->smoothnessHorizontalSlider->setMinimum(0);
    ui->smoothnessHorizontalSlider->setMaximum(100);
    ui->smoothnessHorizontalSlider->setTickInterval(1);
    ui->smoothnessHorizontalSlider->setValue(50);

    auto smoothnessSliderFilter = new SliderEventFilter(ui->smoothnessHorizontalSlider);
    ui->smoothnessHorizontalSlider->installEventFilter(smoothnessSliderFilter);

    connect(ui->smoothnessHorizontalSlider, &QSlider::valueChanged, this,
            &DriverWindow::smoothnessHorizontalSlider_onValueChanged);

    loadFromJson();
}

DriverWindow::~DriverWindow()
{
    saveToJson();

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

void DriverWindow::usbPortComboBox_onActivated(int index) {
    // If port not changed, do nothing and return
    // 如果串口未变，直接返回
    if (serialPort->portName() == ui->usbPortComboBox->currentText()) {
        return;
    }

    // Close serial port if it's opened
    // 若串口已打开，则关闭
    if (serialPort->portName() != ui->usbPortComboBox->currentText()) {
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
        serialPort->setPortName(ui->usbPortComboBox->currentText());

        if (serialPort->open(QIODevice::ReadWrite)) {
            serialPort->setBaudRate(QSerialPort::Baud2400);
            return;
        }
        else {
            QMessageBox::information(this, "Error", "串口打开失败", QMessageBox::Ok);
            ui->usbPortComboBox->setCurrentIndex(0);
            serialPort->setPortName(DEFAULT_PORT_NAME);
        }
    }
}

void DriverWindow::midiChannelComboBox_onActivated(int index) {
    midiChannel = index;
}

void DriverWindow::updateAvailableUsbPort() {
    QString previousText = ui->usbPortComboBox->currentText();
//    QMessageBox::information(this, "info", previousText, QMessageBox::Ok);

    ui->usbPortComboBox->clear();
    ui->usbPortComboBox->addItem(DEFAULT_PORT_NAME);
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for (auto& portInfo : availablePorts) {
        ui->usbPortComboBox->addItem(portInfo.portName());
    }

    // If previous port still exists, set selected item to it,
    // otherwise set to 0 which corresponding "(Not Connected)"
    // 若之前的端口还存在，将其设为当前选项。否则设为 0 即 "(Not Connected)"
    auto index = ui->usbPortComboBox->findText(previousText);
    ui->usbPortComboBox->setCurrentIndex(
            index == -1 ? 0 : index
            );
}

void DriverWindow::smoothnessHorizontalSlider_onValueChanged(int value) {
    // alpha = (ExponentialMovingAverage::MaxAlpha() - ExponentialMovingAverage::MinAlpha())
    // * ( 1 - static_cast<double>(value) / (ui->smoothnessHorizontalSlider->maximum()-ui->smoothnessHorizontalSlider->minimum()))
    // + ExponentialMovingAverage::MinAlpha();
    // Now is 0.9*(1-value/100)+0.1
    EMA->setAlpha(1-0.009*value);
}

#define SETTING_FILE "settings.json"
#define USB_PORT_JSON_KEY "USB Port"
#define MIDI_CHANNEL_JSON_KEY "MIDI Channel"
#define SMOOTHNESS_JSON_KEY "Smoothness"

void DriverWindow::saveToJson() {
    QJsonObject jsonObject;
    jsonObject[USB_PORT_JSON_KEY] = ui->usbPortComboBox->currentText();
    jsonObject[MIDI_CHANNEL_JSON_KEY] = ui->midiChannelComboBox->currentText();
    jsonObject[SMOOTHNESS_JSON_KEY] = ui->smoothnessHorizontalSlider->value();

    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonBytes = jsonDocument.toJson();

    QFile settingFile(SETTING_FILE);
    if (settingFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&settingFile);
        out << jsonBytes;

        settingFile.close();
    }
}

void DriverWindow::loadFromJson() {
    QFile settingFile(SETTING_FILE);
    if (settingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonBytes = settingFile.readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonBytes);
        QJsonObject jsonObject = jsonDocument.object();

        QString previousUsbPort = jsonObject[USB_PORT_JSON_KEY].toString();
        ui->usbPortComboBox->setCurrentText(previousUsbPort);

        QString previousMidiChannel = jsonObject[MIDI_CHANNEL_JSON_KEY].toString();
        ui->midiChannelComboBox->setCurrentText(previousMidiChannel);

        int previousSmoothness = jsonObject[SMOOTHNESS_JSON_KEY].toInt();
        ui->smoothnessHorizontalSlider->setValue(previousSmoothness);

        settingFile.close();
    }
}

#undef SETTING_FILE
#undef USB_PORT_JSON_KEY
#undef MIDI_CHANNEL_JSON_KEY
#undef SMOOTHNESS_JSON_KEY

#undef DEFAULT_PORT_NAME
