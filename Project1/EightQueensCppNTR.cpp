#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;


// Function to generate a random board configuration
vector<int> genRandBoard() 
{
    vector<int> board(8);
    for (int i = 0; i < 8; ++i) 
    {
        board[i] = rand() % 8;
    }
    return board;
}

// Function to calculate the number of conflicts between queens
int calcConflicts(const vector<int>& board) 
{
    int conflicts = 0;
    for (int i = 0; i < 7; ++i) 
    {
        for (int j = i + 1; j < 8; ++j) 
        {
            if (board[i] == board[j] || abs(board[i] - board[j]) == abs(i - j)) 
            {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// Function to print the board
void printBoard(const vector<int>& board) 
{
    for (int i = 0; i < 8; ++i) 
    {
        for (int j = 0; j < 8; ++j) 
        {
            if (board[i] == j) 
            {
                cout << "1 ";
            } 
            else 
            {
                cout << "0 ";
            }
        }
        cout << endl;
    }
}

// Function to count neighbors with a lower heuristic value
int countLowerHeuristicNeighbors(const std::vector<int>& currentBoard) 
{
    int lowerHeuristicNeighbors = 0;
    for (int col = 0; col < 8; ++col) 
    {
        int prevRow = currentBoard[col];

        for (int row = 0; row < 8; ++row) 
        {
            vector<int> neighborBoard = currentBoard;  // Make a copy of the current board
            neighborBoard[col] = row;  // Modify the copy

            int newConflicts = calcConflicts(neighborBoard);  // Calculate conflicts for the neighbor board

            if (newConflicts < calcConflicts(currentBoard)) 
            {
                lowerHeuristicNeighbors++;
            }
        }
    }
    return lowerHeuristicNeighbors;
}



/*
 Hill Climbing with Random Restarts Algorithm

 Logic:

 - Generate a rand starting state by placing a q in a ran row of each colm.
 - Evaluate the current state to check if it is a goal state (no conflicts between q).
 - If current state == goal state, the sln is found. Print sln and exit.
 - If the current state != goal state, eval all poss neighbor states by moving
     each q in each colm to different rows and calc the h value for each neighbor.
 - Cnt the number of neighbor states with a lower h than the current state.
 - If no neighbor states have a lower h, perform a rand restart and go back to step 1.
 - If a neighbor state with a lower h value is found, set it as the new current state and go back to step 2.

 - Repeat steps 2-7 until a solution is found or a termination condition is met.
 
 Termination Condition:
 - sln Found: If a solution state (no conflicts between queens) is found, print the sln
   and exit the algorithm.
 - No sln Found: If the algorithm performs a specified number of random restarts without finding
   a sln, terminate and print that no sln could be found.

    Alg performs rand restarts and hillclimb until a sln is found or another
    termination condition is met.
 */

void hillClimbingWithRandomRestarts() 
{
    srand(time(nullptr)); //seeding random number generator
    int restarts = 0;

    while (1) 
    {
        vector<int> currentBoard = genRandBoard();
        int currentConflicts = calcConflicts(currentBoard);

        cout << "Current h: " << currentConflicts << std::endl;
        cout << "Current State" << std::endl;
        printBoard(currentBoard);

        int lowerHeuristicNeighbors = countLowerHeuristicNeighbors(currentBoard);
        cout << "Neighbors found with lower h: " << lowerHeuristicNeighbors << std::endl;

        if (currentConflicts == 0) 
        { //output valild board
            cout << "Solution found:" << endl;
            printBoard(currentBoard);
            break;
        }

        vector<int> bestBoard = currentBoard;
        int bestConflicts = currentConflicts;
        bool foundBetterState = false;

        for (int col = 0; col < 8; ++col) // Itterate throught for board states
        {
            int prevRow = currentBoard[col];

            for (int row = 0; row < 8; row++) 
            {
                currentBoard[col] = row;
                int newConflicts = calcConflicts(currentBoard);

                if (newConflicts < bestConflicts) 
                {
                    bestBoard = currentBoard;
                    bestConflicts = newConflicts;
                    foundBetterState = true;
                }
            }

            currentBoard[col] = prevRow;
        }

        if (!foundBetterState) // i there is no path the lower huristic from currnet board restart
        {
            cout << "RESTART" << endl;
            restarts++;
        } 
        else 
        {

            currentBoard = bestBoard;
            currentConflicts = bestConflicts;
            cout << "Setting new current state" << endl;
        }
    }

    cout << "Total restarts: " << restarts << endl;


}

int main() 
{
    hillClimbingWithRandomRestarts();

    return 0;
}
