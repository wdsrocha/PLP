#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define N 10
#define MAZE_LEN 2*N+1
#define RIGHT 0
#define DOWN 1
#define SECOND 1000000
#define SLEEP SECOND*.05
#define WALL "\u2588\u2588" // unicode!
#define EMPTY "  "
#define PATH "\u2592\u2592"
#define CLS "\033[2J\033[1;1H"

/*
#define INTERSEC "\u256c"
#define HORIZONT "\u2550"
#define VERTICAL "\u2551"
*/

// Maze: matrix of unicode char
char* maze[MAZE_LEN][MAZE_LEN];

void initMaze() {
    for (int i = 0; i < MAZE_LEN; i++) {
        for (int j = 0; j < MAZE_LEN; j++) {
            if (i%2 && j%2) maze[i][j] = EMPTY;
            /*
            else if (i%2 == 0 && j%2 == 0) maze[i][j] = INTERSEC;
            else if (i%2 == 0 && j%2 != 0) maze[i][j] = HORIZONT;
            else if (i%2 != 0 && j%2 == 0) maze[i][j] = VERTICAL;
            */
            else maze[i][j] = WALL;
        }
    }
    maze[1][0] = maze[MAZE_LEN-2][MAZE_LEN-1] = EMPTY;
}

int toMaze(int k) {
    return 2*k+1;
}

void showMaze() {
    printf(CLS);
    for (int i = 0; i < MAZE_LEN; i++) {
        for (int j = 0; j < MAZE_LEN; j++) {
            printf("%s", maze[i][j]);
        }
        puts("");
    }
    usleep(SLEEP);
}

// Edge: cell pointing to a direction
typedef struct Edge Edge;
struct Edge { int i, j, dir; };

Edge edges[2*N*(N-1)];
int edges_len = 0;

void addEdge(int i, int j, int dir) {
    edges[edges_len].i = i;
    edges[edges_len].j = j;
    edges[edges_len].dir = dir;
    edges_len++;
}

void shuffleEdges() {
    srand(time(NULL));
    for (int i = edges_len; i--;) {
        int r = rand()%(i+1);
        Edge aux = edges[i];
        edges[i] = edges[r];
        edges[r] = aux;
    }
}
void showEdges() {
    for (int i = 0; i < edges_len; i++) {
        printf("%d %d %c\n", edges[i].i, edges[i].j, edges[i].dir ? 'D' : 'R');
    }
}

void initEdges() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j < N-1) addEdge(i, j, RIGHT);
            if (i < N-1) addEdge(i, j, DOWN);
        }
    }
    shuffleEdges();
}

// DisjoinSet Union-Find
int p[N*N], p_len = N*N;

void initDisjointSet() {
    for (int i = 0; i < p_len; i++) p[i] = i;
}

int findSet(int u) {
    if (u != p[u]) p[u] = findSet(p[u]);
    return p[u];
}

int unionSet(int u, int v) {
    u = findSet(u);
    v = findSet(v);
    if (u == v) return 0;
    p[v] = u;
    return 1;
}

// DFS

int di[] = {-1, 0, 1, 0};
int dj[] = { 0, 1, 0,-1};
int seen[MAZE_LEN][MAZE_LEN];
int last_i[MAZE_LEN][MAZE_LEN];
int last_j[MAZE_LEN][MAZE_LEN];

int canVisit(int i, int j) {
    if (i < 0 || i >= MAZE_LEN || j < 0 || j >= MAZE_LEN) return 0;
    if (strcmp(maze[i][j], EMPTY) || seen[i][j]) return 0;
    return 1;
}

void dfs(int i, int j) {
    seen[i][j] = 1;
    for (int k = 0; k < 4; k++) {
        int a = i+di[k], b = j+dj[k];
        if (canVisit(a, b)) {
            last_i[a][b] = i;
            last_j[a][b] = j;
            dfs(a, b);
        }
    }
}

void markPath(int i, int j) {
    if (i == -1) return;
    maze[i][j] = PATH;
    showMaze();
    // maze[i][j] = EMPTY;
    markPath(last_i[i][j], last_j[i][j]);
}

void initDFS() {
    for (int i = 0; i < MAZE_LEN; i++)
        for (int j = 0; j < MAZE_LEN; j++)
            seen[i][j] = 0, last_i[i][j] = last_j[i][j] = -1;
    dfs(MAZE_LEN-2, MAZE_LEN-1);
    markPath(1, 0);
}

// Main

int main() {
    initMaze();
    initEdges();
    initDisjointSet();

    for (int k = 0; k < edges_len; k++) {
        int i = edges[k].i;
        int j = edges[k].j;
        int u = i*N+j;
        int v = u+(edges[k].dir == RIGHT ? 1 : N);
        if (unionSet(u, v)) {
            showMaze();
            i = toMaze(i), j = toMaze(j);
            if (edges[k].dir == RIGHT) {
                maze[i][j+1] = EMPTY;
            } else {
                maze[i+1][j] = EMPTY;
            }
        }
    }

    showMaze();
    initDFS();

    return 0;
}
