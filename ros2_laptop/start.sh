#!/bin/bash

# Give docker perms to access the display
xhost +local:docker


docker compose up -d
# Open a bash terminal inside the container
docker exec -it ros_jazzy_controller bash