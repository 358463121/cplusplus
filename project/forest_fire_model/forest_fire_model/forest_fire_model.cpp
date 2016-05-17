// forest_fire_model.cpp.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <fstream>
#include <Eigen/SparseCore>

using namespace std;
using namespace Eigen;
SparseMatrix<bool> G; // declares a column-major sparse matrix type of double
int t = 0;
int vt = 1;
int Vn;
const float p = 0.5;
const float r = 0.5;
default_random_engine generator;
geometric_distribution<int> dx(1 - p);
geometric_distribution<int> dy(1 - r*p);

bool* visited;
vector<int> sample(vector<int> n, unsigned int size) {
	if (n.size()<size) {
		return n;
	}
	random_shuffle(n.begin(), n.end());
	return vector<int>(n.begin(), n.begin() + size);

}
void AddNode(int v = 0, int w = 0) {
	if (t>Vn-2)return;
	if (v == 0) {
		t = t + 1;
		v = t;
		uniform_int_distribution<int> dunif(0, t - 1);
		w = dunif(generator);
		for (int i = 0; i<t; i++) {
			visited[i] = false;
		}
	}
	unsigned int x = dx(generator);
	unsigned int y = dy(generator);
	visited[w] = true;
	visited[v] = true;
	G.insert(v, w) = 1;
	
	vector<int> wi;
	for (int i = 0; i<t; i++) {
		if (visited[i] != true&&(G.coeff(i,w) == 1 || G.coeff(w, i) == 1 )) {
			wi.push_back(i);
		}
	}
	if (wi.empty())return;
	if (x == 0 && y == 0) {
		return;
	}
	vector<int> wxy;
	if (x>0) {
		wxy = sample(wi, x);
		for (unsigned int i = 0; i<wxy.size(); i++) {
			G.insert(v, wxy[i]) = 1;
			visited[wxy[i]] = true;
		}
	}
	if (y>0) {
		wxy = sample(wi, y);
		for (unsigned int i = 0; i<wxy.size(); i++) {
			G.insert(wxy[i], v) = 1;
			visited[wxy[i]] = true;
		}
	}
	for (unsigned int i = 0; i<wi.size(); i++) {
		if (visited[wi[i]] != true)
			AddNode(v, wi[i]);
	}
}

void ffm(int size) {
	G = SparseMatrix<bool>(size,size);
	Vn = size;
	visited = new bool[Vn];
	char filename[] = "log.txt"; // 此处写入文件名 
	ofstream fout(filename);
	for (int i = 0; i<Vn; i++) {
		fout << "time=" << t + 1 << '/' << size << endl;
		cout << "time=" << t + 1 << '/' << size << endl;
		AddNode();
		
	}
	fout.close();
	//return G;
}
/*
void avgdegree() {
	int* indegree = new int[Vn];
	int* outdegree = new int[Vn];
	for (int i = 0; i < Vn; i++)
	{
		//cout << G.coeff(2, i) << ' ';
		//indegree[i] = G.row(i).sum();
		//indegree[i] = G.col(i).sum();
		//cout << indegree[i] << ' ';
	}

	
}

void print() {
	for (int i = 0; i < Vn; i++)
	{
		for (int j = 0; j < Vn; j++) {
			cout << G.coeff(i, j) << ' ';
		}
		cout << endl;

	}
}
*/
void writeG() {
	char filename[] = "G.txt"; // 此处写入文件名 
	ofstream fout(filename);
	for (int k = 0; k<G.outerSize(); ++k)
		for (SparseMatrix<bool>::InnerIterator it(G, k); it; ++it)
		{
			//cout<<it.value()<<' ';
			fout << it.row()<<','<< it.col()<<'\n';
		}
	fout.close();
}
int main(int argc, char** argv)
{
	int size;
	cout << "产生结点个数：";
	cin >> size;
	generator.seed(1);
	ffm(size);
	//avgdegree();
	//print();
	writeG();
	cout << "结果已输出至G.txt" << endl;
	return 0;
}


