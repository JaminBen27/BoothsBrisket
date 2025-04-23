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
    cout << "Debug: Program starting..." << endl;

    try {
        cout << "Debug: About to initialize SDL_Plotter" << endl;
        SDL_Plotter g(ROWS, COLUMNS);
        cout << "Debug: SDL_Plotter initialized successfully" << endl;

        cout << "Debug: About to initialize BearGame" << endl;
        BearGame game(g);
        cout << "Debug: BearGame initialized successfully" << endl;
        int x, y;
        bool tokenSelected = false;

        while(!g.getQuit()) {
            // Handle mouse click (triggered on mouse button up)
            if(g.mouseClick()) {
                //Only track motion if a token is already selected
                if(g.getMouseMotion(x, y) && tokenSelected) {
                    game.checkAttachedToken(Point(x,y), g);
                }
                point click = g.getMouseClick(); // dequeue to prevent spamming
                x = click.x;
                y = click.y;
                if(game.isTigersTurn()) {
                    cout << "Tigers turn" << endl;
                }
                else
                    cout << "Mans Turn"<< endl;
                //cout << "Mouse clicked at (" << x << ", " << y << ")" << endl;

                if(game.isOver() && game.getPause()) {
                    //cout << "Debug: Game over, resetting" << endl;
                    game.setGameOver(false);
                    g.clear();
                    game = BearGame(g);
                } else {
                    if(!tokenSelected) {
                        game.checkForClickOnToken(Point(x,y));
                        tokenSelected = true;
                    } else {
                        game.updateTokenLocation(Point(x,y), g);
                        tokenSelected = false;

                        // if( game.tigerWins()) {
                        //     // cout << "Debug: Tiger wins!" << endl;
                        //     game.displayFile(Point(150,450), "Tiger.bmp.txt", g);
                        //     game.setGameOver(true);
                        //     game.setPause(true);
                        // } else if(game.manWins()) {
                        //     //cout << "Debug: Man wins!" << endl;
                        //     game.displayFile(Point(210,450), "manwins.bmp.txt", g);
                        //     game.setGameOver(true);
                        //     game.setPause(true);
                        // }
                    }
                }
            }


            game.draw(g);
            g.update();

        }
    }
    catch (exception& e) {
        cout << "Exception caught: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cout << "Unknown exception caught!" << endl;
        return 1;
    }

        //cout.flush();  // flush console output

    cout << "Debug: Program ending..." << endl;
    return 0;
}
