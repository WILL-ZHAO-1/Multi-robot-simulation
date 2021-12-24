#pragma once
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
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


struct goal_node
{
    float x,y,z,w;
};

//与move_base通信的action
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class MAP_TO_NODE;
class multirobot_client
{
    public:
    multirobot_client();
    //通信连接
    bool establish_connect();
    //建立节点列表
    void creat_node_list();
    //执行函数
    void execute();
    //取下一时刻节点
    void get_exe_node();
    //四元数计算
    void count_quaternions(float &x, float &y, float *b);
    //目标点预处理
    void preprocess_goals(int &g);
    //目标点赋值
    void assignment_goal(move_base_msgs::MoveBaseGoal &goal);
    //监听坐标线程
    void listern_pose_thread();
    //监听实时坐标
    void pose_callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg,int* p);
    //节点协调
    void node_coordinate();
    //判断是否到达
    void achieve_goal(goal_node &g,move_base_msgs::MoveBaseGoal &goal);
    //节点范围判断
    bool achieve_node(const int &g);

    ~multirobot_client();

    private:
    std::vector<goal_node> node_list;
    std::deque<int> global_path;
    std::deque<int> exe_node;
    goal_node send_goal;
    MAP_TO_NODE* NODE;


    int clientsocket;
    sockaddr_in client_addr;

    int current_node_list[1];
    int send_buffer[3];

    bool Get_Goal;
    bool robot_stop;

};