// Life simulation
//Kevin Anderson, Ari Soria-Huerta, Timothy Huff, Jan Arroyo, Thomas Menter, Aden Yu

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

const int MAXWORLDROWS = 25;
const int MAXWORLDCOLS = 80;
const int MAXROWS = MAXWORLDROWS + 2;
const int MAXCOLS = MAXWORLDCOLS + 2;
const int FILENAMESIZE = 255;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
// for interesting effect use code 219 and 176 instead of the asterisk and space
const unsigned char CELL = '*';  // 219;
const unsigned char EMPTY = ' '; // 176;

enum class State { dead = 0, alive = 1 };

/*
* initWorld is used to initialize all positions in the world
* array to the 'dead' state
*/
void initWorld(State world[][MAXCOLS]);

/*
* display is used to draw the 'world' onto the screen
* for each 'alive' cell, the value CELL is displayed
* for each 'dead' cell, the value EMPTY is displayed
* does NOT display any items in:
*   row 0, row MAXROWS-1, column 0, or column MAXCOLS-1
* since these rows and columns are used as a "border"
* around the world grid
*/
void display(State world[][MAXCOLS]);

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and update the world
* based on the contents of the file. The file should contain
* only spaces and asterisks. A space represents a 'dead' cell
* and an asterisk represents an 'alive' cell.
* The function stops reading and exits the program if > MAXWORLDCOLS
* characters are found on one row, or if > MAXWORLDROWS rows are
* found in the file.
*/
void getFile(State world[][MAXCOLS]);

/*
* calcNewWorld is used to calculate the new state of the world
* (i.e. a new generation) based upon analyzing the current world state
*/
void calcNewWorld(State world[][MAXCOLS]);

/*
* neighborCount is used to count the number of 'alive' cells in
* the 8 neighboring positions which are surrounding position
* [row][col] in world
*/
int neighborCount(State world[][MAXCOLS], int row, int col);


int main()
{
	State world[MAXROWS][MAXCOLS];
	int generation = 0;
	bool stop = false;
	
	
	initWorld(world);

	getFile(world);
	
	do
	{
		// system("cls") clears the screen; only works on Windows machines
		// You will probably need to comment this out if working on
		// a non-Windows machine
		system("cls");

		display(world);
		generation++;
		cout << "Generation: " << generation << "\t";
		cout << "<ENTER> to continue / <Q + ENTER> to quit: ";
		
		// TODO: get input from user and decide whether to quit the loop
		// and whether to compute a new generation

		//gets a key press for enter or q+enter and decides wether that key press exits the program or continues to the next generation.
		//strings are the only solution that accounts for additional keypressees before enter.
		char c [FILENAMESIZE];
		int i = 0;
		cin.getline(c, FILENAMESIZE);
			
		//detects if q or Q is pressed but only if its the only character pressed before enter. and exits the game.
		if (c[i] == 'q' || c[i] == 'Q')
		{
			stop = true;
		}
		else if (c[i] != '\0')
		{
			stop = false;
			i++;
		}

		calcNewWorld(world);
		
	} while (!stop);

	return 0;
}


//Calculates the number of neighbors around the 8 blocks surrounding each cell in the world array
//uses algebra within for loops to achieve this.
int neighborCount(State world[][MAXCOLS], int row, int col)
{
	// This code checks to make sure you aren't trying to count neighbors for any
	// position on row #0, or on row #MAXWORLDROWS+1,
	// or in column 0, or in columnn MAXWORLDCOLUMNS+1
	// because if you are, you are going to run into array out-of-bounds problems
	if (row < 1 || row > MAXWORLDROWS)
	{
		cerr << "ERROR: invalid row parameter " << row << " in neighborCount\n";
		exit(1);
	}
	else if (col < 1 || col > MAXWORLDCOLS)
	{
		cerr << "ERROR: invalid col parameter " << col << " in neighborCount\n";
		exit(1);
	}

	//i is count.
	int i = 0; 
	int x; 
	int y;

	// TODO: write neighborCount code
	for (x = row - 1; x <= row + 1; x++)
	{
		for (y = col - 1; y <= col + 1; y++)
		{
			//skips the currently selected cell, you cant be your own neighbor
			if (x == row && y == col)
			{
				y++;
			}
			//neighbors mean count goes up
			if (world[x][y] == State::alive)
			{
				i++;
			}	
		}
	}
	return i;
}


