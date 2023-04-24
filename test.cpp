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
#include <bits/stdc++.h>
using namespace std;

int goalBoardOne[9] = {2, 8, 3,
                    1, 6, 4,
                    0, 7, 5};
					
int goalBoardTwo[9] = {2, 1, 6,
                    4, 0, 8,
                    7, 5, 3};

int goalBoard[9] = {1,2,3,
					8,0,4,
					7,6,5};

typedef map<int, std::pair<int, int> > Gridtype; 
Gridtype grid_map; 
struct BoardState 
{	
    bool open; // if generated board is correct path
    int g; // level of board state
    int h; // sum of misplaced tiles
    int board[9] = {1, 2, 3,
                    8, 0, 4,
                    7, 6, 5};

    BoardState()
    {
        open = true;
        g = 0;
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
		board[6] = 0;
		board[7] = 7;
		board[8] = 5;
    }
	
	void gen_boardTwo()
    {
		board[0] = 2;
		board[1] = 1;
		board[2] = 6;
		board[3] = 4;
		board[4] = 0;
		board[5] = 8;
		board[6] = 7;
		board[7] = 5;
		board[8] = 3;
    }

    void print_board()
    {
        cout << endl;
        for (int i = 0; i < 9; i++)
        {
            cout << board[i] << " ";
            if ((i + 1) % 3 == 0)
                cout << endl;
        }
        cout << "fn: " << g+h << endl;
        cout << "gn: " << g << endl; 
    }

void set_HCody(Gridtype map)
{
    h = 0;
    int s = 0; 
    for (int current_loc = 0; current_loc < 9; current_loc++)
    {
        for (int home = 0; home < 9; home++)
        {
            if (board[current_loc] == goalBoard[home])
            {
                h += ( abs(map[current_loc].first - map[home].first) + 
                        abs(map[current_loc].second - map[home].second));
                
            }
        }   
    }
    // s(n) & center
    // check perimeter and if successor is not correct +2
    // goal:    index:
    // 1 2 3    0 1 2
    // 8 0 4 -> 3 4 5
    // 7 6 5    6 7 8
    // if index != successor
    // if 3 != 1
    // if 0 != 2
    // if 1 != 3
    // if 2 != 4
    // if 5 != 5
    // if 8 != 6
    // if 7 != 7
    // if 6 != 8
    // if 4 != 0
    if(board[3] != 1) 
        s += 2;
    if(board[0] != 2)
        s += 2; 
    if(board[1] != 3)
        s += 2;
    if(board[2] != 4)
        s += 2;
    if(board[5] != 5)
        s += 2;
    if(board[8] != 6)
        s += 2;
    if(board[7] != 7)
        s += 2;
    if(board[6] != 8)
        s += 2;
    h += (s * 3);// h3(n)
    if(board[4] != 0)
        h += 1;
    cout << endl << "fn: " << h << endl;
    cout << endl;
}

void set_HEuclidean() {
    int hn = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] != 0) {
            int goalIndex = find(goalBoardOne, goalBoardOne+9, board[i]) - goalBoardOne;
            if (goalIndex != i) {
                int x1 = i % 3, y1 = i / 3;
                int x2 = goalIndex % 3, y2 = goalIndex / 3;
                hn += sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1) * (y2 - y1)));
            }
        }
    }
    h = hn;
}

void set_HRobert(Gridtype map) // this is SET_HEUCLIDEAN not horizontal distance
{
    h = 0;
    for (int current_loc = 0; current_loc < 9; current_loc++)
    {
        for (int home = 0; home < 9; home++)
        {
            if (board[current_loc] == goalBoard[home])
            {
                h += sqrt(((map[home].first - map[current_loc].first)*(map[home].first - map[current_loc].first) +
                            (map[home].second - map[current_loc].second) * (map[home].second - map[current_loc].second)));
            }
        }
    }
}
	
