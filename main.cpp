#include "settingswindow.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "tablet_switch_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    /*
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system. If you're on gnome, install "
                                          "gnome-shell-extension-appindicator."));
        return 1;
    }
    */

    QApplication::setQuitOnLastWindowClosed(false);
    SettingsWindow w;
    //w.show();
    return a.exec();
}
