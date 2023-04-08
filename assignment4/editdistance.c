#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

static int __GetMin3(int a, int b, int c);
static int __GetMin4(int a, int b, int c, int d);
void print_alignment(char align_str[][8], int level);
void backtrace(int* op_matrix, int col_size, char* str1, char* str2, int n, int m);

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태의 연산자 정보가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
static void backtrace_main(int* op_matrix, int col_size, char* str1, char* str2, int n, int m, int level, char align_str[][8]) {
	static int count = 1;
	if (level == 0) {
		count = 1;
	}

	int ins, del, sub, mat, tran;
	int k = strlen(str1) - n;
	int op = op_matrix[col_size * k + m];
	if (op % 2 == 1)
		ins = 1;
	op = op / 2;
	if (op % 2 == 1)
		del = 1;
	op = op / 2;
	if (op % 2 == 1)
		sub = 1;
	op = op / 2;
	if (op % 2 == 1)
		mat = 1;
	op = op / 2;
	if (op % 2 == 1)
		tran = 1;
	op = op / 2;
	if (sub == 1) {
		align_str[level][0] = str1[n - 1];
		align_str[level][1] = 32;
		align_str[level][2] = 45;
		align_str[level][3] = 32;
		align_str[level][4] = str2[m - 1];
		align_str[level][5] = 0;
		
		if (n - 1 > 0 && m - 1 > 0) {
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str);
		}
		else if (n - 1 == 0 && m - 1 == 0) {
			printf("[%d]===================================\n", count++);
			print_alignment(align_str, level);
			printf("\n");
		}
	}
	else if (mat == 1) {
		align_str[level][0] = str1[n - 1];
		align_str[level][1] = 32;
		align_str[level][2] = 45;
		align_str[level][3] = 32;
		align_str[level][4] = str2[m - 1];
		align_str[level][5] = 0;

		if (n - 1 > 0 && m - 1 > 0) {
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str);
		}
		else if (n - 1 == 0 && m - 1 == 0) {
			printf("[%d]===================================\n", count++);
			print_alignment(align_str, level);
			printf("\n");
		}
	}
	if (ins == 1) {
		align_str[level][0] = 42;
		align_str[level][1] = 32;
		align_str[level][2] = 45;
		align_str[level][3] = 32;
		align_str[level][4] = str2[m - 1];
		align_str[level][5] = 0;

		if (m - 1 > 0) {
			backtrace_main(op_matrix, col_size, str1, str2, n , m - 1, level + 1, align_str);
		}
		else if (n - 1 == 0 && m - 1 == 0) {
			printf("[%d]===================================\n", count++);
			print_alignment(align_str, level);
			printf("\n");
		}
	}
	if (del == 1) {
		align_str[level][0] = str1[n - 1];
		align_str[level][1] = 32;
		align_str[level][2] = 45;
		align_str[level][3] = 32;
		align_str[level][4] = 42;
		align_str[level][5] = 0;

		if (n - 1 > 0) {
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m, level + 1, align_str);
		}
		else if (n - 1 == 0 && m - 1 == 0) {
			printf("[%d]===================================\n", count++);
			print_alignment(align_str, level);
			printf("\n");
		}
	}
	if (tran == 1) {
		align_str[level][0] = str1[n - 2];
		align_str[level][1] = str1[n - 1];
		align_str[level][2] = 32;
		align_str[level][3] = 45;
		align_str[level][4] = 32;
		align_str[level][5] = str2[m - 2];
		align_str[level][6] = str2[m - 1];
		align_str[level][7] = 0;

		if (n - 2 > 0 && m - 2 > 0) {
			backtrace_main(op_matrix, col_size, str1, str2, n - 2, m - 2, level + 1, align_str);
		}
		else if (n - 2 == 0 && m - 2 == 0) {
			printf("[%d]===================================\n", count++);
			print_alignment(align_str, level);
			printf("\n");
		}
	}
}

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를 다음과 같은 기호로 표시한다. 삽입:I, 삭제:D, 교체:S, 일치:M, 전위:T
void print_matrix(int* op_matrix, int col_size, char* str1, char* str2, int n, int m) {
	int I, D, S, M, T;
	for (int i = 0; i < n; i++) {
		printf("%c\t", str1[n - 1 - i]);
		for (int j = 1; j < m + 1; j++) {
			I = 0;
			D = 0;
			S = 0;
			M = 0;
			T = 0;
			int a = op_matrix[i * col_size + j];
			I = a % 2;
			a = a / 2;
			D = a % 2;
			a = a / 2;
			S = a % 2;
			a = a / 2;
			M = a % 2;
			a = a / 2;
			T = a % 2;
			if (S)
				printf("S");
			if (M)
				printf("M");
			if (I)
				printf("I");
			if (D)
				printf("D");
			if (T)
				printf("T");
			printf("\t");
		}
		printf("\n");
	}
	printf("\t");
	for (int k = 0; k < m; k++) {
		printf("%c\t", str2[k]);
	}
	printf("\n");
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance(char* str1, char* str2) {
	int n = strlen(str1);
	int m = strlen(str2);

	int i, j;
	int d[n + 1][m + 1];
	int op_matrix[(n + 1) * (m + 1)];

	// d 초기화
	for (i = 0; i < n + 1; i++) {
		for (j = 0; j < m + 1; j++) {
			d[i][j] = 0;
		}
	}
	d[n][0] = 0;
	for (i = n - 1; i >= 0; i--) {
		d[i][0] = d[i + 1][0] + DELETE_COST;
	}
	for (j = 1; j < m + 1; j++) {
		d[n][j] = d[n][j - 1] + INSERT_COST;
	}

	// op_matrix 초기화
	for (i = 0; i < (n + 1) * (m + 1); i++) {
		op_matrix[i] = 0;
	}
	op_matrix[n * (m + 1)] = 0;
	for (i = n - 1; i >= 0; i--) {
		op_matrix[i * (m + 1)] = DELETE_OP;
	}
	for (j = 1; j < m + 1; j++) {
		op_matrix[n * (m + 1) + j] = INSERT_OP;
	}
	for (i = n - 1; i >= 0; i--) {
		for (j = 1; j <= m; j++) {
			if (str1[n - 1 - i] == str2[j - 1]) {					// Match
				d[i][j] = d[i + 1][j - 1] + 0;
				op_matrix[i * (m + 1) + j] += MATCH_OP;
			}
			else {
				if (i > n - 2 && j < 3) {
					d[i][j] = __GetMin3(d[i + 1][j - 1] + SUBSTITUTE_COST, d[i][j - 1] + INSERT_COST, d[i + 1][j] + DELETE_COST);

					if (d[i][j] == d[i + 1][j - 1] + SUBSTITUTE_COST)
						op_matrix[i * (m + 1) + j] += SUBSTITUTE_OP;
					if (d[i][j] == d[i][j - 1] + INSERT_COST)
						op_matrix[i * (m + 1) + j] += INSERT_OP;
					if (d[i][j] == d[i + 1][j] + DELETE_COST)
						op_matrix[i * (m + 1) + j] += DELETE_OP;
				}
				else {
					if (str1[n - 1 - i] == str2[j - 2] && str1[n - 2 - i] == str2[j - 1]) {
						d[i][j] = __GetMin4(d[i + 2][j - 2] + TRANSPOSE_COST, d[i + 1][j - 1] + SUBSTITUTE_COST, d[i][j - 1] + INSERT_COST, d[i + 1][j] + DELETE_COST);

						if (d[i][j] == d[i + 1][j - 1] + SUBSTITUTE_COST)
							op_matrix[i * (m + 1) + j] += SUBSTITUTE_OP;
						if (d[i][j] == d[i][j - 1] + INSERT_COST)
							op_matrix[i * (m + 1) + j] += INSERT_OP;
						if (d[i][j] == d[i + 1][j] + DELETE_COST)
							op_matrix[i * (m + 1) + j] += DELETE_OP;
						if (d[i][j] == d[i + 2][j - 2] + TRANSPOSE_COST)
							op_matrix[i * (m + 1) + j] += TRANSPOSE_OP;
					}

					else {
						d[i][j] = __GetMin3(d[i + 1][j - 1] + SUBSTITUTE_COST, d[i][j - 1] + INSERT_COST, d[i + 1][j] + DELETE_COST);

					if (d[i][j] == d[i + 1][j - 1] + SUBSTITUTE_COST)
						op_matrix[i * (m + 1) + j] += SUBSTITUTE_OP;
					if (d[i][j] == d[i][j - 1] + INSERT_COST)
						op_matrix[i * (m + 1) + j] += INSERT_OP;
					if (d[i][j] == d[i + 1][j] + DELETE_COST)
						op_matrix[i * (m + 1) + j] += DELETE_OP;
					}
				}
			}
		}
	}

	print_matrix(op_matrix, m + 1, str1, str2, n, m);
	backtrace(op_matrix, m + 1, str1, str2, n, m);

	return d[0][m];
}

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
	
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	fprintf( stderr, "INSERT_COST = %d\n", INSERT_COST);
	fprintf( stderr, "DELETE_COST = %d\n", DELETE_COST);
	fprintf( stderr, "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	fprintf( stderr, "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}

