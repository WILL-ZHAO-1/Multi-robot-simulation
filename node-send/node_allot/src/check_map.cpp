#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <signal.h>
#include <thread>
#include <fstream>
#include <std_msgs/String.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <geometry_msgs/Twist.h>
#include <ctime>
#include <mutex>
#include "map_to_node.h"

#define NODE_LIST "config/multi_robot/node_list.txt"
#define ORIGIN "config/multi_robot/origin.txt"
#define pi 3.14159265
#define port 10000


class MAP_TO_NODE;
class ckeck
{
    public:
    ckeck();
    //建立节点列表
    void creat_node_list();
    //监听坐标线程
    void listern_pose_thread();
    //监听实时坐标
    void pose_callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg,int* p);
    ~ckeck();

    private:
    std::vector<goal_node> node_list;
    MAP_TO_NODE* NODE;

    int clientsocket;
    sockaddr_in client_addr;

    int current_node_list[1];
    int send_buffer[3];
};

const char* _ip;
mutex Cur_node_lock;

ckeck::ckeck()
{
    this->NODE=new MAP_TO_NODE;
    //this->creat_node_list();
    //establish_connect();
	listern_pose_thread();
    //close(clientsocket);
}

void ckeck::creat_node_list()
{
    goal_node P;
    float x,y,n,or_x,or_y;
	ifstream ifs;
    //获取地图原点
    ifs.open(ORIGIN, ios::in);
	ifs>> or_x>> or_y;
	ifs.close();
    //获取节点列表
	ifs.open(NODE_LIST, ios::in);

	while (!ifs.eof())
	{
		ifs>>n>> y >> x;
        P.x=or_x+x*0.05;
        P.y=or_y+y*0.05;
        node_list.push_back(P);
	}
	ifs.close();
}

void ckeck::listern_pose_thread()
{
    int current_node;
    ros::NodeHandle lis_pos;
    ros::Subscriber lis=lis_pos.subscribe<geometry_msgs::PoseWithCovarianceStamped>("/amcl_pose",1,
    boost::bind(&ckeck::pose_callback,this,_1,&current_node));
    while (ros::ok())
    {
        ros::spinOnce();
        if (current_node > -1)
        {
	    cout<<"节点序号：";
            cout<<current_node<<endl;
            Cur_node_lock.lock();
            current_node_list[0]=current_node;
            Cur_node_lock.unlock();
        }
        sleep(3);
    }
}

void ckeck::pose_callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg,int* p)
{
    //cout<< msg->pose.pose.position.x <<";"<< msg->pose.pose.position.y<<endl;
	float x,y;
	x=msg->pose.pose.position.x;
	y=msg->pose.pose.position.y;
    *p=this->NODE->find_node(x,y);
}

ckeck::~ckeck()
{
    node_list.clear();

   if (NODE !=NULL){
       delete NODE;
   }  
}

int main(int argc, char** argv)
{
  std::cout<<"<--节点检测-->"<<std::endl;
  ros::init(argc, argv, "ckeck");
  ckeck client;
  ros::spin();
  return(0);
}
