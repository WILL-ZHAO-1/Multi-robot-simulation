#pragma once
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <deque>
#include <set>
#include <stack>
#include "multirobot_client.h"

#define PATH "config/multi_robot/node_list.txt"

using namespace std;
//KD-tree struct
struct tree_node
{
    int name;
    float x,y;
    tree_node* L;
    tree_node* R;

    bool operator < (const tree_node &a) const
    {
        return a.x>x;
    }
};

class MAP_TO_NODE
{
    friend class multirobot_client;

    public:
    MAP_TO_NODE();
    //对外接口，输入点的坐标、返回节点序号
	int find_node(float y, float x);
    ~MAP_TO_NODE();
    //构建kd tree
    private:
    void split(set<tree_node> &t, tree_node* &p);
	void top_split();
	void creat_kdtree();

    private:
    vector<tree_node> table;
	set<tree_node> list;
	stack<tree_node*> path;
	float min_dis;
	int min_name;
};