#pragma once
#include <stdio.h>

#define MAX_VTXS  20
#define INF       9999

// =================================================================
// 1. 인접 행렬 그래프 클래스
// =================================================================
class AdjMatGraph {
protected:
	int size;
	char vertices[MAX_VTXS];
	int adj[MAX_VTXS][MAX_VTXS];
public:
	AdjMatGraph() { reset(); }
	char getVertex(int i) { return vertices[i]; }
	int getEdge(int i, int j) { return adj[i][j]; }
	void setEdge(int i, int j, int val) { adj[i][j] = val; }
	bool isEmpty() { return size == 0; }
	bool isFull() { return size >= MAX_VTXS; }

	void reset() {
		size = 0;
		for (int i = 0; i < MAX_VTXS; i++)
			for (int j = 0; j < MAX_VTXS; j++)
				setEdge(i, j, 0);
	}

	void insertVertex(char name) {
		if (!isFull())
			vertices[size++] = name;
		else
			printf("Error: 그래프 정점 개수 초과\n");
	}

	void insertEdge(int u, int v) {
		setEdge(u, v, 1);
		setEdge(v, u, 1);
	}

	void display(FILE* fp = stdout) {
		fprintf(fp, "%d\n", size);
		for (int i = 0; i < size; i++) {
			fprintf(fp, "%c ", getVertex(i));
			for (int j = 0; j < size; j++) {
				if (getEdge(i, j) == INF)
					fprintf(fp, " INF");
				else
					fprintf(fp, " %3d", getEdge(i, j));
			}
			fprintf(fp, "\n");
		}
	}
};

// =================================================================
// 2. 그래프 탐색 클래스 (DFS)
// =================================================================
class SrchAMGraph : public AdjMatGraph {
	bool visited[MAX_VTXS];
public:
	void resetVisited() {
		for (int i = 0; i < size; i++)
			visited[i] = false;
	}
	bool isLinked(int u, int v) { return getEdge(u, v) != 0; }

	void DFS(int v) {
		visited[v] = true;
		printf("%c ", getVertex(v));

		for (int w = 0; w < size; w++)
			if (isLinked(v, w) && visited[w] == false)
				DFS(w);
	}
};

// =================================================================
// 3. 가중치 그래프 클래스
// =================================================================
class WGraph : public AdjMatGraph {
public:
	WGraph() { wReset(); }

	void wReset() {
		size = 0;
		for (int i = 0; i < MAX_VTXS; i++)
			for (int j = 0; j < MAX_VTXS; j++)
				setEdge(i, j, (i == j) ? 0 : INF);
	}

	void insertEdge(int u, int v, int weight) {
		if (weight > INF) weight = INF;
		setEdge(u, v, weight);
		setEdge(v, u, weight);
	}

	bool hasEdge(int i, int j) { return (getEdge(i, j) < INF && i != j); }

	void load(const char* filename) {
		FILE* fp;
		fopen_s(&fp, filename, "r");
		if (fp != NULL) {
			wReset();
			int n;
			if (fscanf_s(fp, "%d", &n) != EOF) {
				for (int i = 0; i < n; i++) {
					char str[80];
					fscanf_s(fp, "%s", str, (unsigned int)sizeof(str));
					insertVertex(str[0]);
					for (int j = 0; j < n; j++) {
						int val;
						fscanf_s(fp, "%d", &val);
						if (i != j && val == 0) val = INF;
						insertEdge(i, j, val);
					}
				}
			}
			fclose(fp);
		}
		else {
			printf("Error: 파일(%s)을 열 수 없습니다.\n", filename);
		}
	}
};

// =================================================================
// 4. 정점 집합 클래스 (Union-Find 연산)
// =================================================================
class VertexSets {
	int         parent[MAX_VTXS];
	int         nSets;
public:
	VertexSets(int n) : nSets(n) {
		for (int i = 0; i < nSets; i++)
			parent[i] = -1;
	}
	bool isRoot(int i) { return parent[i] < 0; }

	int findSet(int v) {
		while (!isRoot(v)) v = parent[v];
		return v;
	}
	void unionSets(int s1, int s2) {
		parent[s1] = s2;
		nSets--;
	}
};

// =================================================================
// 5. 최소힙에 저장할 노드 클래스
// =================================================================
class HeapNode {
	int key;      // 간선의 가중치
	int v1;       // 정점 1
	int v2;       // 정점 2
public:
	HeapNode() : key(0), v1(-1), v2(-1) {}
	HeapNode(int k, int u, int v) : key(k), v1(u), v2(v) { }
	void setKey(int k, int u, int v) { key = k; v1 = u; v2 = v; }
	int getKey() { return key; }
	int getV1() { return v1; }
	int getV2() { return v2; }
};

// =================================================================
// 6. 최소힙 클래스
// =================================================================
class MinHeap {
	HeapNode node[MAX_VTXS * MAX_VTXS]; // 넉넉하게 배열 할당
	int size;
public:
	MinHeap() : size(0) {}
	bool isEmpty() { return size == 0; }
	bool isFull() { return size >= (MAX_VTXS * MAX_VTXS - 1); }

	HeapNode getParent(int i) { return node[i / 2]; }

	// 삽입 연산 (피피티 구현부)
	void insert(int key, int u, int v) {
		if (isFull()) return;
		int i = ++size;
		while (i != 1 && key < getParent(i).getKey()) {
			node[i] = getParent(i);
			i /= 2;
		}
		node[i].setKey(key, u, v);
	}

	// 삭제 연산 (가장 작은 가중치 노드 꺼내기)
	HeapNode remove() {
		HeapNode item = node[1];
		HeapNode last = node[size--];
		int parent = 1;
		int child = 2;

		while (child <= size) {
			if (child < size && node[child].getKey() > node[child + 1].getKey())
				child++;
			if (last.getKey() <= node[child].getKey())
				break;

			node[parent] = node[child];
			parent = child;
			child *= 2;
		}
		node[parent] = last;
		return item;
	}
};

// =================================================================
// 7. MST 기능이 추가된 가중치 그래프 클래스
// =================================================================
class WGraphMST : public WGraph {
public:
	void Kruskal() {    // kruskal의 최소 비용 신장 트리 프로그램
		MinHeap heap;
		// 상삼각행렬 부분만 검사해서 중복 없이 모든 간선을 힙에 삽입
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (hasEdge(i, j)) {
					heap.insert(getEdge(i, j), i, j);
				}
			}
		}

		VertexSets set(size);       // size개의 집합을 만듦
		int edgeAccepted = 0;         // 선택된 간선의 수

		while (edgeAccepted < size - 1 && !heap.isEmpty()) {
			HeapNode e = heap.remove();
			int uset = set.findSet(e.getV1());
			int vset = set.findSet(e.getV2());

			if (uset != vset) { // 사이클이 형성되지 않는다면
				printf("간선 추가 : %c - %c (비용:%d)\n",
					getVertex(e.getV1()), getVertex(e.getV2()), e.getKey());
				set.unionSets(uset, vset);      // 두개의 집합을 합함.
				edgeAccepted++;
			}
		}
	}
};