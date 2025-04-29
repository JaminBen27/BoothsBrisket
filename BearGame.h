/*
 * BearGame.h
 *
 *  Created on: Dec 13, 2017
 *      Author: booth
 */

#ifndef BEARGAME_H_
#define BEARGAME_H_

#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include "Token.h"
#include "Team.h"
#include "constants.h"


bool validLocationGrid[GRID_ROW][GRID_COL] = {
                                  {0,0,0,0,1,0,0,0,0},
                                  {0,0,0,1,0,1,0,0,0},
                                  {0,0,1,0,1,0,1,0,0},
                                  {0,0,0,1,0,1,0,0,0},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1},
                                  {1,1,1,1,1,1,1,1,1}
                                 };



struct Node{
	Point location;
	Node* path[7];


};

int rowToY(int n){
	return Y_OFFSET + n * LINE_SPACING;
}

int columnToX(int n){
	return X_OFFSET + n * LINE_SPACING;
}



class BearGame{
    private:
	    Token token[19];
	    Team_t team[NUM_TEAMS];
	    Token shadow;
	    int attachedToken;
	    set<pair<Point, Point> > edgeList;
	    bool tigersTurn;
	    bool over;
	    bool pause;
	    bool moving;

    public:

	    BearGame(SDL_Plotter& g){
	    	    attachedToken = -1; //no attached token
	    	    srand(time(0));
	    	    tigersTurn = true;
	    	    over = false;
	    	    pause = false;
	    	    moving = false;
	    	    shadow.setActive(false);
	    		token[0].setColor(red);
	    		token[0].setAttached(false);
	    		token[0].setLocation(Point(2, 4));
	    		//token[0].setLocation(Point(columnToX(4), rowToY(2)));


	    		team[0].name = "Team Name";


	    		team[0].functionName = "Move_TeamName";




            for(int i = 1; i <= 18; i++){
	    		   token[i].setColor(blue);
	    		   token[i].setAttached(false);
	    		   token[i].setActive(true);
	    		   int col = (i-1)%9;
	    		   int row = 11+(i-1)/9;
	    		   token[i].setLocation(Point(row, col));
            }
	    		//draw Title
	    	    displayFile(Point(90,10),"Title.bmp.txt",g);

	    		//row 0
	    		edgeList.insert(make_pair(Point(0,4), Point(1,3)));
	    		edgeList.insert(make_pair(Point(0,4), Point(1,5)));

	    		//row 1
	    		edgeList.insert(make_pair(Point(1,3), Point(0,4)));
	    		edgeList.insert(make_pair(Point(1,3), Point(2,4)));
	    		edgeList.insert(make_pair(Point(1,3), Point(2,2)));

	    		edgeList.insert(make_pair(Point(1,5), Point(0,4)));
	    		edgeList.insert(make_pair(Point(1,5), Point(2,4)));
	    		edgeList.insert(make_pair(Point(1,5), Point(2,6)));

	    		//row 2
	    		edgeList.insert(make_pair(Point(2,2), Point(1,3)));
	    		edgeList.insert(make_pair(Point(2,2), Point(3,3)));

	    		edgeList.insert(make_pair(Point(2,4), Point(1,3)));
	    		edgeList.insert(make_pair(Point(2,4), Point(1,5)));
	    		edgeList.insert(make_pair(Point(2,4), Point(3,3)));
	    		edgeList.insert(make_pair(Point(2,4), Point(3,5)));

	    		edgeList.insert(make_pair(Point(2,6), Point(1,5)));
	    		edgeList.insert(make_pair(Point(2,6), Point(3,5)));

	    		//row 3
	    		edgeList.insert(make_pair(Point(3,3), Point(2,2)));
	    		edgeList.insert(make_pair(Point(3,3), Point(2,4)));
	    		edgeList.insert(make_pair(Point(3,3), Point(4,4)));

	    		edgeList.insert(make_pair(Point(3,5), Point(2,6)));
	    		edgeList.insert(make_pair(Point(3,5), Point(2,4)));
	    		edgeList.insert(make_pair(Point(3,5), Point(4,4)));

	    		//row 4
	    		edgeList.insert(make_pair(Point(4,0), Point(4,1)));
	    		edgeList.insert(make_pair(Point(4,0), Point(5,0)));

	    		edgeList.insert(make_pair(Point(4,1), Point(4,0)));
	    		edgeList.insert(make_pair(Point(4,1), Point(5,1)));
	    		edgeList.insert(make_pair(Point(4,1), Point(4,2)));

	    		edgeList.insert(make_pair(Point(4,2), Point(4,1)));
	    		edgeList.insert(make_pair(Point(4,2), Point(5,2)));
	    		edgeList.insert(make_pair(Point(4,2), Point(4,3)));

	    		edgeList.insert(make_pair(Point(4,3), Point(4,2)));
	    		edgeList.insert(make_pair(Point(4,3), Point(5,3)));
	    		edgeList.insert(make_pair(Point(4,3), Point(4,4)));

	    		edgeList.insert(make_pair(Point(4,4), Point(4,3)));
	    		edgeList.insert(make_pair(Point(4,4), Point(5,4)));
	    		edgeList.insert(make_pair(Point(4,4), Point(4,5)));
	    		//diagonals
	    		edgeList.insert(make_pair(Point(4,4), Point(3,3)));
	    		edgeList.insert(make_pair(Point(4,4), Point(3,5)));
	    		edgeList.insert(make_pair(Point(4,4), Point(5,3)));
	    		edgeList.insert(make_pair(Point(4,4), Point(5,5)));

	    		edgeList.insert(make_pair(Point(4,5), Point(4,4)));
	    		edgeList.insert(make_pair(Point(4,5), Point(5,5)));
	    		edgeList.insert(make_pair(Point(4,5), Point(4,6)));

	    		edgeList.insert(make_pair(Point(4,6), Point(4,5)));
	    		edgeList.insert(make_pair(Point(4,6), Point(5,6)));
	    		edgeList.insert(make_pair(Point(4,6), Point(4,7)));

	    		edgeList.insert(make_pair(Point(4,7), Point(4,6)));
	    		edgeList.insert(make_pair(Point(4,7), Point(5,7)));
	    		edgeList.insert(make_pair(Point(4,7), Point(4,8)));

	    		edgeList.insert(make_pair(Point(4,8), Point(4,7)));
	    		edgeList.insert(make_pair(Point(4,8), Point(5,8)));

	    		//row 5
	    		edgeList.insert(make_pair(Point(5,0), Point(4,0)));
	    		edgeList.insert(make_pair(Point(5,0), Point(5,1)));
	    		edgeList.insert(make_pair(Point(5,0), Point(6,0)));

	    		edgeList.insert(make_pair(Point(5,1), Point(4,1)));
	    		edgeList.insert(make_pair(Point(5,1), Point(5,0)));
	    		edgeList.insert(make_pair(Point(5,1), Point(5,2)));
	    		edgeList.insert(make_pair(Point(5,1), Point(6,1)));

	    		edgeList.insert(make_pair(Point(5,2), Point(4,2)));
	    		edgeList.insert(make_pair(Point(5,2), Point(5,1)));
	    		edgeList.insert(make_pair(Point(5,2), Point(5,3)));
	    		edgeList.insert(make_pair(Point(5,2), Point(6,2)));

	    		edgeList.insert(make_pair(Point(5,3), Point(4,3)));
	    		edgeList.insert(make_pair(Point(5,3), Point(5,2)));
	    		edgeList.insert(make_pair(Point(5,3), Point(5,4)));
	    		edgeList.insert(make_pair(Point(5,3), Point(6,3)));

	    		edgeList.insert(make_pair(Point(5,4), Point(4,4)));
	    		edgeList.insert(make_pair(Point(5,4), Point(5,3)));
	    		edgeList.insert(make_pair(Point(5,4), Point(5,5)));
	    		edgeList.insert(make_pair(Point(5,4), Point(6,4)));

	    		edgeList.insert(make_pair(Point(5,5), Point(4,5)));
	    		edgeList.insert(make_pair(Point(5,5), Point(5,4)));
	    		edgeList.insert(make_pair(Point(5,5), Point(5,6)));
	    		edgeList.insert(make_pair(Point(5,5), Point(6,5)));

	    		edgeList.insert(make_pair(Point(5,6), Point(4,6)));
	    		edgeList.insert(make_pair(Point(5,6), Point(5,5)));
	    		edgeList.insert(make_pair(Point(5,6), Point(5,7)));
	    		edgeList.insert(make_pair(Point(5,6), Point(6,6)));

	    		edgeList.insert(make_pair(Point(5,7), Point(4,7)));
	    		edgeList.insert(make_pair(Point(5,7), Point(5,6)));
	    		edgeList.insert(make_pair(Point(5,7), Point(5,8)));
	    		edgeList.insert(make_pair(Point(5,7), Point(6,7)));

	    		edgeList.insert(make_pair(Point(5,8), Point(4,8)));
	    		edgeList.insert(make_pair(Point(5,8), Point(5,7)));
	    		edgeList.insert(make_pair(Point(5,8), Point(6,8)));

	    		//row 5 diagonals
	    		edgeList.insert(make_pair(Point(5,3), Point(4,4)));
	    		edgeList.insert(make_pair(Point(5,3), Point(6,2)));
	    		edgeList.insert(make_pair(Point(5,5), Point(4,4)));
	    		edgeList.insert(make_pair(Point(5,5), Point(6,6)));


	    		//row 6
	    		edgeList.insert(make_pair(Point(6,0), Point(5,0)));
	    		edgeList.insert(make_pair(Point(6,0), Point(6,1)));
	    		edgeList.insert(make_pair(Point(6,0), Point(7,0)));

	    		edgeList.insert(make_pair(Point(6,1), Point(5,1)));
	    		edgeList.insert(make_pair(Point(6,1), Point(6,0)));
	    		edgeList.insert(make_pair(Point(6,1), Point(6,2)));
	    		edgeList.insert(make_pair(Point(6,1), Point(7,1)));

	    		edgeList.insert(make_pair(Point(6,2), Point(5,2)));
	    		edgeList.insert(make_pair(Point(6,2), Point(6,1)));
	    		edgeList.insert(make_pair(Point(6,2), Point(6,3)));
	    		edgeList.insert(make_pair(Point(6,2), Point(7,2)));

	    		edgeList.insert(make_pair(Point(6,3), Point(5,3)));
	    		edgeList.insert(make_pair(Point(6,3), Point(6,2)));
	    		edgeList.insert(make_pair(Point(6,3), Point(6,4)));
	    		edgeList.insert(make_pair(Point(6,3), Point(7,3)));

	    		edgeList.insert(make_pair(Point(6,4), Point(5,4)));
	    		edgeList.insert(make_pair(Point(6,4), Point(6,3)));
	    		edgeList.insert(make_pair(Point(6,4), Point(6,5)));
	    		edgeList.insert(make_pair(Point(6,4), Point(7,4)));

	    		edgeList.insert(make_pair(Point(6,5), Point(5,5)));
	    		edgeList.insert(make_pair(Point(6,5), Point(6,4)));
	    		edgeList.insert(make_pair(Point(6,5), Point(6,6)));
	    		edgeList.insert(make_pair(Point(6,5), Point(7,5)));

	    		edgeList.insert(make_pair(Point(6,6), Point(5,6)));
	    		edgeList.insert(make_pair(Point(6,6), Point(6,5)));
	    		edgeList.insert(make_pair(Point(6,6), Point(6,7)));
	    		edgeList.insert(make_pair(Point(6,6), Point(7,6)));

	    		edgeList.insert(make_pair(Point(6,7), Point(5,7)));
	    		edgeList.insert(make_pair(Point(6,7), Point(6,6)));
	    		edgeList.insert(make_pair(Point(6,7), Point(6,8)));
	    		edgeList.insert(make_pair(Point(6,7), Point(7,7)));

	    		edgeList.insert(make_pair(Point(6,8), Point(5,8)));
	    		edgeList.insert(make_pair(Point(6,8), Point(6,7)));
	    		edgeList.insert(make_pair(Point(6,8), Point(7,8)));

	    		//row 6 diagonals
	    		edgeList.insert(make_pair(Point(6,2), Point(5,3)));
	    		edgeList.insert(make_pair(Point(6,2), Point(7,1)));
	    		edgeList.insert(make_pair(Point(6,6), Point(5,5)));
	    		edgeList.insert(make_pair(Point(6,6), Point(7,7)));

	    		//row 7
	    		edgeList.insert(make_pair(Point(7,0), Point(6,0)));
	    		edgeList.insert(make_pair(Point(7,0), Point(7,1)));
	    		edgeList.insert(make_pair(Point(7,0), Point(8,0)));

	    		edgeList.insert(make_pair(Point(7,1), Point(6,1)));
	    		edgeList.insert(make_pair(Point(7,1), Point(7,0)));
	    		edgeList.insert(make_pair(Point(7,1), Point(7,2)));
	    		edgeList.insert(make_pair(Point(7,1), Point(8,1)));

	    		edgeList.insert(make_pair(Point(7,2), Point(6,2)));
	    		edgeList.insert(make_pair(Point(7,2), Point(7,1)));
	    		edgeList.insert(make_pair(Point(7,2), Point(7,3)));
	    		edgeList.insert(make_pair(Point(7,2), Point(8,2)));

	    		edgeList.insert(make_pair(Point(7,3), Point(6,3)));
	    		edgeList.insert(make_pair(Point(7,3), Point(7,2)));
	    		edgeList.insert(make_pair(Point(7,3), Point(7,4)));
	    		edgeList.insert(make_pair(Point(7,3), Point(8,3)));

	    		edgeList.insert(make_pair(Point(7,4), Point(6,4)));
	    		edgeList.insert(make_pair(Point(7,4), Point(7,5)));
	    		edgeList.insert(make_pair(Point(7,4), Point(7,3)));
	    		edgeList.insert(make_pair(Point(7,4), Point(8,4)));

	    		edgeList.insert(make_pair(Point(7,5), Point(6,5)));
	    		edgeList.insert(make_pair(Point(7,5), Point(7,4)));
	    		edgeList.insert(make_pair(Point(7,5), Point(7,6)));
	    		edgeList.insert(make_pair(Point(7,5), Point(8,5)));

	    		edgeList.insert(make_pair(Point(7,6), Point(6,6)));
	    		edgeList.insert(make_pair(Point(7,6), Point(7,5)));
	    		edgeList.insert(make_pair(Point(7,6), Point(7,7)));
	    		edgeList.insert(make_pair(Point(7,6), Point(8,6)));

	    		edgeList.insert(make_pair(Point(7,7), Point(6,7)));
	    		edgeList.insert(make_pair(Point(7,7), Point(7,6)));
	    		edgeList.insert(make_pair(Point(7,7), Point(7,8)));
	    		edgeList.insert(make_pair(Point(7,7), Point(8,7)));

	    		edgeList.insert(make_pair(Point(7,8), Point(6,8)));
	    		edgeList.insert(make_pair(Point(7,8), Point(7,7)));
	    		edgeList.insert(make_pair(Point(7,8), Point(8,8)));

	    		//row 7 diagonals
	    		edgeList.insert(make_pair(Point(7,1), Point(6,2)));
	    		edgeList.insert(make_pair(Point(7,1), Point(8,0)));
	    		edgeList.insert(make_pair(Point(7,7), Point(6,6)));
	    		edgeList.insert(make_pair(Point(7,7), Point(8,8)));


	    		//row 8
	    		edgeList.insert(make_pair(Point(8,0), Point(7,0)));
	    		edgeList.insert(make_pair(Point(8,0), Point(8,1)));
	    		edgeList.insert(make_pair(Point(8,0), Point(9,0)));

	    		edgeList.insert(make_pair(Point(8,1), Point(7,1)));
	    		edgeList.insert(make_pair(Point(8,1), Point(8,0)));
	    		edgeList.insert(make_pair(Point(8,1), Point(8,2)));
	    		edgeList.insert(make_pair(Point(8,1), Point(9,1)));

	    		edgeList.insert(make_pair(Point(8,2), Point(7,2)));
	    		edgeList.insert(make_pair(Point(8,2), Point(8,1)));
	    		edgeList.insert(make_pair(Point(8,2), Point(8,3)));
	    		edgeList.insert(make_pair(Point(8,2), Point(9,2)));

	    		edgeList.insert(make_pair(Point(8,3), Point(7,3)));
	    		edgeList.insert(make_pair(Point(8,3), Point(8,2)));
	    		edgeList.insert(make_pair(Point(8,3), Point(8,4)));
	    		edgeList.insert(make_pair(Point(8,3), Point(9,3)));

	    		edgeList.insert(make_pair(Point(8,4), Point(7,4)));
	    		edgeList.insert(make_pair(Point(8,4), Point(8,5)));
	    		edgeList.insert(make_pair(Point(8,4), Point(8,3)));
	    		edgeList.insert(make_pair(Point(8,4), Point(9,4)));

	    		edgeList.insert(make_pair(Point(8,5), Point(7,5)));
	    		edgeList.insert(make_pair(Point(8,5), Point(8,4)));
	    		edgeList.insert(make_pair(Point(8,5), Point(8,6)));
	    		edgeList.insert(make_pair(Point(8,5), Point(9,5)));

	    		edgeList.insert(make_pair(Point(8,6), Point(7,6)));
	    		edgeList.insert(make_pair(Point(8,6), Point(8,5)));
	    		edgeList.insert(make_pair(Point(8,6), Point(8,7)));
	    		edgeList.insert(make_pair(Point(8,6), Point(9,6)));

	    		edgeList.insert(make_pair(Point(8,7), Point(7,7)));
	    		edgeList.insert(make_pair(Point(8,7), Point(8,6)));
	    		edgeList.insert(make_pair(Point(8,7), Point(8,8)));
	    		edgeList.insert(make_pair(Point(8,7), Point(9,7)));

	    		edgeList.insert(make_pair(Point(8,8), Point(7,8)));
	    		edgeList.insert(make_pair(Point(8,8), Point(8,7)));
	    		edgeList.insert(make_pair(Point(8,8), Point(9,8)));

	    		//row 8 diagonals
	    		edgeList.insert(make_pair(Point(8,0), Point(7,1)));
	    		edgeList.insert(make_pair(Point(8,0), Point(9,1)));
	    		edgeList.insert(make_pair(Point(8,8), Point(7,7)));
	    		edgeList.insert(make_pair(Point(8,8), Point(9,7)));


	    		//row 9
	    		edgeList.insert(make_pair(Point(9,0), Point(8,0)));
	    		edgeList.insert(make_pair(Point(9,0), Point(9,1)));
	    		edgeList.insert(make_pair(Point(9,0), Point(10,0)));

	    		edgeList.insert(make_pair(Point(9,1), Point(8,1)));
	    		edgeList.insert(make_pair(Point(9,1), Point(9,0)));
	    		edgeList.insert(make_pair(Point(9,1), Point(9,2)));
	    		edgeList.insert(make_pair(Point(9,1), Point(10,1)));

	    		edgeList.insert(make_pair(Point(9,2), Point(8,2)));
	    		edgeList.insert(make_pair(Point(9,2), Point(9,1)));
	    		edgeList.insert(make_pair(Point(9,2), Point(9,3)));
	    		edgeList.insert(make_pair(Point(9,2), Point(10,2)));

	    		edgeList.insert(make_pair(Point(9,3), Point(8,3)));
	    		edgeList.insert(make_pair(Point(9,3), Point(9,2)));
	    		edgeList.insert(make_pair(Point(9,3), Point(9,4)));
	    		edgeList.insert(make_pair(Point(9,3), Point(10,3)));

	    		edgeList.insert(make_pair(Point(9,4), Point(8,4)));
	    		edgeList.insert(make_pair(Point(9,4), Point(9,3)));
	    		edgeList.insert(make_pair(Point(9,4), Point(9,5)));
	    		edgeList.insert(make_pair(Point(9,4), Point(10,4)));

	    		edgeList.insert(make_pair(Point(9,5), Point(8,5)));
	    		edgeList.insert(make_pair(Point(9,5), Point(9,4)));
	    		edgeList.insert(make_pair(Point(9,5), Point(9,6)));
	    		edgeList.insert(make_pair(Point(9,5), Point(10,5)));

	    		edgeList.insert(make_pair(Point(9,6), Point(8,6)));
	    		edgeList.insert(make_pair(Point(9,6), Point(9,5)));
	    		edgeList.insert(make_pair(Point(9,6), Point(9,7)));
	    		edgeList.insert(make_pair(Point(9,6), Point(10,6)));

	    		edgeList.insert(make_pair(Point(9,7), Point(8,7)));
	    		edgeList.insert(make_pair(Point(9,7), Point(9,6)));
	    		edgeList.insert(make_pair(Point(9,7), Point(9,8)));
	    		edgeList.insert(make_pair(Point(9,7), Point(10,7)));

	    		edgeList.insert(make_pair(Point(9,8), Point(8,8)));
	    		edgeList.insert(make_pair(Point(9,8), Point(9,7)));
	    		edgeList.insert(make_pair(Point(9,8), Point(10,8)));

	    		//row 9 diagonals
	    		edgeList.insert(make_pair(Point(9,1), Point(8, 0)));
	    		edgeList.insert(make_pair(Point(9,1), Point(10,2)));
	    		edgeList.insert(make_pair(Point(9,7), Point(8, 8)));
	    		edgeList.insert(make_pair(Point(9,7), Point(10,6)));


	    		//row 10
	    		edgeList.insert(make_pair(Point(10,0), Point(9,0)));
	    		edgeList.insert(make_pair(Point(10,0), Point(10,1)));
	    		edgeList.insert(make_pair(Point(10,0), Point(11,0)));

	    		edgeList.insert(make_pair(Point(10,1), Point(9,1)));
	    		edgeList.insert(make_pair(Point(10,1), Point(10,0)));
	    		edgeList.insert(make_pair(Point(10,1), Point(10,2)));
	    		edgeList.insert(make_pair(Point(10,1), Point(11,1)));

	    		edgeList.insert(make_pair(Point(10,2), Point(9,2)));
	    		edgeList.insert(make_pair(Point(10,2), Point(10,1)));
	    		edgeList.insert(make_pair(Point(10,2), Point(10,3)));
	    		edgeList.insert(make_pair(Point(10,2), Point(11,2)));

	    		edgeList.insert(make_pair(Point(10,3), Point(9,3)));
	    		edgeList.insert(make_pair(Point(10,3), Point(10,2)));
	    		edgeList.insert(make_pair(Point(10,3), Point(10,4)));
	    		edgeList.insert(make_pair(Point(10,3), Point(11,3)));

	    		edgeList.insert(make_pair(Point(10,4), Point(9,4)));
	    		edgeList.insert(make_pair(Point(10,4), Point(10,3)));
	    		edgeList.insert(make_pair(Point(10,4), Point(10,5)));
	    		edgeList.insert(make_pair(Point(10,4), Point(11,4)));

	    		edgeList.insert(make_pair(Point(10,5), Point(9,5)));
	    		edgeList.insert(make_pair(Point(10,5), Point(10,4)));
	    		edgeList.insert(make_pair(Point(10,5), Point(10,6)));
	    		edgeList.insert(make_pair(Point(10,5), Point(11,5)));

	    		edgeList.insert(make_pair(Point(10,6), Point(9,6)));
	    		edgeList.insert(make_pair(Point(10,6), Point(10,5)));
	    		edgeList.insert(make_pair(Point(10,6), Point(10,7)));
	    		edgeList.insert(make_pair(Point(10,6), Point(11,6)));

	    		edgeList.insert(make_pair(Point(10,7), Point(9,7)));
	    		edgeList.insert(make_pair(Point(10,7), Point(10,6)));
	    		edgeList.insert(make_pair(Point(10,7), Point(10,8)));
	    		edgeList.insert(make_pair(Point(10,7), Point(11,7)));

	    		edgeList.insert(make_pair(Point(10,8), Point(9,8)));
	    		edgeList.insert(make_pair(Point(10,8), Point(10,7)));
	    		edgeList.insert(make_pair(Point(10,8), Point(11,8)));

	    		//row 10 diagonals
	    		edgeList.insert(make_pair(Point(10,2), Point(9, 1)));
	    		edgeList.insert(make_pair(Point(10,2), Point(11,3)));
	    		edgeList.insert(make_pair(Point(10,6), Point(9, 7)));
	    		edgeList.insert(make_pair(Point(10,6), Point(11,5)));


	    		//row 11
	    		edgeList.insert(make_pair(Point(11,0), Point(10,0)));
	    		edgeList.insert(make_pair(Point(11,0), Point(11,1)));
	    		edgeList.insert(make_pair(Point(11,0), Point(12,0)));

	    		edgeList.insert(make_pair(Point(11,1), Point(10,1)));
	    		edgeList.insert(make_pair(Point(11,1), Point(11,0)));
	    		edgeList.insert(make_pair(Point(11,1), Point(11,2)));
	    		edgeList.insert(make_pair(Point(11,1), Point(12,1)));

	    		edgeList.insert(make_pair(Point(11,2), Point(10,2)));
	    		edgeList.insert(make_pair(Point(11,2), Point(11,1)));
	    		edgeList.insert(make_pair(Point(11,2), Point(11,3)));
	    		edgeList.insert(make_pair(Point(11,2), Point(12,2)));

	    		edgeList.insert(make_pair(Point(11,3), Point(10,3)));
	    		edgeList.insert(make_pair(Point(11,3), Point(11,2)));
	    		edgeList.insert(make_pair(Point(11,3), Point(11,4)));
	    		edgeList.insert(make_pair(Point(11,3), Point(12,3)));

	    		edgeList.insert(make_pair(Point(11,4), Point(10,4)));
	    		edgeList.insert(make_pair(Point(11,4), Point(11,3)));
	    		edgeList.insert(make_pair(Point(11,4), Point(11,5)));
	    		edgeList.insert(make_pair(Point(11,4), Point(12,4)));

	    		edgeList.insert(make_pair(Point(11,5), Point(10,5)));
	    		edgeList.insert(make_pair(Point(11,5), Point(11,4)));
	    		edgeList.insert(make_pair(Point(11,5), Point(11,6)));
	    		edgeList.insert(make_pair(Point(11,5), Point(12,5)));

	    		edgeList.insert(make_pair(Point(11,6), Point(10,6)));
	    		edgeList.insert(make_pair(Point(11,6), Point(11,5)));
	    		edgeList.insert(make_pair(Point(11,6), Point(11,7)));
	    		edgeList.insert(make_pair(Point(11,6), Point(12,6)));

	    		edgeList.insert(make_pair(Point(11,7), Point(10,7)));
	    		edgeList.insert(make_pair(Point(11,7), Point(11,6)));
	    		edgeList.insert(make_pair(Point(11,7), Point(11,8)));
	    		edgeList.insert(make_pair(Point(11,7), Point(12,7)));

	    		edgeList.insert(make_pair(Point(11,8), Point(10,8)));
	    		edgeList.insert(make_pair(Point(11,8), Point(11,7)));
	    		edgeList.insert(make_pair(Point(11,8), Point(12,8)));

	    		//row 11 diagonals
	    		edgeList.insert(make_pair(Point(11,3), Point(10,2)));
	    		edgeList.insert(make_pair(Point(11,3), Point(12,4)));
	    		edgeList.insert(make_pair(Point(11,5), Point(10,6)));
	    		edgeList.insert(make_pair(Point(11,5), Point(12,4)));


	    		//row 12
	    		edgeList.insert(make_pair(Point(12,0), Point(11,0)));
	    		edgeList.insert(make_pair(Point(12,0), Point(12,1)));

	    		edgeList.insert(make_pair(Point(12,1), Point(11,1)));
	    		edgeList.insert(make_pair(Point(12,1), Point(12,0)));
	    		edgeList.insert(make_pair(Point(12,1), Point(12,2)));

	    		edgeList.insert(make_pair(Point(12,2), Point(11,2)));
	    		edgeList.insert(make_pair(Point(12,2), Point(12,1)));
	    		edgeList.insert(make_pair(Point(12,2), Point(12,3)));

	    		edgeList.insert(make_pair(Point(12,3), Point(11,3)));
	    		edgeList.insert(make_pair(Point(12,3), Point(12,2)));
	    		edgeList.insert(make_pair(Point(12,3), Point(12,4)));

	    		edgeList.insert(make_pair(Point(12,4), Point(11,4)));
	    		edgeList.insert(make_pair(Point(12,4), Point(12,3)));
	    		edgeList.insert(make_pair(Point(12,4), Point(12,5)));

	    		edgeList.insert(make_pair(Point(12,5), Point(11,5)));
	    		edgeList.insert(make_pair(Point(12,5), Point(12,4)));
	    		edgeList.insert(make_pair(Point(12,5), Point(12,6)));

	    		edgeList.insert(make_pair(Point(12,6), Point(11,6)));
	    		edgeList.insert(make_pair(Point(12,6), Point(12,5)));
	    		edgeList.insert(make_pair(Point(12,6), Point(12,7)));

	    		edgeList.insert(make_pair(Point(12,7), Point(11,7)));
	    		edgeList.insert(make_pair(Point(12,7), Point(12,6)));
	    		edgeList.insert(make_pair(Point(12,7), Point(12,8)));

	    		edgeList.insert(make_pair(Point(12,8), Point(11,8)));
	    		edgeList.insert(make_pair(Point(12,8), Point(12,7)));

	    		//row 12 diagonals
	    		edgeList.insert(make_pair(Point(12,4), Point(11,3)));
	    		edgeList.insert(make_pair(Point(12,4), Point(11,5)));

	    }

