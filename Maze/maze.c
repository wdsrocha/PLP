// Adham Lucas 1715310001
// Ana Beatriz Faria Frota 1615310027
// Wesley da Silva Rocha 1715310026

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define N 10
#define MAZE_LEN 2*N+1
#define SECOND 1000000
#define SLEEP SECOND*.05
#define CLS "\033[2J\033[1;1H"
#define WHITE "\u2588\u2588"
#define GRAY "\u2592\u2592"
#define BLACK "  "

// Maze: matrix of unicode char
char* maze[MAZE_LEN][MAZE_LEN];

void init_maze()
{
    for (int i = 0; i < MAZE_LEN; i++)
        for (int j = 0; j < MAZE_LEN; j++)
            maze[i][j] = WHITE;
    maze[1][0] = GRAY;
    maze[MAZE_LEN-2][MAZE_LEN-1] = WHITE;
}

void show_maze()
{
    printf(CLS);
    for (int i = 0; i < MAZE_LEN; i++) {
        for (int j = 0; j < MAZE_LEN; j++) {
            printf("%s", maze[i][j]);
        }
        puts("");
    }
    usleep(SLEEP);
}

void show_pos(int i, int j)
{
    maze[i][j] = BLACK;
    show_maze();
    maze[i][j] = GRAY;
}

// DFS

int perm[] = {0, 1, 2, 3};
int dir_i[] = {-1, 0, 1, 0};
int dir_j[] = { 0, 1, 0,-1};

void shuffle(int* perm, int n)
{
    for (int i = n; i--;)
    {
        int r = rand()%(i+1);
        int aux = perm[i];
        perm[i] = perm[r];
        perm[r] = aux;
    }
}

int can_visit(int i, int j)
{
    if (i < 0 || i >= MAZE_LEN || j < 0 || j >= MAZE_LEN) return 0;
    if (strcmp(maze[i][j], WHITE)) return 0;
    return 1;
}

int found_exit(int i, int j)
{
    return i == MAZE_LEN-2 && j == MAZE_LEN-2;
}

int dfs(int i, int j, int dir)
{
    show_pos(i, j);

    int is_path = 0;
    if (found_exit(i, j)) {
        maze[i][j+1] = BLACK;
        show_maze();
        is_path = 1;
    }

    if (dir != -1) {
        is_path |= dfs(i+dir_i[dir], j+dir_j[dir], -1);
        show_pos(i, j);
    } else {
        shuffle(perm, 4);
        for (int k = 0; k < 4; k++) {
            int di = dir_i[perm[k]];
            int dj = dir_j[perm[k]];
            if (can_visit(i+2*di, j+2*dj)) {
                is_path |= dfs(i+di, j+dj, perm[k]);
                show_pos(i, j);
            }
        }
    }

    if (is_path) {
        maze[i][j] = BLACK;
        return 1;
    } else {
        return 0;
    }
}

int main() {
    srand(time(NULL));
    init_maze();
    dfs(1, 1, -1);
    maze[1][0] = BLACK;
    show_maze();

    return 0;
}