void set_HAustin(Gridtype map)
{
    h = 0; 
    for (int current_loc = 0; current_loc < 9; current_loc++)
    {
        for (int home = 0; home < 9; home++)
        {
            if (board[current_loc] == goalBoard[home])
            {
                h += ( abs(map[current_loc].first - map[home].first) + 
                        abs(map[current_loc].second - map[home].second));
            }
        }
    }
}
void set_h()
{
    h=0;
    //std::cout << "hey" << std::endl;
    for(int i =0; i<9; i++)
    {
        //std::cout << "=" << std::endl;
        if((board[i] != goalBoard[i]))
        {
            h++;
        }
    }
    //std::cout << "hey2" << std::endl;
}

    int get_mismatch() { return h; }              
};

bool sort_by_fn(const BoardState& b1, const BoardState& b2)
{
    if ((b1.g + b1.h) > (b2.g + b2.h))
        return true;
    else if ((b1.g + b1.h) >= (b2.g + b2.h))
    {
        if(b1.g < b2.g)
            return true; 
    }

    return false; 
}

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
    b.board[swap_index] = 0;
	b.g++;
    // b.set_HCody(grid_map);
	// b.set_HAustin(grid_map);
    // b.set_h();
    // b.set_HEuclidean(); 
    b.set_HRobert(grid_map);
    return b; 
}

int check_mismatch(BoardState b)
{
    int mismatch = 0;
    for (int i = 0; i < 9; i++)
    {
        if (b.board[i] != goalBoard[i])
            mismatch++;
    }
    return mismatch;
}

void check_moves(const BoardState initial, vector<BoardState>& boardVector)
{
    int board_index;
    int array_index; 
    for (int i = 0; i < 9; i++)
    {
        if (initial.board[i] == 0)
            board_index = i; 
    }
    cout << "The blank index is: " << board_index << endl;

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

    sort(boardVector.begin(), boardVector.end(), sort_by_fn);            
}

void set_map(Gridtype& map)
{
    int i = 0;
    int y = 3;
    while(i < 9)
    {
        for (int x = 1; x <= 3; x++)
        {
            map[i] = make_pair(x, y);
            i++;
        }
        y--;
    }

    for (int i = 0; i < 9; i++)
    {
        cout <<"index: " << i << " x: " << map[i].first << " y: " << map[i].second << endl; 
    }
}

