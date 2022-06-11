#Nandax [Jun 11, 2022]: CMakeLists follow ros_essential_cpp by Prof. Anis Kouba:
#    https://github.com/aniskoubaa/ros_essentials_cpp/tree/ros-noetic
#
####################################################################

cmake_minimum_required(VERSION 2.8.3)
project(PART_B_C)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  actionlib_msgs
  image_transport
  message_generation
)


## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   IoTSensor.msg 
#)

## Generate services in the 'srv' folder
# add_service_files(
# FILES 
# AddTwoInts.srv
# )

## Generate actions in the 'action' folder
add_action_files(
  DIRECTORY action
  FILES Fibonacci.action
)

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
   actionlib_msgs
 )


###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES PART_B_C
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)


#robot cleaner
add_executable(robot_follow_waypoints src/robot_follow_wp.cpp)
target_link_libraries (robot_follow_waypoints ${catkin_LIBRARIES})
