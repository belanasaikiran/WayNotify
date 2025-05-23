# WayNotify 

A background service that listens for desktop notifications (via D-Bus), then displays them on-screen using Wayland protocols.

Learning the notification system and I'm implementing my own notification daemon for Hyprland similar to dunst.

![Screenshot](./showoff/hyprshot.png)


### Libraries

- dbus
- wlroots
- SDL2
- lohmann/json


Build & Run

```
mkdir build
cd build
cmake ..
make
```


TODO:
- Implement notification stacking
- Add support for actions and buttons
- Improve error handling and logging
- Optimize performance and resource usage
