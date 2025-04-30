#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <on/off>"
    exit 1
fi

if [ "$1" = "off" ]; then
    systemctl --user stop swaync.service
    systemctl --user disable swaync.service
    systemctl --user mask swaync.service
elif [ "$1" = "on" ]; then
    systemctl --user unmask swaync.service
    systemctl --user enable swaync.service
    systemctl --user start swaync.service
else
    echo "Invalid argument: $1"
    echo "Usage: $0 <on/off>"
    exit 1
fi
