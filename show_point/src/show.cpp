#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <fstream>
#include <cmath>
#include <vector>

#define PATH "config/node_list.txt"
#define PATH2 "config/match_node.txt"
#define ORIGIN "config/origin.txt"

struct goal_node
{
    float x,y;
};

std::vector<goal_node> node_list;
std::vector<goal_node> match_node;

void creat_node()
{
  goal_node P;
  float x,y,n,or_x,or_y;
	std::ifstream ifs;
  ifs.open(ORIGIN, std::ios::in);
	ifs>> or_x>> or_y;
	ifs.close();
	ifs.open(PATH, std::ios::in);

	while (!ifs.eof())
	{
		ifs>>n>> y >> x;
        P.x=or_x+x*0.05;
        P.y=or_y+y*0.05;
        node_list.push_back(P);
	}
	ifs.close();
}

void read()
{
  goal_node P,P1;

	std::ifstream ifs;
	ifs.open(PATH2, std::ios::in);
  int a,b,c,d,e,f;
	while (!ifs.eof())
	{
		ifs>>a>>b>>c>>d>>e>>f;;
    P.x=node_list[a].x;
    P.y=node_list[a].y;
    match_node.push_back(P);
    P1.x=node_list[d].x;
    P1.y=node_list[d].y;
    match_node.push_back(P1);
	}
	ifs.close();
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "SHOW");
  ros::NodeHandle n;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("NODE", 10);

  creat_node();
  //read();

  ros::Rate r(30);
  float f = 0.0;

  while (ros::ok())
  {

    // visualization_msgs::Marker points,line_strip;
    // points.header.frame_id = line_strip.header.frame_id = "map";
    // points.header.stamp = line_strip.header.stamp = ros::Time::now();
    // points.ns = line_strip.ns = "node";
    // points.action = line_strip.action = visualization_msgs::Marker::ADD;
    // points.pose.orientation.w = line_strip.pose.orientation.w = 1.0;

    visualization_msgs::Marker points;
    points.header.frame_id ="map";
    points.header.stamp =ros::Time::now();
    points.ns = "node";
    points.action =visualization_msgs::Marker::ADD;
    points.pose.orientation.w =1.0;

    points.id = 0;
    //line_strip.id = 1;


    points.type = visualization_msgs::Marker::POINTS;
    //line_strip.type = visualization_msgs::Marker::LINE_STRIP;



    points.scale.x = 0.1;
    //line_strip.scale.x = 0.01;

    points.color.r =0.93f;
    points.color.g = 0.93f;
    points.color.b =0.0f;
    points.color.a = 1.0;

    // line_strip.color.r =0.55f;
    // line_strip.color.g =0.10f;
    // line_strip.color.b =0.10f;
    // line_strip.color.a = 0.5;

    geometry_msgs::Point p;
    for (auto t:node_list)
    {
      p.x=t.x;
      p.y=t.y;
      points.points.push_back(p);
    }
    marker_pub.publish(points);
    //marker_pub.publish(line_strip);
    r.sleep();
  }
}
