## Intro  
Tablet-switch is a systemtray program to manually toggle different operating modes for your 2-in-1 on Linux.  
Here's what the different operating modes do:  
* **Tablet Mode**, both touch and stylus input are available. The built-in keyboard and trackpad are disabled.  
* **Stylus Only Mode**, only the sytlus is available for input. The touch screen, built-in keyboard, and trackpad are disabled.
* **Computer Mode** all inputs will be enabled.

tablet-switch currently works on my HP ENVY x360 Convert 13-ay1035nr.  
It should work on other laptops with a tablet switch device.  
  
The software will scan through input devices looking 2-in-1 tablet switches which have the event codes EV_SW,SW_TABLET_MODE.  
Those devices will be populated and you can select the tablet switch for your device.

Upon selecting either Tablet mode or Stylus Only Mode, a SW_TABLET_MODE event will be sent via the tablet switch input device.  
In my case the tablet switch input device is "HP WMI Hotkeys".  
The SW_TABLET_MODE event will alert the operating system that the computer is in tablet mode.  
Gnome for instance will disable the keyboard and trackpad while enabling the on screen keyboard and screen rotation.
  
## Getting started  
Review the code.  
Clone the repo.  
Install Qt Creator.  
Open the .pro file.  
Setup your build environment.  
Compile!  
  
You will need to add your user to the input group.  
This is **not** desireable from a security perspective.  
TODO: Figure out if it's possible to do this via udev.  
```bash
# useradd -aG input andrew
```
The following packages are required to run this on ArchLinux  
* gnome-shell-extension-appindicator  
* qt6-base  
* libevdev  
  
Finally you can run the executable bin tablet-switch.

![Screenshot from 2023-12-30 11-25-58](https://github.com/ademarest/tablet-switch/assets/17366618/ecbde600-9d92-4e91-b106-f3dcb2556698)