	    void setGameOver(bool flag){
	    	   over = flag;
	    }

	    bool isOver(){
	    	    return over;
	    }

	    void draw(SDL_Plotter& g){
	    	    eraseTokens(g);
	    	    drawBoard(g);
	    		drawTokens(g);
	    }

	    void drawTokens(SDL_Plotter& g){
	    	    for(int i = 0; i < NUM_TOKENS; i++){
	    	    	    if(token[i].isActive()){
	    	    	    	    token[i].draw(g);
	    	    	    }
	    	    	    else{
	    	    	    	   //token[i].erase(g);
	    	    	    }
	    	    }
	    	    //shadow.drawHere(g);
	    }

	    void eraseTokens(SDL_Plotter& g){
	    	    for(int i = 0; i < NUM_TOKENS; i++){
	    	    	    	    token[i].erase(g);
	    	    }
	    	   // shadow.eraseHere(g);
	    }

	    void drawBoard(SDL_Plotter& g){
	    	    //draw horizontal lines
	    		Point p1(SIDE_PADDING * LINE_SPACING,   (TOP_PADDING + UPPER_DIAMOND)*LINE_SPACING);
	    		Point p2(p1.x + V_LINES * LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND)*LINE_SPACING);
	    		Line h_line(p1,p2, LINE_WIDTH);
	    	    for(int line = 0; line <= H_LINES; line++){
	    			h_line.draw(g);
	    			p1.y += LINE_SPACING;
	    			p2.y += LINE_SPACING;
	    			h_line.setP1(p1);
	    			h_line.setP2(p2);
	    		}

