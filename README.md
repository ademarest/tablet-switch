## Intro  
Tablet-switch is a systemtray program to manually toggle different operating modes for your 2-in-1 on Linux.  
This is what the different operating modes do:  

* **Tablet Mode**, both touch and stylus input are available. The built-in keyboard and trackpad are disabled.  
* **Stylus Only Mode**, only the sytlus is available for input. The touch screen, built-in keyboard, and trackpad are disabled.
* **Computer Mode** all inputs will be enabled.

tablet-switch currently works on my HP ENVY x360 Convertable 13-ay1035nr.  
It should work on other laptops with a similar tablet switch device.  

tablet-switch will find 2-in-1 tablet switch devices which have the event codes `EV_SW`,`SW_TABLET_MODE`.  
Those devices will be populated in the settings ui where they can then be selected and toggled. 

Upon selecting either Tablet mode or Stylus Only Mode, a `SW_TABLET_MODE` event will be emitted via the tablet switch input device.  
In my case the tablet switch input device is `HP WMI Hotkeys`.  
The `SW_TABLET_MODE` event will alert the operating system that the computer is in tablet mode.  
Gnome for instance will disable the keyboard and trackpad while enabling the on screen keyboard and screen rotation.

## Installing via Package

* [Arch Linux](https://aur.archlinux.org/packages/tablet-switch-git)

## Building from source 
Review the code.  
Clone the repository
```bash
$ git clone https://github.com/ademarest/tablet-switch.git
```

Change directory into the git repository
```bash
$ cd ./tablet-switch
```

Compile tablet-switch
```bash
$ ./compile.sh
```

Add the user which will run tablet-switch  to the input group.  
```bash
$ sudo useradd -aG input $USER
```

The following packages are required to run this on ArchLinux  
* qt6-base  
* qt6-tools  
* libevdev  
* gnome-shell-extension-appindicator (Required for tray support on Gnome)  

tablet-switch can now be executed as a program. 
```bash
$ cd ./build && ./tablet-switch
```

![Screenshot from 2023-12-30 11-25-58](https://github.com/ademarest/tablet-switch/assets/17366618/ecbde600-9d92-4e91-b106-f3dcb2556698)
