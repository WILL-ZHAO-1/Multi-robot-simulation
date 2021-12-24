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
#define PATH "config/goal.txt"
#define ORIGIN "config/orign.txt"
#define pi 3.14159265


struct goal_node {
	float x, y, z,w;
};

//重命名，简化名字
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

vector<goal_node> node_list;
deque<goal_node> temp2;
deque<goal_node> send_goal;
deque<goal_node> send_goal2;
std::atomic<bool> pose_check={false};

void DoShutdown(int sig){
	ROS_INFO("shutting down!");
	ros::shutdown(); 
    exit(sig);
}
void count_quaternions(float &x, float &y, float *b){
	float Z;
	if (x == 0 && y>=0) {
		b[0] = sin(pi/4);
		b[1] = cos(pi/4);
		return;
	}
	else if (x == 0 && y < 0) {
		Z = -pi;
		b[0] = -sin(pi/4);
		b[1] = cos(pi/4);
		return;
	}

	float anger = atan(y / x);
	if (x > 0) {
		Z = anger;
	}
	else if (x < 0 && y>=0) {
		Z = pi + anger;
	}
	else if (x < 0 && y < 0) {
		Z = anger-pi;
	}
	else {
		cout << "坐标违法";
		cout << y << " " << x << endl;
	}
	float z,w,t;
	z= sin(Z / 2);
	w = cos(Z / 2);
	t = 1e-3;
	if (abs(z) <= t) {
		z = 0;
	}
	if (abs(w) <= t) {
		w = 0;
	}
    b[0] = z;
	b[1] = w;
	return;
}
void read(){
    goal_node P;
    float x,y,n,or_x,or_y;
	ifstream ifs;
    ifs.open(ORIGIN, ios::in);
	ifs>> or_x>> or_y;
	ifs.close();
	ifs.open(PATH, ios::in);

	while (!ifs.eof())
	{
		ifs>>n>> y >> x;
        P.x=or_x+x*0.05;
        P.y=or_y+y*0.05;
        node_list.push_back(P);
	}
	ifs.close();
}
void get_goal(){
	goal_node p,p2, p3;
	for (int i=0;i<node_list.size();++i){
		p.x = node_list[i].x;
		p.y = node_list[i].y;
		temp2.push_back(p);
		while (temp2.size() > 1) {
			p2 = temp2.front();
			p3 = temp2.back();
			float x = p3.x - p2.x;
			float y = p3.y - p2.y;
			float arr[2] = { 0 };
			count_quaternions(x, y, arr);
			p2.z = arr[0];
			p2.w = arr[1];
			send_goal.push_back(p2);
			temp2.pop_front();
	    }
	}
	cout << send_goal.size() << endl;
}
void GOAL(move_base_msgs::MoveBaseGoal &goal){
    goal_node g;
    g=send_goal.front();
    // cout<<g.x<<endl;
    // cout<<g.y<<endl;
    // cout<<g.z<<endl;
    // cout<<g.w<<endl;
    goal.target_pose.header.frame_id="map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x=g.x;
    goal.target_pose.pose.position.y=g.y;
    goal.target_pose.pose.orientation.z=g.z;
    goal.target_pose.pose.orientation.w=g.w;
    send_goal.pop_front();
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"send_goal");
    MoveBaseClient ac("move_base",true);
    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    move_base_msgs::MoveBaseGoal goal_A;
    read();
    get_goal();
	send_goal2=send_goal;
    GOAL(goal_A);
    ac.sendGoal(goal_A);

    while (true){
		if (pose_check==true)
		{
			GOAL(goal_A);
			ac.sendGoal(goal_A);
		}
		pose_check=false;
        ac.waitForResult();
        if (send_goal.empty())
        {
            break;
        }
    }
    return 0;
}