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
#include <sys/time.h>
#include <vector>
using namespace std;

int goalBoardOne[9] = {2, 8, 3,
                    1, 6, 4,
                    0, 7, 5};
					
int goalBoardTwo[9] = {2, 1, 6,
                    4, 0, 8,
                    7, 5, 3};

struct BoardState 
{	
    bool open; // if generated board is correct path
    int g; // level of board state
    int h; // sum of misplaced tiles
    int board[9] = {1, 2, 3,
                    8, 9, 4,
                    7, 6, 5};

    BoardState()
    {
        open = true;
        g = -1;
        h = -1;
    }

    // bool contains(int board[], int n, int num)
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         if (board[i] == num)
    //         return true;
    //     }
    //         return false;
    // }

    // void gen_board()
    // {
    //     for (int i = 0; i < 9; i++)
    //     {
    //         int num = rand() % 9 + 1;
    //         while (contains(board, i, num))
    //             num = rand() % 9 + 1;
    //         board[i] = num;
    //     }
    // }
	
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
	
	void set_HAustin()
    {
        int hn = 0;
        for (int i = 0; i < 9; i++)
        {
			for(int j=0; j<9; j++)
			{
				if(board[i] == goalBoardOne[j])
				{
					hn+= (abs(i/3 - j/3) + abs(i%3 - j%3));
				}
			}
        }
		h = hn;
    }

    int get_mismatch() { return h; }              
};

bool operator==(const BoardState& b1 , const BoardState& b2)
{
	return (b1.board[0] == b2.board[0]) &&
		(b1.board[1] == b2.board[1]) &&
		(b1.board[2] == b2.board[2]) &&
		(b1.board[3] == b2.board[3]) &&
		(b1.board[4] == b2.board[4]) &&
		(b1.board[5] == b2.board[5]) &&
		(b1.board[6] == b2.board[6]) &&
		(b1.board[7] == b2.board[7]) &&
		(b1.board[8] == b2.board[8]);
}
/*
bool operator==( const BoardState& b1 , const BoardState& b2)
{
	return b1.board[0] == b2.board[0] &&
		b1.board[1] == b2.board[1] &&
		b1.board[2] == b2.board[2] &&
		b1.board[3] == b2.board[3] &&
		b1.board[4] == b2.board[4] &&
		b1.board[5] == b2.board[5] &&
		b1.board[6] == b2.board[6] &&
		b1.board[7] == b2.board[7] &&
		b1.board[8] == b2.board[8];
}
*/
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
    b.h = mismatch; 
}

void check_moves(const BoardState& initial, vector<BoardState>& boardVector)
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
        boardVector.insert(boardVector.begin(), swap(initial, 1, 0));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 3, 0));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 1)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 0, 1));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 2, 1));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 4, 1));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 2)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 1, 2));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 5, 2));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 3)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 0, 3));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 4, 3));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 6, 3));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 4)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 1, 4));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 3, 4));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 5, 4));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 7, 4));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 5)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 2, 5));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 4, 5));	
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 8, 5));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 6)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 3, 6));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 7, 6));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 7)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 4, 7));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 6, 7));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 8, 7));
        cout << endl;
        boardVector.front().print_board();
    }
    if (board_index == 8)
    {
        boardVector.insert(boardVector.begin(), swap(initial, 5, 8));
        cout << endl;
        boardVector.front().print_board();
        boardVector.insert(boardVector.begin(), swap(initial, 7, 8));
        cout << endl;
        boardVector.front().print_board();
    }            
}

int main()
{
	//time values
	timeval beg, end;
	//init variables
    srand(time(0));
    BoardState goal;
	//BoardState goalTwo;
    BoardState start;
	BoardState boardOne;
	BoardState boardTwo;
    vector<BoardState> open, closed;
	
	//begin timer
	gettimeofday(&beg, NULL);
	
	//start with OPEN containing only the initial node
	boardOne.gen_boardOne();
	open.push_back(boardOne);
	// boardTwo.gen_boardTwo();
	boardOne.print_board();
    boardTwo.print_board();

    check_moves(boardOne, open);
    BoardState* BESTNODE;
	BoardState* SUCCESSOR;
	
	std::cout << "sanity check: goal is goal" << (goal==goal) << std::endl;
	
	int in;
	//until goal node is found, repeat following procedure
	bool GOALNOTFOUND = true;
	bool firstPass = true;
	std::cout << "Loop Start:" << std::endl;
	while(GOALNOTFOUND)
	{
		if(open.size() == 0)
		{
			std::cout << "FAILURE: no nodes on open" << std::endl;
			GOALNOTFOUND=false;
			break;
		}
		
		//pick node on open with lowest f' value. call it BESTNODE
		//pick node here
		if(firstPass)
		{
			BESTNODE = &open[0];
			firstPass= false;
		}
		
		for(int i=0; i<open.size(); i++)
		{
			//check if SUCCESSOR is the same as any node on OPEN
			BESTNODE = ((BESTNODE->g + BESTNODE->h)>(open[i].g + open[i].h)) ? &open[i]:BESTNODE; // assign lowest value to bestnode here
		}
		std::cout << "	best node selected" << std::endl;
		if(*BESTNODE == goal)
		{
			//goal node found set condition to false and exit
			GOALNOTFOUND=false;
		}
		else
		{
			//generate successors of BESTNODE (based on directions available to 0)
			std::cout << "	OPEN SIZE B4" << open.size() << std::endl;
			
			check_moves(*BESTNODE, open);
			std::cout << "	OPEN SIZE populated" << open.size() << std::endl;
			closed.push_back(open.back());
			open.pop_back();
			std::cout << "	OPEN SIZE after" << open.size() << std::endl;
		}
		std::cout << "	generated successors" << std::endl;
		//generate hueistic values of all successors of BESTNODE
		//set BESTNODE to point to successor
		//g(SUCCESSOR) = g(BESNODE) + cost of getting from BESTNODE to SUCCESSOR
		//f'(SUCCESSOR) = g(SUCCESSOR) + h(SUCCESSOR)
		for(int i=0; i<open.size(); i++)
		{
			//check if SUCCESSOR is the same as any node on OPEN
			BESTNODE = ((BESTNODE->g + BESTNODE->h)>(open[i].g + open[i].h)) ? &open[i]:BESTNODE; // assign lowest value to bestnode here
		}
		
		
		for(int i=0; i<open.size(); i++)//iterate through 'open' vector
		{
			//check if SUCCESSOR is the same as any node on OPEN
			//std::cout << "loop: " << i << std::endl;
			if(open[i] == *BESTNODE)
			{
				//we can throw away SUCCESSOR and add OLD to list of BESTNODE's Successors
				//delete SUCCESSOR;
				//open.
				//SUCCESSOR = nullptr;
				
			}
		}
		
		std::cout << "compared bestnode to best node to successors" << std::endl;
		std::cin >> in;
			
	}
	std::cout << "Loop End" << std::endl;
	
	
	
	
	//start.gen_board();
    //start.print_board();
	/*
    cout << endl;
    goal.print_board();
    check_mismatch(start, goal);
    cout << "Number of misplaced tiles: " << start.get_mismatch() << endl;
    check_moves(start, open, openUsed);
    cout << endl;
    open[0].print_board();
	*/
	
	
	//end timer
	gettimeofday(&end, NULL);
	//print execution time
	const double runtime = end.tv_sec - beg.tv_sec + (end.tv_usec - beg.tv_usec) / 1000000.0;
	printf("compute time: %.6f s\n", runtime);
    return 0;
}
