import sys

## 입력 받는 코드입니다. 수정할 필요 없습니다.
sys.stdin = open('case.txt')
N, M = list(map(int,input().split()))
print(N, M)
concerts = []
for v in range(N):
    values = list(map(int, input().split()))
    concerts.append(values)
# print(concerts)
# [[1, 0, 0, 1, 1, 0], [1, 0, 1, 1, 0, 0], [1, 1, 1, 1, 0, 1], [0, 1, 1, 0, 1, 1], [0, 1, 0, 0, 1, 0]]
###################################

def count_stages(concerts):
    """
    콘서트장 약도에서 무대 공간의 개수를 세는 함수
    
    무대 공간은 0으로 표시되고, 펜스는 1로 표시됨
    연결된 0들의 묶음이 하나의 무대 공간을 의미함
    
    접근 방법:
    - DFS를 사용하여 연결된 무대 공간(0)들을 탐색
    - 방문하지 않은 0을 발견할 때마다 새로운 무대 공간으로 카운트
    - 해당 위치에서 DFS를 수행하여 연결된 모든 0을 방문 처리
    - 상하좌우로 연결된 0들을 같은 무대 공간으로 간주
    
    시간 복잡도: O(N*M) - 모든 칸을 한 번씩 방문
    공간 복잡도: O(N*M) - visited 배열과 재귀 호출 스택
    """
    
    answer = 0
    
    # 예외 처리: 빈 맵이거나 None인 경우
    if not concerts or len(concerts) == 0:
        return 0
    
    # 콘서트장의 세로 길이 (행의 개수)
    N = len(concerts)
    
    # 콘서트장의 가로 길이 (열의 개수)
    if len(concerts[0]) == 0:
        return 0
    M = len(concerts[0])
    
    # 방문 여부를 체크하는 2차원 배열
    # False로 초기화하여 아직 방문하지 않았음을 표시
    visited = [[False] * M for _ in range(N)]
    
    def dfs(row, col):
        """
        특정 위치에서 시작하여 연결된 모든 무대 공간을 탐색하는 DFS 함수
        
        DFS를 선택한 이유:
        1. 재귀로 구현이 간단하고 직관적함
        2. 연결된 영역을 찾는 문제에 적합함
        3. 메모리 사용이 BFS보다 효율적임
        
        동작 방식:
        1. 현재 위치를 방문 처리
        2. 상하좌우 4방향을 확인
        3. 유효한 위치이고, 방문하지 않았고, 무대 공간(0)이면 재귀 호출
        """
        
        # 현재 위치를 방문했다고 표시
        # 같은 무대 공간을 중복으로 세지 않기 위함
        visited[row][col] = True
        
        # 상하좌우 4방향을 탐색하기 위한 방향 벡터
        # 상: (-1, 0), 하: (1, 0), 좌: (0, -1), 우: (0, 1)
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        
        # 4방향 각각에 대해 탐색 수행
        for dr, dc in directions:
            # 다음 위치 계산
            next_row = row + dr
            next_col = col + dc
            
            # 다음 위치가 유효한지 검사
            # 1. 행이 범위 내에 있는가
            # 2. 열이 범위 내에 있는가
            if 0 <= next_row < N and 0 <= next_col < M:
                
                # 다음 위치를 방문하지 않았고, 무대 공간(0)인 경우
                # 재귀적으로 DFS 수행하여 연결된 모든 무대 공간 방문 처리
                if not visited[next_row][next_col] and concerts[next_row][next_col] == 0:
                    dfs(next_row, next_col)
    
    # 콘서트장의 모든 위치를 순회
    # 이중 for문으로 2차원 배열의 모든 칸을 확인
    for i in range(N):
        for j in range(M):
            
            # 현재 위치가 무대 공간(0)이고 아직 방문하지 않은 경우
            # 새로운 무대 공간을 발견한 것임
            if concerts[i][j] == 0 and not visited[i][j]:
                
                # 무대 공간 개수 증가
                answer += 1
                
                # 이 위치에서 DFS를 시작하여
                # 연결된 모든 무대 공간을 방문 처리함
                # 이렇게 하면 같은 무대 공간을 중복으로 세지 않음
                dfs(i, j)
    
    # 최종적으로 센 무대 공간의 개수를 반환
    return answer

print(count_stages(concerts))

