#include "AdjMatGraph.h"

int main()
{
	// --------------------------------------------------
	// 1. 인접 행렬 기본 그래프 및 DFS 테스트
	// --------------------------------------------------
	printf("==================================================\n");
	printf("1. 인접 행렬로 표현한 기본 그래프 (g)\n");
	printf("==================================================\n");
	SrchAMGraph g;

	for (int i = 0; i < 4; i++)
		g.insertVertex('A' + i);

	g.insertEdge(0, 1);
	g.insertEdge(0, 3);
	g.insertEdge(1, 2);
	g.insertEdge(1, 3);
	g.insertEdge(2, 3);
	g.display();

	// DFS 탐색 수행
	printf("\nDFS 탐색 순서 => ");
	g.resetVisited();
	g.DFS(0);
	printf("\n\n\n");


	// --------------------------------------------------
	// 2. 가중치 그래프 로드 데이터 및 Kruskal MST 테스트
	// --------------------------------------------------
	WGraphMST mstrgaph;
	mstrgaph.load("graph.txt"); // 실행 폴더에 graph.txt 파일이 있어야 합니다.

	printf("==================================================\n");
	printf("2. 로드된 가중치 그래프 데이터 (mstrgaph)\n");
	printf("==================================================\n");
	mstrgaph.display(); // 가중치 행렬 상태 출력
	printf("\n\n");

	printf("==================================================\n");
	printf("3. MST By Kruskal's Algorithm (최소 비용 신장 트리)\n");
	printf("==================================================\n");
	mstrgaph.Kruskal(); // 최소 힙에서 하나씩 꺼내며 가중치 정렬 출력
	printf("\n");

	return 0;
}