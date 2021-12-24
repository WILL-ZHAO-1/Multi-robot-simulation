#include "map.h"
using namespace std;

Graph::Graph()
{
	this->initial();
	this->read();
	this->creat_arc();
}

void Graph::initial()
{
	//填充边权重为999，两边不可达
	for (int i = 0; i < MAX; ++i) {
		for (int j = 0; j < MAX; ++j) {
			this->m_matrix[i][j] = 999;
		}
	}
	//this->print();
	//顶点可达性，0未访问，1已经访问
	/*for (int k = 0; k < MAX; ++k) {
	m_vertable.push_back(0);
	}*/
}

void Graph::creat_arc()
{
	graph_node g1, g2;
	int i, j;
	float weight;
	for (auto arc : this->arc_set) {
		g1 = arc.first;
		g2 = arc.second;
		i = g1.g_name;
		j = g2.g_name;

		weight = this->calculate_weight(g1, g2);
		//填充邻接矩阵
		this->m_matrix[i][j] = weight;
		m_matrix[j][i] = weight;
	}
	//this->print();
	//this->short_path(39, 88);
	//this->Heuristically_path(39,88);
}

void Graph::read()
{
	//读取边集
	ifstream ifs;
	ifs.open(TEXT4, ios::in);
	graph_node p1, p2;
	while (!ifs.eof())
	{
		ifs >> p1.g_name >> p1.g_y >> p1.g_x
			>> p2.g_name >> p2.g_y >> p2.g_x;
		this->arc_set.insert(make_pair(p1, p2));
	}
	ifs.close();
	//cout << arc_set.size() << endl;

	//读取定点
	ifstream ifs2;
	ifs2.open(TEXT1, ios::in);
	graph_node p;
	while (!ifs2.eof())
	{
		ifs2 >> p.g_name >> p.g_y >> p.g_x;
		p.g_visit = false;
		this->m_vertable.push_back(p);
	}
	ifs2.close();
	//cout << m_vertable.size() << endl;
}

float Graph::calculate_weight(graph_node &g1, graph_node &g2)
{
	int x1, y1, x2, y2;
	x1 = g1.g_x;
	y1 = g1.g_y;
	x2 = g2.g_x;
	y2 = g2.g_y;
	float temp1 = static_cast<float>((x1 - x2)*(x1 - x2));
	float temp2 = static_cast<float>((y1 - y2)*(y1 - y2));
	float weight = static_cast<float>(sqrt(temp1 + temp2)*0.05);
	return weight;
}

void Graph::print()
{
	for (int i = 0; i < MAX; ++i) {
		for (int j = 0; j < MAX; ++j) {
			cout << this->m_matrix[i][j] << " ";
		}
		cout << endl;
	}

	/*for (auto t : this->arc_set) {
	cout << t.first.g_name << " " << t.first.g_y << " " << t.first.g_x << ";"
	<< t.second.g_name << " " << t.second.g_y << " " << t.second.g_x << endl;
	}*/
}

void Graph::short_path(int S, int F)
{
	//前驱节点表
	int ahead_node[MAX] = { 0 };
	//起始点到该点的距离
	float dis[MAX];
	for (int k = 0; k < MAX; ++k) {
		dis[k] = 9999;
	}

	float ahead_dis = 0;
	this->m_vertable[S].g_visit = true;;
	int s = S;
	//到达终点结束
	int count = 0;
	while (s != F) {
		//遍历该点的邻节点
		for (int i = 0; i < MAX; ++i) {
			float temp_dis = this->m_matrix[s][i] + ahead_dis;
			if (dis[i] > temp_dis) {
				dis[i] = temp_dis;
				ahead_node[i] = s;
			}
		}
		//找到距离最短且未访问过的节点
		float t = 100;
		int p = 0;
		for (int j = 0; j < MAX; ++j) {
			if (dis[j] < t && this->m_vertable[j].g_visit == false) {
				t = dis[j];
				p = j;
			}
		}
		//更新最短点
		m_vertable[p].g_visit = true;
		ahead_dis = dis[p];
		s = p;
		count++;
	}
	/*for (auto m : ahead_node) {
	cout << m << " ";
	}
	cout << endl;*/

	//打印路径
	int f = F;
	stack<int> route;
	route.push(f);
	while (true) {
		int temp = ahead_node[f];
		route.push(temp);
		f = temp;
		if (f == S) {
			break;
		}
	}
	//输出路径
	while (!route.empty()) {
		cout << route.top() << " ";
		route.pop();
	}
	cout << endl;
	cout << "迭代次数" << count << endl;
}

stack<int>* Graph::Heuristically_path(int S, int F)
{
	//前驱节点表
	int ahead_node[MAX] = { 0 };
	//起始点到该点的距离
	float dis[MAX];
	for (int k = 0; k < MAX; ++k) {
		dis[k] = 9999;
	}

	float ahead_dis = 0;
	this->m_vertable[S].g_visit = true;;
	int s = S;
	//到达终点结束
	int count = 0;
	while (s != F) {
		//遍历该点的邻节点
		for (int i = 0; i < MAX; ++i) {
			float temp_his = calculate_forwardcost(F, i);
			float temp_dis = this->m_matrix[s][i] + ahead_dis + temp_his;
			if (dis[i] > temp_dis) {
				dis[i] = temp_dis;
				ahead_node[i] = s;
			}
		}
		//找到距离最短且未访问过的节点
		float t = 100;
		int p = 0;
		for (int j = 0; j < MAX; ++j) {
			if (dis[j] < t && this->m_vertable[j].g_visit == false) {
				t = dis[j];
				p = j;
			}
		}
		//更新最短点
		m_vertable[p].g_visit = true;
		ahead_dis = dis[p];
		s = p;
		count++;
	}
	/*for (auto m : ahead_node) {
	cout << m << " ";
	}
	cout << endl;*/
	for (int it=0;it<m_vertable.size();++it){
		m_vertable[it].g_visit=false;
	}

	//打印路径
	int f = F;
	m_path.push(f);
	while (true) {
		int temp = ahead_node[f];
		f = temp;
		if (f == S) {
			break;
		}
		m_path.push(temp);
	}
	//输出路径
	// while (!m_path.empty()) {
	// 	cout << m_path.top() << " ";
	// 	m_path.pop();
	// }
	// cout << endl;
	// cout << "迭代次数" << count << endl;
	return &m_path;
}

float Graph::calculate_forwardcost(int F, int N)
{
	graph_node f_p, n_p;
	f_p = this->m_vertable[F];
	n_p = m_vertable[N];
	//计算前进代价值
	int x1, y1, x2, y2;
	x1 = f_p.g_x;
	y1 = f_p.g_y;
	x2 = n_p.g_x;
	y2 = n_p.g_y;
	float temp1 = static_cast<float>((x1 - x2)*(x1 - x2));
	float temp2 = static_cast<float>((y1 - y2)*(y1 - y2));
	float cost = static_cast<float>(sqrt(temp1 + temp2)*0.05);
	return cost;
}

void Graph::delete_arc(int a, int b)
{
	//显示当前两点之间的关系
	cout << this->m_matrix[a][b] << endl;
	//删除边关系
	m_matrix[a][b] = 999;
}
