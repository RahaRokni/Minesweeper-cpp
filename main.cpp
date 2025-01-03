#include <iostream>
#include <string>
#include <limits> 
#include <fstream>
#include <math.h>
#include "menu.h"
#include "game.h"
#include "leaderBoard.h"

using namespace std;

bool exitFlag = false;
int level;

void menu();
void topFrame ();
void bottomFrame();
void PrintFrame(string Mass, int Num);
void gameLevel();

int main ()
{
    
    int choice;
    while (!exitFlag) 
    {
    menu(); 
    cin >> choice;
    switch(choice) 
    {
        case 1:
        {
            #ifdef _WIN32
                system("cls"); //for windows
            #else
                system("clear"); //for mac and linux
            #endif

            cout << "please enter your name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, player.name);

            cout << "\n" << "Choose the game level 🎚 " << "\n"; //you can choose your game level
            setColor(32);
            cout << "1.Easy" << "\n";
            setColor(33);
            cout << "2.Medium" << "\n";
            setColor(31);
            cout << "3.Hard" << "\n" << "🎚 ";

            cin >> level;
            gameLevel();

    
            break;
        }
        case 2:
            #ifdef _WIN32
                system("cls"); //for windows
            #else
                system("clear"); //for mac and linux
            #endif

            leaderBoard();
            break;
        case 3:
            cout << "Exiting the game :(" << endl;
            exitFlag = true;
            break;
        default:
            cout << "Invalid choice. Please try again. " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignoring enter to avoid getting stuck in an infinite loop
            break;
    }

}
}

//top frame
void topFrame ()
{
    cout << "💣💣💣💣💣💣💣💣💣💣💣" << "\n";
}

//bottom frame
void bottomFrame ()
{
    cout << "💣💣💣💣💣💣💣💣💣💣💣" << "\n";
}

//to have a neat frame
void PrintFrame (string Mass, int Num)
{
    cout << "💣" << Num << "-" << Mass;
    for (int i = 0; i < 24 - Mass.size() - 6; i++)
    {
        cout << " ";
    }
    cout << "💣" << "\n";

}

//print menu
void menu()
{
    setColor(37); 
    cout << "\n";
    cout << "   💣 Minesweeper 💣" << "\n";
    cout << "\n";
    topFrame ();
    PrintFrame("Game 👾", 1);
    PrintFrame("LeaderBoard 📃", 2);
    PrintFrame("Exit ⍈", 3);
    bottomFrame();

    cout << "Enter your choise: " ;

}

//یepending on the user's choice, the number of mines in the game varies from 16 to 30.
void gameLevel()
{
    switch (level)
    {
    case 1:
        countMines = pow(14,2) * 0.1;
        Play();
        break;
    case 2:
        countMines = pow(14,2) * 0.15;
        Play();
        break;
    case 3:
        countMines = pow(14,2) * 0.2;
        Play();
        break;
    }
}