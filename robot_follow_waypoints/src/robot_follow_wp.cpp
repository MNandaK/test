/* Originally "robot_cleaner.cpp" by:
 *  Author: Anis Koubaa
 *  Year: 2018
 *  Prince Sultan University
 *  Robotics and Internet of Things Lab
 * 
 * Currently modified some parts by Nandax:
 *  Date: 220611
 */


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
#include <vector>

using std::cout;
using std::vector;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;


const double x_min = 0.0;
const double y_min = 0.0;
const double x_max = 11.0;
const double y_max = 11.0;

const double PI = 3.14159265359;

double degrees2radians(double angle_in_degrees);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void moveGoal(turtlesim::Pose  goal_pose, double distance_tolerance);

// Nandax [220612]: new function moveToWaypoints() to make the robot move to several points in sequence
void moveToWaypoints(vector<turtlesim::Pose> waypoint_poses, double distance_tolerance);

void robotMission();

int main(int argc, char **argv)
{
	// Initiate new ROS node named "talker"
	ros::init(argc, argv, "robot_folow_wp");
	ros::NodeHandle n;
	double speed, angular_speed;
	double distance, angle;
	bool isForward, clockwise;


	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);

	/** test your code here **/
	ROS_INFO("\n\n\n******START TESTING************\n");
	ros::Rate loop_rate(0.5);
	robotMission();
	ros::spin();

	return 0;
}


double degrees2radians(double angle_in_degrees)
{
	return angle_in_degrees *PI /180.0;
}


void poseCallback(const turtlesim::Pose::ConstPtr & pose_message)
{
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}

double getDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

void moveGoal(turtlesim::Pose  goal_pose, double distance_tolerance)
{

	geometry_msgs::Twist vel_msg;

	ros::Rate loop_rate(100);
	double E = 0.0;
	do{
		/****** Proportional Controller ******/
		//linear velocity in the x-axis
		double Kp=1.0;
		double Ki=0.02;
		//double v0 = 2.0;
		//double alpha = 0.5;
		double e = getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y);
		double E = E+e;
		//Kp = v0 * (exp(-alpha)*error*error)/(error*error);
		vel_msg.linear.x = (Kp*e);
		vel_msg.linear.y =0;
		vel_msg.linear.z =0;
		//angular velocity in the z-axis
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z =4*(atan2(goal_pose.y-turtlesim_pose.y, goal_pose.x-turtlesim_pose.x)-turtlesim_pose.theta);

		velocity_publisher.publish(vel_msg);

		ros::spinOnce();
		loop_rate.sleep();

	}while(getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)>distance_tolerance);
	//cout<<"end move goal"<<endl;
	vel_msg.linear.x =0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);
}

void moveToWaypoints(vector<turtlesim::Pose> waypoint_poses, double distance_tolerance)
{
	for (unsigned int point = 0; point < waypoint_poses.size(); point++)
	{
		ROS_INFO_STREAM("Move to point " << point+1 << ", target coordinate (" <<
			waypoint_poses[point].x << ", " << waypoint_poses[point].y << ")" );

		moveGoal(waypoint_poses[point], distance_tolerance);
	}
	cout << "\n";
	ROS_INFO_STREAM("The robot has gone through all waypoints,\nMISSION ACCOMPLISHED!" );
}

void robotMission(){

	ros::Rate loop(0.5);
	// turtlesim::Pose pose;
	// pose.x=1;
	// pose.y=1;
	// pose.theta=0;
	// moveGoal(pose, 0.01);

	vector<turtlesim::Pose> waypoint_poses;

	turtlesim::Pose pose1;
	pose1.x = 1.5;
	pose1.y = 1.5;
	waypoint_poses.push_back(pose1);

	turtlesim::Pose pose2;
	pose2.x = 7;
	pose2.y = 7;
	waypoint_poses.push_back(pose2);

	turtlesim::Pose pose3;
	pose3.x = 3.2;
	pose3.y = 3.2;
	waypoint_poses.push_back(pose3);

	turtlesim::Pose pose4;
	pose4.x = 10;
	pose4.y = 4;
	waypoint_poses.push_back(pose4);
	
	turtlesim::Pose pose5;
	pose5.x = 1;
	pose5.y = 9;
	waypoint_poses.push_back(pose5);

	moveToWaypoints(waypoint_poses, 0.01);


	loop.sleep();
	
	double distance = getDistance(turtlesim_pose.x, turtlesim_pose.y, x_max, y_max);

}








