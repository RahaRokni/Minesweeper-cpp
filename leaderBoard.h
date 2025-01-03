#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "game.h"

using namespace std;

//Function to remove extra spaces from the beginning and end of the string
string trim(const string& str) 
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    if (first == string::npos || last == string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

//function to read leaderboard from file
void leaderBoard() {
    Data PlayerRank[1000];
    string line;
    int index = 0, count = 0;

    //open the file for reading
    ifstream rankFile("leaderBoard.txt");

    //check if the file is open
    if (!rankFile.is_open()) 
    {
        cout << "Error opening the file." << endl;
        return;
    }

    //read line by line from the file
    while (getline(rankFile, line)) 
    {
        string name, registrationTime;
        int time, openCells;
        count++;
        char sepertion;
 
        //separate the name and time from each line
        stringstream Q(line);
        
        if (getline(Q, name, '|') && Q >> time >> sepertion >> openCells) 
        {
            //remove extra spaces from the name
            name = trim(name);
            //ensure data is entered correctly
            if (!name.empty() && time > 0) 
            {
                PlayerRank[index].name = name;
                PlayerRank[index].time = time;
                PlayerRank[index].openCells= openCells;
                index++;
            }
        }
    }

    rankFile.close();

    // Sort by opened cells
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = i + 1; j < count; j++) 
        {
            if (PlayerRank[i].openCells < PlayerRank[j].openCells) 
            {
                Data temp = PlayerRank[i];
                PlayerRank[i] = PlayerRank[j];
                PlayerRank[j] = temp;
            }
            else if (PlayerRank[i].openCells == PlayerRank[j].openCells && PlayerRank[i].time > PlayerRank[j].time)
            {
                Data temp = PlayerRank[i];
                PlayerRank[i] = PlayerRank[j];
                PlayerRank[j] = temp;
            }
        }
    }

    //show leaderboard
    setColor(37);
    cout << "Leaderboard:" << endl;
    setColor(35);
    cout << "🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 " << endl;

    for (int i = 0; i < count; i++) 
    {
        setColor(32);
        cout<< i+1 << ". " << PlayerRank[i].name << " - " 
            << PlayerRank[i].openCells << " opened" << " - " 
                << PlayerRank[i].time << " seconds" << endl;
    }

    setColor(35);
    cout << "🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 🙜 " << endl;

    //save sorted data to file
    ofstream outFile("leaderBoard.txt", ios::trunc);
    if (!outFile.is_open()) 
    {
        cout << "Error opening file to write." << endl;
        return;
    }

    //valid data to the file
    for (int i = 0; i < count; i++) 
    {
        outFile<< " | " << PlayerRank[i].name << "| " << PlayerRank[i].time 
            << " | " << PlayerRank[i].openCells << endl;
    }

    outFile.close();
}

#endif
