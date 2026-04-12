#!/bin/bash

# Give docker perms to access the display
xhost +local:docker
# Give permissions to access the docker socket and entrypoint script
sudo chmod 666 /var/run/docker.sock
sudo chmod +x ./entrypoint.sh

# Start the docker container
docker compose up -d
# Open a bash terminal inside the container
docker exec -it ros_jazzy_controller bash