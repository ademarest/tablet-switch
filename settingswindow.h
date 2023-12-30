#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDir>
#include <QSettings>

#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    SettingsWindow();
    ~SettingsWindow();

private slots:
    void readSettings();
    void writeSettings();

    void showSettingsWindow();
    void activateTabletStylusOnlyInputMode();
    void activateComputerInputMode();
    void activateTabletInputMode();
    void enableStylusOnlyOptions();
    void setupSystemTray();

    void lockTouchFileDescriptor(QString touchDeviceName);
    void unlockTouchFileDescriptor();
    void showTrayIconMenu();

private:
    Ui::SettingsWindow *ui;

    enum InputMode{COMPUTER=0, TABLET=1, TABLET_STYLUS_ONLY=2};
    enum IconTheme{LIGHT=0,DARK=1};

    QStringList tabletSwitchEventDeviceNames;
    QStringList allEventDeviceNames;
    IconTheme currentIconTheme = DARK;
    InputMode currentInputMode;
    int touchDeviceFd = -1;

    QString tabletSwitchDeviceName  = "HP WMI hotkeys";
    QString touchInputDeviceName    = "ELAN2513:00 04F3:2AF1"; //Hardcoded for now. It's my touch screen device on my HP ENVY

    QAction *showSettingsWindowAction;
    QAction *tabletStylusOnlyModeAction;
    QAction *tabletModeAction;
    QAction *computerModeAction;
    QAction *quitAction;
    QAction *showIconTrayMenuAtCursorPosition;

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QMenu *trayIconMenu;

    void setSystemTrayIconThemeComboBox();
    void findTabletSwitchEventDeviceNames();
    void populateAllEventDeviceNames();
    void setupTrayMenuActions();

    void changeInputMode(InputMode inputMode);
    void changeTrayIcon(InputMode inputMode);
    QIcon getIconForInputMode(InputMode inputMode);

    //devName is name of the the device that contains SW_TABLET_MODE
    //as outputted by the evtest command for the device of interest.
    static InputMode determineCurrentInputMode(QString devName);

    static input_event createInputEvent(int type, int code, int value);
    static int writeInputEventToDeviceFile(int fd, input_event event);
    static int getDeviceFileDescriptorByName(QString devName);
};
#endif // SETTINGSWINDOW_H
