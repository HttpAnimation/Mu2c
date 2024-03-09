#!/bin/bash

# Compile the program
gcc `pkg-config --cflags gtk+-3.0` -o Mu2c Mu2c.c `pkg-config --libs gtk+-3.0`

# Add date and time to Version.ini
echo "$(date +'%Y-%m-%d %H:%M:%S')" >> Version.ini
