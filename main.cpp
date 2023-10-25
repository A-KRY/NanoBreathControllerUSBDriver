#include "driverwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "NanoBreathControllerUSBDriver_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    /**
     * @brief   <p>系统托盘图标</p>
     * <p>System tray icon.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:20
     */
    auto trayIcon = new QSystemTrayIcon(QIcon(":icon/SystemTrayIcon.png"));
    trayIcon->setVisible(true);

    /**
     * @brief   <p>系统托盘菜单</p>
     * <p>System tray menu.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:21
     */
    auto trayMenu = new QMenu();

    /**
     * @brief   <p>退出操作</p>
     * <p>Exit action.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:52
     */
    auto exitAction = new QAction("Exit");

    trayMenu->addAction(exitAction);
    trayIcon->setContextMenu(trayMenu);
    QObject::connect(exitAction, &QAction::triggered, &application, &QApplication::quit);

    DriverWindow driverWindow;

    /**
     * @brief   <p>双击系统托盘图标时恢复主窗口显示</p>
     * <p>Show main window when double-clicked system tray icon.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:52
     */
    auto trayIcon_onDblClicked = [&driverWindow](QSystemTrayIcon::ActivationReason reason)
    {
        if (reason == QSystemTrayIcon::DoubleClick) {
            driverWindow.show();
            driverWindow.activateWindow();
        }
    };
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, &driverWindow, trayIcon_onDblClicked);

    driverWindow.show();

    return QApplication::exec();
}
