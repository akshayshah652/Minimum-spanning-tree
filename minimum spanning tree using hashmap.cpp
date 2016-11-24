#include<iostream>
#include<stdio.h>
#include<map>
#include<list>
#pragma warning (disable : 4996)
using namespace std;

int N, M;
map<int, map<int, int> > adjList2;
map<int, map<int, int> >::iterator it2;
long long totatMSTWeight = 0;
int parentArr[10001];
bool visited[10001];

int Distarr[10001];
int vertexArr[10001];
int vertexIndexArr[10001];
int arrSize = 0;

void heapifyUp(int i)
{
	int j, temp;
	while (i > 1) {
		j = i / 2;
		if (Distarr[i] < Distarr[j]) {
			temp = Distarr[i];
			Distarr[i] = Distarr[j];
			Distarr[j] = temp;

			temp = vertexArr[i];
			vertexArr[i] = vertexArr[j];
			vertexArr[j] = temp;

			temp = vertexIndexArr[vertexArr[i]];
			vertexIndexArr[vertexArr[i]] = vertexIndexArr[vertexArr[j]];
			vertexIndexArr[vertexArr[j]] = temp;

			i = j;
		}
		else
			break;
	}
}

void heapifyDown(int i)
{
	int j = 2 * i, temp;
	while (j <= arrSize) {
		if (j < arrSize && Distarr[j + 1] < Distarr[j])
			j = j + 1;
		if (Distarr[j] < Distarr[i]) {
			temp = Distarr[j];
			Distarr[j] = Distarr[i];
			Distarr[i] = temp;

			temp = vertexArr[i];
			vertexArr[i] = vertexArr[j];
			vertexArr[j] = temp;

			temp = vertexIndexArr[vertexArr[i]];
			vertexIndexArr[vertexArr[i]] = vertexIndexArr[vertexArr[j]];
			vertexIndexArr[vertexArr[j]] = temp;

			i = j;
			j = 2 * i;
		}
		else
			break;
	}
}

void insert(int node, int n)
{
	arrSize++;
	Distarr[arrSize] = n;
	vertexArr[arrSize] = node;
	vertexIndexArr[node] = arrSize;
	heapifyUp(arrSize);
}

int extractMin(int *node)
{
	int ret = Distarr[1];
	(*node) = vertexArr[1];
	Distarr[1] = Distarr[arrSize];
	vertexArr[1] = vertexArr[arrSize];
	vertexIndexArr[vertexArr[1]] = 1;
	arrSize--;
	if (arrSize >= 1)
		heapifyDown(1);
	return ret;
}

void decreaseKey(int node, int newKey)
{
	int index = vertexIndexArr[node];
	Distarr[index] = newKey;
	heapifyUp(index);
}

void calcPrimMST()
{
	insert(1, 0);
	for (int i = 2; i <= N; i++)
		insert(i, 2147483647);
	parentArr[1] = -1;
	int count = 0;
	int minWeight, u;
	while (count < N) {
		count++;
		int minWeight = extractMin(&u);
		totatMSTWeight += minWeight;
		visited[u] = true;
		it2 = adjList2.find(u);
		map<int, int> temp = it2->second;
		map<int, int>::iterator list_itr;
		for (list_itr = temp.begin(); list_itr != temp.end(); ++list_itr) {
			int v = list_itr->first;
			int w = list_itr->second;
			if ( !visited[v] && w < Distarr[vertexIndexArr[v]]) {
				decreaseKey(v, w);
				parentArr[v] = u;
			}
		}
	}
}

void printGraph()
{
	cout << totatMSTWeight << endl;
	for (int i = 2; i <= N; i++) {
		cout << i << " " << parentArr[i] << endl;
	}
}

int main()
{
	int i, j, u, v, w;
	totatMSTWeight = 0;
	for (i = 0; i < 10001; i++) {
		parentArr[i] = -1;
		visited[i] = false;
	}
	freopen("input.txt", "r", stdin);
	cin >> N >> M;
	for (i = 1; i <= M; i++) {
		cin >> u >> v >> w;
		it2 = adjList2.find(u);
		if (it2 != adjList2.end()) {
			(adjList2.find(u)->second).insert(pair<int, int>(v, w));
		}
		else {
			map<int, int> temp;
			temp.insert(pair<int, int>(v,w));
			adjList2.insert(pair<int, map<int, int> >(u, temp));
		}
		it2 = adjList2.find(v);
		if (it2 != adjList2.end()) {
			(adjList2.find(v)->second).insert(pair<int, int>(u,w));
		}
		else {
			map<int, int> temp;
			temp.insert(pair<int, int>(u,w));
			adjList2.insert(pair<int, map<int, int> >(v, temp));
		}
	}
	fclose(stdin);
	freopen("output.txt", "w", stdout);
	calcPrimMST();
	printGraph();
	fclose(stdout);
	return 0;
}