	    	    //draw vertical lines
	    	    p1.x = (SIDE_PADDING * LINE_SPACING);
	    	    p1.y = ((TOP_PADDING + UPPER_DIAMOND) * LINE_SPACING);
	    	    p2.x = (SIDE_PADDING * LINE_SPACING);
	    	    p2.y = (p1.y + (H_LINES * LINE_SPACING));
	    		Line v_line(p1,p2, LINE_WIDTH);
	    	    for(int line = 0; line <= V_LINES; line++){
	    	    	    v_line.draw(g);
	    			p1.x += LINE_SPACING;
	    			p2.x += LINE_SPACING;
	    			v_line.setP1(p1);
	    			v_line.setP2(p2);
	    		}

	    	    //draw diagonals
	    	    Line d1_line(Point(SIDE_PADDING * LINE_SPACING,                   (TOP_PADDING + UPPER_DIAMOND + H_LINES/2) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING + V_LINES/2 + 2) * LINE_SPACING, UPPER_DIAMOND * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d2_line(Point((SIDE_PADDING + V_LINES) * LINE_SPACING,   (TOP_PADDING + UPPER_DIAMOND + H_LINES/2) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING+V_LINES/2-2) * LINE_SPACING, UPPER_DIAMOND * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d3_line(Point(SIDE_PADDING*LINE_SPACING,             (TOP_PADDING +  UPPER_DIAMOND + H_LINES/2) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING+V_LINES/2)*LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND + H_LINES) * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d4_line(Point((SIDE_PADDING + V_LINES)*LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND + H_LINES/2) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING+V_LINES/2)*LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND + H_LINES) * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d5_line(Point((SIDE_PADDING + V_LINES/2 - 2) * LINE_SPACING, (UPPER_DIAMOND) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING + V_LINES/2) * LINE_SPACING,     (UPPER_DIAMOND - 2) * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d6_line(Point((SIDE_PADDING + V_LINES/2 + 2) * LINE_SPACING, (UPPER_DIAMOND) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING + V_LINES/2) * LINE_SPACING,     (UPPER_DIAMOND - 2) * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d7_line(Point((SIDE_PADDING + V_LINES/2 - 1) * LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND/2 - 1) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING + V_LINES/2 + 1) * LINE_SPACING, (TOP_PADDING + UPPER_DIAMOND/2 + 1 ) * LINE_SPACING),
						 LINE_WIDTH);
	    	    Line d8_line(Point((SIDE_PADDING + V_LINES/2 - 1) * LINE_SPACING, (UPPER_DIAMOND + 1) * LINE_SPACING),
	    	    		         Point((SIDE_PADDING + V_LINES/2 + 1) * LINE_SPACING, (UPPER_DIAMOND - 1) * LINE_SPACING),
						 LINE_WIDTH);

	    	    d1_line.draw(g);
	    	    d2_line.draw(g);
	    	    d3_line.draw(g);
	    	    d4_line.draw(g);
	    	    d5_line.draw(g);
	    	    d6_line.draw(g);
	    	    d7_line.draw(g);
	    	    d8_line.draw(g);
	    }

