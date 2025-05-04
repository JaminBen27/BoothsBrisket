//============================================================================
// Name        : Bear.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define SDL_MAIN_HANDLED

#include <iostream>

using namespace std;

#include "SDL_Plotter.h"
#include "Line.h"
#include "BearGame.h"
#include "Team.h"
#include "GroupName.h"
//#include <stdio.h>
//#include <windows.h>


int main() {
	SDL_Plotter g(ROWS, COLUMNS);

	BearGame game(g);
	int x,y;
	Point p1, p2;
    int count = 0;
    int currentRed = 0;
    int currentBlue = 0;

    Move_t redPrevMove1, bluePrevMove1;
    Move_t redPrevMove2, bluePrevMove2;
    int redSameMoveCount = 0, blueSameMoveCount = 0;
    Token redDot(Point(START_DOT_ROW, START_DOT_COL),20, red),
    		  blueDot(Point(START_DOT_ROW, START_DOT_COL+DOT_SIZE), 20, blue);



    game.displayFile(Point(COLUMNS-274-400, ROWS-247-25),"teams.bmp.txt",g);

	while(!g.getQuit() ){
		game.eraseTokens(g);

		if(g.kbhit()){
			switch(g.getKey()){

			}
		}

		g.Sleep(200);
		Move_t m;
		 if(game.isTigersTurn()){
		 	//Move Tiger
		 	switch(currentRed){
		 	    case 0: m =   Move_BoothsBrisket(game.makeVector(), game.getTurn()); break;
		 	}
		 }
		 else{
		 	//Move Man
		 	switch(currentBlue){
		         case 0: m =   Move_BoothsBrisket(game.makeVector(), game.getTurn()); break;
		 	}
		 }

		//Call Student Code
		p2 = Point(m.destination.row, m.destination.col);
		p1 = Point(m.token.location.row, m.token.location.col);
		game.updateTokenLocation(p1,p2, g);


		//Print Game state
		//bool TESTING1 = true;
		if(TESTING1){
			if(game.getTurn() == TIGER){
			cout << "RED - ";
			}
			else{
				cout << "BLUE - ";
			}
			cout << "Move" << p1 << p2 << endl;

			auto v = game.makeVector();
			for(auto i: v){
				//cout << "Token: " << i.color << " " << i.location.row << " " << i.location.col << endl;
			}
			//cout << "Turn: "<< game.getTurn() << endl;
		}


		if(game.legalMove(p1,p2)){
			 game.updateTokenLocation(p1,p2, g);
		}
		else{
			if(TESTING2){
				if(game.getTurn() == TIGER){
					cout << "RED - ";
				}
				else{
					cout << "BLUE - ";
				}
				cout << "Random Move" << endl;
			}

			//game.randomMove(g);
		}
		game.draw(g);

		//check for winner
		if(game.tigerWins()){
			game.setGameOver(true);
			game.displayFile(Point(150,450),"Tiger.bmp.txt",g);
			redSameMoveCount = blueSameMoveCount = 0;
			//Update Tiger Score

			game.updateScore(currentRed, 20);
			g.update();
			g.Sleep(2000);
			game.eraseBoard(g);
			if(nextMatch(currentRed,currentBlue)){
			    game.eraseTokens(g);
			    redDot.eraseHere(g);
			    blueDot.eraseHere(g);

			    redDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentRed, START_DOT_COL ));
			    blueDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentBlue, START_DOT_COL+ DOT_SIZE));
			    g.clear();

			    game.reset(g);
			    game.displayFile(Point(COLUMNS-274-400, ROWS-247-25),"teams.bmp.txt",g);
			    count = 0;
			}
		}
		if(game.manWins()){
			game.displayFile(Point(210,450),"manwins.bmp.txt",g);
			redSameMoveCount = blueSameMoveCount = 0;
			game.updateScore(currentBlue, 20);
			g.update();
			g.Sleep(2000);
			game.eraseBoard(g);
			if(nextMatch(currentRed,currentBlue)){
			    game.eraseTokens(g);

			    redDot.eraseHere(g);
			    blueDot.eraseHere(g);


			    redDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentRed, START_DOT_COL ));
			    blueDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentBlue, START_DOT_COL+ DOT_SIZE));
			    g.clear();

			    game.reset(g);
			    game.displayFile(Point(COLUMNS-274-400, ROWS-247-25),"teams.bmp.txt",g);
			    count = 0;
			}
		}


		if(count > MAX_MOVES){
			redSameMoveCount = blueSameMoveCount = 0;
			game.updateScore(currentRed, MAX_MEN - game.activeMen());

			if(nextMatch(currentRed,currentBlue)){

			    redDot.eraseHere(g);
			    blueDot.eraseHere(g);
			    redDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentRed, START_DOT_COL ));
			    blueDot.setLocation(Point(START_DOT_ROW + DOT_SPACE*currentBlue, START_DOT_COL+ DOT_SIZE));

			    game.reset(g);
			    count = 0;
			}

		}

		count++;
		redDot.drawHere(g);
		blueDot.drawHere(g);
		g.update();

		if(currentRed >= NUM_TEAMS || currentBlue >= NUM_TEAMS){
			cout << "\n\n\n\n" << endl;
			cout << "Game Over" << endl;
			game.dumpScore(g);
			//while(!g.getMouseDown(x,y)){

			//}
			//g.setQuit(true);
			currentRed = currentBlue =0;
			game.reset(g);
		}

		if(game.isOver()){
			game.reset(g);
		}

		g.Sleep(0);
	}
	return 0;
}