#!/bin/bash

colcon build --packages-select ros2_rover
source install/setup.bash
ros2 run ros2_rover ros2_rover_node