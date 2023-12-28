// A* solver ITSC 3153
// SUM 2023
// Joshua Ayers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define GRID_SIZE 15
#define BLOCK_PERCENTAGE 10

typedef struct {
    int x, y, g, h, f;
    bool blocked;
    struct Node* parent;
} Node;

Node** create_grid() {
    Node** grid = (Node**)malloc(GRID_SIZE * sizeof(Node*));
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i] = (Node*)malloc(GRID_SIZE * sizeof(Node));
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].x = j;
            grid[i][j].y = i;
            grid[i][j].blocked = rand() % 100 < BLOCK_PERCENTAGE;
            grid[i][j].g = 0;
            grid[i][j].h = 0;
            grid[i][j].f = 0;
            grid[i][j].parent = NULL;
        }
    }
    return grid;
}

void display_grid(Node** grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].blocked)
                printf("# ");
            else
                printf(". ");
        }
        printf("\n");
    }
}

int get_input(const char* message) {
    int input;
    printf("%s", message);
    scanf("%d", &input);
    return input;
}

void get_start_and_goal_nodes(Node** grid, Node** start_node, Node** goal_node) {
    int start_x = get_input("Enter the X coordinate for the starting node: ");
    int start_y = get_input("Enter the Y coordinate for the starting node: ");
    *start_node = &grid[start_y][start_x];

    int goal_x = get_input("Enter the X coordinate for the goal node: ");
    int goal_y = get_input("Enter the Y coordinate for the goal node: ");
    *goal_node = &grid[goal_y][goal_x];
}

int manhattan_distance(Node* node1, Node* node2) {
    return abs(node1->x - node2->x) + abs(node1->y - node2->y);
}

Node** get_neighbors(Node* node, Node** grid) {
    Node** neighbors = (Node**)malloc(4 * sizeof(Node*));
    int count = 0;
    int x = node->x;
    int y = node->y;

    if (x > 0 && !grid[y][x - 1].blocked)
        neighbors[count++] = &grid[y][x - 1];
    if (x < GRID_SIZE - 1 && !grid[y][x + 1].blocked)
        neighbors[count++] = &grid[y][x + 1];
    if (y > 0 && !grid[y - 1][x].blocked)
        neighbors[count++] = &grid[y - 1][x];
    if (y < GRID_SIZE - 1 && !grid[y + 1][x].blocked)
        neighbors[count++] = &grid[y + 1][x];

    neighbors = realloc(neighbors, count * sizeof(Node*));
    return neighbors;
}

void reconstruct_path(Node* node) {
    if (node->parent != NULL)
        reconstruct_path(node->parent);
    printf("[%d, %d] ", node->x, node->y);
}

Node** astar(Node** grid, Node* start_node, Node* goal_node) {
    Node** open_list = (Node**)malloc(GRID_SIZE * GRID_SIZE * sizeof(Node*));
    int open_list_count = 0;
    Node* closed_set[GRID_SIZE][GRID_SIZE];
    memset(closed_set, 0, sizeof(closed_set));

    start_node->g = 0;
    start_node->h = manhattan_distance(start_node, goal_node);
    start_node->f = start_node->g + start_node->h;
    open_list[open_list_count++] = start_node;

    while (open_list_count > 0) {
        int current_index = 0;
        for (int i = 1; i < open_list_count; i++) {
            if (open_list[i]->f < open_list[current_index]->f)
                current_index = i;
        }
        Node* current_node = open_list[current_index];

        if (current_node == goal_node) {
            Node** path = (Node**)malloc(GRID_SIZE * GRID_SIZE * sizeof(Node*));
            int path_length = 0;
            Node* node = current_node;
            while (node != NULL) {
                path[path_length++] = node;
                node = node->parent;
            }
            path = realloc(path, path_length * sizeof(Node*));
            return path;
        }

        open_list[current_index] = open_list[--open_list_count];
        closed_set[current_node->y][current_node->x] = current_node;

        Node** neighbors = get_neighbors(current_node, grid);
        int num_neighbors = open_list_count;  // Fixed calculation of neighbors count
        for (int i = 0; i < num_neighbors; i++) {
            Node* neighbor = neighbors[i];

            if (closed_set[neighbor->y][neighbor->x] != NULL)
                continue;

            int tentative_g = current_node->g + 1;
            bool in_open_list = false;
            for (int j = 0; j < open_list_count; j++) {
                if (open_list[j] == neighbor) {
                    in_open_list = true;
                    break;
                }
            }

            if (!in_open_list || tentative_g < neighbor->g) {
                neighbor->g = tentative_g;
                neighbor->h = manhattan_distance(neighbor, goal_node);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current_node;

                if (!in_open_list)
                    open_list[open_list_count++] = neighbor;
            }
        }
    }

    return NULL;
}

void free_grid(Node** grid) {
    for (int i = 0; i < GRID_SIZE; i++)
        free(grid[i]);
    free(grid);
}


int main() {
    srand(time(NULL));

    Node** grid = create_grid();
    display_grid(grid);

    while (true) {
        Node* start_node;
        Node* goal_node;
        get_start_and_goal_nodes(grid, &start_node, &goal_node);

        Node** path = astar(grid, start_node, goal_node);
        if (path != NULL) {
            printf("Path found: ");
            int path_length = sizeof(path) / sizeof(path[0]);  // Fixed calculation of path length
            for (int i = 0; i < path_length; i++)
                printf("[%d, %d] ", path[i]->x, path[i]->y);
            printf("\n");
            free(path);
        } else {
            printf("Path not found.\n");
        }

        char choice;
        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &choice);
        if (choice != 'y')
            break;

    free_grid(grid);

    return 0;
}
