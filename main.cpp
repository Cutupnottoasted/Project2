#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int goalBoard[9] = {1, 2, 3,
					8, 0, 4,
					7, 6, 5};

int NG = 0;
int NE = 0;
int D = 0;
int TP = 0;

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

    bool operator==(const BoardState& rhs) const
    {
        for (int i = 0; i < 9; i++)
        {
            if (board[i] != rhs.board[i])
                return false;
        }
        return true; 
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

    // Heuristic functions
    // Standard
    void set_h()
    {
        h=0;
        for(int i =0; i<9; i++)
        {
            if((board[i] != goalBoard[i]))
            {
                h++;
            }
        }
    }

    // Manhattan Distance
    void set_HAustin(Gridtype map)
    {
        h = 0;
        for (int current_loc = 0; current_loc < 9; current_loc++)
        {
            for (int home = 0; home < 9; home++)
            {
                if (board[current_loc] == goalBoard[home])
                {
                    h += (abs(map[current_loc].first - map[home].first) + abs(map[current_loc].second - map[home].second));
                }
            }
        }
    }

    // Euclidean Distance
    void set_HRobert(Gridtype map) 
    {
        h = 0;
        for (int current_loc = 0; current_loc < 9; current_loc++)
        {
            for (int home = 0; home < 9; home++)
            {
                if (board[current_loc] == goalBoard[home])
                {
                    h += sqrt( ((map[home].first - map[current_loc].first) * (map[home].first - map[current_loc].first) +
                                (map[home].second - map[current_loc].second) * (map[home].second - map[current_loc].second)) );
                }
            }
        }
    }

    // Non-Heuristic function
    // Manhattan Distance/Perimeter & Center Check
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
        // perimeter check
        if(board[0] != 1)
            s += 2;
        if(board[1] != 2)
            s += 2;
        if(board[2] != 3)
            s += 2;
        if(board[3] != 8)
            s += 2;
        if(board[5] != 4)
            s += 2;
        if(board[6] != 7)
            s += 2;
        if(board[7] != 6)
            s += 2;
        if(board[8] != 5)
            s += 2;
        // center check
        if(board[4] != 0)
            s += 1;    
        h += (s * 3);
        
        cout << endl << "fn: " << h << endl;
        cout << endl;
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
        D = g;
    }
};

// used to sort vector by fn
bool sort_by_fn(const BoardState& b1, const BoardState& b2)
{
    if (b1.h >= b2.h )
    {
        return true; 
    }
    return false;
}

// swaps the blank with chosen tile
BoardState swap(BoardState b, int swap_index, int blank_index, int choice)
{
    b.board[blank_index] = b.board[swap_index];
    b.board[swap_index] = 0;
	b.g++;
    switch(choice)
    {
        case 1:
            b.set_h();
            break;
        case 2:
            b.set_HAustin(grid_map);
            break;
        case 3:
            b.set_HCody(grid_map);
            break;
        case 4:
            b.set_HRobert(grid_map);
            break;
    }
    return b;
}

// checks all avaliable moves on the board. Generates the boards and pushes into open
void check_moves(const BoardState initial, vector<BoardState>& open, int choice)
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
        open.insert(open.begin(), swap(initial, 1, 0, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 3, 0, choice));
        cout << endl;
        open.front().print_board();
        NG += 2;
        TP += 2;
    }
    if (board_index == 1)
    {
        open.insert(open.begin(), swap(initial, 0, 1, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 2, 1, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 4, 1, choice));
        cout << endl;
        open.front().print_board();
        NG += 3;
        TP += 3;
    }
    if (board_index == 2)
    {
        open.insert(open.begin(), swap(initial, 1, 2, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 5, 2, choice));
        cout << endl;
        open.front().print_board();
        NG += 2;
        TP += 2;
    }
    if (board_index == 3)
    {
        open.insert(open.begin(), swap(initial, 0, 3, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 4, 3, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 6, 3, choice));
        cout << endl;
        open.front().print_board();
        NG += 3;
        TP += 3;

    }
    if (board_index == 4)
    {
        open.insert(open.begin(), swap(initial, 1, 4, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 3, 4, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 5, 4, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 7, 4, choice));
        cout << endl;
        open.front().print_board();
        NG += 4;
        TP += 4;
    }
    if (board_index == 5)
    {
        open.insert(open.begin(), swap(initial, 2, 5, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 4, 5, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 8, 5, choice));
        cout << endl;
        open.front().print_board();
        NG += 3;
        TP += 3;
    }
    if (board_index == 6)
    {
        open.insert(open.begin(), swap(initial, 3, 6, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 7, 6, choice));
        cout << endl;
        open.front().print_board();
        NG += 2;
        TP += 2;
    }
    if (board_index == 7)
    {
        open.insert(open.begin(), swap(initial, 4, 7, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 6, 7, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 8, 7, choice));
        cout << endl;
        open.front().print_board();
        NG += 3;
        TP += 3;
    }
    if (board_index == 8)
    {
        open.insert(open.begin(), swap(initial, 5, 8, choice));
        cout << endl;
        open.front().print_board();
        open.insert(open.begin(), swap(initial, 7, 8, choice));
        cout << endl;
        open.front().print_board();
        NG += 2;
        TP += 2;
    }

    sort(open.begin(), open.end(), sort_by_fn);
}