	    bool checkForClickOnToken(Point p){
	    	    Point gridPoint = snapToGrid(p);
	    	    bool found = false;
	    	    for(int i = 0; !found && i < NUM_TOKENS; i++){
	    	    	    if(token[i].isOn(gridPoint)){
	    	    	    	    setAttachedToken(i);
	    	    	    	    setMoving(true);

	    	    	        	found = true;

	    	    	    		if(attachedToken == 0){
	    	    	    			shadow.setColor(red);
	    	    	    		}
	    	    	    		else{
	    	    	    			shadow.setColor(blue);
	    	    	    		}
	        	    	    token[i].setActive(false);
	        	    	    token[i].setOldLocation(p);
	        	    	    token[i].setLocation(p);
	        	    	    updateShadowToken(gridPoint);
	    			}
	    	    }
	    	    return found;
	    }

	    bool updateTokenLocation(Point p1, Point p2, SDL_Plotter& g){
	    	    bool flag = false;
	    	    Point a,b;
	    	    Point middle;

	    		      	if(correctTurn(p1)){
						int moveStatus = legalMove(p1, p2);

						switch(moveStatus){
							case BAD_MOVE:  // do nothing
											// cout << "Bad Move" << endl;
											 //token[attachedToken].erase(g);
											 token[attachedToken].revertLocation();
											 //token[attachedToken].draw(g);
											 break;

							case GOOD_MOVE:  // do nothing
											 //cout << "Good Move" << endl;
											// token[getAttachedToken()].erase(g);
											 token[getTokenAtLocation(p1)].setLocation(p2);
											 tigersTurn = !tigersTurn;  //switch turn
											 flag = true;
											 break;

							case JUMP_MOVE:  // do nothing
											 cout << "Jump" << endl;

											 tigersTurn = !tigersTurn;  //switch turn
											 middle = p1.midPoint(p2);
											 token[activeTokenAt(p1)].erase(g);
											 token[activeTokenAt(middle)].erase(g);
											 token[activeTokenAt(middle)].setActive(false);
											 token[getTokenAtLocation(p1)].setLocation(p2);
											 flag = true;
											 break;
						}
					}
	    	    return flag;
	    }

