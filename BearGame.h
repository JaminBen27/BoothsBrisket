
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
using namespace std;

#include "Token.h"

const int TIGER = 0;
const int MIN_MEN = 1;
const int NUM_TOKENS = 19;  //token ZERO is  the Bear
const int H_LINES  = 8;
const int V_LINES = 8;
const int LINE_WIDTH = 3;
const int LINE_SPACING = 60;
const int TOP_PADDING = 2;
const int BOTTOM_PADDING = 2;
const int SIDE_PADDING = 2;
const int SQUARE_WIDTH = LINE_SPACING;
const int UPPER_DIAMOND = 4;
const int X_OFFSET = LINE_SPACING * SIDE_PADDING;
const int Y_OFFSET = LINE_SPACING * TOP_PADDING;

const int ROWS = LINE_SPACING * (TOP_PADDING + UPPER_DIAMOND + H_LINES + BOTTOM_PADDING);
const int COLUMNS = LINE_SPACING * (SIDE_PADDING + V_LINES + SIDE_PADDING);

const int GRID_ROW = 13;
const int GRID_COL = 9;

const int BAD_MOVE  =  0;
const int GOOD_MOVE =  1;
const int JUMP_MOVE = -1;

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
	    set<pair<Point, Point> > edgeList;
	    bool tigersTurn;
	    bool over;
	    bool pause;
	    SDL_Plotter* plotter;

    public:

	    BearGame(SDL_Plotter& g){
	    	    plotter = &g;
	    	    srand(time(0));
	    	    tigersTurn = true;
	    	    over = false;
	    	    pause = false;
	    		token[0].setColor(RED);
	    		token[0].setAttached(false);
	    		token[0].setLocation(Point(columnToX(4), rowToY(2)));

	    		int i =1;
	    		for(int r = 11; r < GRID_ROW; r++){
	    			for(int c = 0; c < GRID_COL; c++){
	    	    		   token[i].setColor(BLUE);
	    	    		   token[i].setAttached(false);
	    	    		   token[i].setLocation(Point(columnToX(c), rowToY(r)));
                   i++;
	    			}
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
	    	    drawBoard(g);
	    		drawTokens(g);
	    }

	    void drawTokens(SDL_Plotter& g){
	    	    for(int i = 0; i < NUM_TOKENS; i++){
	    	    	    token[i].draw(g);
	    	    }
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

	    void checkForClickOnToken(Point p){
	    	    bool found = false;
	    	    for(int i = 0; !found && i < NUM_TOKENS; i++){
	    	    	    if(token[i].isOn(p)){
	    	    	        	token[i].setAttached(true);
	    	    	        	found = true;
	    			}
	    	    }
	    }

	    bool updateTokenLocation(Point p, SDL_Plotter& g){
	    	    bool flag = false;
	    	    Point a,b,p1,p2;
	    	    Point middle;

	    	    for(int i = 0; i < NUM_TOKENS; i++){
	    		    if(token[i].isAttached()){
	    		      	cout << "Token: " << i <<" is Attached" << endl;
	    		     	token[i].dumpState();
				    token[i].erase(g);
				    token[i].setLocation(snapToGrid(p));

				    int moveStatus = legalMove(token[i].getOldLocation(), token[i].getLocation());

					switch(moveStatus){
						case BAD_MOVE:  // do nothing
										 cout << "Bad Move" << endl;
										 token[i].revertLocation();
										 break;

						case GOOD_MOVE:  // do nothing
										 cout << "Good Move" << endl;
										 tigersTurn = !tigersTurn;  //switch turn
										 flag = true;
										 break;

						case JUMP_MOVE:  // do nothing
										 cout << "Jump" << endl;
										 tigersTurn = !tigersTurn;  //switch turn
										 middle = token[i].getOldLocation().midPoint(token[i].getLocation());
										 p1 = token[i].getOldLocation();
										 p2 = token[i].getLocation();
										 //swap x and y to use row column representation
										 a.x = (p1.y - Y_OFFSET)/LINE_SPACING;
										 a.y = (p1.x - X_OFFSET)/LINE_SPACING;
										 b.x = (p2.y - Y_OFFSET)/LINE_SPACING;
										 b.y = (p2.x - X_OFFSET)/LINE_SPACING;
										 middle.x = (a.x + b.x)/2;
									     middle.y = (a.y + b.y)/2;

										 token[activeTokenAt(middle)].erase(g);
										 token[activeTokenAt(middle)].setActive(false);
										 flag = true;
										 break;
					}

					token[i].setAttached(false);
			    }
	    	    }
	    	    return flag;
	    }

	    void checkAttachedToken(Point p, SDL_Plotter& g){
	        	for(int i = 0; i < NUM_TOKENS; i++){
	    		    if(token[i].isAttached()){
				    token[i].erase(g);
				    token[i].setLocation(p);
			    }
	        	}
	    }

	    bool openLocation(Point p){
	    		bool open = true;
	    		for(int i = 0; open && i < NUM_TOKENS; i++){
	    			if(token[i].isActive() && !token[i].isAttached() && token[i].getLocation() == p ){
	    				open = false;
	    			}
	    		}

            return open;
	    }

	    bool validLocation(Point p){
            int row, col;
            bool flag = true;
            p = snapToGrid(p);

            row = (p.y - Y_OFFSET)/LINE_SPACING;
           	col = (p.x - X_OFFSET)/LINE_SPACING;

           	if(flag) flag = row >= 0;
        		if(flag) flag = row < GRID_ROW;
        		if(flag) flag = col >= 0;
        		if(flag) flag = col < GRID_COL;
        		if(flag) flag = validLocationGrid[row][col];
        		if(flag) flag = openLocation(p);

	        return 	 flag;
	    }

	    Point snapToGrid(Point p){
	    	    Point np;
	    	    np.x = (static_cast<double>(p.x)/LINE_SPACING + 0.5);
	    	    np.y = (static_cast<double>(p.y)/LINE_SPACING + 0.5);
	    	    np.x *= LINE_SPACING;
	    	    np.y *= LINE_SPACING;
	    	    return np;
	    }

	    // Return index of token at a point p
	    int activeTokenAt(Point p){
	    	   bool flag = false;
	    	   int index = -1;

	    	   int x = (p.y * LINE_SPACING) + Y_OFFSET;
	    	   int y = (p.x * LINE_SPACING) + X_OFFSET;

	    	   for(int i = 1; i < NUM_TOKENS && !flag; i++){
	    		   if(token[i].isActive() && token[i].getLocation() == Point(x,y)){
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
            bool flag = false;
            bool jump = false;
            bool jumpPath = false;

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

	    int legalMove(Point p1, Point p2, bool allPossibleMoves = false){  // from p1 to p2,  p1 ---> p2, 0 - BAD_MOVE
            Point a, b, middle;             //                             1 - GOOD_MOVE
            int flag = BAD_MOVE;            //                            -1 - JUMP_MOVE
            bool jump = false;
            bool jumpPath = false;

            //swap x and y to use row column representation
			a.x = (p1.y - Y_OFFSET)/LINE_SPACING;
			a.y = (p1.x - X_OFFSET)/LINE_SPACING;
			b.x = (p2.y - Y_OFFSET)/LINE_SPACING;
			b.y = (p2.x - X_OFFSET)/LINE_SPACING;

            //check to make sure destination is open
            if(!openLocation(p2)) return BAD_MOVE;

            //check to make sure source is occupied
            //if(openLocation(p1)) return BAD_MOVE;

            // Check to make sure it is the correct token's turn
            int sourceToken = 0;
            if(!tigersTurn) sourceToken = getTokenAtLocation(p1);
            if(!allPossibleMoves){
       	       if(tigersTurn && sourceToken  != 0) return BAD_MOVE;
    	           if(!tigersTurn && sourceToken == 0) return BAD_MOVE;
            }

     	       if(!allPossibleMoves) cout <<"Real Move - SourceToken = " << sourceToken << " " << p1 << endl;
            	   if(tigersTurn && sourceToken != 0  && !allPossibleMoves) return BAD_MOVE;
            	   if(!tigersTurn && sourceToken == 0 && !allPossibleMoves) return BAD_MOVE;





    	        if(edgeList.find(make_pair(a,b))->first == a && edgeList.find(make_pair(a,b))->second == b) flag = true;
    	        if(flag) flag = validLocation(p2);

    	        if(!flag and token[TIGER].isAttached()){ // could be a jump

	            	if(!jumpPath) jumpPath = (a.x == b.x) && (a.y == b.y+2 || a.y == b.y-2);
	            	if(!jumpPath) jumpPath = (a.y == b.y) && (a.x == b.x+2 || a.x == b.x-2);
	            	if(!jumpPath) jumpPath = (a.x == b.x+2) && (a.y == b.y+2 || a.y == b.y-2);
	            	if(!jumpPath) jumpPath = (a.x == b.x-2) && (a.y == b.y+2 || a.y == b.y-2);

    	            //check for path through middle
	            	if(jumpPath){
	                	middle.x = (a.x + b.x)/2;
	               	middle.y = (a.y + b.y)/2;
    	                if(edgeList.find(make_pair(a,middle))->first  == a      &&
    	            	       edgeList.find(make_pair(a,middle))->second == middle &&
    	            	       edgeList.find(make_pair(middle,b))->first  == middle &&
				       edgeList.find(make_pair(middle,b))->second == b) jump = true;

    	                   int i = activeTokenAt(middle);
    	                   if(jump && i >= 0){
                           flag = JUMP_MOVE;
                           //desctivarte jumped token
                           //token[i].setActive(false);
    	                   }
	            	}
    	        }

	    	    return flag;
	    }

	    bool yourTurn(){
	    	   return (token[TIGER].isAttached() == tigersTurn);
	    }

	    void randomMove(SDL_Plotter& g){
	           int index;
	           int row, col;
	    	       do{
	    	    	   	   if(isTigersTurn()){
	    	    	   		   index = 0;
	    	    	   	   }
	    	    	   	  else{
	    	    	   		  do{
	    	    	   		      index = rand() % (NUM_TOKENS-1) + 1;
	    	    	   	      }while(!token[index].isActive());
	    	    	   	  }
	    	    	   	   row = rand() % GRID_ROW;
	    	    	   	   col = rand() % GRID_COL;
	    	    	   	   token[index].setAttached(true);
	           }while(updateTokenLocation(Point(X_OFFSET + LINE_SPACING * col, Y_OFFSET + LINE_SPACING * row), g));
	    }

	    bool isTigersTurn(){
	    	   return tigersTurn;
	    }

	    void dumpState(){
	    		for(int i = 0; i < 19; i++){
	    			cout << i << " " << token[i].isActive( ) << " " << token[i].isAttached() << " " << token[i].getLocation().x << " " << token[i].getLocation().y << endl;
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
	    	    Point p = token[TIGER].getLocation();

	    	    for(int x = 0; flag && x < GRID_COL; x++){
	    	    	   for(int y = 0; flag && y < GRID_ROW; y++){

	    	    		   flag = !legalMove(p, Point(x * LINE_SPACING + X_OFFSET , y * LINE_SPACING + Y_OFFSET), true);
	    	    		  // cout << "Man Win: " << p << " " << Point(x * LINE_SPACING + X_OFFSET , y * LINE_SPACING + Y_OFFSET) << " flag = " << flag << endl;
	    	    		   if(!flag) cout << "Man Wins- Legal Move  "<< flag << " " << p << " --> " << Point(x * LINE_SPACING + X_OFFSET , y * LINE_SPACING + Y_OFFSET) << endl;
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
	    		ifstream file;
	    		int row, col;
	    		int R,G,B;

	    		file.open(name.c_str());
	    		if(!file){
	    			cout << "Error: File Not Open" << endl;
	    		}

	    		file >> row >> col;

	    		for(int y = 0; y < row; y++){
	    			for(int x = 0; x < col; x++){
	    				file >> R >> G >> B;
	    				g.plotPixel(p.x + x, p.y + y, R,G,B);
	    			}
	    		}
	    		file.close();
	    }

	    bool getPause(){
	    	   return pause;
	    }

	    void setPause(bool flag){
	    	    pause = flag;
	    }

	    int getTokenAtLocation(Point p){
    		   for(int i = 0; i < 19; i++){
    			  if(token[i].isActive( ) && token[i].getOldLocation().x == p.x && token[i].getOldLocation().y == p.y) return i;
    		   }
           return -1; // no active token
	    }

};


#endif /* BEARGAME_H_ */