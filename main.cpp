#define SDL_MAIN_HANDLED
#include <SDL.h>

//============================================================================
// Name        : Bear.cpp
// Author      : Dr. Booth
// Version     :
// Copyright   : Your copyright notice
// Description : Tiger Game
//============================================================================

#include <iostream>
using namespace std;

#include "SDL_Plotter.h"
#include "Line.h"
#include "BearGame.h"

int main() {

    SDL_Plotter g(ROWS, COLUMNS);

    BearGame game(g);
    int x,y;


    while(!g.getQuit()){


        if(g.kbhit()){
            switch(g.getKey()){

            }
        }

        if(g.getMouseDown(x,y)){
            game.checkForClickOnToken(Point(x,y));
        }

        if(g.getMouseUp(x,y)){
            if(game.isOver() && game.getPause()){
                game.setGameOver(false);
                g.clear();
                game = BearGame(g);
            }
            else{
                game.updateTokenLocation(Point(x,y), g);
                if(!game.isTigersTurn() && game.tigerWins()){
                    game.displayFile(Point(150,450),"Tiger.bmp.txt",g);
                    game.setGameOver(true);
                    game.setPause(true);
                }

                else if(game.isTigersTurn() && game.manWins()){
                    game.displayFile(Point(210,450),"manwins.bmp.txt",g);
                    game.setGameOver(true);
                    game.setPause(true);
                }
            }


        }

        if(g.getMouseMotion(x,y)){

            game.checkAttachedToken(Point(x,y), g);
        }

        if(game.isTigersTurn()){
            //game.randomMove(g); //Move Tiger
        }
        else{
            //game.randomMove(g); //Move Man
        }






        game.draw(g);
        g.update();
    }
    return 0;
}