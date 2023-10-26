// clazy:excludeall=lambda-in-connect
#include "driverwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QActionGroup>

#define INFO(TEXT) QMessageBox::information(nullptr, "INFO", (TEXT), QMessageBox::Ok);

int main(int argc, char *argv[])
{
#define APP application
    QApplication APP(argc, argv);

#define TRANSLATOR translator
    auto TRANSLATOR = new QTranslator();

#define WINDOW driverWindow
    DriverWindow WINDOW;

    /**
     * @brief   <p>系统托盘图标</p>
     * <p>System tray icon.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:20
     */
    auto trayIcon = new QSystemTrayIcon(QIcon(":icon/SystemTrayIcon.png"));
    trayIcon->setVisible(true);

#define TRAY_MENU trayMenu
    /**
     * @brief   <p>系统托盘菜单</p>
     * <p>System tray menu.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:21
     */
    auto TRAY_MENU = new QMenu();

    trayIcon->setContextMenu(trayMenu);

    // 多语言
    // Multi language

// region MULTI_LANG_DEFINE
#ifdef TRAY_MENU
#ifdef TRANSLATOR
#ifdef APP
#ifdef WINDOW
#define MULTI_LANG_SET_UP() auto languageActionGroup = new QActionGroup(nullptr);
#define ACTION(LAN_SGN) switch_to_##LAN_SGN##_action
/**
 * @brief <p>想要新增语言，只需创建相应.ts .pm文件，将.pm文件加入resource.qrc并调用本宏即可</p>
 * <p>To add a new language, just create the .ts .pm file, then add .pm file into
 * resource.qrc and call this macro.</p>
 * @param LAN_SGN <p>语言符号，与 .ts .pm 文件名称相同</p>
 * <p>Sign of language, which same as name of .ts and .tm files.</p>
 * @param MENU_TEXT <p>该语言显示在系统托盘菜单中的名称</p>
 * <p>Name of language in system tray menu.</p>
 * @attention <p>
 * 想使这个宏正常工作，需定义TRAY_MENU(QMenu)，TRANSLATOR(QTranslator)，
 * APP(QApplication)，WINDOW(DriverWindow) </p>
 * <p>
 * To make this macro work properly, you need to define
 * TRAY_MENU(QMenu), TRANSLATOR(QTranslator),
 * APP(QApplication), WINDOW(DriverWindow)
 * </p>
 * @author A-KRY
 * @date 2023-10-26 17:47
 */
#define ADD_LANG(LAN_SGN, MENU_TEXT) \
    auto ACTION(LAN_SGN) = new QAction(#MENU_TEXT); \
    TRAY_MENU->addAction(ACTION(LAN_SGN)); \
    QObject::connect(ACTION(LAN_SGN), &QAction::triggered, &APP, [&](){ \
        if (TRANSLATOR and TRANSLATOR->load(":languages/"#LAN_SGN)) { \
            QApplication::installTranslator(TRANSLATOR); \
            WINDOW.setCurrentLanguage(ACTION(LAN_SGN)->text()); \
            WINDOW.reloadText(); \
            TRAY_MENU->actions().back()->setText(QObject::tr("Exit")); \
        } \
    });                             \
    ACTION(LAN_SGN)->setCheckable(true);  \
    ACTION(LAN_SGN)->setActionGroup(languageActionGroup); \

#endif // WINDOW
#endif // APP
#endif // TRANSLATOR
#endif // TRAY_MENU
// endregion

    MULTI_LANG_SET_UP()
    ADD_LANG(en_US, English)
    ADD_LANG(zh_CN, 简体中文)
    ADD_LANG(ja_JP, 日本語)

// region MULTI_LANG_UNDEF
#undef ADD_LANG
#undef NEW_LANG
#undef MULTI_LANG_SET_UP
// endregion

    trayMenu->addSeparator();

    // 退出操作
    // Exit Action
    /**
     * @brief   <p>退出操作</p>
     * <p>Exit action.</p>
     * @author  A-KRY
     * @date    2023/10/25 17:52
     */
    auto exitAction = new QAction(QObject::tr("Exit"));
    trayMenu->addAction(exitAction);
    QObject::connect(exitAction, &QAction::triggered, &application, &QApplication::quit);

    driverWindow.setTrayMenu(trayMenu);

    // 显示主窗口
    // Show main window
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
