
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
//
// A* Rule List Original
// If child f(n) is <= to parent f(n) then generate current node's new children and move current to closed
// else if child f(n) is > to parent's f(n) then go to parent's depth and search for the lowest f(n) of that depth (if f(n) is the same then choose most left)
//
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <queue>
using namespace std;

int goalBoard[9] = {1, 2, 3,
                    8, 9, 4,
                    7, 6, 5};
int num_states_created = 0;
struct BoardState 
{	
    int stateID; // board ID & creation order
    int g; // level of board state
    int h; // sum of misplaced tiles
    int f; // board_depth + misplaced_tiles
    int board[9] = {-1, -1, -1,
                    -1, -1, -1,
                    -1, -1, -1 }; 

    BoardState()
    {
        num_states_created++;
        stateID = num_states_created;
        g = 0;
        h = 0;
        f = 0;
    }

    bool operator<(const BoardState& rhs) const
    {
        return rhs.f < f; 
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
	
	void gen_boardOne()
    {
		board[0] = 2;
		board[1] = 8;
		board[2] = 3;
		board[3] = 1;
		board[4] = 6;
		board[5] = 4;
		board[6] = 9;
		board[7] = 7;
		board[8] = 5;
        g = 0;
        set_h();
        f = h;
    }
	
	void gen_boardTwo()
    {
		board[0] = 2;
		board[1] = 1;
		board[2] = 6;
		board[3] = 4;
		board[4] = 9;
		board[5] = 8;
		board[6] = 7;
		board[7] = 5;
		board[8] = 3;
        g = 0;
        h = 7;
        f = 7;
    }
    
    void set_g(int parent_g) { g = parent_g + 1; }

    void set_h()
    {
        int misplaced = 0;
        for (int i = 0; i < 9; i++)
        {
            if (board[i] != goalBoard[i])
                misplaced++;
        }
        h = misplaced;
    }

    int get_blank()
    {
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == 9)
                return i;
        }
    }

    BoardState swap(BoardState b, int swap, int blank)
    {
        b.board[blank] = b.board[swap];
        b.board[swap] = 9;
        num_states_created++;
        b.stateID = num_states_created;
        b.set_g(b.g);
        b.set_h();
        b.f = b.g + b.h; 
        return b;
    }

    void gen_children(priority_queue<BoardState>& open, priority_queue<BoardState>& closed)
    {
        int blank = get_blank();
        if (blank == 0)
        {
            open.push(swap(*this, 1, 0));
            open.push(swap(*this, 3, 0)); 
            closed.push(*this); 
        }
        if (blank == 1)
        {
            open.push(swap(*this, 0, 1));
            open.push(swap(*this, 2, 1));
            open.push(swap(*this, 4, 1)); 
            closed.push(*this);             
        }
        if (blank == 2)
        {
            open.push(swap(*this, 1, 2));
            open.push(swap(*this, 5, 2)); 
            closed.push(*this); 
        }
        if (blank == 3)
        {
            open.push(swap(*this, 0, 3));
            open.push(swap(*this, 4, 3));
            open.push(swap(*this, 6, 3));  
            closed.push(*this); 
        }
        if (blank == 4)
        {
            open.push(swap(*this, 1, 4));
            open.push(swap(*this, 3, 4));
            open.push(swap(*this, 5, 4));
            open.push(swap(*this, 7, 4));  
            closed.push(*this); 
        }
        if (blank == 5)
        {
            open.push(swap(*this, 2, 5));
            open.push(swap(*this, 4, 5));
            open.push(swap(*this, 8, 5)); 
            closed.push(*this); 
        }
        if (blank == 6)
        {
            open.push(swap(*this, 3, 6));
            open.push(swap(*this, 7, 6)); 
            closed.push(*this); 
        }
        if (blank == 7)
        {
            open.push(swap(*this, 4, 7));
            open.push(swap(*this, 6, 7));
            open.push(swap(*this, 8, 7)); 
            closed.push(*this); 
        }
        if (blank == 8)
        {
            open.push(swap(*this, 5, 8));
            open.push(swap(*this, 7, 8)); 
            closed.push(*this); 
        }
                
    }

    BoardState find_best(priority_queue<BoardState>& open, priority_queue<BoardState>& closed)
    {
        // because the priority queue orders based on f(n) and order of creation
        // then
        
        BoardState child;

        child = open.top();

        if (child.f <= f)
        {
            // place child into closed and return child board
            closed.push(child);
            return child;
        }
        //else // 
        
    }

    void print_board()
    {
        for (int i = 0; i < 9; i++)
        {
            cout << board[i] << " ";
            if ((i + 1) % 3 == 0)
                cout << endl;
        }
        cout << "Depth: " << g << endl;
        cout << "Misplaced Tiles: " << h << endl;
        cout << "F(N): " << f << endl << endl;
    }     
};

void print_queue(priority_queue<BoardState> queue)
{
    BoardState temp; 
    while(!queue.empty())
    { 
        temp = queue.top();
        cout << "BoardState ID: " << temp.stateID << endl; 
        temp.print_board();
        queue.pop();  
    }
}

int main()
{
    priority_queue<BoardState> open;
    priority_queue<BoardState> closed;
    BoardState current;

    current.gen_boardOne();
    current.print_board(); 
    current.gen_children(open, closed);

    cout << "Printing closed: " << endl;
    print_queue(closed); 
    cout << "Print open: " << endl;
    print_queue(open);

}