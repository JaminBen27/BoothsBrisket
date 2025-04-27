#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
    bool checkAdj(Token_t tiger, Point_t p) {

    }
    bool checkSuicide(const vector<Token_t>& tokens, Token_t human, Point_t newLocation) {
        Token_t tiger = tokens[0];

        if(checkAdj(tiger,newLocation)) {

        }
    }
    inline Move_t Move_BoothsBrisket(const vector<Token_t>& tokens, Color_t color){
        Move_t move;
        int row, col;
        Point_t p1,p2;
        int m = 1, n = 3;
        bool tigersTurn, mansTurn;
            if(color == RED)
            {
                tigersTurn = true;
                mansTurn = false;
            }
            else {
                mansTurn = true;
                tigersTurn = false;
            }
            if(tigersTurn){
                p1 = tokens[0].location;
                p1.col+=1;
                p1.row-=1;
                move.destination =p1;
                move.token = tokens[0];
            }
            else {
                p1 = tokens[1].location;
                p2.row = p1.row-1;
                p2.col = p1.col;
                move.destination = p2;
                move.token = tokens[1];
            }
        return move;

    }

