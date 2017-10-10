#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

enum Cell { Dead = 0, Live };                         // a cell is either Dead or Live (we use the fact that dead = 0 and live = 1)

const char DEAD             = '.' ;                // the presentation of a dead cell (both on file and screen)
const char LIVE             = '*' ;                // the presentation of a live cell (both on file and screen)
const int NO_OF_ROWS        = 40;                  // the number of rows (height) of the universe (both on file and screen)
const int NO_OF_COLUMNS     = 60;                  // the number of columns (width) of the universe (both on file and screen)
const int ROWS              = NO_OF_ROWS    + 2 ; // the number of rows in a universe array, including the 'frame' of dead cells
const int COLUMNS           = NO_OF_COLUMNS + 2 ; // the number of columns in a universe array, including the 'frame' of dead cells

const int MAX_FILENAME_LENGTH = 80 ;              // the maximum number of characters for a file name (including termination character)

//  Part 1: one-dimensional arrays
bool enter_filename (char filename [MAX_FILENAME_LENGTH])
{
    // pre-conditions
    assert (MAX_FILENAME_LENGTH > 0);
    char inputToken; // initial
    int position = 0;
    // keep reading charecters till user presses ENTER
    while(inputToken != '\n') {
        inputToken = getchar();
        filename[position] = inputToken;
        position = position + 1;
    }
    // At this point, user pressed ENTER -> replace the termination char
    filename[position - 1] = '\0';

    int inputFileNameLength = position + 1; 
    if (inputFileNameLength > MAX_FILENAME_LENGTH) {
        return false;
    } else {
        return true;
    }
}

//  Part 2: setting the scene
bool read_universe_file (ifstream& inputFile, Cell universe [ROWS][COLUMNS])
{
    char rowInput[NO_OF_COLUMNS];

    if (!inputFile) {
        cout << "Unable to read the input file stream" << endl;
        return false;
    }
    
    for(int row = 0; row < ROWS; row++)
    {
        // Read lines only when we are not on first nor last row
        if (row != 0 && row != ROWS - 1) {
            inputFile.getline(rowInput, NO_OF_COLUMNS + 1);
        }

        for(int column = 0; column < COLUMNS; column++) {
            // if we are on edge cell, make it DEAD!
            if (row == 0 || row == ROWS - 1 || column == 0 || column == COLUMNS - 1) {
                universe[row][column] = Dead;
            } else {
                char cell = rowInput[column - 1];
                // read every token and turn in into a cell
                if (cell == DEAD) {
                    universe[row][column] = Dead;
                } else {
                    universe[row][column] = Live;
                }
            }
        }
    }

    inputFile.close();
    return true;
}

void show_universe (Cell universe [ROWS][COLUMNS])
{
    for(int row = 0; row < ROWS; row++) { 
        for(int col = 0; col < COLUMNS; col++) {
            if (universe[row][col] == Dead) {
                cout << DEAD;
            } else {
                cout << LIVE;
            }
        }
        // begin new line
        cout << endl;
    }
}

//  Part 3: the next generation
//void next_generation (Cell now [ROWS][COLUMNS], Cell next [ROWS][COLUMNS])
//{
//    // pre-conditions, post-conditions, implementation
//}

int main ()
{
    cout << "Please enter the name of file containing initial configuration: ";
    char filename[MAX_FILENAME_LENGTH];
    
    if (!enter_filename(filename)) {
        cout << "Wrong file name entered, closing..." << endl;
        return;
    }

    // turn the charecter array into string
    // discarding trailing unused space in the array
    string file = "";
    int tokenPosition = 0;
    while (filename[tokenPosition] != '\0') {
        file += filename[tokenPosition];
        tokenPosition++;
    }

    cout << "You entered: '" << file << "'" << endl;
    ifstream fileInput(file, ifstream::in);
    Cell universe[ROWS][COLUMNS];
    cout << "Start reading file" << endl;
    bool result = read_universe_file(fileInput, universe);
    cout << "Finished reading file" << endl;
    show_universe(universe);
}
