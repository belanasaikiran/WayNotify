Tree:

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
