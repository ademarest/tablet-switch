#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow()
    : ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->readSettings();
    this->setSystemTrayIconThemeComboBox();
    this->populateAllEventDeviceNames();
    this->findTabletSwitchEventDeviceNames();
    this->currentInputMode = determineCurrentInputMode(this->tabletSwitchDeviceName);
    this->setupSystemTray();

    connect(trayIcon,&QSystemTrayIcon::activated,this,&SettingsWindow::showTrayIconMenu);
    connect(ui->systemTrayIconThemeComboBox,&QComboBox::currentIndexChanged,this,&SettingsWindow::setupSystemTray);
    connect(ui->stylusOnlyOptionsCheckBox,&QCheckBox::checkStateChanged,this,&SettingsWindow::setupSystemTray);
    connect(ui->stylusOnlyOptionsCheckBox,&QCheckBox::checkStateChanged,this,&SettingsWindow::enableStylusOnlyOptions);

    for(auto widget : this->children()){
        if(qobject_cast<QCheckBox*>(widget)){
            connect(qobject_cast<QCheckBox*>(widget),&QCheckBox::checkStateChanged,this,&SettingsWindow::writeSettings);
        }
        if(qobject_cast<QComboBox*>(widget)){
            connect(qobject_cast<QComboBox*>(widget),&QComboBox::currentIndexChanged,this,&SettingsWindow::writeSettings);
        }
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::readSettings(){
    QSettings settings("tablet-settings", "settings");

    this->currentIconTheme = settings.value("currentIconTheme").isValid()
	? static_cast<IconTheme>(settings.value("currentIconTheme").toInt()) : this->currentIconTheme;

    this->tabletSwitchDeviceName = settings.value("tabletSwitchDeviceName").isValid()
	? settings.value("tabletSwitchDeviceName").toString() : this->tabletSwitchDeviceName;

    this->touchInputDeviceName = settings.value("touchInputDeviceName").isValid()
	? settings.value("touchInputDeviceName").toString() : this->touchInputDeviceName;

    if(settings.value("showStylusOnlyOptions").toInt() == 2){
        ui->stylusOnlyOptionsCheckBox->setCheckState(Qt::Checked);
        this->enableStylusOnlyOptions();
    }
}

void SettingsWindow::writeSettings(){

    this->currentIconTheme = ui->systemTrayIconThemeComboBox->currentText().toLower() == "dark"
	? DARK : LIGHT;

    QSettings settings("tablet-settings", "settings");
    settings.setValue("currentIconTheme",static_cast<int>(this->currentIconTheme));
    settings.setValue("tabletSwitchDeviceName",ui->tabletSwitchDeviceNameComboBox->currentText());
    settings.setValue("touchInputDeviceName",ui->touchDeviceComboBox->currentText());
    settings.setValue("showStylusOnlyOptions", static_cast<int>(this->ui->stylusOnlyOptionsCheckBox->checkState()));
}

void SettingsWindow::showSettingsWindow(){
    this->show();
}

void SettingsWindow::setSystemTrayIconThemeComboBox(){
    for(int i = 0; i < ui->systemTrayIconThemeComboBox->count(); i++){
        if(this->currentIconTheme == DARK && ui->systemTrayIconThemeComboBox->itemText(i).toLower() == "dark")
            ui->systemTrayIconThemeComboBox->setCurrentIndex(i);
        if(this->currentIconTheme == LIGHT && ui->systemTrayIconThemeComboBox->itemText(i).toLower() == "light")
            ui->systemTrayIconThemeComboBox->setCurrentIndex(i);
    }
}

void SettingsWindow::enableStylusOnlyOptions(){
    if(ui->stylusOnlyOptionsCheckBox->isChecked()){
        ui->touchDeviceComboBox->setDisabled(false);
    } else {
        ui->touchDeviceComboBox->setDisabled(true);
    }
}

void SettingsWindow::populateAllEventDeviceNames(){
    int touchDeviceComboBoxIdxOfKnownDevice = 0;
    QDir directory("/dev/input");
    QStringList eventDevices = directory.entryList(QStringList() << "event*",QDir::System);

    for(int i = 0; i < eventDevices.size(); i++){
        auto devFile = eventDevices.at(i);
        struct libevdev *dev = NULL;
        int fd = ::open(QString("/dev/input/" + devFile).toLocal8Bit().data(), O_RDWR);
        libevdev_new_from_fd(fd, &dev);
        touchDeviceComboBoxIdxOfKnownDevice = libevdev_get_name(dev) == this->touchInputDeviceName ? i : touchDeviceComboBoxIdxOfKnownDevice;
        this->allEventDeviceNames << QString(libevdev_get_name(dev));
        ui->touchDeviceComboBox->addItem(QString(libevdev_get_name(dev)));
        libevdev_free(dev);
        ::close(fd);
    }
    ui->touchDeviceComboBox->setCurrentIndex(touchDeviceComboBoxIdxOfKnownDevice);
}

void SettingsWindow::findTabletSwitchEventDeviceNames(){
    QDir directory("/dev/input");
    const QStringList eventDevices = directory.entryList(QStringList() << "event*",QDir::System);

    for(const auto &devFile : eventDevices){
        struct libevdev *dev = NULL;
        int fd = ::open(QString("/dev/input/" + devFile).toLocal8Bit().data(), O_RDWR);
        libevdev_new_from_fd(fd, &dev);

        if(libevdev_has_event_code(dev,EV_SW,SW_TABLET_MODE)){
            this->tabletSwitchEventDeviceNames << QString(libevdev_get_name(dev));
            ui->tabletSwitchDeviceNameComboBox->addItem(QString(libevdev_get_name(dev)));
        }

        libevdev_free(dev);
        ::close(fd);
    }
}

void SettingsWindow::setupTrayMenuActions(){
    QFont font = QFont("System",16);

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setFont(font);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    tabletStylusOnlyModeAction = new QAction(tr("&Stylus Only Mode"), this);
    tabletStylusOnlyModeAction->setFont(font);
    connect(tabletStylusOnlyModeAction, &QAction::triggered, this, &SettingsWindow::activateTabletStylusOnlyInputMode);

    computerModeAction = new QAction(tr("&Computer Mode"), this);
    computerModeAction->setFont(font);
    connect(computerModeAction, &QAction::triggered, this, &SettingsWindow::activateComputerInputMode);

    tabletModeAction = new QAction(tr("&Tablet Mode"), this);
    tabletModeAction->setFont(font);
    connect(tabletModeAction, &QAction::triggered, this, &SettingsWindow::activateTabletInputMode);

    showSettingsWindowAction = new QAction(tr("&Settings"), this);
    showSettingsWindowAction->setFont(font);
    connect(showSettingsWindowAction, &QAction::triggered, this, &SettingsWindow::showSettingsWindow);
}

void SettingsWindow::setupSystemTray(){
    this->setupTrayMenuActions();
    trayIconMenu->clear();
    trayIconMenu->addAction(showSettingsWindowAction);

    if(ui->stylusOnlyOptionsCheckBox->isChecked()){
        trayIconMenu->addAction(tabletStylusOnlyModeAction);
    }

    trayIconMenu->addAction(tabletModeAction);
    trayIconMenu->addAction(computerModeAction);
    trayIconMenu->addAction(quitAction);

    if(!trayIcon->isVisible()){
        trayIcon->show();
    }

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(getIconForInputMode(this->currentInputMode));
}

void SettingsWindow::activateTabletStylusOnlyInputMode(){
    changeInputMode(TABLET_STYLUS_ONLY);
    trayIcon->setIcon(getIconForInputMode(TABLET_STYLUS_ONLY));
}

void SettingsWindow::activateComputerInputMode(){
    changeInputMode(COMPUTER);
    trayIcon->setIcon(getIconForInputMode(COMPUTER));
}

void SettingsWindow::activateTabletInputMode(){
    changeInputMode(TABLET);
    trayIcon->setIcon(getIconForInputMode(TABLET));
;}

void SettingsWindow::lockTouchFileDescriptor(QString touchDeviceName)
{
    //EVIOCGRAB isn't really documented except in kernel sources at "drivers/input/evdev.c"
    //at the function "static long evdev_do_ioctl"
    //Explanation @ https://unix.stackexchange.com/questions/126974/where-do-i-find-ioctl-eviocgrab-documented
    //Anyway, this locks the device's file descriptor via ioctl and can't be used for inputs.
    this->touchDeviceFd = getDeviceFileDescriptorByName(touchDeviceName);
    ioctl(this->touchDeviceFd, EVIOCGRAB, true);
}

void SettingsWindow::unlockTouchFileDescriptor()
{
    if(this->touchDeviceFd == -1){
        //Nothing to do, there is no file descriptor open for the touch device.
        return;
    }

    //This unlocks the dev via ioctl;
    ioctl(this->touchDeviceFd, EVIOCGRAB, false);
    ::close(this->touchDeviceFd);
    this->touchDeviceFd = -1;
}

SettingsWindow::InputMode SettingsWindow::determineCurrentInputMode(QString devName)
{
    struct libevdev *dev = NULL;
    int fd = getDeviceFileDescriptorByName(devName);
    if(fd < 0){
        //Input mode is an enum. In the event that the current inpute mode cannot be determined it's safed to assume that this is a standard PC.
        return COMPUTER;
    }
    libevdev_new_from_fd(fd,&dev);
    InputMode inputMode = static_cast<InputMode>(libevdev_get_event_value(dev,EV_SW,SW_TABLET_MODE));
    libevdev_free(dev);
    ::close(fd);
    return inputMode;
}


QIcon SettingsWindow::getIconForInputMode(InputMode inputMode){
    QIcon icon;

    QString theme = ui->systemTrayIconThemeComboBox->currentText().toLower();

    switch(inputMode){
        case COMPUTER               : icon = QIcon(":/512/computer_mode_"+theme+".png"); break;
        case TABLET_STYLUS_ONLY     : icon = QIcon(":/512/tablet_stylus_only_mode_"+theme+".png"); break;
        case TABLET                 : icon = QIcon(":/512/tablet_mode_"+theme+".png"); break;
    };
    return icon;
}

void SettingsWindow::changeInputMode(InputMode inputMode)
{
    int fd = getDeviceFileDescriptorByName(ui->tabletSwitchDeviceNameComboBox->currentText());
    qDebug() << fd;
    if(fd < 0){
        return;
    }
    struct input_event event = createInputEvent(EV_SW,SW_TABLET_MODE,inputMode);
    writeInputEventToDeviceFile(fd, event);

    event = createInputEvent(EV_SYN,0,0);
    writeInputEventToDeviceFile(fd, event);
    ::close(fd);

    if(inputMode == TABLET_STYLUS_ONLY){
        lockTouchFileDescriptor(this->touchInputDeviceName);
    } else {
        unlockTouchFileDescriptor();
    }
}

input_event SettingsWindow::createInputEvent(int type, int code, int value)
{
    struct input_event event;
    struct timeval tval;
    gettimeofday(&tval,0);
    event.input_event_usec = tval.tv_usec;
    event.input_event_sec = tval.tv_sec;
    event.type = type;
    event.code = code;
    event.value = value;
    return event;
}

int SettingsWindow::writeInputEventToDeviceFile(int fd, input_event event){
    if(write(fd, &event, sizeof(event)) != sizeof(event)){
        return -1;
    }
    return 0;
}

int SettingsWindow::getDeviceFileDescriptorByName(QString devName){

    QDir directory("/dev/input");
    const QStringList eventDevices = directory.entryList(QStringList() << "event*",QDir::System);

    for(const auto &devFile : eventDevices){
        struct libevdev *dev = NULL;
        int fd = ::open(QString("/dev/input/" + devFile).toLocal8Bit().data(), O_RDWR);
        int rc = libevdev_new_from_fd(fd, &dev);
        if(rc < 0){
            libevdev_free(dev);
            return -1;
        } else if(QString(libevdev_get_name(dev)) != devName){
            libevdev_free(dev);
            ::close(fd);
        } else {
            libevdev_free(dev);
            //REMEMBER TO CLOSE THE FD! >:(
            return fd;
        }
    }
    return -1;
}

void SettingsWindow::showTrayIconMenu(){
    trayIconMenu->exec(QCursor::pos());
}
