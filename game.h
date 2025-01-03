#ifndef GAME_H 
#define GAME_H

#include <iostream>
#include <unistd.h>
#include <termios.h> //to manage terminal input and output
#include <cstdlib> //random number generation
#include <ctime> //for using time-related functions
#include <iomanip> //to control the input/output format
#include <stdlib.h> //random number generation
#include <unistd.h> //provides for working with processes, input/output, time, and the system environment.
#include <string>
#include <fstream>
#include "game.h"
#include "menu.h"


using namespace std;

int currentPosX = 0, currentPosY = 0;//currentPost position
int countMines = 10; //depending on the user's choice, the number of mines in the game varies from 19 to 30.
int** matrix;
bool visited[14][14] = {false};
int visitedFlag[14][14] = {10};
bool gameOver = false; //for Game status
bool showMines = false; //all mines are shown after losing Except for those marked and the one the user selected.
bool firstMove = true; //to place mines after the first move
int SafeCells = 0; //for game status
time_t startTime; 
bool timerStarted = false;
std::fstream rankFile ("leaderBoard.txt", ios::out | ios::app ); //file for recording player information
struct Data //struct for recording player information
{
    string name;
    int time;
    int openCells;
};
Data player;

void setColor(int color);
void clearScreen();
char getInput();
void drawGrid(int currentPosX, int currentPosY, int selected[14][14]);
void fillRandomWithMinusOne(int** matrix, int rows, int cols, int count);
void AssigningValuesToCells();
void CheckSell(int currentPosX, int currentPosY);
void resetGame ();

//main fun
int Play ()
{
    
    resetGame();


    int selected[14][14] = {0}; // Status of the squares (selected or not)

    drawGrid(currentPosX, currentPosY, selected);

    while (true) {
        if (gameOver) 
        {
            break; // If the user loses, we end the loop
        }

        if (SafeCells == 169 - countMines) 
        {
            break; // If the user wins, we end the loop
        }

        if (!timerStarted) 
        {
            startTime = time(0);  // Save the game start time
            timerStarted = true;
        }

        char input = getInput();

        if (input == 27) 
            break;

        switch (input) 
        {
            case 'w': // Move up
                if (currentPosY > 0) 
                    currentPosY--;
                break;
            case 's': // Move down
                if (currentPosY < 13) 
                    currentPosY++;
                break;
            case 'a': // Move left
                if (currentPosX > 0) 
                    currentPosX--;
                break;
            case 'd': // Move right
                if (currentPosX < 13) 
                    currentPosX++;
                break;
            case 'f': // Marking as Mine
                selected[currentPosY][currentPosX] = -1;
                break;
            case '\n': // Select cell
                if(firstMove) //if it is the first time the player clicks, then the mines will be placed randomly and the matrix will be completed accordingly.
                {
                    fillRandomWithMinusOne(matrix, 14, 14, countMines);
                    AssigningValuesToCells();
                    firstMove = false;
                }
                CheckSell(currentPosX, currentPosY);
                break;
        }

        drawGrid(currentPosX, currentPosY, selected);
    }

    for (int i = 0; i < 14; ++i) {
        delete[] matrix[i];
    }

    delete[] matrix;

    return 0;
}

//function for color
void setColor(int color) 
{
    cout << "\033[" << color << "m"; // ANSI color code
}

//function to clear the screen
void clearScreen() {
    cout << "\033[H\033[J";
}

//function to read input from terminal without pressing Enter
char getInput() 
{
    char buf = 0;
    struct termios old = {0};
    tcgetattr(STDIN_FILENO, &old);

    struct termios NewSetting = old;

    NewSetting.c_lflag &= ~ICANON;
    NewSetting.c_lflag &= ~ECHO; //don't show input

    tcsetattr(STDIN_FILENO, TCSANOW, &NewSetting);

    read(STDIN_FILENO, &buf, 1);

    tcsetattr(STDIN_FILENO, TCSADRAIN, &old);

    return buf;
}