// sets the grid map coordinates to calculate effective distance
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


bool check_dup(vector<BoardState> open, vector<BoardState> closed)
{
    for (BoardState b : closed)
    {
        if(b == open.back())
            return true;
    }
    return false;
}

int main()
{
    BoardState initial, goal; // initial board
    vector<BoardState> open, closed; // open and closed vectors
    BoardState* BESTNODE; // pointer to best node
	BoardState* SUCCESSOR; // pointer to successor node
    set_map(grid_map); // set map
    srand(time(NULL));
    int boardChoice = 0;
	int algChoice = 0;
    cout << "Please select 1 or 2 for which board you would like to solve:" << endl;
	// cin >> boardChoice;
    boardChoice = 2;
    switch(boardChoice)
	{
        case 1:
            initial.gen_boardOne();
            break;
        case 2:
            initial.gen_boardTwo();
            break;
	}

    cout << "Please select a heuristic you would like to use:" << endl;
    cout << "1: Default" << endl << "2: set_HAustin" << endl << "3: set_HCody" << endl << "4: set_HRobert" << endl;
    // cin >> algChoice;
    algChoice = 2;
    switch(algChoice)
    {
        case 1:
            initial.set_h();
            break;
        case 2:
            initial.set_HAustin(grid_map);
            break;
        case 3:
            initial.set_HCody(grid_map);
            break;
        case 4:
            initial.set_HRobert(grid_map);
            break;
    }

    timeval beg, end; // initiate time values
    gettimeofday(&beg, NULL); // begin timer
    // insert initial board into open
    open.insert(open.begin(), initial);
    // print initial board
    cout <<"initial board: " << endl;
	initial.print_board();
    // insert open board into closed
    closed.insert(closed.begin(), open.back());
    open.pop_back(); // remove board from open
    check_moves(initial, open, algChoice);
    NE +=1;
    while(true) // begin loop
    {
        while(check_dup(open, closed))
        {
            open.insert(open.begin(), open.back());
            open.pop_back(); 
        }
        SUCCESSOR = &open.back();
        cout << endl << "Successor's board: " << endl;
        SUCCESSOR -> print_board();
        cout << endl << "Most signficant closed node: " << endl;
        closed.front().print_board();
        if (*SUCCESSOR == goal)
            break;
        open.pop_back();
        closed.insert(closed.begin(), *SUCCESSOR);
        // sort(closed.begin(), closed.end(), sort_by_fn);
        cout << endl << "Successor entered the closed vector " << endl;
        cout << endl << "Generating children" << endl;
        // if the successor matches any in the closed, choose a new successor
        check_moves(*SUCCESSOR, open, algChoice);
        sort(open.begin(), open.end(), sort_by_fn);
        NE +=1;
    }

    // end timer
	gettimeofday(&end, NULL);
	//print execution time & metrics 
	const double runtime = end.tv_sec - beg.tv_sec + (end.tv_usec - beg.tv_usec) / 1000000.0;
	printf("Time till solution found: %.6f s\n", runtime);
	cout << "Nodes Generated: " << NG << endl;
	cout << "Nodes Expanded: " << NE << endl;
	cout << "Depth of Tree: " << D << endl;
	cout << "Effective Branching Factor: " << (NG/D) << endl;
	cout << "Total Path: " << TP << endl;
}