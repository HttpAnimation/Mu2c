#!/bin/bash

# Compile the program
gcc -o music_player Mu2c.c $(pkg-config --cflags --libs gstreamer-1.0) `pkg-config --cflags --libs gtk+-3.0`

# Remove old contents and add date and time to Version.ini
echo "$(date +'%Y-%m-%d %H:%M:%S')" > Version.ini
