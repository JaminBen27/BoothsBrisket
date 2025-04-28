#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "constants.h"

using namespace std;
const vector<Point_t> DIAGONAL_COORDINATES = {
        {4, 4}, {5,5}, {6,6}, {7,7}, {8,8}, {9,7}, {10,6}, {11,5}, {12,4},
        {11,3}, {10,2}, {9,1}, {8,0}, {7,1}, {6,2}, {5,3}, {4,7}
};
const vector<Point_t> CAGE_COORDINATES = {
        {0, 4}, {1, 3}, {2, 2}, {3, 3}, {2, 4}, {1, 5}, {2, 6}, {3, 5}, {4, 4}
};

static int TIGERMOVECOUNT = 0;

Move_t humanFunction(vector<Token_t>& tokens );
bool checkAdj(Token_t tiger, Point_t p);
bool checkCapture(vector<Token_t> tokens, Token_t human, Point_t newLocation);
bool checkSameToken(Token_t token1, Token_t token2);
Point_t mirror(Point_t pivot, Point_t  mirroredVal);

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Move_t tigerFunction(const vector<Token_t>&);
Token_t findTiger(const vector<Token_t>&);
bool onDiag(Token_t);
bool inCage(Token_t);
vector<Point_t> getLegalMovesCage(const vector<Token_t>&, Token_t);
vector<Point_t> getLegalMovesSquare(const vector<Token_t>&, Token_t);
bool isOccupied(const vector<Token_t>&, Point_t);
Move_t moveDiag(Point_t, int);
Move_t moveHorz(Point_t, int);
Move_t moveVert(Point_t, int);

inline Move_t Move_BoothsBrisket(const vector<Token_t>& tokens,
                                 Color_t color) {
    Move_t move{};
    int row, col;
    Point_t p1{}, p2{};
    bool tigersTurn, mansTurn;
    if (color == RED) {
        tigersTurn = true;
        mansTurn = false;
    } else {
        mansTurn = true;
        tigersTurn = false;
    }
    if (tigersTurn) {
        move = tigerFunction(tokens);
    } else {
       // move = humanFunction(tokens);
    }
    return move;
}
inline Move_t humanFunction(vector<Token_t>& tokens ) {
    Move_t m;
    return m;
}

//Checks a token is 1 move away from another piece
bool checkAdj(Token_t tiger, Point_t p) {
    if(abs(tiger.location.col - p.col ) <= 1 && abs(tiger.location.row - p.row) <=0) {
        return true;
    }
    Token_t temp{};
    temp.location = p;
    temp.color = BLUE;
    if(onDiag(tiger) && onDiag(temp)) {
        if(abs(tiger.location.col - p.col ) == 1 && abs(tiger.location.row - p.row) ==1) {
            return true;
        }
    }
    return false;
}

//checks if 2 tokens are the same piece
bool checkSameToken(Token_t token1, Token_t token2) {
        return token1.location.row == token2.location.row && token1.location.col == token2.location.col;
}
//Mirror takes in to points and reutnrs the point reflected across the first point
// EX: mirror ( (2,1) , (2,2) ) returns 2,3
// works for diagonals
// be careful not to mirror long distances
Point_t mirror(Point_t pivot, Point_t  mirroredVal) {
    Point_t m;
     m.row = pivot.row - (mirroredVal.row- pivot.row);
     m.col = pivot.col - (mirroredVal.col- pivot.col);
    return m;

}
//checkCapture Takes in the tokens, the peice moving and the location its moving to
// Returns true if the move results in self sacrifice
bool checkSelfSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
    Token_t tiger = tokens[0];
    bool capture = true;
    if(checkAdj(tiger,newLocation)) {
        Point_t theSpot = mirror(tiger.location,newLocation);
        for(int i=0; i < tokens.size(); i++) {
            if(checkSameToken(human, tokens[i])) {
                tokens.erase(tokens.begin() + i);
            }
        }
        for(Token_t t: tokens) {
            if(t.location.row == theSpot.row && t.location.col == theSpot.col) {
                capture = false;
            }
        }
    }
    return capture;
}
inline Move_t tigerFunction(const vector<Token_t>& tokens) {

    Move_t move;
    Token_t tigerToken = findTiger(tokens);
    if (TIGERMOVECOUNT <= 8){
        if (TIGERMOVECOUNT == 0) {
            move.destination.col = (tigerToken.location.col ++);
            move.destination.row = (tigerToken.location.row ++);
        }
        else if (TIGERMOVECOUNT == 1) {
            move.destination.col = (tigerToken.location.col --);
            move.destination.row = (tigerToken.location.row ++);
        }
        else if (TIGERMOVECOUNT == 2 || TIGERMOVECOUNT == 3) {
            move.destination.col = (tigerToken.location.col ++);
            move.destination.row = (tigerToken.location.row ++);
        }

        if (onDiag(tigerToken)) {

        }
    }

    TIGERMOVECOUNT++;
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
Move_t moveDiag(Point_t location, int direction){
    Move_t newLocation;
    switch (direction) {
        case 1: // up right
            newLocation.destination.row = location.row++;
            newLocation.destination.col = location.col++;
            break;
        case 2: // down left
            newLocation.destination.row = location.row--;
            newLocation.destination.col = location.col--;
            break;
        case 3: // down right
            newLocation.destination.row = location.row++;
            newLocation.destination.col = location.col--;
            break;
        case 4: // up left
            newLocation.destination.row = location.row--;
            newLocation.destination.col = location.col++;
            break;
    }
    return newLocation;
}

Move_t moveHorz(Point_t location, int direction){
    Move_t newLocation;
    switch (direction){
        case 1: // right
            newLocation.destination.row = location.row++;
            newLocation.destination.col = location.col;
            break;
        case 2: //left
            newLocation.destination.row = location.row--;
            newLocation.destination.col = location.col;
            break;
    }
    return newLocation;
}
Move_t moveVert(Point_t location, int direction){
    Move_t newLocation;
    switch (direction){
        case 1: // up
            newLocation.destination.row = location.row;
            newLocation.destination.col = location.col++;
            break;
        case 2: // down
            newLocation.destination.row = location.row;
            newLocation.destination.col = location.col--;
            break;
    }
    return newLocation;
}

