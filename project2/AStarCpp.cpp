#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

// Define the size of the grid
const int GRID_SIZE = 15;

// Define the Node structure
struct Node {
    int x;
    int y;
    int g;
    int h;
    int f;
    Node* parent;

    Node(int x, int y, int g, int h, Node* parent)
        : x(x), y(y), g(g), h(h), parent(parent) {
        f = g + h;
    }
};

// Define the Manhattan distance heuristic
int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Check if a given node is valid (within the grid and not blocked)
bool isValidNode(int x, int y, const std::vector<std::vector<bool>>& grid) {
    return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && !grid[x][y]);
}

// Find a path using the A* algorithm
std::vector<Node*> findPath(const Node& start, const Node& goal, const std::vector<std::vector<bool>>& grid) {
    // Define a priority queue for open nodes
    std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> openNodes;

    // Create a 2D vector to store the visited state of each node
    std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

    // Enqueue the start node
    openNodes.push(new Node(start.x, start.y, 0, 0, nullptr));

    // Iterate until the openNodes is empty or the goal node is reached
    while (!openNodes.empty()) {
        // Get the node with the lowest f score from the openNodes
        Node* current = openNodes.top();
        openNodes.pop();

        // Check if the current node is the goal node
        if (current->x == goal.x && current->y == goal.y) {
            // Trace back the path from the goal to the start node
            std::vector<Node*> path;
            Node* node = current;
            while (node != nullptr) {
                path.push_back(node);
                node = node->parent;
            }
            return path;
        }

        // Mark the current node as visited
        visited[current->x][current->y] = true;

        // Generate the neighboring nodes
        const int dx[] = {-1, 0, 1, 0};  // Offsets for left, up, right, down
        const int dy[] = {0, -1, 0, 1};
        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            // Check if the neighbor is valid and not visited
            if (isValidNode(nx, ny, grid) && !visited[nx][ny]) {
                int ng = current->g + 1;  // Cost to move to the neighbor
                int nh = calculateManhattanDistance(nx, ny, goal.x, goal.y);
                int nf = ng + nh;

                // Check if the neighbor is already in the openNodes
                bool found = false;
                for (Node* node : openNodes._Get_container()) {
                    if (node->x == nx && node->y == ny) {
                        found = true;
                        if (ng < node->g) {
                            node->g = ng;
                            node->f = nf;
                            node->parent = current;
                        }
                        break;
                    }
                }

                // If the neighbor is not in the openNodes, enqueue it
                if (!found) {
                    openNodes.push(new Node(nx, ny, ng, nh, current));
                }
            }
        }
    }

    // No path found
    return std::vector<Node*>();
}

// Print the grid with the path
void printGrid(const std::vector<std::vector<bool>>& grid, const std::vector<Node*>& path) {
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            if (grid[x][y]) {
                std::cout << "[X]";  // Blocked node
            } else if (std::find_if(path.begin(), path.end(), [x, y](const Node* node) {
                           return node->x == x && node->y == y;
                       }) != path.end()) {
                std::cout << "[*]";  // Node in the path
            } else {
                std::cout << "[ ]";  // Pathable node
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    // Generate the grid with 10% blocked nodes
    std::vector<std::vector<bool>> grid(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            if (rand() % 100 < 10) {
                grid[x][y] = true;  // Block the node
            }
        }
    }

    // Display the generated grid
    std::cout << "Generated Grid:" << std::endl;
    printGrid(grid, {});

    // Get the starting and goal nodes from the user
    int startX, startY, goalX, goalY;
    std::cout << "Enter the starting node coordinates (x y): ";
    std::cin >> startX >> startY;
    std::cout << "Enter the goal node coordinates (x y): ";
    std::cin >> goalX >> goalY;

    // Find the path using A*
    std::vector<Node*> path = findPath(Node(startX, startY, 0, 0, nullptr), Node(goalX, goalY, 0, 0, nullptr), grid);

    // Print the grid with the path
    std::cout << "Grid with Path:" << std::endl;
    printGrid(grid, path);

    // Clean up memory
    for (Node* node : path) {
        delete node;
    }

    return 0;
}
