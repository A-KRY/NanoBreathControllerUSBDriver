#include "driverwindow.h"
#include "./ui_driverwindow.h"

#define DEFAULT_PORT_NAME "(Not Connected)"
//#define INFO(TITLE, TEXT) QMessageBox::information(this, (TITLE), (TEXT), QMessageBox::Ok);

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

    /*
     * Available USB port update
     * 可用 USB 端口更新
     */
    usbPortUpdateTimer->setInterval(1000);

    /**
     * @brief   <p>更新 <<b><i>usbPortComboBox</i></b> 内显示的可用 USB 端口</p>
     * <p>Update available USB port in <b><i>usbPortComboBox</i></b></p>
     * @author  A-KRY
     * @date    2023/10/24 11:29
     */
    auto usbPortComboBox_updateAvailablePort = [this]() {
        QString previousText = ui->usbPortComboBox->currentText();

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
    };
    connect(usbPortUpdateTimer, &QTimer::timeout, this, usbPortComboBox_updateAvailablePort);
    usbPortComboBox_updateAvailablePort();
    usbPortUpdateTimer->start();

    /*
     * midiChannelComboBox initialization
     * 初始化 midiChannelComboBox
     */
    for (auto i = 1; i <= 16; ++i) {
        ui->midiChannelComboBox->addItem(QString::number(i));
    }

    /*
     * usbPortComboBox Callbacks
     * usbPortComboBox 回调函数
     */
    /**
     * @brief   <p>选中 <b><i>usbPortComboBox</i></b> 的选项后尝试打开相应端口</p>
     * <p>Try to open the port when item of <b><i>usbPortComboBox</i></b> selected.</p>
     * @param index <p>选中项的 ID</p>
     * <p>ID of selected item.</p>
     * @author  A-KRY
     * @date    2023/10/24 11:41
     */
    auto usbPortComboBox_onActivated = [this](int index) {
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
    };
    connect(ui->usbPortComboBox, &QComboBox::activated,
            this, usbPortComboBox_onActivated);

    /*
     * midiChannelComboBox Callbacks
     * midiChannelComboBox 回调函数
     */
    /**
     * @brief   <p>选中 <b><i>midiChannelComboBox</i></b> 的选项后设置对应 MIDI 通道</p>
     * <p>Set MIDI channel to which selected by <b><i>midiChannelComboBox</i></b>.</p>
     * @param   index <p>选中项的 ID</p>
     * <p>ID of selected item.</p>
     * @author  A-KRY
     * @date    2023/10/24 11:44
     */
    auto midiChannelComboBox_onActivated = [this](int index){
            midiChannel = index;
    };
    connect(ui->midiChannelComboBox, &QComboBox::activated,
            this, midiChannelComboBox_onActivated);

    /*
     * serialPort 回调函数
     * serialPort callbacks
     */
    /**
     * @brief   <p>接收到 Arduino Nano 发来的数据后发送 MIDI CC11 消息。</p>
     * <p>Once received data from Arduino Nano, send MIDI CC11 message.</p>
     * @author  A-KRY
     * @date    2023/10/24 11:47
     */
    auto serialPort_onDataReceived = [this]() {
        auto data = serialPort->read(1);
        if (!data.isEmpty()) {
            BYTE midiMessage[3] = {static_cast<BYTE>(0xB0|midiChannel), 11, static_cast<BYTE>(EMA->filter(data[0]))};
            if (midiPort) {
                virtualMIDISendData(midiPort, midiMessage, sizeof(midiMessage));
            }
        }
    };
    connect(serialPort, &QSerialPort::readyRead, this, serialPort_onDataReceived);

    /*
     * smoothnessHorizontalSlider
     */
    ui->smoothnessHorizontalSlider->setTickPosition(QSlider::NoTicks);
    ui->smoothnessHorizontalSlider->setMinimum(0);
    ui->smoothnessHorizontalSlider->setMaximum(100);
    ui->smoothnessHorizontalSlider->setTickInterval(1);
    ui->smoothnessHorizontalSlider->setValue(50);

    /**
     * @brief   <p>当按下 <b><i>Ctrl+鼠标左键</i></b> 时重置 smoothnessHorizontalSlider 位置</p>
     * <p>Reset the position of <b>smoothnessHorizontalSlider</b> when <b><i>Ctrl+MouseLeftButton</i> pressed.</b></p>
     * @author  A-KRY
     * @date    2023/10/24 11:27
     */
    auto smoothnessHorizontalSlider_CtrlLeftClickFilter_callback = [this](NEF_PARAMS){
        if (watched == ui->smoothnessHorizontalSlider and event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent and (mouseEvent->button() == Qt::LeftButton)
                and (mouseEvent->modifiers() == Qt::ControlModifier)) {
                ui->smoothnessHorizontalSlider->setValue((ui->smoothnessHorizontalSlider->maximum()+ui->smoothnessHorizontalSlider->minimum())>>1);
                return true;
            }
        }
        NEF_RETURN
    };
    auto smoothnessHorizontalSlider_CtrlLeftClickFilter = new NanoEventFilter(smoothnessHorizontalSlider_CtrlLeftClickFilter_callback);
    ui->smoothnessHorizontalSlider->installEventFilter(smoothnessHorizontalSlider_CtrlLeftClickFilter);

    /**
     * @brief   <p>滑块移动时更新 EMA 滤波器的 <b><i>alpha</i></b> 值</p>
     * <p>Update <b><i>alpha</i></b> of EMA filter when slider moves.</p>
     * @param   value <p>移动后的值</p>
     * <p>New value after moved.</p>
     * @author  A-KRY
     * @date    2023/10/24 11:51
     */
    auto smoothnessHorizontalSlider_onValueChanged = [this](int value){
        // alpha = (ExponentialMovingAverage::MaxAlpha() - ExponentialMovingAverage::MinAlpha())
        // * ( 1 - static_cast<double>(value) / (ui->smoothnessHorizontalSlider->maximum()-ui->smoothnessHorizontalSlider->minimum()))
        // + ExponentialMovingAverage::MinAlpha();
        // Now is 0.9*(1-value/100)+0.1
        EMA->setAlpha(1-0.009*value);
    };
    connect(ui->smoothnessHorizontalSlider, &QSlider::valueChanged, this,
            smoothnessHorizontalSlider_onValueChanged);

    // smoothnessLineEdit