	    void checkAttachedToken(Point p, SDL_Plotter& g){
	        	//for(int i = 0; i < NUM_TOKENS; i++){
	    		    if(attachedToken >=0){
				    token[attachedToken].erase(g);
				    token[attachedToken].setLocation(p);
				    token[attachedToken].draw(g);
			    }
	        	//}
	    }

	    bool openLocation(Point p){
	    		bool open = true;
	    		for(int i = 0; open && i < NUM_TOKENS; i++){
	    			if(token[i].isActive() && token[i].getLocation() == p ){
	    				open = false;
	    			}
	    		}

            return open;
	    }

	    bool validLocation(Point p){
            bool flag = true;
	    	    int row = p.x;
           	int col = p.y;

           	if(flag) flag = row >= 0;
        		if(flag) flag = row < GRID_ROW;
        		if(flag) flag = col >= 0;
        		if(flag) flag = col < GRID_COL;
        		if(flag) flag = validLocationGrid[row][col];
	        return 	 flag;
	    }

	    Point snapToGrid(Point p){
	    	    Point np;
	    	    np.x = (static_cast<double>(p.x - X_OFFSET + SQUARE_WIDTH/2)/SQUARE_WIDTH);
	    	    np.y = (static_cast<double>(p.y - Y_OFFSET + SQUARE_WIDTH/2)/SQUARE_WIDTH);
	    	    //np.x *= LINE_SPACING;
	    	    //np.y *= LINE_SPACING;
	    	    return np;
	    }

