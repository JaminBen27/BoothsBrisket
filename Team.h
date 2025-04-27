/*
* Team.h
 *
 *  Created on: Apr 25, 2018
 *      Author: booth
 */

#ifndef TEAM_H_
#define TEAM_H_


#include <string>

using namespace std;

const int NUM_TEAMS = 8;

struct Team_t{
    string name;
    string functionName;
    int score = 0;
    bool red = false;
    bool blue = false;

};


bool nextMatch(int& red, int& blue);



bool nextMatch(int& red, int& blue){
    bool flag = true;
    if(red == -1 && blue == -1){
        red = blue = 0;
    }
    else if(blue < NUM_TEAMS){
        red++;
        if(red >= NUM_TEAMS){
            blue++;
            red = 0;
        }
    }
    if(blue >= NUM_TEAMS){
        flag = false;
    }
    return flag;
}


#endif /* TEAM_H_ */