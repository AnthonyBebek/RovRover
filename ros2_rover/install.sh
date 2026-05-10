#!/bin/bash
set -e  # Stop on error

echo "=== Starting YDLIDAR Installation ==="

# 1. Build and install YDLidar-SDK
echo "Building YDLidar-SDK..."
cd YDLidar-SDK
rm -rf build 2>/dev/null || true
mkdir -p build && cd build

cmake ..
make -j2
sudo make install

# Return to workspace root
cd ../../

# 2. Build the ROS 2 driver
echo "Building ROS 2 ydlidar driver..."
cd workspace

rm -rf build/ install/ log/ 2>/dev/null || true

colcon build --packages-select ydlidar --symlink-install --event-handlers console_direct+

# 3. Run startup script
echo "Running YDLIDAR startup script..."
cd src/ydlidar/startup
sudo chmod 777 ./*
sudo sh initenv.sh

# 4. Source
cd ../../..
echo "Sourcing workspace..."
source install/setup.bash

echo "=== Installation Finished ==="
echo "You can now try: ros2 launch ydlidar ydlidar_launch.py"