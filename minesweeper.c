#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


typedef struct minepoint_struct {
    int x;
    int y;
} minepoint;

typedef struct minefield_struct {
    char** board;
    int num_rows;
    int num_cols;
    int num_mines;
} minefield;

typedef struct cell_struct {
    int cell_row;
    int cell_col;
    int type;
} cell;

bool isValidFormat(const int numArgsRead, const int numArgsNeed); 
minefield InitializeField(const int num_rows, const int num_cols, const int num_mines);
void InitializeMines(minefield* minesboard, minepoint *minelocations);
bool RevealTilesBlank(minefield* minesboard, minefield* topboard, int playerrow, int playercol, int num_rows, int num_cols);
void printfield(minefield* topboard);
void PlayGame(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol);
bool GameWin(minefield* minesboard, int* playerrow, int* playercol, int* correctminesmarked);
void RevealTiles(minefield* minesboard, minefield* topboard, int playerrow, int playercol);
void GetPlayerMove(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol, int* playerflagrow, int* playerflagcol, int* correctminesmarked);
void getValidRow(const char* prompt, int* playerrow);
void getValidCol(const char* prompt, int* playercol);
int getValidInt(const char* prompt);
char getValidChar(const char* prompt);
bool my_isspace(char c);
void red();
void blue();
void green();
void purple();
void RevealAllTiles(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol);
void flagmine(minefield* topboard, int* playerflagrow, int* playerflagcol, int* correctminesmarked);


int main() {
    minefield bottomfield;
    minefield topfield;
    minepoint locations[81];
    int playerrow, playercol;
    int correctminesmarked;
    topfield = InitializeField(9, 9, 10);
    bottomfield = InitializeField(9, 9, 10);
    InitializeMines(&bottomfield, locations);
    PlayGame(&bottomfield, &topfield, &playerrow, &playercol);

    return 0; 
}

void PlayGame(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol) {
    int correctminesmarked;
    int playerflagrow, playerflagcol;
    printfield(topboard);
    printfield(minesboard);
    do {
        GetPlayerMove(minesboard, topboard, playerrow, playercol, &playerflagrow, &playerflagrow, &correctminesmarked);
        RevealAllTiles(minesboard, topboard, playerrow, playercol);
        printfield(topboard);
    } while (!GameWin(minesboard, playerrow, playercol, &correctminesmarked));
    printf("You Win!");
    exit(0);
    
}

void flagmine(minefield* topboard, int* playerflagrow, int* playerflagcol, int* correctminesmarked) {
    (*correctminesmarked)--;
    topboard->board[*playerflagrow][*playerflagcol] = 'F';
}

void GetPlayerMove(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol, int* playerflagrow, int* playerflagcol, int* correctminesmarked) {
    int flagchoicerow, flagchoicecol;
    char if_want_flag;
    
    char* rowprompt = (char*)calloc(100, sizeof(char));
    char* columnprompt = (char*)calloc(100, sizeof(char));
    sprintf(rowprompt, "Enter a row between 0 and %d to play in: ", minesboard->num_rows - 1);
    getValidRow(rowprompt, playerrow);
    sprintf(columnprompt, "Enter a column between 0 and %d to play in: ", minesboard->num_cols - 1);
    getValidCol(columnprompt, playercol);
    if_want_flag = getValidChar("Enter y if you would like to flag the mine. Enter n if you would not like to flag the mine.");
    if (if_want_flag == 'y' || if_want_flag == 'Y') {
        flagchoicerow = getValidInt("Enter the row you want to flag the mine at: ");
        flagchoicecol = getValidInt("Enter the column you want to flag the mine at: ");
        *playerflagrow = flagchoicerow;
        *playerflagcol = flagchoicecol;
        flagmine(topboard, playerflagrow, playerflagcol, correctminesmarked);
    }
    else {
        ;
    }
    
}

bool GameWin(minefield* minesboard, int* playerrow, int* playercol, int* correctminesmarked) {
    if (*correctminesmarked == minesboard->num_mines) {
        return true;
    }
    else {
        return false;
    }

}