//function to draw the grid
void drawGrid(int currentPosX, int currentPosY, int selected[14][14]) 
{
    clearScreen();

    for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 14; ++j) {
            if (i == currentPosY && j == currentPosX) //shows player positions.
            {
                setColor(37);
                cout << "⦑✗⦒";
            } 
            else if (selected[i][j] == -1)  //to place the flag
            {
                setColor(33);
                cout << "⦑F⦒";
            } 
            else if (visitedFlag[i][j] == 9) //to show safe cells
            {
                setColor(32);
                cout << "⦑" << matrix[i][j] << "⦒";
            } 
            else if (showMines && matrix[i][j] == -1) ////to show mines
            {
                setColor(31);
                cout << "⦑M⦒";
                gameOver = true;
            }
            else 
            {
                setColor(34); //for unknown cells
                cout << "⦑ ⦒";
            }
            setColor(37);
            cout << " ";
        }
        cout << endl;
    }

    if (timerStarted) 
    {
        player.time = 0;
        time_t currentPostTime = time(0);  //currentPost time
        int elapsed = difftime(currentPostTime, startTime);  //time elapsed since the start
        player.time += elapsed;
        int minutes = elapsed / 60;
        int seconds = elapsed % 60;

        setColor(37);  //color for timer
        cout << "⏳ : " << setw(2) << setfill('0') << minutes << ":"
            << setw(2) << setfill('0') << seconds <<"          🙜  "<< player.name << " 🙞"<<endl;
    }

    if (gameOver) //if the player loses, their time, the number of cells they opened, and their name are saved in the file.
    {

        ofstream rankFile ("leaderBoard.txt", ios::out | ios::app );
        if (rankFile.is_open())
        {
            rankFile << player.name << " | " << player.time << " | " << player.openCells << endl;
        }
        setColor(31);
        cout << "you lose!" << endl;
    }

    if (SafeCells == 169 - countMines) //if the player wins, their time, the number of cells they opened, and their name are saved in the file.
    {
        ofstream rankFile ("leaderBoard.txt", ios::out | ios::app );
        if (rankFile.is_open())
        {
            rankFile << player.name << " | " << player.time << " | " << player.openCells << endl;
        }
        setColor(32);
        cout << "you Won!" << endl; 
    }
}

//function to randomly place -1 in the grid
void fillRandomWithMinusOne(int** matrix, int rows, int cols, int count) 
{
    srand(static_cast<unsigned>(time(0))); //initialize random seed

    while (count > 0) {
        int randomRow = rand() % rows;
        int randomCol = rand() % cols;
        if (matrix[randomRow][randomCol] != -1) 
        {
            matrix[randomRow][randomCol] = -1;
            count--;
        }
    }
}

//function to assign values to cells (number of adjacent mines)
void AssigningValuesToCells() 
{
    int count = 0, NewX = 0, NewY = 0;

    for (int i = 0; i < 14; i++) 
    {
        for (int j = 0; j < 14; j++) 
        {
            if (matrix[i][j] == -1) 
            {
                continue; 
            }

            count = 0; //reset counter for each cell

            for (int I = -1; I <= 1; I++) 
            {
                for (int J = -1; J <= 1; J++) 
                {
                    if (I == 0 && J == 0) 
                    {
                        continue;
                    }

                    NewX = i + I;
                    NewY = j + J;

                    //is it within range?
                    if (NewX >= 0 && NewX < 14 && NewY >= 0 && NewY < 14) 
                    {
                        if (matrix[NewX][NewY] == -1) 
                        {
                            count++; 
                        }
                    }
                }
            }

            matrix[i][j] = count; 
        }
    }
}

void CheckSell(int currentPosX, int currentPosY)
{
    int NewX, NewY;

//check if the cell is outside the matrix bounds or has already been visited
    if (currentPosX < 0 || currentPosX >= 14 || currentPosY < 0 || currentPosY >= 14 
        || visited[currentPosY][currentPosX]) 
    {
        return;
    }

    visited[currentPosY][currentPosX] = true;

//if the cell value is 0, we need to open the cells around it
    if (matrix[currentPosY][currentPosX] == 0) 
    {
        player.openCells++;
        visitedFlag[currentPosY][currentPosX] = 9;
        SafeCells++;
//we return for each of the 14 cells adjacent to the currentPost cell
        for (int I = -1; I <= 1; I++) 
        {
            for (int J = -1; J <= 1; J++) 
            {
                if (I == 0 && J == 0) 
                {
                    continue;
                }

                NewX = currentPosX + I;
                NewY = currentPosY + J;

                //callback to open surrounding cells
                CheckSell(NewX, NewY);
            }
        }
    } 
    else if (matrix[currentPosY][currentPosX] >= 1) 
    {
        //if the cell value is greater than 1, we just mark it.
        player.openCells++;
        visitedFlag[currentPosY][currentPosX] = 9;
        SafeCells++;
    }
    else if (matrix[currentPosY][currentPosX] == -1)
    {
        visitedFlag[currentPosY][currentPosX] = 11;
        showMines = true; //game is OVER
    }
}

void resetGame() {
    //reset all game variables
    currentPosX = 0;
    currentPosY = 0;
    gameOver = false;
    showMines = false;
    firstMove = true;
    SafeCells = 0;
    timerStarted = false;
    player.openCells = 0; 
    for (int i = 0; i < 14; i++) 
    {
        for (int j = 0; j < 14; j++) 
        {
            visited[i][j] = false;
            visitedFlag[i][j] = 10;
        }
    }

    matrix = new int*[14];
    for (int i = 0; i < 14; ++i) 
    {
        matrix[i] = new int[14]();  
    }
}
#endif
