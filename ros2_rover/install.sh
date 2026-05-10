cd YDLidar-SDK
mkdir build && cd build

cmake ..
make -j2 # Use 2 cores for building, adjust as needed
sudo make install

cd ../workspace
rm -rf build/ install/ log/

colcon build --packages-select ydlidar --symlink-install

cd src/ydlidar/startup
sudo chmod 777 ./*
sudo sh initenv.sh

cd ../../..
source install/setup.bash