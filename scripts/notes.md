I'm in Hyprland right now!

I already have swaync active and I have to disable this to make my app work.

disable swaync in hyprland.conf

Disable and Mask swaync.service

```bash
systemctl --user list-units --type=service | grep swaync
```

If running,

```bash
systemctl --user stop swaync.service
systemctl --user disable swaync.service
systemctl --user mask swaync.service
```



### Tests

To test the app, run `notify-send "HELLO" -t 3000` in bash and see if you receive any notification.
