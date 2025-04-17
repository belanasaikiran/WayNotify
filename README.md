# HyprNotify [work in progress]

A background service that listens for desktop notifications (via D-Bus), then displays them on-screen using Wayland protocols.

Learning the notification system and I'm implementing my own notification daemon for Hyprland similar to dunst.


### Libraries

- dbus
- wlroots
- Cairo
- lohmann/json


Project Tree:

```css
notification-daemon/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── dbus_listener.cpp / .hpp   ← Listens for Notify()
│   ├── renderer.cpp / .hpp        ← Wayland surface & draw
│   ├── notification.cpp / .hpp    ← Notification object
│   └── config.cpp / .hpp          ← Config reading (optional)
├── assets/ (fonts, icons)
└── config.json
```
