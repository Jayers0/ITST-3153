// A C++ Program to implement A* Search Algorithm
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <stack>
#include <cstring>
#include <set>
#include <ctime>
#include <cfloat>
using namespace std;

#define ROW 15
#define COL 15

typedef pair<int, int> Pair;

typedef pair<double, pair<int, int> > pPair;

struct cell {

	int parent_i, parent_j;
	double f, g, h;
};
bool isUnBlocked(const vector<vector<int>>& grid, int row, int col)
{
    if (grid[row][col] == 1)
        return false; // Blocked
    else
        return true; // Unblocked
}
bool traversable(int row, int col, vector<vector<int>>& grid) {
    bool valid = (row >= 0) && (row < ROW) && (col >= 0) && (col < COL) && isUnBlocked(grid, row, col);
    //cout << "traversable(" << row << ", " << col << ") = " << (valid ? "true" : "false") << endl;
    return valid;
}


bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateH(int row, int col, Pair dest)
{
	// Return using the distance formula
	return ((int)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
}


void tracePath(cell cellDetails[][COL], Pair dest)
{
	printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
			&& cellDetails[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);
	}

	return;
}
void printSearchMap(const vector<vector<int>>& grid, const cell cellDetails[][COL], Pair src, Pair dest)
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (i == src.first && j == src.second) {
                printf("S ");
            } else if (i == dest.first && j == dest.second) {
                printf("D ");
            } else if (grid[i][j] == 1) {
                printf("X ");
            } else if (cellDetails[i][j].f == FLT_MAX) {
                printf(". ");
            } else {
                printf("+ ");
            }
        }
        printf("\n");
    }
}
void aStarSearch(vector<vector<int>>& grid, Pair src, Pair dest)
{
	// If the source is out of range

	if (traversable(src.first, src.second,grid) == false) {
		printf("Source is invalid\n");

	}

	// If the destination is out of range
	if (traversable(dest.first, dest.second,grid) == false) {
		printf("Destination is invalid\n");

	}

	// Either the source or the destination is blocked
	if (isUnBlocked(grid, src.first, src.second) == false
		|| isUnBlocked(grid, dest.first, dest.second)
			== false) {
		printf("Source or the destination is blocked\n");

	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
	}

	bool closedList[ROW][COL];
	memset(closedList, false, sizeof(closedList));

	// Declare a 2D array of structure to hold the details
	// of that cell
	cell cellDetails[ROW][COL];

	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;
	
	set<pPair> openList;


	openList.insert(make_pair(0.0, make_pair(i, j)));


	bool foundDest = false;

	while (!openList.empty()) {
      pPair p = *openList.begin();
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        // Check if the current cell is the destination
        if (isDestination(i, j, dest) == true) {
            tracePath(cellDetails, dest);
            foundDest = true;
            break; // Break the while loop once destination is found
        }

        double gNew, hNew, fNew;


		//(North) ------------
		printSearchMap(grid, cellDetails, src, dest);
		std::cout << std::endl;
		// Only process this cell if this is a valid one
		if (traversable(i - 1, j,grid) == true) {
			printSearchMap(grid, cellDetails, src, dest);
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i - 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			else if (closedList[i - 1][j] == false
					&& isUnBlocked(grid, i - 1, j)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateH(i - 1, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i - 1][j].f == FLT_MAX
					|| cellDetails[i - 1][j].f > fNew) {
					openList.insert(make_pair(
						fNew, make_pair(i - 1, j)));

					// Update the details of this cell
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
					
				}
			}
		}

		//(South) ------------
		
		// Only process this cell if this is a valid one
		if (traversable(i + 1, j,grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i + 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			else if (closedList[i + 1][j] == false
					&& isUnBlocked(grid, i + 1, j)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateH(i + 1, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i + 1][j].f == FLT_MAX
					|| cellDetails[i + 1][j].f > fNew) {
					openList.insert(make_pair(
						fNew, make_pair(i + 1, j)));
					// Update the details of this cell
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//(East) ------------
		
		// Only process this cell if this is a valid one
		if (traversable(i, j + 1,grid) == true) {
			
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i, j + 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			else if (closedList[i][j + 1] == false
					&& isUnBlocked(grid, i, j + 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateH(i, j + 1, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j + 1].f == FLT_MAX
					|| cellDetails[i][j + 1].f > fNew) {
					openList.insert(make_pair(
						fNew, make_pair(i, j + 1)));

					// Update the details of this cell
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//(West) ------------
		// Only process this cell if this is a valid one
		if (traversable(i, j - 1,grid) == true) {
			printSearchMap(grid, cellDetails, src, dest);
			if (isDestination(i, j - 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			else if (closedList[i][j - 1] == false
					&& isUnBlocked(grid, i, j - 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateH(i, j - 1, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j - 1].f == FLT_MAX
					|| cellDetails[i][j - 1].f > fNew) {
					openList.insert(make_pair(
						fNew, make_pair(i, j - 1)));

					// Update the details of this cell
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}
		
    }

	if (foundDest == false)
		printf("Failed to find the Goal\n");

	return;
}
int getRandomNumber() {
    return rand() % 100;
}
void printGrid(const vector<vector<int>>& grid, Pair src, Pair dest)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (i == src.first && j == src.second)
            {
                printf("S ");
            }
            else if (i == dest.first && j == dest.second)
            {
                printf("D ");
            }
            else
            {
                printf("%d ", grid[i][j]);
            }
        }
        printf("\n");
    }
}
// Function to create a 2D vector grid with a 10% chance of 1 and 90% chance of 0
vector<vector<int>> createGrid(int rows, int cols) {
    vector<vector<int>> grid(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int randomNumber = getRandomNumber();
            if (randomNumber < 10) { // 10% chance of 1 (0 to 9)
                grid[i][j] = 1;
            }
        }
    }

    return grid;
}

// Function to find a valid cell that is not blocked
Pair findValidCell(vector<vector<int>>& grid) {
    Pair cell;
    do {
        cell = make_pair(rand() % ROW, rand() % COL);
    } while (grid[cell.first][cell.second] == 1);
    return cell;
}

int main() {
    srand(time(0));
    vector<vector<int>> grid = createGrid(15, 15);

    Pair src, dest;

    // Find a valid source cell that is not blocked
    src = findValidCell(grid);

    // Find a valid destination cell that is not blocked and not the same as the source
    do {
        dest = findValidCell(grid);
    } while (dest == src);

    std::cout << "starting node: " << src.first << "," << src.second << "   ";
    std::cout << "destination node: " << dest.first << "," << dest.second << endl;
    printGrid(grid, src, dest);
    aStarSearch(grid, src, dest);

    return 0;
}