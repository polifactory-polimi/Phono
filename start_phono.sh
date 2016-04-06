#!/bin/bash

echo "Starting Pd..."
/usr/bin/puredata -noadc -nogui -audiooutdev 3 -audiobuf 100 \
-blocksize 1024 -rt /home/pi/phono/phono.pd &