#define SLE_NAME "Smoothness"
    auto smoothnessLineEdit_DblClickFilter_callback = [this](NEF_PARAMS){
        if (watched == ui->smoothnessLineEdit and event->type() == QEvent::MouseButtonDblClick) {
            ui->smoothnessLineEdit->setText(QString::number(ui->smoothnessHorizontalSlider->value()));
            ui->smoothnessLineEdit->setReadOnly(false);
            ui->smoothnessLineEdit->setStyleSheet("");
            ui->smoothnessLineEdit->setSelection(0, static_cast<int>(ui->smoothnessLineEdit->text().length()));
            return true;
        }
        NEF_RETURN
    };
    auto smoothnessLineEdit_DblClickFilter = new NanoEventFilter(smoothnessLineEdit_DblClickFilter_callback);
    ui->smoothnessLineEdit->installEventFilter(smoothnessLineEdit_DblClickFilter);

    auto smoothnessLineEdit_onEditFinished = [this]() {
        const static QRegularExpression rx("\\d+");
        auto match = rx.match(ui->smoothnessLineEdit->text());
        if (match.hasMatch() and match.captured(0) == ui->smoothnessLineEdit->text()
            and 0 <= ui->smoothnessLineEdit->text().toInt() and ui->smoothnessLineEdit->text().toInt() <= 100) {
            ui->smoothnessHorizontalSlider->setValue(ui->smoothnessLineEdit->text().toInt());
        }
        ui->smoothnessLineEdit->setReadOnly(true);
        ui->smoothnessLineEdit->setText(SLE_NAME);
        ui->smoothnessLineEdit->setStyleSheet("background-color: transparent; border: none;padding: 0; margin: 0;");
    };

    connect(ui->smoothnessLineEdit, &QLineEdit::editingFinished, this, smoothnessLineEdit_onEditFinished);

    // Event filter which triggers when click outside smoothnessLineEdit
    auto smoothnessLineEdit_ClickOutsideFilter_callback = [this](NEF_PARAMS) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (ui->smoothnessLineEdit->hasFocus() and !ui->smoothnessLineEdit->rect().contains(ui->smoothnessLineEdit->mapFromGlobal(QCursor::pos()))) {
                /**
                 * @note
                 * <p>在这个 <b>if</b> 内本可以调用 <b><i>smoothnessLineEdit_onEditFinished()</i></b><br>
                 * We could have called <b><i>smoothnessLineEdit_onEditFinished()</i></b> inside this <b>if</b>.</p>
                 * <p>但是从这里调用的 smoothnessLineEdit_onEditFinished() 不知为何无法捕获 'this' 指针，最终会导致程序崩溃 <br>
                 * But smoothnessLineEdit_onEditFinished() called from here can not capture 'this' pointer somehow, which would cause crash.</p>
                 * <p>因此只能将 smoothnessLineEdit_onEditFinished() 的内容再复制一遍 <br>
                 * Therefore I have no choice but to copy the content of smoothnessLineEdit_onEditFinished() again.</p>
                 * <p>————————————————</p>
                 * <p>实际上 lambda 表达式的这种用法不应出错，例如：<br>
                 * In fact, this usage of lambda should not cause error, for example:<br></p>
                 * @code
                 * auto a = 1;
                 * auto l1 = [&a]() { return a; };
                 * auto l2 = [&l1] { return l1(); };
                 * l2();
                 * @endcode
                 * <br>
                 * <p>上述代码经过测试完全不存在任何问题 <br>
                 * The code above has been tested with no problem at all!</p>
                 * @author A-KRY
                 * @date 2023/10/25 10:04
                 */
                [[maybe_unused]]auto READ_ME = nullptr;

                const static QRegularExpression rx("\\d+");
                auto match = rx.match(ui->smoothnessLineEdit->text());
                if (match.hasMatch() and match.captured(0) == ui->smoothnessLineEdit->text()
                    and 0 <= ui->smoothnessLineEdit->text().toInt() and ui->smoothnessLineEdit->text().toInt() <= 100) {
                    ui->smoothnessHorizontalSlider->setValue(ui->smoothnessLineEdit->text().toInt());
                }
                ui->smoothnessLineEdit->setReadOnly(true);
                ui->smoothnessLineEdit->setText(SLE_NAME);
                ui->smoothnessLineEdit->setStyleSheet("background-color: transparent; border: none;padding: 0; margin: 0;");
                return true;
            }
        }
        NEF_RETURN
    };
    auto smoothnessLineEdit_ClickOutsideFilter = new NanoEventFilter(smoothnessLineEdit_ClickOutsideFilter_callback);
    this->installEventFilter(smoothnessLineEdit_ClickOutsideFilter);

#undef SLE_NAME

    // 永远最后调用
    // Always called last.
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
