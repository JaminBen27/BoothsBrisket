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

    SDL_Plotter g(ROWS, COLUMNS);
    cout << "Debug: SDL_Plotter initialized" << endl;

    BearGame game(g);
    int x, y;
    bool tokenSelected = false;

    while(!g.getQuit()) {
        // Handle mouse click (triggered on mouse button up)
        if(g.mouseClick()) {
            point click = g.getMouseClick(); // dequeue to prevent spamming
            x = click.x;
            y = click.y;
            cout << "Mouse clicked at (" << x << ", " << y << ")" << endl;

            if(game.isOver() && game.getPause()) {
                cout << "Debug: Game over, resetting" << endl;
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

                    if(!game.isTigersTurn() && game.tigerWins()) {
                        cout << "Debug: Tiger wins!" << endl;
                        game.displayFile(Point(150,450), "Tiger.bmp.txt", g);
                        game.setGameOver(true);
                        game.setPause(true);
                    } else if(game.isTigersTurn() && game.manWins()) {
                        cout << "Debug: Man wins!" << endl;
                        game.displayFile(Point(210,450), "manwins.bmp.txt", g);
                        game.setGameOver(true);
                        game.setPause(true);
                    }
                }
            }
        }

        // Only track motion if a token is already selected
        if(g.getMouseMotion(x, y) && tokenSelected) {
            game.checkAttachedToken(Point(x,y), g);
        }

        game.draw(g);
        g.update();

        //cout.flush();  // flush console output
    }

    cout << "Debug: Program ending..." << endl;
    return 0;
}
