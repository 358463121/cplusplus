// forest_fire_model.cpp.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <Eigen/SparseCore>

using namespace std;
using namespace Eigen;
SparseMatrix<bool> G; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<bool> T;


int t = 0;
int vt = 1;
int Vn;
const float p = 0.5;
const float r = 0.5;

bool* visited;
vector<int> sample(vector<int> n, int size) {
	if (n.size()<size) {
		return n;
	}
	random_shuffle(n.begin(), n.end());
	return vector<int>(n.begin(), n.begin() + size);

}
void AddNode(int v = 0, int w = 0) {
	if (t>Vn-2)
		return;
	default_random_engine generator;
	generator.seed(1);
	geometric_distribution<int> dx(1 - p);
	geometric_distribution<int> dy(1 - r*p);
	if (v == 0) {
		t = t + 1;
		v = t;
		uniform_int_distribution<int> dunif(0, t - 1);
		w = dunif(generator);
		for (int i = 0; i<Vn; i++) {
			visited[i] = false;
		}
	}

	int x = dx(generator);
	int y = dy(generator);
	visited[w] = true;
	visited[v] = true;
	G.insert(v, w) = 1;
	
	vector<int> wi;
	for (int i = 0; i<Vn; i++) {
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
		if (wi.size()>x) {
			wxy = sample(wi, x);
		}
		else {
			wxy = wi;
		}
		for (int i = 0; i<wxy.size(); i++) {
			G.insert(v, wxy[i]) = 1;
			visited[wxy[i]] = true;
		}
	}
	if (y>0) {
		if (wi.size()>y) {
			wxy = sample(wi, y);
		}
		else {
			wxy = wi;
		}
		for (int i = 0; i<wxy.size(); i++) {
			G.insert(wxy[i], v) = 1;
			visited[wxy[i]] = true;
		}
	}
	for (int i = 0; i<wi.size(); i++) {
		if (visited[wi[i]] != true)
			AddNode(v, wi[i]);
	}


}

SparseMatrix<bool> ffm(int size) {
	G = SparseMatrix<bool>(size,size);
	Vn = size;
	visited = new bool[Vn];
	for (int i = 0; i<Vn; i++) {
		AddNode();
	}
	return G;
}

void avgdegree() {
	int* indegree = new int[Vn];
	int* outdegree = new int[Vn];
	for (int i = 0; i < Vn; i++)
	{
		cout << G.coeff(1, i) << ' ';
		//indegree[i] = G.row(i).sum();
		//indegree[i] = G.col(i).sum();
		//cout << indegree[i] << ' ';
	}

	
}
int main(int argc, char** argv)
{
	ffm(500);
	avgdegree();
	

	return 0;
}


/*





/*
/*** R
set.seed(1)
Vn<-100L
# G<-ffm(2)
*/
