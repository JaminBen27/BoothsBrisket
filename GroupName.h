#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "constants.h"

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
const vector<Point_t> DIAGONAL_COORDINATES = {
        {4, 4}, {5,5}, {6,6}, {7,7}, {8,8}, {9,7}, {10,6}, {11,5}, {12,4},
        {11,3}, {10,2}, {9,1}, {8,0}, {7,1}, {6,2}, {5,3}, {4,7}
};
const vector<Point_t> CAGE_COORDINATES = {
        {0, 4}, {1, 3}, {2, 2}, {3, 3}, {2, 4}, {1, 5}, {2, 6}, {3, 5}, {4, 4}
};

Move_t tigerFunction(const vector<Token_t>&, int);
Token_t findTiger(const vector<Token_t>&);
bool onDiag(Token_t);
bool inCage(Token_t);
vector<Point_t> getLegalMovesCage(const vector<Token_t>&, Token_t);
vector<Point_t> getLegalMovesSquare(const vector<Token_t>&, Token_t);
bool isOccupied(const vector<Token_t>&, Point_t);

inline Move_t Move_BoothsBrisket(const vector<Token_t>& tokens,
                                 Color_t color) {
    static int moveCount = 0;
    Move_t move;
    int row, col;
    Point_t p1, p2;
    int m = 1, n = 3;
    bool tigersTurn, mansTurn;
    if (color == RED) {
        tigersTurn = true;
        mansTurn = false;
    } else {
        mansTurn = true;
        tigersTurn = false;
    }
    if (tigersTurn) {
        move = tigerFunction(tokens, moveCount);
    } else {
        p1 = tokens[1].location;
        p2.row = p1.row - 1;
        p2.col = p1.col;
        move.destination = p2;
        move.token = tokens[1];
    }
    return move;
}

inline Move_t tigerFunction(const vector<Token_t>& tokens, int moveCount) {

    Move_t move;
    Token_t tigerToken = findTiger(tokens);
    if (moveCount <= 8){

    }


    return move;
}

Token_t findTiger(const vector<Token_t>& tokens) {
    for (const Token_t& token_t : tokens) {
        if (token_t.color == RED) {
            return token_t;
        }
    }
    cout << "ERROR: TIGER TOKEN WAS NOT FOUND" << endl;
    exit(1);  // TODO: Maybe temporary crash, need to test
}

bool onDiag(Token_t token){
    for (const Point_t& diagPoint : DIAGONAL_COORDINATES){
        if (token.location.row == diagPoint.row) {
            if (token.location.col == diagPoint.col) {
                return true;
            }
        }
    }
    return false;
}

bool inCage(Token_t token){
    for (const Point_t& cagePoint : CAGE_COORDINATES) {
        if (token.location.row == cagePoint.row) {
            if (token.location.col == cagePoint.col) {
                return true;
            }
        }
    }
    return false;
}

bool isOccupied(const vector<Token_t>& tokens, Point_t point){
    for (const Token_t& token_t : tokens) {
        if (token_t.location == point) {
            return true;
        }
    }
    return false;
}

vector<Point_t> getLegalMoveCage(const vector<Token_t>& tokens, Token_t token){
    vector<Point_t> moves = {};
    if (inCage(token)){
        switch(token.location.row){
            case 0:
                moves.push_back(Point_t{1, 5});
                moves.push_back(Point_t{1, 3});
                break;
            case 1:
                if (token.location.col == 3) {
                    moves.push_back(Point_t{0, 4});
                    moves.push_back(Point_t{2, 4});
                    moves.push_back(Point_t{2, 2});
                } else if (token.location.col == 5){
                    moves.push_back(Point_t{0, 4});
                    moves.push_back(Point_t{2, 6});
                    moves.push_back(Point_t{2, 4});
                }
                break;
            case 2:
                if (token.location.col == 2){
                    moves.push_back(Point_t{1, 3});
                    moves.push_back(Point_t{2, 4});
                    moves.push_back(Point_t{3, 3});
                } else if (token.location.col == 4){
                    moves.push_back(Point_t{1, 3});
                    moves.push_back(Point_t{1, 5});
                    moves.push_back(Point_t{3, 5});
                    moves.push_back(Point_t{3, 3});
                } else if (token.location.col == 6){
                    moves.push_back(Point_t{1, 5});
                    moves.push_back(Point_t{2, 4});
                    moves.push_back(Point_t{3, 5});
                }
                break;
            case 3:
                if (token.location.col == 3){
                    moves.push_back(Point_t{2, 2});
                    moves.push_back(Point_t{2, 4});
                    moves.push_back(Point_t{4, 4});
                } else if (token.location.col == 5){
                    moves.push_back(Point_t{2, 6});
                    moves.push_back(Point_t{2, 4});
                    moves.push_back(Point_t{4, 4});
                }
                break;
            case 4:
                if (token.location.col == 4){
                    moves.push_back(Point_t{3, 5});
                    moves.push_back(Point_t{3, 3});
                }
                break;
        }
        for (auto it = moves.begin(); it != moves.end(); ) {
            if (isOccupied(tokens, *it)) {
                it = moves.erase(it);
            } else {
                ++it;
            }
        }
    }

    return moves;
}
