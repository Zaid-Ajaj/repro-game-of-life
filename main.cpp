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
    char inputToken = 0; // initial
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
    char rowInput[NO_OF_COLUMNS + 1];

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

int is_alive(Cell cell) 
{
    return cell == Live ? 1 : 0;
}
//  Part 3: the next generation
void next_generation (Cell universe [ROWS][COLUMNS], Cell next [ROWS][COLUMNS])
{
    // pre-conditions, post-conditions, implementation
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLUMNS; col++) {

            bool currentCellIsEdgeCell =  row == 0 
                                       || row == ROWS - 1
                                       || col == 0
                                       || col == COLUMNS - 1;
             
            if (currentCellIsEdgeCell) {
                // Cells on the edges or (walls of universe) must be all DEAD!
                next[row][col] = Dead;
                continue;
            }

            Cell current = universe[row][col];
            // get surrounding 8 cells
            Cell topCenter = universe[row - 1][col];
            Cell topRight = universe[row - 1][col + 1];
            Cell topLeft = universe[row - 1][col - 1];
            Cell right = universe[row][col + 1];
            Cell left = universe[row][col - 1];
            Cell bottomCenter = universe[row + 1][col];
            Cell bottomRight = universe[row + 1][col - 1];
            Cell bottomLeft = universe[row + 1][col + 1];

            int aliveCount = is_alive(topCenter) + is_alive(bottomCenter)
                           + is_alive(topRight ) + is_alive(bottomRight)
                           + is_alive(topLeft  ) + is_alive(bottomLeft)
                           + is_alive(right    ) + is_alive(left);

            int deadCount = 8 - aliveCount;
            
            if (universe[row][col] == Live) {
                if (aliveCount < 2) {
                    // Under population :(
                    cout << "Cell[" << row << ", " << col << "] died (under population)" << endl;
                    next[row][col] = Dead;
                } else if (aliveCount == 2 || aliveCount == 3) {
                    // Still happy and rich ;-)
                    cout << "Cell[" << row << ", " << col << "] stayed as is" << endl;
                    next[row][col] = Live;
                } else {
                    cout << "Cell[" << row << ", " << col << "] died (over population)" << endl;
                    // Over population -_-
                    next[row][col] = Dead;
                }
            } else {
                if (aliveCount == 3) {
                    cout << "Cell[" << row << ", " << col << "] Became alive" << endl;
                    // exactly 3? I see what you did there
                    next[row][col] == Live;
                } else {
                    next[row][col] = Dead;
                }
            }
        }
    }
}

void main ()
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
    Cell nextState[ROWS][COLUMNS];
    cout << "Next state transition becomes: " << endl;
    next_generation(universe, nextState);
    show_universe(nextState);
}
