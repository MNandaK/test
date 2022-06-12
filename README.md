# 0. Intro
This works explain some points regarding mobile robot and coding in ROS


# 1. How to Launch in docker

#Create a container for roscore
sudo docker run -it --net=host --name ros_docker_1 osrf/ros:noetic-desktop-full roscore 

#Create a container for any purpose with graphical support
sudo docker run -it --net=host --name [a_ros_docker_N] \
    --env="DISPLAY=:0.0" \
    --env="QT_X11_NO_MITSHM=1" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --volume="/dev/dri:/dev/dri" \
    osrf/ros:noetic-desktop-full \
    bash

#Run roscore
sudo docker container start os_docker_1 -i

#Run turtle sim
sudo docker container start [a_ros_docker_2nd] -i
rosrun turtlesim turtlesim_node

#Run robot to follow waypoints
sudo docker container start [a_ros_docker_3nd] -i
rosrun robot_follow_waypoints robot_follow_waypoints

#Run rqt_graph
sudo docker container start [a_ros_docker_4th] -i
rosrun rqt_graph rqt_graph




