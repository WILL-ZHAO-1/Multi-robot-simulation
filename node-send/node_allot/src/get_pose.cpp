#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <deque>

using namespace std;

void amcl_pose_callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg)
{
  ros::Time begin = ros::Time::now();
  cout<<begin<<endl;
  cout<< msg->pose.pose.position.x <<endl;
  cout<< msg->pose.pose.position.y <<endl;
}

int main(int argc,char** argv)
{
  ros::init(argc,argv,"get_pose");
	ros::NodeHandle n;
	ros::Subscriber sub=n.subscribe("/amcl_pose", 1, &amcl_pose_callback);
  ros::Rate looprate(0.5);

  while (ros::ok())
  {
    ros::spinOnce();
    looprate.sleep();
  }
  return 0;
}