void RevealAllTiles(minefield* minesboard, minefield* topboard, int* playerrow, int* playercol) {
   if (minesboard->board[*playerrow][*playercol] == 'M') {
       for (int rows = 0; rows < minesboard->num_rows; rows++) {
           for (int columns = 0; columns < minesboard->num_rows; columns++) {
               if (minesboard->board[rows][columns] == 'M') {
                   topboard->board[rows][columns] = 'M';
               }
           }
       }
       printf("Game over! You Stepped on a Mine!");
       printfield(topboard);
       exit(0);
   }
   else if (minesboard->board[*playerrow][*playercol] == '-') {
       RevealTilesBlank(minesboard, topboard, *playerrow, *playercol, 9, 9);
       RevealTiles(minesboard, topboard, *playerrow, *playercol);
   }
   else {
       topboard->board[*playerrow][*playercol] = minesboard->board[*playerrow][*playercol];
   }

}

void RevealTiles(minefield* minesboard, minefield* topboard, int playerrow, int playercol) {
    for (int rows = 0; rows < minesboard->num_rows; rows++) {
        for (int columns = 0; columns < minesboard->num_cols; columns++) {
            if (minesboard->board[rows][columns] == '*') {
                if (playercol - 1 < 0 || playercol > minesboard->num_cols - 1) {
                    ;
                }
                else if (minesboard->board[rows][columns - 1] != 'M' && minesboard->board[rows][columns - 1] != '-') {
                   topboard->board[rows][columns - 1] = minesboard->board[rows][columns - 1];
                }
                if (playercol + 1 > minesboard->num_cols - 1) {
                    ;
                }
                else if (minesboard->board[rows][columns + 1] != 'M' && minesboard->board[rows][columns + 1] != '-') {
                    topboard->board[rows][columns + 1] = minesboard->board[rows][columns + 1];
                }
                if (playerrow + 1 > minesboard->num_rows - 1) {
                    ;
                }
                else if (minesboard->board[rows + 1][columns] != 'M' && minesboard->board[rows + 1][columns] != '-') {
                    topboard->board[rows + 1][columns] = minesboard->board[rows + 1][columns];
                }
                if (playerrow - 1 < 0) {
                    ;
                }
                else if (minesboard->board[rows - 1][columns] != 'M' && minesboard->board[rows - 1][columns] != '-') {
                   topboard->board[rows - 1][columns] = minesboard->board[rows - 1][columns];
                }
                if (playerrow + 1 > minesboard->num_rows - 1 || playercol + 1 > minesboard->num_cols - 1) {
                    ;
                }
                else if (minesboard->board[rows + 1][columns + 1] != 'M' && minesboard->board[rows + 1][columns + 1] != '-') {
                    topboard->board[rows + 1][columns + 1] = minesboard->board[rows + 1][columns + 1];
                }
                if (playerrow - 1 < 0 || playercol - 1 < 0) {
                    ;
                }
                else if (minesboard->board[rows - 1][columns - 1] != 'M' && minesboard->board[rows - 1][columns - 1] != '-') {
                    topboard->board[rows - 1][columns - 1] = minesboard->board[rows - 1][columns - 1];
                }
                if (playerrow + 1 > minesboard->num_rows - 1 || playercol - 1 < 0) {
                    ;
                }
                else if (minesboard->board[rows + 1][columns - 1] != 'M' && minesboard->board[rows + 1][columns - 1] != '-') {
                    topboard->board[rows + 1][columns - 1] = minesboard->board[rows + 1][columns - 1];
                }
                if (playerrow - 1 < 0 || playercol + 1 > minesboard->num_cols) {
                    ;
                }
                else if (minesboard->board[rows - 1][columns + 1] != 'M' && minesboard->board[rows - 1][columns + 1] != '-') {
                    topboard->board[rows - 1][columns + 1] = minesboard->board[rows - 1][columns + 1];
                }

            }
        }
    }


}



minefield InitializeField(const int num_rows, const int num_cols, const int num_mines)
{
    minefield minesboard;
    minesboard.num_rows = num_rows + 2;
    minesboard.num_cols = num_cols + 2;
    minesboard.num_mines = num_mines;
    minesboard.board = (char**)malloc(minesboard.num_rows * sizeof(char*));
    for (int rows = 0; rows < minesboard.num_rows; rows++) {
        minesboard.board[rows] = (char*)malloc(minesboard.num_cols * sizeof(char));
    }
    for (int rows = 0; rows < minesboard.num_rows; rows++) {
        if (rows == 0) {
            for (int truecols = 0; truecols < minesboard.num_cols; truecols++) {
                minesboard.board[0][truecols] = '#';
            }
        }
        else if (rows == minesboard.num_rows - 1) { 
            for (int truecols = 0; truecols < minesboard.num_cols; truecols++) {
                minesboard.board[rows][truecols] = '#';
            }
        }
        else {
            for (int columns = 0; columns < minesboard.num_cols; columns++) {
                if (columns > 0 && columns < minesboard.num_cols - 1) {
                        minesboard.board[rows][columns] = '-';
                    }
                }
       }
    }
    for (int rows = 0; rows < minesboard.num_rows; rows++) {
        minesboard.board[rows][0] = '#';
    }
    for (int rows = 0; rows < minesboard.num_rows; rows++) {
        minesboard.board[rows][minesboard.num_rows - 1] = '#';
    }

    return minesboard; 
}

