#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 552
#define INF 9999
#define NODES 552
#define SUBWAY_NAME 18
#define MAX_QUEUE_SIZE 552

typedef struct GraphType {
    int n; // 정수형 데이터 정의
    int **adj_mat; // 이중 포인터 변수 정의
} GraphType; // 그래프 타입 정의

int key, tmp = 0;
char number_inf[552][30]; 
char name_inf[552][30]; 
int distance[MAX_VERTICES]; 
int found[MAX_VERTICES]; 
int path[NODES]; 
char subway_number[SUBWAY_NAME][30] = {{"1호선"}, {"1지선"}, {"2호선"}, {"2지선"}, {"3호선"}, {"4호선"}, {"5호선"}, {"5지선"}, {"6호선"}, {"7호선"}, {"8호선"}, {"9호선"}, {"분당선"}, {"인천1선"}, {"중앙선"}, {"경춘선"}, {"경의선"}, {"공항철도"}}; 
char subway_name[552][30];
char transfer_mat[150][151][30]; 
int transfer_time = 0; 
int subway_time = 0; 
int transfer_moment = 0; 
int station_count = 0; 
char start_name[30]; 
char end_name[30]; 
int visited[MAX_VERTICES];
int data[552][552];
int finish = 0;

/*
    함수명 : NewGraph
    인 자 : int
    리턴형 : GraphType
    설 명 : 총 역의 개수의 2차원 배열을 동적으로 생성한다.
*/
GraphType* NewGraph(int n) // 매개변수로 총 역의 개수를 전달받는다
{
    int i, j;
    GraphType *graph = (GraphType *)malloc(sizeof(GraphType)); // 그래프 동적 메모리 할당
    graph->n = n; // 총 역의 개수로 초기화
    graph->adj_mat = (int **)malloc(sizeof(int *) * n); // 총 역의 개수인 n만큼 동적 메모리 할당(배열의 세로)
    for (i = 0; i < n; i++)
    {
        graph->adj_mat[i] = (int *)malloc(sizeof(int) * n); // 총 역의 개수인 n만큼 동적 메모리 할당(배열의 가로)
        memset(graph->adj_mat[i], 0, sizeof(int) * n);
    }
    for (i = 0; i < n; i++) // 2차원 배열(n x n)의 모든 값을 9999로 초기화
        for (j = 0; j < n; j++)
            graph->adj_mat[i][j] = INF; // 값 초기화
    return graph; // graph 반환
}
/*
    함수명 : make_station_inf
    인 자 : char
    리턴형 : X
    설 명 : 역이름 파일을 읽어 역번호와 역이름을 배열에 저장한다.
*/
void make_station_inf(char* FileName) // 매개변수로 파일이름을 전달받는다
{
    FILE *pFile = NULL; // NULL로 초기화
    pFile = fopen(FileName, "r"); // 읽기 모드로 파일 열기
    char *c; // 문자형 포인터 변수 지정
    char str_tmp[1024]; // 행 데이터 공간 지정
    int cnt; // 정수형 변수 지정
    if (pFile != NULL) 
    {
        cnt = 0;
        fgets(str_tmp, 1024, pFile); // 두 번째줄부터 읽기 위해 먼저 한줄을 읽는다(총역수와 552는 불필요하다)
        while (!feof(pFile)) { // 두 번째 줄부터 파일의 마지막 줄까지 읽는다
            fgets(str_tmp, 1024, pFile); // 한 줄을 읽는다 
            c = strtok(str_tmp, ","); // 행 데이터 저장
            while (c != NULL) { // 데이터값이 없으면 반복문 종료
                if (cnt == 552) // 개행을 만나면 break
                    break;
                strcpy(number_inf[cnt], c); // 역번호 배열에 역번호 저장(100)
                c = strtok(NULL, ","); // 다음 데이터값 복사
                strcpy(name_inf[cnt], c); // 역이름 배열에 역이름 저장(소요산)
                name_inf[cnt][strlen(name_inf[cnt]) - 1] = '\0'; // 문자열 마지막에 '\0'을 추가(소요산\0, 동두천\0)
                c = strtok(NULL, ","); // 다음 데이터값 복사
            }
            cnt++; // 인덱스 값 증가시키기 위해 +1
        }
        // 반복문이 끝나면 number_inf[0] ~ number_inf[551], name_inf[0] ~ name_inf[551] 데이터가 저장
    }
    fclose(pFile); // 파일 포인터 닫기
}
/*
    함수명 : make_adj_inf
    인 자 : char, GraphType
    리턴형 : X
    설 명 : 역정보 파일을 읽어 인접행렬을 생성한다.
*/
void make_adj_mat(char* FileName, GraphType* g, char number_inf[][30])
{
    FILE *pFile = NULL; // NULL로 초기화
    pFile = fopen(FileName, "r"); // 읽기 모드로 파일 열기
    char *c; // 문자형 포인터 변수 지정
    char str_tmp[1024]; // 행 데이터 공간 지정
    int cnt; // 정수형 변수 지정
    int index = tmp; // 전역변수인 tmp값을 index로 초기화
    int i = tmp; // 전역변수인 tmp값을 index로 초기화

    if (pFile != NULL) // 파일의 마지막 줄인 개행전까지 읽기 위해 i값을 생성한다.
    {
        fgets(str_tmp, 1024, pFile); // 첫 번째 줄을 읽는다
        while (!feof(pFile)) { // 두 번째 줄부터 마지막 줄까지 읽는다
            fgets(str_tmp, 1024, pFile); // 한 줄을 읽는다
            i++; // 예를 들어 1호선 파일을 읽으면 i값은 63
        }
    }
    fclose(pFile); // 파일 포인터 닫기

    pFile = fopen(FileName, "r"); // 읽기 모드로 파일 다시 열기

    if (pFile != NULL)
    {
        fgets(str_tmp, 1024, pFile); // 첫 번째줄을 읽는다
        while (!feof(pFile)) { // 두 번째줄부터 마지막 줄까지 읽는다
            fgets(str_tmp, 1024, pFile); // 한 줄을 읽는다
            c = strtok(str_tmp, ","); // 행 데이터 저장
            cnt = index; // cnt 초기화 
            while (c != NULL) { // 데이터 값이 없으면 종료
                if (tmp == i - 1) // 마지막 줄인 개행이면 break
                    break;
                if (strcmp(number_inf[tmp], c) == 0) { // 역번호 배열에 저장한 값과 파일의 역번호를 비교하여 다음 문자로 넘어가기 위해 검사한다. 
                    c = strtok(NULL, ","); // 다음 데이터 값 복사
                }
                strcpy(subway_name[cnt], subway_number[key]); // 역호선 배열에 호선저장(subway_name[0]~subway[61] "1호선")
                g->adj_mat[tmp][cnt++] = atoi(c); // 파일의 가중치값을 정수로 변환하여 인접행렬에 초기화(adj_mat[0][0] ~ adj_mat[0][61], adj_mat[1][0] ~ adj_mat[1][61] -- adj_mat[61][0] ~ adj_mat[61][61])
                c = strtok(NULL, ","); // 다음 데이터 값 복사
            }
            tmp++; // 인덱스 증가
        }
    }
    key++; // 전역변수인 key값 1증가
    tmp--; // 전역변수인 tmp값 1감소
    // 만약 1호선 파일을 읽을 경우 tmp는 62이며 key는 1이다. tmp와 key는 전역변수이므로 다음 1지선 파일을 읽을 때 tmp를 통해 인접행렬의 인덱스 [62][62]부터 값을 저장할 수 있고 key를 통해 역호선 배열도 인덱스에 맞게 호선을 저장할 수 있다.
    // 이런식으로 모든 역정보 파일을 읽어 인접행렬에 값을 저장할 수 있다.
    fclose(pFile); // 파일 포인터 닫기
}
/*
    함수명 : make_transfer_time
    인 자 : GraphType, char
    리턴형 : X
    설 명 : 환승정보 파일을 읽어 인접행렬을 생성한다.
*/
void make_transfer_time(GraphType* g, char number_inf[][30]) 
{
    FILE *pFile = NULL; // NULL로 초기화
    pFile = fopen("환승정보.csv", "r"); // 읽기 모드로 파일 열기
    char *c, *s1, *s2; // 문자형 포인터 변수 지정
    char str_tmp[1024]; // 행 데이터 공간 지정
    int i, j, cnt, tmp; // 정수형 변수 지정
    int n, t1, t2; // 정수형 변수 지정
    if (pFile != NULL)
    {
        tmp = 0; // 0으로 초기화
        fgets(str_tmp, 1024, pFile); // 첫 번째 줄을 먼저 읽는다
        while (!feof(pFile)) { // 두 번째줄부터 마지막 줄까지 읽는다
            fgets(str_tmp, 1024, pFile); // 한 줄을 읽는다
            c = strtok(str_tmp, ","); // 행 데이터 저장
            cnt = 0; // 0으로 초기화
            while (c != NULL) { 
                if (tmp == 150) // 마지막 줄인 개행이면 break
                    break;
                strcpy(transfer_mat[tmp][cnt++], c); // 환승정보 배열(150 x 151)에 데이터 저장
                c = strtok(NULL, ","); // 다음 데이터 값 복사
            }
            tmp++;
        }
        // 반복문이 끝나면 (transfer_mat[0][0] ~ transfer_mat[149][150])까지 데이터 값이 저장되어 있다(각 행의 첫 번째 인덱스 값은 역번호이다 transfer_mat[0][0] = "113", transfer_mat[1][0] = "116")
    }   
    // 현재 tmp 값은 151
    // 환승정보 배열을 검사
    for (i = 0; i < tmp - 1; i++) { // i = 0 ~ 149 모든 행 검사
        for (j = 1; j < tmp; j++) { // 1부터 검사하는 이유는 각 행의 첫 번째 인덱스 값은 역번호이기 때문이다
            if ((atoi(transfer_mat[i][j]) != 0) && (atoi(transfer_mat[i][j]) != 9999)) { // 환승정보 배열에 0이나9999 값이 아닌 다른 값인지 검사
                n = atoi(transfer_mat[i][j]); // 참이면 정수로 변환하여 n 초기화
                s1 = transfer_mat[i][0]; // 해당 행의 역번호를 s1에 저장
                s2 = transfer_mat[j - 1][0]; // 대칭을 이용하여 해당 열의 역번호 저장(150 x 151 배열이므로 -1을 해준다)
                // 예를 들어 transfer_mat[101][1]이 8이면 s1은 transfer_mat[101][0]이고 s2는 transfer_mat[0][0]이다
                for (int a = 0; a < 552; a++) { // 해당 행의 역번호 인덱스 값을 찾는다
                    if (strcmp(s1, number_inf[a]) == 0) // s1과 역번호 배열을 검사하여 같은 경우 해당 인덱스 값을 t1에 저장
                        t1 = a; // t1 초기화
                }
                for (int b = 0; b < 552; b++) { // 해당 열의 역번호의 인덱스 값을 찾는다
                    if (strcmp(s2, number_inf[b]) == 0) // s2와 역번호 배열을 검사하여 같은 경우 해당 인덱스 값을 t2에 저장
                        t2 = b; // t2 초기화
                }
                g->adj_mat[t1][t2] = n; // t1,t2를 이용하여 인접행렬에 n 저장 (g->adj_mat[t1][t2] = 9999 -> g->adj_mat[t1][t2] = 8)
            }
        }
    }
    fclose(pFile); // 파일 포인터 닫기
}
/*
    함수명 : random_transfer_time
    인 자 : GraphType, int
    리턴형 : int
    설 명 : 환승할 경우 호출하여 최대 환승 소요시간 안에서 난수 값을 생성하여 리턴한다(g->adj_mat[101][1] = 8 이면 1 ~ 8까지 난수 생성)
*/
int random_transfer_time(GraphType* g, int start, int end) {
    srand(time(NULL));
    return rand() % g->adj_mat[start][end] + 1; // rand() % g->adj_mat[101][1]는 (0 ~ 7) 이므로 1을 더해준다
}
/*
    함수명 : choose
    인 자 : int
    리턴형 : int
    설 명 : 방문하지 않은 역에서 distance 배열의 값이 가장 작은 역에 인덱스를 반환한다.
*/
int choose(int distance[], int n, int found[])
{
    int i, min, minpos; // 정수형 변수 지정
    min = INT_MAX; // 9999 복사
    minpos = -1; // -1 복사
    for (i = 0; i < n; i++) // 총 역의 개수만큼 반복
        if (distance[i] < min && !found[i]) { // 값이 작은 지와 방문 여부 검사
            min = distance[i]; // 참이면 배열에 값 복사
            minpos = i; // 참이면 배열의 인덱스 값 복사
        }
    return minpos; // 배열의 값이 가장 작은 인덱스를 반환
}
/*
    함수명 : shortest_path
    인 자 : GrapType, int
    리턴형 : X
    설 명 : 다익스트라 알고리즘을 이용하여 출발역으로부터 모든 역의 최단경로를 알아낸다.
*/
void shortest_path(GraphType* g, int start) 
{
    int i, u, w; // 정수형 변수 지정
    for (i = 0; i < g->n; i++) // 총 역의 개수만큼 반복
    {
        distance[i] = g->adj_mat[start][i]; // distance 배열 초기화
        found[i] = FALSE; // founf 배열 초기화
        path[i] = -1; // path 배열 초기화
    }
    found[start] = TRUE; // 방문 표시
    distance[start] = 0; // 출발역이므로 0 복사
    for (i = 0; i < g->n-1; i++) { // 총 역의 개수의 -1만큼 반복(출발역은 이미 방문하였기 때문에)
        u = choose(distance, g->n, found); // choose 함수 호출하여 반환값 복사
        found[u] = TRUE; // 방문 표시
        for (w = 0; w < g->n; w++) // 총 역의 개수 만큼 반복
            if (!found[w]) // 방문 여부 검사
                if (distance[u] + g->adj_mat[u][w] < distance[w]) { // 기존의 경로보다 u를 통해 가는 길이 더 빠른지 검사
                    distance[w] = distance[u] + g->adj_mat[u][w]; // 참이면 distance[w] 값 변경
                    path[w] = u; // path 배열 업데이트
                }
    }
}
/*
    함수명 : print_path
    인 자 : GrapType, int
    리턴형 : X
    설 명 : 순환함수를 사용하여 출발역으로부터 도착역까지 최단경로 혹은 최소환승 경로의 모든 역을 출력한다.
*/
void print_path(GraphType* g, int start, int end)
{
    if (path[start] == path[end]) { // 값 비교를 통해 출발역인지 검사 
        printf("<출발>\n"); // 출발 표시
        if (strcmp(name_inf[start], name_inf[end]) == 0) // 이름이 같은 환승역이 존재하므로 name_inf 배열을 통해 검사
            return; // 함수 종료
        printf("-> <%s> %s\n", subway_name[start], name_inf[start]); // 출발역의 호선과 역이름 출력
        station_count++; // 정거장 수 +1
        subway_time += g->adj_mat[start][end]; // 지하철 소요시간 더하기 
    }
    else {
        print_path(g, start, path[end]); // print_path 함수 호출(순환함수 사용)
        if (finish == 1) // 도착했는지 검사
            return; // 함수 종료
        if (strcmp(name_inf[path[end]], name_inf[end]) == 0) { // 환승역인지 검사
            int random_time = random_transfer_time(g, path[end], end); // random_transfer_time 함수 호출하여 난수 값을 생성하여 randoom_time에 복사
            printf("-> <환승 : 소요시간 %d 분> %s\n", random_time, name_inf[end]); // 환승 소요시간의 난수 값과 역이름 출력
            transfer_time += random_time; // 환승 소요시간 더하기
            transfer_moment = 1; // 1로 초기화
            station_count++; // 정거장 수 +1
        }    
        else {
            if (transfer_moment == 1) { // 직전 역이 환승역인지 검사
                subway_time += g->adj_mat[path[end]][end]; // 지하철 소요시간 더하기 
                transfer_moment = 0; // 0으로 초기화
                if (strcmp(end_name, name_inf[end]) == 0) { // 입력받은 도착역과 역이름 배열을 통해 도착역인지 검사
                    printf("-> <%s> %s <도착>\n", subway_name[end], name_inf[end]);
                    station_count++; // 정거장 수 +1
                    finish = 1; // 도착 표시
                }  
            }
            else { // 직전 역이 환승역이 아니면 
                printf("-> <%s> %s\n", subway_name[path[end]], name_inf[path[end]]); // 호선과 역이름 출력
                station_count++; // 정거장 수 +1
                subway_time += g->adj_mat[path[end]][end];  // 지하철 소요시간 더하기 
                if (strcmp(end_name, name_inf[end]) == 0) { // 입력받은 도착역과 역이름 배열을 통해 도착역인지 검사
                    printf("-> <%s> %s <도착>\n", subway_name[end], name_inf[end]);
                    station_count++; // 정거장 수 +1
                    finish = 1; // 도착 표시
                }    
            }
        }
    }
}
/*
    함수명 : smalles_transfer
    인 자 : GrapType, int
    리턴형 : X
    설 명 : 환승역일 경우 가중치를 임의의 큰 값으로 변경하여 다익스트라 알고리즘을 통해 출발역으로부터 모든 역의 최소환승 경로를 알아낸다.
*/
void smallest_transfer(GraphType* g, int start) 
{
    int i, j, u, w; // 정수형 변수 지정
    for (i = 0; i < g->n; i++) // 총 역의 개수만큼 반복
        for (j = 0; j < g->n; j++) { // 총 역의 개수만큼 반복
            if (strcmp(name_inf[i], name_inf[j]) == 0) { // 역이름 배열을 이용하여 이름이 같을 경우 환승역이므로 환승역인지 검사
                data[i][j] = g->adj_mat[i][j]; // 참이면 data배열에 값 저장
                g->adj_mat[i][j] = 500; // 참이면 임의의 큰 값으로 저장(최소환승을 하기 위해 환승할 경우 가중치를 큰 값으로 변경)
            }
        }
    for (i = 0; i < g->n; i++) // 총 역의 개수만큼 반복
    {
        distance[i] = g->adj_mat[start][i]; // distance 배열 초기화
        found[i] = FALSE; // found 배열 초기화
        path[i] = -1; // path 배열 초기화
    }
    found[start] = TRUE; // 방문 표시
    distance[start] = 0; // 출발역이므로 0 저장
    for (i = 0; i < g->n-1; i++) { // 총 역의 개수의 -1만큼 반복(출발역은 이미 방문하였기 때문에)
        u = choose(distance, g->n, found); // choose 함수 호출하여 반환값 복사
        found[u] = TRUE; // 방문 표시
        for (w = 0; w < g->n; w++) // 총 역의 개수만큼 반복
            if (!found[w]) // 방문 여부 검사
                if (distance[u] + g->adj_mat[u][w] < distance[w]) { // 기존의 경로보다 u를 통해 가는 길이 더 빠른지 검사
                    distance[w] = distance[u] + g->adj_mat[u][w]; // 참이면 distance[w] 값 변경
                    path[w] = u; // path 배열 업데이트
                }
    }
    for (i = 0; i < g->n; i++) // 총 역의 개수만큼 반복
        for (j = 0; j < g->n; j++) { // 총 역의 개수만큼 반복
            if (strcmp(name_inf[i], name_inf[j]) == 0) { // 역이름 배열을 이용하여 이름이 같을 경우 환승역이므로 환승역인지 검사
                g->adj_mat[i][j] = data[i][j]; // data 배열에 저장한 값을 통해 기존의 가중치로 변경
            }
        }
}
/*
    함수명 : error
    인 자 : int
    리턴형 : X
    설 명 : 사용자가 출발역과 도착역을 입력했을 때 오류가 있는지 검사하여 오류가 있으면 1을 리턴하고 그렇지 않으면 0을 리턴한다.
*/
int error(int select1, int select2, int start, int end)
{
    if (select1 == 0 && select2 == 0) { // 출발역과 도착역을 잘못 입력했는지 검사
        printf("잘못된 출발역과 도착역입니다\n"); 
        return 1; // 1 반환
    }
    else if (select1 == 0) { // 출발역을 잘못 입력했는지 검사
        printf("잘못된 출발역입니다\n");
        return 1; // 1 반환
    }
    else if (select2 == 0) { // 도착역을 잘못 입력했는지 검사
        printf("잘못된 도착역입니다\n");
        return 1; // 1 반환
    }
    else if (start == end) { // 출발역과 도착역이 같은지 검사
        printf("출발역과 도착역이 같습니다\n");
        return 1; // 1 반환
    } 
    else
        return 0; // 0 반환
}
int main (void)
{   
    GraphType *g; // GraptType 포인터 변수 g 정의
    g = NewGraph(552); // NewGraph 함수 호출하여 리턴값 g에 저장
    make_station_inf("역이름.csv");
    make_adj_mat("1호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("1지선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("2호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("2지선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("3호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("4호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("5호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("5지선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("6호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("7호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("8호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("9호선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("분당선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("인천1선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("중앙선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("경춘선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("경의선.csv", g, number_inf); // make_adj_mat 함수 호출
    make_adj_mat("공항철도.csv", g, number_inf); // make_adj_mat 함수 호출
    int i, j, choose, start, end;
    while (1) { // 오류가 없을 때 까지 무한반복
        int select1 = 0;
        int select2 = 0;
        printf("출발역을 입력해주세요: ");
        scanf("%s", start_name);
        printf("도착역을 입력해주세요: ");
        scanf("%s", end_name);
        for (i = 0; i < 552; i++) { 
            if (strcmp(start_name, name_inf[i]) == 0) { // 입력받은 출발역이 역이름 배열에 존재하는지 검사
                start = i; // 해당 역이름 배열에 인덱스 값을 start에 저장
                select1 = 1; // select1 초기화
            }
        }
        for (i = 0; i < 552; i++) { 
            if (strcmp(end_name, name_inf[i]) == 0) { // 입력받은 도착역이 역이름 배열에 존재하는지 검사
                end = i; // 해당 역이름 배열에 인덱스 값을 end에 저장
                select2 = 1; // select2 초기화
            }
        }
        if (error(select1, select2, start, end)) // 오류가 있는지 검사
            continue; // 참이면 다시 입력받는다
        printf("방식? 1. 최단경로 2. 최소환승\n"); 
        printf(":");
        scanf("%d", &choose);
        make_transfer_time(g, number_inf); // make_transfer_time 함수 호출
        if (choose == 1) { // 입력받은 값이 1인지 검사
            shortest_path(g, start); // shortest_path 함수 호출
            print_path(g, start, end); // print_path 함수 호출
            printf("소요시간 : %d분 (%d + 환승 소요시간 : %d) 분\n", subway_time + transfer_time, subway_time, transfer_time); // 소요시간과 환승 소요시간 출력
            printf("정거장 수 : %d\n", station_count); // 정거장 수 출력
        }
        else { // 입력받은 값이 2
            smallest_transfer(g, start); // smallest_transfer 함수 호출
            print_path(g, start, end); // print_path 함수 호출
            printf("소요시간 : %d분 (%d + 환승 소요시간 : %d) 분\n", subway_time + transfer_time, subway_time, transfer_time); // 소요시간과 환승 소요시간 출력
            printf("정거장 수 : %d\n", station_count); // 정거장 수 출력
        }
        break; // 무한루프 탈출
    }
    free(g); // 메모리 해제
    return 0;
}