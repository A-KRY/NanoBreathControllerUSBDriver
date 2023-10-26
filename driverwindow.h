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

    /**
     * @brief   <p>系统托盘菜单</p>
     * <p>System Tray Menu</p>
     * @author  A-KRY
     * @date    2023/10/26 15:37
     */
     QMenu* trayMenu;

     /**
      * @brief   <p>当前语言</p>
      * <p>Current language</p>
      * @author  A-KRY
      * @date    2023/10/26 15:45
      */
     QString currentLanguage;
public:
    /**
     * @brief   <p>这个方法只是用来更新记录当前语言的 QString 对象</p>
     * <p>This method just update the QString object that records current language.</p>
     * @param language <p>和系统托盘菜单的文字严格一致 <br>
     * Strictly consistent with text in system tray menu.
     * </p>
     * @author  A-KRY
     * @date    2023/10/26 15:51
     */
    void setCurrentLanguage(const QString &language);

    /**
     * @brief   <p>设置 <b><i>trayMenu</i></b> 指针</p>
     * <p>Set pointer <b><i>trayMenu</i></b></p>
     * @attention <p>在设置好 <b><i>trayMenu</i></b> 的所有 <b><i>QAction</i></b>s 后调用！</p>
     * <p>Call after all <b><i>QAction</i></b>s of <b><i>trayMenu</i></b> set! </p>
     * @param
     * @return
     * @author  A-KRY
     * @date    2023/10/26 16:35
     */
    void setTrayMenu(QMenu *menu);

public:
    explicit DriverWindow(QWidget *parent = nullptr);
    ~DriverWindow() override;

    /**
     * @brief   <p>切换语言后刷新文本</p>
     * <p>Refresh text after language switched.</p>
     * @author  A-KRY
     * @date    2023/10/26 11:13
     */
    void reloadText();

    /**
     * @brief   <p>更改语言</p>
     * <p>Switch language.</p>
     * @author  A-KRY
     * @date    2023/10/26 15:34
     */
     void switchLanguage(const QString& language);

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
