#include "map_to_node.h"
using namespace std;

MAP_TO_NODE::MAP_TO_NODE()
{
	this->table.clear();
	this->list.clear();
	this->creat_kdtree();
}

void MAP_TO_NODE::split(set<tree_node> &t, tree_node* &p) {
	//子节点set容器
	set<tree_node> s1, s2;
	//确定分割点
	set<tree_node>::iterator i = t.begin();
	for (int j = 0; j < (t.size() / 2); ++j) {
		i++;
	}
	//于前驱节点建立关系
	tree_node* p1 = new tree_node;
	p1->name = i->name;
	p1->y = i->y;
	p1->x = i->x;
	p1->L = NULL;
	p1->R = NULL;
	p = p1;
	//按照分割值进行划分
	float s = p1->x;
	for (set<tree_node>::iterator it = t.begin(); it != t.end(); ++it) {
		if (it->x < s) {
			s1.insert(*it);
		}
		else if (it->x>s) {
			s2.insert(*it);
		}
	}
	if (s1.size() != 0) {
		this->split(s1, p1->L);
	}
	if (s2.size() != 0) {
		this->split(s2, p1->R);
	}
}

void MAP_TO_NODE::top_split()
{
	//子节点set容器
	set<tree_node> s1, s2;
	//确定分割点
	set<tree_node>::iterator i = list.begin();
	for (int j = 0; j < list.size() / 2; ++j) {
		i++;
	}
	//于前驱节点建立关系
	tree_node top;
	top.name = i->name;
	top.y = i->y;
	top.x = i->x;
	top.L = NULL;
	top.R = NULL;
	//按照分割值进行划分
	float s = top.x;
	for (set<tree_node>::iterator it = list.begin(); it != list.end(); ++it) {
		if (it->x < s) {
			s1.insert(*it);
		}
		else if (it->x>s) {
			s2.insert(*it);
		}
	}
	if (s1.size() != 0) {
		this->split(s1, top.L);
	}
	if (s2.size() != 0) {
		this->split(s2, top.R);
	}
	this->table.push_back(top);
	this->list.clear();
}

void MAP_TO_NODE::creat_kdtree()
{
	tree_node P;
	deque<tree_node> temp_list;
  	float y,x,pre_clos,or_x,or_y;

	ifstream ifs;
	ifs.open(ORIGIN, ios::in);
	ifs>> or_x>> or_y;
	ifs.close();
	
	//read the node list
	ifs.open(PATH, ios::in);
	while (!ifs.eof())
	{
		ifs >> P.name >> y >> x;
    	P.x=or_x+x*0.05;
    	P.y=or_y+y*0.05;
		temp_list.push_back(P);
	}
	ifs.close();

	//creat the kd tree
	pre_clos = temp_list.front().y;
	while (!temp_list.empty()) {
		P = temp_list.front();
		//换行了，并且不为空
		if (pre_clos != P.y && !this->list.empty()) {
			pre_clos = P.y;
			this->top_split();
		}		
		list.insert(P);
		temp_list.pop_front();
	}
	this->top_split();
}

int MAP_TO_NODE::find_node(float x,float y)
{
	//判断距离最近的行
	int min;
	float dis, com=100;
	for (int i = 0; i < this->table.size(); ++i)
	{
		dis= abs(table[i].y - y);
		if (dis < com) {
			com = dis;
			min = i;
		}
	}

	//判断该行中最近的列
	tree_node top = table[min];
	if (top.x == x)
	{
		return top.name;
	}
	tree_node* P = &top;
	tree_node* temp = P;
	this->path.push(P);
	while (P->L != NULL && P->R != NULL) {
		if (P->x == x) {
			this->min_name=temp->name;
			break;
		}
		else if (P->x > x){
			P = temp->L;
		}
		else {
			P = temp->R;
		}
		temp = P;
		this->path.push(P);
	}
	//遍历完成后，开始回溯
	this->min_dis = abs(P->x - x);
	this->min_name = P->name;
	path.pop();
	while (!path.empty()) {
		float temp_dis = abs(path.top()->x - x);
		if (temp_dis < min_dis) {
			this->min_dis = temp_dis;
			this->min_name = path.top()->name;
		}
		path.pop();
	}
  return this->min_name;
}

MAP_TO_NODE::~MAP_TO_NODE()
{
	
}