	    // Return index of token at a point p
	    int activeTokenAt(Point p){
	    	   bool flag = false;
	    	   int index = -1;

	    	   for(int i = 1; i < NUM_TOKENS && !flag; i++){
	    		   if(token[i].isActive() && token[i].getLocation() == p){
	    			   flag = true;
	    			   index = i;
	    		   }
	    	   }
	    	   return index;
	    }


	    //may not need this function
	    //seems to do the same thing as updateTokenLocation
	    void Move(Point p1, Point p2){
            Point a, b, middle;

            switch(legalMove(p1, p2)){
				case BAD_MOVE:  // do nothing
					               cout << "Bad Move" << endl;
								   break;
				case GOOD_MOVE:  // do nothing
					               cout << "Good Move" << endl;
								   break;
				case JUMP_MOVE:  // do nothing
					             cout << "Jump" << endl;
								   break;
            }
	    }

	    int legalMove(Point p1, Point p2){  // from p1 to p2,  p1 ---> p2, 0 - BAD_MOVE
            Point a, b, middle;             //                             1 - GOOD_MOVE
            int flag = BAD_MOVE;            //                            -1 - JUMP_MOVE
            bool jump = false;
            bool jumpPath = false;

			if(!validLocation(p1)){
				   return BAD_MOVE;
			}

			if(!validLocation(p2)){
				   return BAD_MOVE;
			}

            //check to make sure destination is open
            if(!openLocation(p2)){
            	    //cout << "Destination not open" << endl;
            	    return BAD_MOVE;
            }


            if(edgeList.find(make_pair(p1,p2)) != edgeList.end()){
    	        	   // cout << "Valid Path" << endl;
    	        	    return GOOD_MOVE;
    	        }



	        //cout << "Valid Location" << endl;

    	        if(getTokenAtLocation(p1) == 0){ // could be a jump
    	        		//cout << "Check for Jump" << endl;
	            	if(!jumpPath) jumpPath = (p1.x == p2.x)   && (p1.y == p2.y+2 || p1.y == p2.y-2);
	            	if(!jumpPath) jumpPath = (p1.y == p2.y)   && (p1.x == p2.x+2 || p1.x == p2.x-2);
	            	if(!jumpPath) jumpPath = (p1.x == p2.x+2) && (p1.y == p2.y+2 || p1.y == p2.y-2);
	            	if(!jumpPath) jumpPath = (p1.x == p2.x-2) && (p1.y == p2.y+2 || p1.y == p2.y-2);

    	            //check for path through middle
	            	if(jumpPath){
	            		//cout << "Check For Jump" << endl;
	                	middle = p1.midPoint(p2);
	               	int i = activeTokenAt(middle);
   	                if(
    	                	   edgeList.find(make_pair(p1,middle)) != edgeList.end() &&
    	            	       edgeList.find(make_pair(middle,p2)) != edgeList.end()
					  ){
    	                	    jump = true;
    	                }


    	                   if(jump && i > 0){
                           flag = JUMP_MOVE;
                           //deactivate jumped token
                           //cout << "deactivate jumped token" << endl;
                           //token[i].setActive(false);
                           return JUMP_MOVE;
    	                   }
	            	}
    	        }
	    	    return BAD_MOVE;
	    }

