#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits> // For INT_MAX
#include <algorithm> // For reverse
#include <unordered_set>
using namespace std;

struct Node {
    int row;
    int col;
    int g;
    int h;
    int f;
    Node* parent;

    Node(int r, int c) : row(r), col(c), g(INT_MAX), h(0), f(INT_MAX ), parent(nullptr) {}
};

int manhattanDistance(Node* node, Node* goal) {
    return abs(node->row - goal->row) + abs(node->col - goal->col);
}

void updateNode(Node* current, Node* neighbor, Node* goal) {
    int gScore = current->g + 1; // Assuming uniform cost for each step

    if (gScore < neighbor->g) {
        neighbor->g = gScore;
        neighbor->h = manhattanDistance(neighbor, goal);
        neighbor->f = neighbor->g + neighbor->h;
        neighbor->parent = current;
    }
}

vector<pair<int, int>> aStar(vector<vector<int>>& maze, Node* start, Node* goal) {
    // ... (previous code remains unchanged)

    auto cmp = [](Node* a, Node* b) { return a->f > b->f; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> openList(cmp);
    unordered_set<Node*> openSet; // To efficiently check if a node is in the openList

    // ... (previous code remains unchanged)

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current == goal) {
            vector<pair<int, int>> path;
            while (current != nullptr) {
                path.push_back({current->row, current->col});
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        visited[current->row][current->col] = true;

        for (const auto& dir : directions) {
            // ... (rest of the code remains unchanged)

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && maze[newRow][newCol] == 0 && !visited[newRow][newCol]) {
                Node* neighbor = new Node(newRow, newCol);

                // Check if the neighbor is in the openSet
                if (openSet.find(neighbor) != openSet.end()) {
                    auto itOpen = find_if(openList.c.begin(), openList.c.end(), [neighbor](Node* node) { return node->row == neighbor->row && node->col == neighbor->col; });
                    if ((*itOpen)->g > current->g + cost) {
                        (*itOpen)->g = current->g + cost;
                        (*itOpen)->f = (*itOpen)->g + (*itOpen)->h;
                        (*itOpen)->parent = current;
                    }
                } else {
                    neighbor->g = current->g + cost;
                    neighbor->h = manhattanDistance(neighbor, goal);
                    neighbor->f = neighbor->g + neighbor->h;
                    neighbor->parent = current;
                    openList.push(neighbor);
                    openSet.insert(neighbor);
                }
            }
        }
    }

    return {}; // No path found
}

vector<vector<int>> generateRandomMaze(int rows, int cols, double obstacleProbability) {
    vector<vector<int>> maze(rows, vector<int>(cols, 0));
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (rand() % 100 < obstacleProbability * 100) {
                maze[row][col] = 1; // Set as obstacle (blocked)
            }
        }
    }

    return maze;
}


void printMaze(const vector<vector<int>>& maze, Node* start, Node* goal) {
    int rows = maze.size();
    int cols = maze[0].size();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (r == start->row && c == start->col)
                cout << "S"; // Mark start node
            else if (r == goal->row && c == goal->col)
                cout << "G"; // Mark goal node
            else if (maze[r][c] == 0)
                cout << "."; // Unblocked cell
            else
                cout << "#"; // Blocked (obstacle) cell
        }
        cout << endl;
    }
}


void printMazeWithPath(const vector<vector<int>>& maze, Node* start, Node* goal, const vector<pair<int, int>>& path) {
    int rows = maze.size();
    int cols = maze[0].size();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (r == start->row && c == start->col)
                cout << "S"; // Mark start node
            else if (r == goal->row && c == goal->col)
                cout << "G"; // Mark goal node
            else if (find(path.begin(), path.end(), make_pair(r, c)) != path.end())
                cout << "*"; // Mark nodes in the path with "*"
            else if (maze[r][c] == 0)
                cout << "."; // Unblocked cell
            else
                cout << "#"; // Blocked (obstacle) cell
        }
        cout << endl;
    }
}

int main() {
    int rows = 15;
    int cols = 15;
    double obstacleProbability = 0.10;
    vector<vector<int>> maze = generateRandomMaze(rows, cols, obstacleProbability);
    
    // Generate random positions for the start and goal nodes
    srand(time(0));
    int startRow = rand() % rows;
    int startCol = rand() % cols;
    int goalRow = rand() % rows;
    int goalCol = rand() % cols;
    
    // Make sure the start and goal nodes are not in blocked cells
    while (maze[startRow][startCol] != 0 || maze[goalRow][goalCol] != 0) {
        startRow = rand() % rows;
        startCol = rand() % cols;
        goalRow = rand() % rows;
        goalCol = rand() % cols;
    }

    Node* start = new Node(startRow, startCol);
    Node* goal = new Node(goalRow, goalCol);
    
    cout << "Generated Maze with Start (S) and Goal (G):" << endl;
    printMaze(maze, start, goal);

    vector<pair<int, int>> path = aStar(maze, start, goal);

    if (!path.empty()) {
        cout << "Shortest Path: " << endl;
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ")" << endl;
        }

        cout << "Maze with Path: " << endl;
        printMazeWithPath(maze, start, goal, path);
    } else {
        cout << "No path found!" << endl;
    }

    return 0;
}