bool RevealTilesBlank(minefield* minesboard, minefield* topboard, int playerrow, int playercol, int num_rows, int num_cols)
{
    if (minesboard->board[playerrow][playercol] == '-') {
       minesboard->board[playerrow][playercol] = '*';
       if (playercol - 1 < 0 || playercol > num_cols - 1) {
         ;
       }
       else {
         if (minesboard->board[playerrow][playercol - 1] == '-') {
            RevealTilesBlank(minesboard, topboard, playerrow, playercol - 1, num_rows, num_cols);
        }
       }
       if (playercol + 1 > num_cols - 1) {
         ;
       }
       else {
         if (minesboard->board[playerrow][playercol + 1] == '-') {
            RevealTilesBlank(minesboard, topboard, playerrow, playercol + 1, num_rows, num_cols);
        }
       }
       if (playerrow + 1 > num_rows - 1) {
         ;
       }
       else {
        if (minesboard->board[playerrow + 1][playercol] == '-') {
            RevealTilesBlank(minesboard, topboard, playerrow + 1, playercol, num_rows, num_cols);
        }
       }
       if (playerrow - 1 < 0) {
         ;
       }
       else {
        if (minesboard->board[playerrow - 1][playercol] == '-') {
            RevealTilesBlank(minesboard, topboard, playerrow - 1, playercol, num_rows, num_cols);        
        }
       }
       if (playerrow + 1 > num_rows - 1 || playercol + 1 > num_cols - 1) {
         ;
       }
       else {
        if (minesboard->board[playerrow + 1][playercol + 1] == '-') {;
            RevealTilesBlank(minesboard, topboard, playerrow + 1, playercol + 1, num_rows, num_cols);
                  
        }
       }
       if (playerrow - 1 < 0 || playercol - 1 < 0) {
         ;
       }
       else {
         if (minesboard->board[playerrow - 1][playercol - 1] == '-') {
            RevealTilesBlank(minesboard, topboard, playerrow - 1, playercol - 1, num_rows, num_cols);
        }
       }
        if (playerrow + 1 > num_rows - 1 || playercol - 1 < 0) {
            ;
        }
        else {
            if (minesboard->board[playerrow + 1][playercol - 1] == '-') {
                RevealTilesBlank(minesboard, topboard, playerrow + 1, playercol - 1, num_rows, num_cols);
            }
        }
        if (playerrow - 1 < 0 || playercol + 1 > minesboard->num_cols + 1) {
            ;
        }
        else {
            if (minesboard->board[playerrow - 1][playercol + 1] == '-') {
                RevealTilesBlank(minesboard, topboard, playerrow - 1, playercol + 1, num_rows, num_cols);
            }
        }
    
    }
    else if (minesboard->board[playerrow][playercol] != '*' && minesboard->board[playerrow][playercol] != 'M') {
        return false;
    }
    return false;
}
   
void red() {
    printf("\033[1;31m");
}

void blue() {
    printf("\033[0;34m");
}

void green() {
    printf("\033[0;32m");
}

void purple() {
    printf("\033[0;35m");
}


