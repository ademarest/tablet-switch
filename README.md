## Intro  
tablet-switch currently works on my HP ENVY x360 Convert 13-ay1035nr.  
  
This program creates a system tray icon to toggle different operating modes for your 2-in-1.  
Here's what the different operating modes do:  
* **Tablet Mode**, both touch and stylus input are available. The built-in keyboard and trackpad are disabled.  
* **Stylus Only Mode**, only the sytlus is available for input. The touch screen, built-in keyboard, and trackpad are disabled.
* **Computer Mode** all inputs will be enabled.

Upon selecting either Tablet mode or Stylus Only Mode, HP WMI Hotkeys will be triggerd.  
HP WMI Hotkeys lets the operating system know that it's in tablet mode.  
Next it will disable the touchpad and keyboard via libevdev.  
  
## Getting started  
Install Qt Creator.  
Open the .pro file.  
Setup your build environment.  
Compile!  
  
You will need to add your user to the input group.  
This is not desireable from a security perspective.  
```bash
# useradd -aG input andrew
```
The following packages are required to run this on ArchLinux  
* gnome-shell-extension-appindicator  
* qt6-base  
* libevdev  
  
Finally you can run the executable bin tablet-switch.
