# Adham Lucas 1715310001
# Ana Beatriz Faria Frota 1615310027
# Wesley da Silva Rocha 1715310026

import random
import time

N = 10
MAZE_LEN = 2*N+1
WHITE = "\u2588\u2588"
GRAY = "\u2592\u2592"
BLACK = "  "
DELAY = 0.05
CLS = "\033[2J\033[1;1H"

maze = []

def init_maze():
    for i in range(MAZE_LEN):
        maze.append([WHITE]*MAZE_LEN)
    maze[1][0] = GRAY;
    maze[MAZE_LEN-2][MAZE_LEN-1] = WHITE

def show_maze():
    print(CLS)
    for row in maze:
        print("".join(row))
    time.sleep(DELAY)

def show_pos(i, j):
    maze[i][j] = BLACK
    show_maze()
    maze[i][j] = GRAY

def found_exit(i, j):
    return i == MAZE_LEN-2 and j == MAZE_LEN-2

def can_visit(i, j):
    if i < 0 or i >= MAZE_LEN or j < 0 or j >= MAZE_LEN: return 0
    if maze[i][j] != WHITE: return 0
    return 1

dir_i = [-1, 0, 1, 0]
dir_j = [ 0, 1, 0,-1]
def dfs(i, j, d):
    show_pos(i, j)
    is_path = 0
    if found_exit(i, j):
        maze[i][j+1] = BLACK
        show_maze()
        is_path = 1
    if d != -1:
        is_path |= dfs(i+dir_i[d], j+dir_j[d], -1)
        show_pos(i, j)
    else:
        for k in random.sample(range(4), 4):
            di, dj = dir_i[k], dir_j[k]
            if can_visit(i+2*di, j+2*dj):
                is_path |= dfs(i+di, j+dj, k)
                show_pos(i, j)
    if is_path:
        maze[i][j] = BLACK
        return 1
    else:
        return 0

def main():
    init_maze()
    dfs(1, 1, -1)
    maze[1][0] = BLACK
    show_maze()

if __name__ == '__main__':
    main()