void InitializeMines(minefield* minesboard, minepoint *minelocations)
{
    srand(time(NULL));
    char numminestouching = '0';
    for (int nummines = 0; nummines < minesboard->num_mines; nummines++) {
        minelocations[nummines].x = rand() % (minesboard->num_rows - 2);
        minelocations[nummines].y = rand() % (minesboard->num_cols - 2);
        while (minelocations[nummines].x == 0 || minelocations[nummines].y == 0) {
            minelocations[nummines].x = rand() % (minesboard->num_rows - 2);
            minelocations[nummines].y = rand() % (minesboard->num_cols - 2);
        }
        minesboard->board[minelocations[nummines].x][minelocations[nummines].y] = 'M';
    }
    for (int rows = 0; rows < minesboard->num_rows; rows++) {
        for (int columns = 0; columns < minesboard->num_cols; columns++) {
            if ((rows > 0 && rows < minesboard->num_rows - 1) && (columns > 0 && columns < minesboard->num_cols - 1)) {
              if (minesboard->board[rows][columns] != '#' && minesboard->board[rows][columns] != 'M') {
                if (minesboard->board[rows][columns - 1] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows][columns + 1] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows + 1][columns] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows - 1][columns] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows + 1][columns + 1] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows - 1][columns - 1] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows + 1][columns - 1] == 'M') {
                    numminestouching++;
                }
                if (minesboard->board[rows - 1][columns + 1] == 'M') {
                    numminestouching++;
                }
                if (numminestouching > '0') {
                minesboard->board[rows][columns] = numminestouching;
                }
                numminestouching = '0';  
               }  
            }    
        }
    }
}

void printfield(minefield* topboard) 
{
    putchar('\n');
    printf("  ");
    for (int columns = 0; columns < topboard->num_cols; columns++) {
        if (topboard->num_cols >= 10) {
            red();
            printf("%-3d", columns);
        } else {
            red();
            printf("%-2d", columns);
        }
    }
    putchar('\n');
    for (int rows = 0, i = 0; rows < topboard->num_rows; rows++, i++) {
        if(topboard->num_rows >= 10) {
            red();
            printf("%-3d", i);
        }
        else {
            red();
            printf("%-2d", i);
        }
        for (int columns = 0; columns < topboard->num_cols; columns++) {
          if(topboard->num_cols >= 10) {
                if (topboard->board[rows][columns] == '1') {
                    blue();
                }
                else if (topboard->board[rows][columns] == '2') {
                    green();
                }
                else if (topboard->board[rows][columns] == '3') {
                    red();
                }
                else if (topboard->board[rows][columns] == '4') {
                    purple();
                }
                else {
                    red();
                }
            printf("%-3c", topboard->board[rows][columns]);
            } else {
             if (topboard->board[rows][columns] == '1') {
                    blue();
                }
                else if (topboard->board[rows][columns] == '2') {
                    green();
                }
                else if (topboard->board[rows][columns] == '3') {
                    red();
                }
                else if (topboard->board[rows][columns] == '4') {
                    purple();
                }
                else {
                    red();
                }
            printf("%-2c", topboard->board[rows][columns]);
            }
        }
        putchar('\n');
    }
}


void destroyboard(minefield* minesboard) {
  for(int row = 0; row < minesboard->num_rows; ++row){
    free((minesboard->board)[row]);
  }
  free(minesboard->board);
  minesboard->num_rows = 0;
  minesboard->num_cols = 0;
  minesboard->board = NULL;
}


bool isValidFormat(const int numArgsRead, const int numArgsNeed) 
{
  bool formatIsGood = numArgsRead == numArgsNeed;
  char character;
  do{
    scanf("%c", &character);
    if(!my_isspace(character)){
      formatIsGood = false;
    }
  }while(character != '\n');
  return formatIsGood;
}


void getValidRow(const char* prompt, int* playerrow)
{
  int num;
  const int numArgsNeeded = 1;
  int numArgsRead;
    do{
    printf("%s", prompt);
    numArgsRead = scanf(" %d", &num);
    }while(!isValidFormat(numArgsRead, numArgsNeeded));
    *playerrow = num;
}

void getValidCol(const char* prompt, int* playercol)
{
  int num;
  const int numArgsNeeded = 1;
  int numArgsRead;
    do{
    printf("%s", prompt);
    numArgsRead = scanf(" %d", &num);
    }while(!isValidFormat(numArgsRead, numArgsNeeded));
    *playercol = num;
}

int getValidInt(const char* prompt){
  int num;
  const int numArgsNeeded = 1;
  int numArgsRead;
  do{
    printf("%s", prompt);
    numArgsRead = scanf(" %d", &num); //45\n
  }while(!isValidFormat(numArgsRead, numArgsNeeded));

  return num;
}

char getValidChar(const char* prompt) {
    char c;
    const int numArgsNeeded = 1;
    int numArgsRead; 
    do{
    printf("%s", prompt);
    numArgsRead = scanf(" %c", &c); //45\n
  }while(!isValidFormat(numArgsRead, numArgsNeeded));
  return c;
}

bool my_isspace(char c)
{
	return c == ' ' || c == '\n' || c == '\t';
}
