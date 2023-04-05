// project #2
// CS4346: Artificial Intelligence
// Due: April 17th, 2023
// Authors: Cody Hoang, Austin Brightly, Robert Kubos
// 
//
// Overview: Implement A* algorithm in C++ to a 8-puzzle game
// Each team member must create their own heuristic function.
// Analyze the performance of the algorithm by running the program
// with the heuristic function discuessed in class and the team functions.
//
// Variables:
//  Execution Time (ET): For each run print the execution time
//  Nodes Generated (NG): Total number of nodes generated
//  Nodes Expanded (NE): The total number of nodes expanded
//  Depth of Tree (D): The depth
//  Branching factor b*(NG/D): Effective branching factor b* = Total nodes generated / Depth of the tree
//
// Indvidual Work:
//      1. Create a heuristic function (1 per individual)
//      2. Test program with all functions and generate tables
//      3. Analyze the result and write the conclusion
//      4. Write the report and present to Moonis.
//
//
// Initial States:
//
// 1) 2 | 8 | 3         2) 2 | 1 | 6
//    1 | 6 | 4            4 | x | 8
//    x | 7 | 5            7 | 5 | 3
//
// f(n) = g(n) + h(n)
// g(n) = distance from start state (depth)
// h(n) = sum of tiles out of place
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

struct BoardState 
{
    bool open; // if generated board is correct path
    int board_depth; // level of board state
    int misplaced_tiles; // sum of misplaced tiles
    int board[9] = {1, 2, 3,
                    8, 9, 4,
                    7, 6, 5};

    BoardState()
    {
        open = true;
        board_depth = -1;
        misplaced_tiles = -1;
    }

    bool contains(int board[], int n, int num)
    {
        for (int i = 0; i < n; i++)
        {
            if (board[i] == num)
            return true;
        }
            return false;
    }

    void gen_board()
    {
        for (int i = 0; i < 9; i++)
        {
            int num = rand() % 9 + 1;
            while (contains(board, i, num))
                num = rand() % 9 + 1;
            board[i] = num;
        }
    }

    void print_board()
    {
        for (int i = 0; i < 9; i++)
        {
            cout << board[i] << " ";
            if ((i + 1) % 3 == 0)
                cout << endl;
        }
    }

    int get_mismatch() { return misplaced_tiles; }              
};

BoardState swap(BoardState b, int swap_index, int blank_index)
{
    b.board[blank_index] = b.board[swap_index];
    b.board[swap_index] = 9; 
    return b; 
}

void check_mismatch(BoardState& b, BoardState goal)
{
    int mismatch = 0;
    for (int i = 0; i < 9; i++)
    {
        if (b.board[i] != goal.board[i])
            mismatch++;
    }
    b.misplaced_tiles = mismatch; 
}

void check_moves(const BoardState& initial, BoardState array[10], int& used)
{
    int board_index;
    int array_index; 
    for (int i = 0; i < 9; i++)
    {
        if (initial.board[i] == 9)
            board_index = i; 
    }
    cout << "9 index in passed board: " << board_index << endl;

    if (board_index == 0)
    {
        array[used] = swap(initial, 1, 0);
        cout << endl;
        array[used].print_board();
        used++;
        array[used] = swap(initial, 3, 0);
        cout << endl;
        array[used].print_board();
        used++; 
    }
    if (board_index == 1)
    {
        array[used] = swap(initial, 0, 1);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 2, 1);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 4, 1);
        cout << endl;
        array[used].print_board();        
        used++;
    }
    if (board_index == 2)
    {
        array[used] = swap(initial, 1, 2);
        cout << endl;
        array[used].print_board();
        used++;
        array[used] = swap(initial, 5, 2);
        cout << endl;
        array[used].print_board();
        used++; 
    }
    if (board_index == 3)
    {
        array[used] = swap(initial, 0, 3);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 4, 3);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 6, 3);
        cout << endl;
        array[used].print_board();        
        used++;
    }
    if (board_index == 4)
    {
        array[used] = swap(initial, 1, 4);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 3, 4);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 5, 4);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 7, 4);
        cout << endl;
        array[used].print_board();        
        used++;
    }
    if (board_index == 5)
    {
        array[used] = swap(initial, 2, 5);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 4, 5);
        cout << endl;
        array[used].print_board();        
        used++;
        array[used] = swap(initial, 8, 5);
        cout << endl;
        array[used].print_board();        
        used++;
    }
    if (board_index == 6)
    {
        array[used] = swap(initial, 3, 6);
        cout << endl;
        array[used].print_board();
        used++;
        array[used] = swap(initial, 7, 6);
        cout << endl;
        array[used].print_board();
        used++; 
    }            
}

int main()
{
    srand(time(0));
    BoardState goal;
    BoardState start;
    BoardState open[10], closed[10];
    int openUsed = 0, closedUsed = 0;
    start.gen_board();
    start.print_board();
    cout << endl;
    goal.print_board();

    check_mismatch(start, goal);
    cout << "Number of misplaced tiles: " << start.get_mismatch() << endl;
    check_moves(start, open, openUsed);
    cout << endl;
    open[0].print_board();
    return 0;
}