	    bool yourTurn(){
	    	   return (token[TIGER].isAttached() == tigersTurn);
	    }

	    void randomMove(SDL_Plotter& g){
	           int row, col;
	           Point p1,p2;
	           int m = 1, n = 3;

	    	       do{
	    	    	   	   if(isTigersTurn()){
	    	    	   		   p1 = token[TIGER].getLocation();
	    	    	   		   m = 2;
	    	    	   		   n = 5;
	    	  	    	   }
	    	    	   	   else{
	    	    	   		   do{
	    	    	   			   p1 = token[(rand()%MAX_MEN+1)].getLocation();
	    	    	   			   cout << "p1 = " << p1 << endl;
	    	    	   			   cout << activeMen();
	    	    	   			   if(activeMen() == 0){
	    	    	   				   cout << "SLEEP" << endl;
	    	    	   				   g.Sleep(30000);
	    	    	   			   }
	    	    	   		   }while(openLocation(p1));
	    	    	   	   }

	    	    	   	   p2.x = p1.x + (m-rand()%n);
				   p2.y = p1.y + (m-rand()%n);

				   //cout << p1 << p2 << endl;
	           }while(!legalMove(p1,p2));
	    	       updateTokenLocation(p1,p2, g);
	    }

	    bool isTigersTurn(){
	    	   return tigersTurn;
	    }

	    void dumpState(){
	    		for(int i = 0; i < 19; i++){
	    			//cout << i << " " << token[i].isActive( ) << " " << token[i].isAttached() << " " << token[i].getLocation().x << " " << token[i].getLocation().y << endl;
	    		}
	    }

	    int activeMen(){
	    		int count = 0;
	    		for(int i = 1; i < NUM_TOKENS; i++){
	    			if(token[i].isActive()){
	    				count++;
	    			}
	    		}
	    		return count;
	    }

	    bool tigerWins(){
	    	    bool flag = false;
	    	    flag = activeMen() < MIN_MEN;
	    	    if(flag) setPause(true);
	    	    return flag;
	    }

	    bool manWins(){
	    	    bool flag = true;
	    	    Point p1 = token[TIGER].getLocation();

	    	    for(int x = 0; flag && x < GRID_COL; x++){
	    	    	   for(int y = 0; flag && y < GRID_ROW; y++){

	    	    		   flag = !legalMove(p1, Point(y,x));
	    	    		  // cout << "Man Win: " << p << " " << Point(x * LINE_SPACING + X_OFFSET , y * LINE_SPACING + Y_OFFSET) << " flag = " << flag << endl;
	    	    	   }
	    	    }

/*
	    	    if(!flag) flag = legalMove(p, Point(p.x + 2 * LINE_SPACING, p.y - 2 * LINE_SPACING));
	    	    cout << "Man Win: " << p << " " << Point(p.x + 2 * LINE_SPACING, p.y - 2 * LINE_SPACING) << endl;
	    	    if(!flag) flag = legalMove(p, Point(p.x, p.y - 2 * LINE_SPACING));
	    	    if(!flag) flag = legalMove(p, Point(p.x - 2 * LINE_SPACING, p.y - 2 * LINE_SPACING));

	    	    if(!flag) flag = legalMove(p, Point(p.x + 2 * LINE_SPACING, p.y));
	    	    if(!flag) flag = legalMove(p, Point(p.x - 2 * LINE_SPACING, p.y));

	    	    if(!flag) flag = legalMove(p, Point(p.x + 2 * LINE_SPACING, p.y + 2 * LINE_SPACING));
	    	    if(!flag) flag = legalMove(p, Point(p.x, p.y + 2 * LINE_SPACING));
	    	    if(!flag) flag = legalMove(p, Point(p.x - 2 * LINE_SPACING, p.y + 2 * LINE_SPACING));
	    	    if(flag) setPause(true);
	    	    cout << "Man WIns Flag = " << flag << endl;
*/
	    	    return flag;
	    }

