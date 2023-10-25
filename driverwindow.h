#ifndef DRIVERWINDOW_H
#define DRIVERWINDOW_H

#include <QWidget>
#include "teVirtualMIDI.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QSerialPort>
#include <QMessageBox>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ExponentialMovingAverage.h"
#include "NanoEventFilter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DriverWindow; }
QT_END_NAMESPACE

class DriverWindow : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

    /**
     * @author  A-KRY
     * @brief   <p>虚拟 MIDI 端口</p>
     * <p>Virtual MIDI Port</p>
     * @date    2023-10-20
     */
    LPVM_MIDI_PORT midiPort;

    /**
     * @brief   <p>USB 端口更新定时器</p>
     * <p>Timer for updating available USB port.</p>
     * @author  A-KRY
     * @date    2023/10/22 14:07
     */
    QTimer* usbPortUpdateTimer;

    /**
     * @brief   <p>发送的 CC 值的 MIDI 通道</p>
     * <p>The MIDI channel about to send CC value to.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:31
     */
    BYTE midiChannel;

    /**
     * @brief   <p>与 Arduino Nano 通信的串口</p>
     * <p>The serial port which Arduino Nano communicate with.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:33
     */
    QSerialPort* serialPort;

    /**
     * @brief   <p>指数移动平均滤波器</p>
     * <p>EMA Filter</p>
     * @author  A-KRY
     * @date    2023/10/23 9:05
     */
    std::unique_ptr<ExponentialMovingAverage> EMA;

public:
    explicit DriverWindow(QWidget *parent = nullptr);
    ~DriverWindow() override;

private:
    Ui::DriverWindow *ui;

    /**
     * @brief   <p>保存参数到 JSON</p>
     * <p>Save params to JSON</p>
     * @author  A-KRY
     * @date    2023/10/23 10:53
     */
     void saveToJson();

     /**
      * @brief   <p>从 JSON 读取参数</p>
      * <p>Load params from JSON</p>
      * @author  A-KRY
      * @date    2023/10/23 10:54
      */
    void loadFromJson();

protected:
};
#endif // DRIVERWINDOW_H