int main()
{
	//time values
	timeval beg, end;
	//init variables
    srand(time(0));

	//BoardState goalTwo;

	BoardState initial;

    vector<BoardState> open, closed;
    set_map(grid_map); 
	
	//begin timer
	gettimeofday(&beg, NULL);
	
	//start with OPEN containing only the initial node
	initial.gen_boardOne();
    // initial.set_HRobert(grid_map);
    initial.set_HRobert(grid_map);
    // initial.set_h();
    // initial.set_HAustin(grid_map);
	open.insert(open.begin(), initial); 

	// boardTwo.gen_boardTwo();
    cout <<"initial board: " << endl;
	initial.print_board();
    // boardTwo.print_board();
    closed.insert(closed.begin(), open.back());
    open.pop_back();
    cout << endl << "Open's size: " << open.size() << endl; 
    cout << endl << "initial board moved to closed and open popped" << endl;
    cout << endl << "Checking moves" << endl;
    check_moves(initial, open);
    cout << endl << "initial board's children generated" << endl;
    BoardState* BESTNODE;
	BoardState* SUCCESSOR;
    cout << endl << "Printing all new children" << endl; 
    for(int i = 0; i < open.size(); i++)
    {
        open.at(i).print_board();
    }

    bool run = true;
    while (run)
    {
        SUCCESSOR = &open.back();
        cout << endl << "Successor's board: " << endl;
        SUCCESSOR -> print_board();
        if (check_mismatch(*SUCCESSOR) == 0)
            break;
        open.pop_back();
        closed.insert(closed.begin(), *SUCCESSOR);
        cout << endl << "Closed's size: " << closed.size() << endl;
        cout << endl << "sorting closed" << endl;
        sort(closed.begin(), closed.end(), sort_by_fn);
        cout << endl << "Most significant closed node: " << endl; 
        closed.back().print_board(); 
        cout << endl << "Checking moves" << endl;
        check_moves(*SUCCESSOR, open);
        cout << endl << "Generated Successors children" << endl;
        cout << endl << "Printing all new children" << endl;
        sort(open.begin(), open.end(), sort_by_fn);  
    }
    cout << endl << "found goal" << endl;
	// std::cout << "sanity check: goal is goal" << (goal==goal) << std::endl;
	
	// int in;
	// //until goal node is found, repeat following procedure
	// bool GOALNOTFOUND = true;
	// bool firstPass = true;
	// std::cout << "Loop Start:" << std::endl;
	// while(GOALNOTFOUND)
	// {
	// 	if(open.size() == 0)
	// 	{
	// 		std::cout << "FAILURE: no nodes on open" << std::endl;
	// 		GOALNOTFOUND=false;
	// 		break;
	// 	}
		
	// 	//pick node on open with lowest f' value. call it BESTNODE
	// 	//pick node here
	// 	if(firstPass)
	// 	{
	// 		BESTNODE = &open[0];
	// 		firstPass= false;
	// 	}
		
	// 	for(int i=0; i<open.size(); i++)
	// 	{
	// 		//check if SUCCESSOR is the same as any node on OPEN
	// 		BESTNODE = ((BESTNODE->g + BESTNODE->h)>(open[i].g + open[i].h)) ? &open[i]:BESTNODE; // assign lowest value to bestnode here
	// 	}
	// 	std::cout << "	best node selected" << std::endl;
	// 	if(*BESTNODE == goal)
	// 	{
	// 		//goal node found set condition to false and exit
	// 		GOALNOTFOUND=false;
	// 	}
	// 	else
	// 	{
	// 		//generate successors of BESTNODE (based on directions available to 0)
	// 		std::cout << "	OPEN SIZE B4" << open.size() << std::endl;
			
	// 		check_moves(*BESTNODE, open);
	// 		std::cout << "	OPEN SIZE populated" << open.size() << std::endl;
	// 		closed.push_back(open.back());
	// 		open.pop_back();
	// 		std::cout << "	OPEN SIZE after" << open.size() << std::endl;
	// 	}
	// 	std::cout << "	generated successors" << std::endl;
	// 	//generate hueistic values of all successors of BESTNODE
	// 	//set BESTNODE to point to successor
	// 	//g(SUCCESSOR) = g(BESNODE) + cost of getting from BESTNODE to SUCCESSOR
	// 	//f'(SUCCESSOR) = g(SUCCESSOR) + h(SUCCESSOR)
	// 	for(int i=0; i<open.size(); i++)
	// 	{
	// 		//check if SUCCESSOR is the same as any node on OPEN
	// 		BESTNODE = ((BESTNODE->g + BESTNODE->h)>(open[i].g + open[i].h)) ? &open[i]:BESTNODE; // assign lowest value to bestnode here
	// 	}
		
		
	// 	for(int i=0; i<open.size(); i++)//iterate through 'open' vector
	// 	{
	// 		//check if SUCCESSOR is the same as any node on OPEN
	// 		//std::cout << "loop: " << i << std::endl;
	// 		if(open[i] == *BESTNODE)
	// 		{
	// 			//we can throw away SUCCESSOR and add OLD to list of BESTNODE's Successors
	// 			//delete SUCCESSOR;
	// 			//open.
	// 			//SUCCESSOR = nullptr;
				
	// 		}
	// 	}
		
	// 	std::cout << "compared bestnode to best node to successors" << std::endl;
	// 	//std::cin >> in;
			
	// }
	// std::cout << "Loop End" << std::endl;
	
	
	
	
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