//calls the neighbor count function for each cell currently called in the calcNewWorld function.
//a temp array is used for this to hold values and allow the changes to occur across the board simaltaniously.
void calcNewWorld(State world[][MAXCOLS])
{
	//TODO: write calcNewWorld code

	//uses a temp array to avoid current changes from overwriting changes to the next cell. 
	State tempworld[MAXROWS][MAXCOLS];

	for (int x = 1; x <= MAXWORLDROWS; x++)
	{
		for (int y = 1; y <= MAXWORLDCOLS; y++)
		{
			int neighbors = neighborCount(world, x, y);

			//statements to check the conditions at which a cell lives or dies
			if (world[x][y] == State::alive && (neighbors == 2 || neighbors == 3))
			{
				tempworld[x][y] = State::alive;
			}
			else if (world[x][y] == State::dead && neighbors == 3)
			{
				tempworld[x][y] = State::alive;
			}
			else
			{
				tempworld[x][y] = State::dead;
			}
		}
	}
	//places temp array back into the world array
	for (int i = 1; i <= MAXWORLDROWS; i++)
	{
		for (int j = 1; j <= MAXWORLDCOLS; j++)
		{
			world[i][j] = tempworld[i][j];
		}
	}
}


//Uses getline to load each row of the file into the line array and then compare that line against its corresponding line in the world 2Darray.
//Checks for Errors along the way
void getFile(State world[][MAXCOLS])
{
	int row = 1;
	size_t lineSize;
	
	char input[FILENAMESIZE];
	char line[MAXWORLDCOLS + 1];

	
	//asks the user to enter a file name to be opened.
	printf("Enter the name of the file you want to use: ");
	printf("\n");
	cin.getline(input, FILENAMESIZE);

	//initializes filestream and opens the user entered file
	ifstream inFile;
	inFile.open(input);

	//checks if a file is open and throws an error if not and closes the program
	if (!inFile.is_open())
	{
		cerr << "ERROR: Input file cannot be opened.";
		exit(1);
	}
	//grabs a line from each row and stores them in a string array
	while(row <= MAXWORLDROWS && inFile.getline(line, MAXWORLDCOLS + 1))
	{
		//gives the size of the line entered in the string line.
		lineSize = strlen(line);
		
		//checks if the value stored in each column of the string array line is alive or dead.
		for (int i = 0; i < lineSize; i++)
		{
			if (line[i] == '*')
			{
				world[row][i+1] = State::alive;
			}
		}
		row++;
	}

	// After file reading is finished, this code checks to see if the reading stopped
	// because of too many characters on a line in the file, or too many rows in the file
	// It is possible that you might have to change the conditions here depending on
	// exactly how you choose to read the file
	if (!inFile.eof())
	{
		if (row <= MAXWORLDROWS)
		{
			cerr << "\nERROR: Line " << row << " in input file contains more than " << MAXWORLDCOLS << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXWORLDROWS << " lines in the file\n";
		}
		inFile.close();
		exit(1);
	}

	inFile.close();
}


//displays the current state of the world array in context of CELLs and EMPTYs. 
// or as astrisks and spaces for human eyes. 
void display(State world[][MAXCOLS])
{
	for (int i = 1; i < MAXROWS; i++)
	{
		for (int j = 1; j < MAXCOLS; j++)
		{
			if (world[i][j] == State::alive)
			{
				printf("%c", CELL);
			}
			else
			{
				printf("%c", EMPTY);
			}
		}
		printf("\n");
	}
	printf("\n");
}


//initialize the world with all zero's so that the board is an even 80 by 25.
void initWorld(State world[][MAXCOLS])
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			world[i][j] = State::dead;
		}
	}
}