	    void displayFile(Point p, string name, SDL_Plotter& g){
	    		// ifstream file;
	    		// int row, col;
	    		// int R,G,B;
			    //
	    		// file.open(name.c_str());
	    		// if(!file){
	    		// 	cout << "Error: File Not Open" << endl;
	    		// }
			    //
	    		// file >> row >> col;
			    //
	    		// for(int y = 0; y < row; y++){
	    		// 	for(int x = 0; x < col; x++){
	    		// 		file >> R >> G >> B;
	    		// 		g.plotPixel(p.x + x, p.y + y, R,G,B);
	    		// 	}
	    		// }
	    		// file.close();
	    }

	    bool getPause(){
	    	   return pause;
	    }

	    void setPause(bool flag){
	    	    pause = flag;
	    }

	    int getTokenAtLocation(Point p){
			   //cout  << "Point:  " << p.x << " " << p.y << endl;

    		   for(int i = 0; i < 19; i++){
    			   //cout << i << " " << token[i].getLocation().x << " " << token[i].getLocation().y << endl;
    			  if(token[i].isActive( ) && token[i].getLocation() == p) return i;
    		   }
           return -1; // no active token
	    }

	    int getAttachedToken(){
	    	   return attachedToken;
	    }

	    void setAttachedToken(int t){
	       	attachedToken = t;
	    }

	    bool correctTurn(Point p1){
	    	    bool flag = false;
	    	    int tokenIndex = getTokenAtLocation(p1);
	    	    if( tokenIndex >= 0){
	    	    		if(tigersTurn && tokenIndex == 0){
	    	    			flag = true;
	    	    		}
	    	    		else if(!tigersTurn && tokenIndex > 0){
	    	    			flag = true;
	    	    		}
	    	    }
	    	   // cout << "Correct tokenIndex: " << tokenIndex << endl;
	    	   // cout << "Correct Turn: " << flag << endl;
	    		return flag;
	    }

	    void drawAttachedToken(Point p, SDL_Plotter& g){
	    		if(attachedToken == 0){
	    			shadow.setColor(red);
	    		}
	    		else{
	    			shadow.setColor(blue);
	    		}
	    		shadow.drawHere(g);
	    }

	    void updateShadowToken(Point p){
	    	    shadow.setOldLocation(shadow.getLocation());
	    	    shadow.setLocation(p);
	    }

        bool getMoving(){
        	    return moving;
        }

        void setMoving(bool flag){
        	    moving = flag;
            shadow.setActive(flag);
        }

        void eraseToken(int i, SDL_Plotter& g){
        		token[i].erase(g);
        }

        void drawShadow(SDL_Plotter& g){
           shadow.setOldLocation(shadow.getLocation());
        	   shadow.draw(g);
        }

        void eraseShadow(SDL_Plotter& g){
        	    shadow.eraseHere(g);
        	    shadow.setOldLocation(shadow.getLocation());
        }

        vector<Token_t> makeVector(){
        		vector<Token_t> v;
        		Token_t T;
        		Point p;
        		for(int i = 0; i < NUM_TOKENS; i++){
        			if(token[i].isActive()){
        				if(i>0){
        					T.color = BLUE;
        				}
        				else{
        					T.color = RED;
        				}
        				p = token[i].getLocation();
        				T.location.row = p.x;
        				T.location.col = p.y;
        				v.push_back(T);
        			}
        		}

        	    return v;
        }

        Color_t getTurn(){
        		if(tigersTurn)
        			return RED;
        		else
        			return BLUE;
        }

        void updateScore(int t, int points ){
        		team[t].score += points;
        }

        void eraseBoard(SDL_Plotter& g){
        		for(int x = SIDE_PADDING * LINE_SPACING; x < (SIDE_PADDING + H_LINES) * LINE_SPACING; x++){
        			for(int y = (TOP_PADDING + UPPER_DIAMOND)*LINE_SPACING; y < (TOP_PADDING + UPPER_DIAMOND + V_LINES)*LINE_SPACING; y++){
        				g.plotPixel(x, y,255,255,255);
        			}
        		}
        }

        void dumpScore(SDL_Plotter& g){
        	    double percent;
        	    int maxLength = 350;
        	    Line bar(Point(START_DOT_ROW + DOT_SPACE, START_DOT_COL ), Point(), 15, GREEN);
        	    Point p1, p2;

        	    double sum = 0;
        		for(int i = 0; i < NUM_TEAMS; i++){
        			sum += team[i].score;
        		}
        		//cout << "Total = " << sum << endl;
        		//cout << endl;

        		for(int i = 0; i < NUM_TEAMS; i++){
        			percent = 0;
        			if(team[i].score > 0){
        				percent = team[i].score/sum;
        			}
        			//cout << team[i].name << " " << team[i].score << endl;
        			p1.x = (START_BAR_COL) ;
        			p1.y = (START_DOT_ROW + i*(DOT_SPACE+1) );
        			p2.x = (START_BAR_COL + int(maxLength * percent));
        			p2.y = (START_DOT_ROW + i*(DOT_SPACE+1) );

        			bar.setP1(p1);
        			bar.setP2(p2);

        			bar.draw(g);
        		}
        }

        void reset(SDL_Plotter& g){
			attachedToken = -1; //no attached token
			srand(time(0));
			tigersTurn = true;
			over = false;
			pause = false;
			moving = false;
			shadow.setActive(false);
			token[0].setColor(red);
			token[0].setAttached(false);
			token[0].setLocation(Point(2, 4));

			for(int i = 1; i <= 18; i++){
				   token[i].setColor(blue);
				   token[i].setAttached(false);
				   token[i].setActive(true);
				   int col = (i-1)%9;
				   int row = 11+(i-1)/9;
				   token[i].setLocation(Point(row, col));
				   token[i].setSize(TOKEN_SIZE);
			}
      		//draw Title
		    g.clear();
		    eraseTokens(g);
      	    displayFile(Point(90,10),"Title.bmp.txt",g);
		    dumpScore(g);
		    displayFile(Point(COLUMNS-274-400, ROWS-247-25),"teams.bmp.txt",g);

        }



};


#endif /* BEARGAME_H_ */