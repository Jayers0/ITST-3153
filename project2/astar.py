import heapq
import random

# Define a class to represent a node in the grid
class Node:
    def __init__(self, row, col, parent=None, cost=0, heuristic=0):
        self.row = row  # Row index of the node in the grid
        self.col = col  # Column index of the node in the grid
        self.parent = parent  # Parent node
        self.cost = cost  # Cost to reach this node from the start
        self.heuristic = heuristic  # Heuristic estimate of the cost to reach the goal

    def __lt__(self, other):
        return (self.cost + self.heuristic) < (other.cost + other.heuristic)

# A* algorithm implementation
def astar(start, goal, grid):
    rows = len(grid)
    cols = len(grid[0])

    open_list = []  # List to store nodes to be explored
    closed_set = set()  # Set to store explored nodes

    # Create the start node
    start_node = Node(start[0], start[1], None, 0, heuristic(start, goal))
    heapq.heappush(open_list, start_node)

    while open_list:
        current_node = heapq.heappop(open_list)

        # Check if the current node is the goal node
        if (current_node.row, current_node.col) == goal:
            path = []
            while current_node:
                path.append((current_node.row, current_node.col))
                current_node = current_node.parent
            path.reverse()
            return path

        closed_set.add((current_node.row, current_node.col))

        # Generate neighboring nodes
        neighbors = get_neighbors(current_node, grid, rows, cols)
        for neighbor_row, neighbor_col, neighbor_cost in neighbors:
            if (neighbor_row, neighbor_col) in closed_set:
                continue

            g = current_node.cost + neighbor_cost
            h = heuristic((neighbor_row, neighbor_col), goal)
            neighbor_node = Node(neighbor_row, neighbor_col, current_node, g, h)

            # Check if the neighbor is already in the open list
            in_open_list = False
            for open_node in open_list:
                if (open_node.row, open_node.col) == (neighbor_row, neighbor_col):
                    in_open_list = True
                    if neighbor_node.cost < open_node.cost:
                        open_list.remove(open_node)
                        heapq.heappush(open_list, neighbor_node)
                    break

            if not in_open_list:
                heapq.heappush(open_list, neighbor_node)

    # If the goal node is not found, return an empty path
    return []

# Function to calculate the heuristic (Euclidean distance)
def heuristic(node, goal):
    return ((node[0] - goal[0]) ** 2 + (node[1] - goal[1]) ** 2) ** 0.5

# Function to get the neighboring nodes of a given node
def get_neighbors(node, grid, rows, cols):
    neighbors = []
    row, col = node.row, node.col

    # Define the possible neighbor positions (up, down, left, right)
    possible_neighbors = [(row - 1, col), (row + 1, col), (row, col - 1), (row, col + 1)]

    for neighbor_row, neighbor_col in possible_neighbors:
        # Check if the neighbor is within the grid boundaries
        if 0 <= neighbor_row < rows and 0 <= neighbor_col < cols:
            # Check if the neighbor is traversable (not an obstacle)
            if grid[neighbor_row][neighbor_col] != 1:
                # Calculate the cost to move to the neighbor (1 in this case)
                cost = 1
                neighbors.append((neighbor_row, neighbor_col, cost))

    return neighbors

# Function to generate a random grid
def generate_random_grid(rows, cols, obstacle_prob):
    grid = []
    for _ in range(rows):
        row = []
        for _ in range(cols):
            # Generate a random number and check if it's less than the obstacle probability
            if random.random() < obstacle_prob:
                row.append(1)  # 1 represents an obstacle
            else:
                row.append(0)  # 0 represents an empty space
        grid.append(row)
    return grid

# Example usage
def main():
    # Define the size of the grid and the obstacle probability
    rows = 10
    cols = 10
    obstacle_prob = 0.3

    # Generate a random grid
    grid = generate_random_grid(rows, cols, obstacle_prob)
    print("Random Grid:")
    for row in grid:
        print(row)

    start_node = (0, 0)
    goal_node = (rows - 1, cols - 1)

    path = astar(start_node, goal_node, grid)
    if path:
        print("Shortest path:")
        for node in path:
            print(node)
    else:
        print("No path found.")

if __name__ == '__main__':
    main()
