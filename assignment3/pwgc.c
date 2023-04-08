#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

static void get_pwgc(int state, int* p, int* w, int* g, int* c);


// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename(FILE* fp, int state) {				// pwgc.net 만들때 사용
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	fprintf(fp, "%d \"<%d%d%d%d>\"\n",state + 1, p, w, g, c);
}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc(int state, int* p, int* w, int* g, int* c) {
	*c = state % 2;
	state = state / 2;
	*g = state % 2;
	state = state / 2;
	*w = state % 2;
	state = state / 2;
	*p = state % 2;
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end(int state) {

	if (state == 3 || state == 6 || state == 7 || state == 8 || state == 9 || state == 12)				// 0011,1100 : 염소 양배추,  0110,1001 : 늑대 염소,  0111,1000 : 늑대 염소, 염소 양배추
		return 1;

	else
		return 0;

}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition(int state1, int state2) {
	int p1, w1, g1, c1;
	int p2, w2, g2, c2;
	get_pwgc(state1, &p1, &w1, &g1, &c1);
	get_pwgc(state2, &p2, &w2, &g2, &c2);

	if (is_dead_end(state1) == 1 || is_dead_end(state2) == 1) {
		
	}
	else {
		if (p1 != p2 && w1 == w2 && g1 == g2 && c1 == c2)
			return 1;
		else if (p1 != p2 && w1 != w2 && g1 == g2 && c1 == c2)
			return 1;
		else if (p1 != p2 && g1 != g2 && w1 == w2 && c1 == c2)
			return 1;
		else if (p1 != p2 && c1 != c2 && w1 == w2 && g1 == g2)
			return 1;
		else
			return 0;
	}
}

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP(int state) {
	if (state < 8)
		state += 8;
	else if (state >= 8)
		state -= 8;

	return state;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW(int state) {
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	int next_state;
	
	if (p == 1 && w == 1)
		next_state = state - 12;

	else if (p == 0 && w == 0)
		next_state = state + 12;

	else if ((p == 0 && w == 1) || (p == 1 && w == 0))
		return -1;

	int pos = is_possible_transition(state, next_state);
	if (pos == 1)
		return next_state;
	else
		return -1;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG(int state) {
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	int next_state;

	if (p == 1 && g == 1)
		next_state = state - 10;

	else if (p == 0 && g == 0)
		next_state = state + 10;

	else if ((p == 0 && g == 1) || (p == 1 && g == 0))
		return -1;

	int pos = is_possible_transition(state, next_state);
	if (pos == 1)
		return next_state;
	else
		return -1;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC(int state) {
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	int next_state;

	if (p == 1 && c == 1)
		next_state = state - 9;

	else if (p == 0 && c == 0)
		next_state = state + 9;

	else if ((p == 0 && c == 1) || (p == 1 && c == 0))
		return -1;

	int pos = is_possible_transition(state, next_state);
	if (pos == 1) 
		return next_state;
	else
		return -1;
}

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited(int visited[], int level, int state) {
	int v;
	for (int i = 0; i <= level; i++) {
		if (visited[i] == state)
			v = 1;
	}

	return v;
}

// 방문한 상태들을 차례로 화면에 출력
static void print_states(int visited[], int count) {
	int p, w, g, c;
	for (int i = 0;i <= count; i++) {
		get_pwgc(visited[i], &p, &w, &g, &c);
		printf("<%d%d%d%d>\n", p, w, g, c);
	}
}

// recursive function
static void dfs_main(int state, int goal_state, int level, int visited[]) {
	int next_state;
	int check = 0;
	int p, w, g, c;
	int p2, w2, g2, c2;
	get_pwgc(state, &p, &w, &g, &c);
	printf("cur state is <%d%d%d%d> (level %d)\n", p, w, g, c, level);
	visited[level] = state;
	if (state == goal_state) {
		printf("Goal-state found!\n");
		print_states(visited, level);
	}

	
	next_state = changeP(state);
	get_pwgc(next_state, &p2, &w2, &g2, &c2);
	if (is_visited(visited, level, next_state) == 1)
		check = 1;
	if (is_dead_end(next_state) == 1) {
		printf("\tnext state <%d%d%d%d> is dead-end\n", p2, w2, g2, c2);
	}
	else if (is_dead_end(next_state) == 0 && check == 1) {
		printf("\tnext state <%d%d%d%d> has been visited\n", p2, w2, g2, c2);
	}
	else if (is_dead_end(next_state) == 0 && check == 0) {
		dfs_main(next_state, goal_state, level + 1, visited);
		printf("back to <%d%d%d%d> (level %d)\n", p, w, g, c, level);
	}
	check = 0;



	next_state = changePW(state);
	if (next_state != -1) {
		get_pwgc(next_state, &p2, &w2, &g2, &c2);
		if (is_visited(visited, level, next_state) == 1)
			check = 1;
		if (is_dead_end(next_state) == 1) {
			printf("\tnext state <%d%d%d%d> is dead-end\n", p2, w2, g2, c2);
		}
		else if (is_dead_end(next_state) == 0 && check == 1) {
			printf("\tnext state <%d%d%d%d> has been visited\n", p2, w2, g2, c2);
		}
		else {
			dfs_main(next_state, goal_state, level + 1, visited);
			printf("back to <%d%d%d%d> (level %d)\n", p, w, g, c, level);
		}
		check = 0;
	}



	next_state = changePG(state);
	if (next_state != -1) {
		get_pwgc(next_state, &p2, &w2, &g2, &c2);
		if (is_visited(visited, level, next_state) == 1)
			check = 1;
		if (is_dead_end(next_state) == 1) {
			printf("\tnext state <%d%d%d%d> is dead-end\n", p2, w2, g2, c2);
		}
		else if (is_dead_end(next_state) == 0 && check == 1) {
			printf("\tnext state <%d%d%d%d> has been visited\n", p2, w2, g2, c2);
		}
		else {
			dfs_main(next_state, goal_state, level + 1, visited);
			printf("back to <%d%d%d%d> (level %d)\n", p, w, g, c, level);
		}
		check = 0;
	}



	next_state = changePC(state);
	if (next_state != -1) {
		get_pwgc(next_state, &p2, &w2, &g2, &c2);
		if (is_visited(visited, level, next_state) == 1)
			check = 1;
		if (is_dead_end(next_state) == 1) {
			printf("\tnext state <%d%d%d%d> is dead-end\n", p2, w2, g2, c2);
		}
		else if (is_dead_end(next_state) == 0 && check == 1) {
			printf("\tnext state <%d%d%d%d> has been visited\n", p2, w2, g2, c2);
		}
		else {
			dfs_main(next_state, goal_state, level + 1, visited);
			printf("back to <%d%d%d%d> (level %d)\n", p, w, g, c, level);
		}
	}

}

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix(int graph[][16]) {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			graph[i][j] = is_possible_transition(i, j);
		}
	}
}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph(int graph[][16], int num) {
	for (int i = 0;i < num; i++) {
		for (int j = 0; j < 16; j++) {
			printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph(char* filename, int graph[][16], int num) {
	FILE* fp;
	fp = fopen(filename, "w");

	fprintf(fp, "*Vertices %d\n", num);
	for (int i = 0; i < num; i++) {
		print_statename(fp, i);
	}
	fprintf(fp, "*Edges\n");
	for (int i = 0; i < num; i++) {
		for (int j = i + 1; j < num; j++) {
			if (is_possible_transition(i, j) == 1 && is_dead_end(i) == 0 && is_dead_end(j) == 0)
				fprintf(fp, " %d %d\n", i + 1, j + 1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int init_state, int goal_state)
{
	int level = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( init_state, goal_state, level, visited); 
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging)
	//print_graph( graph, 16);

	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}


