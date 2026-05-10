cd ~/RovRover/ros2_rover

# === Build YDLidar-SDK ===
cd YDLidar-SDK
rm -rf build 2>/dev/null || true
mkdir -p build && cd build

cmake ..
make -j2
sudo make install

# Go back to main folder
cd ~/RovRover/ros2_rover

# === Build ROS driver ===
cd workspace
rm -rf build/ install/ log/ 2>/dev/null || true

colcon build --packages-select ydlidar --symlink-install --event-handlers console_direct+

# === Startup script ===
cd src/ydlidar/startup
sudo chmod 777 ./*
sudo sh initenv.sh

# === Source ===
cd ~/RovRover/ros2_rover/workspace
source install/setup.bash

echo "Done!"