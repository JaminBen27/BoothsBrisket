#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "constants.h"
#include "BearGame.h"

using namespace std;
const vector<Point_t> DIAGONAL_COORDINATES = {
        {4, 4}, {5,5}, {6,6}, {7,7}, {8,8}, {9,7}, {10,6}, {11,5}, {12,4},
        {11,3}, {10,2}, {9,1}, {8,0}, {7,1}, {6,2}, {5,3}, {4,7}
};
const vector<Point_t> CAGE_COORDINATES = {
        {0, 4}, {1, 3}, {2, 2}, {3, 3}, {2, 4}, {1, 5}, {2, 6}, {3, 5}, {4, 4}
};

static int TIGERMOVECOUNT = 0;
static int HUMAN_PROGRESSION_ROW=10;
static vector<Token_t> ILLEGALTOKENS;
//GENERIC USEFUL FUNCTIONS
double dist(Point_t p1, Point_t p2);
Point_t mirror(Point_t pivot, Point_t  mirroredVal);
bool onDiag(Token_t);
bool inCage(Token_t);
Move_t moveDiag(Token_t, int);
Move_t moveHorz(Token_t, int);
Move_t moveVert(Token_t, int);
Token_t getHumanAt(vector<Token_t> tokens);
bool inBounds(Point_t pt);
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//HUMAN SPECIFIC FUNTIONS
Move_t humanFunction(const vector<Token_t>& tokens );
bool checkAdj(Token_t tiger, Point_t p);
bool checkSelfSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation);
bool checkSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation);
bool checkSameToken(Token_t token1, Token_t token2);
Move_t getFurthestPiece(vector<Token_t> tokens);
bool checkRowVulnrability(vector<Token_t> tokens, Token_t piece);
bool checkColumnDanger(vector<Token_t> tokens, Token_t piece);
//Find an unprotected piece
Token_t checkProtected(vector<Token_t> tokens);
//Protects unprotected piece via row protection
Move_t protect(vector<Token_t> tokens, Token_t vulnPiece);
void updateProgressionRow(vector<Token_t> tokens);
vector<Point_t> availableDiag(vector<Token_t> tokens);
Move_t takeDiag(vector<Token_t> tokens);
bool checkBadMove(vector<Token_t> tokens, Move_t m);
bool checkLegalToken(Token_t);
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//TIGER SPECIFIC FUNCTIONS
Move_t tigerFunction(const vector<Token_t>&);
vector<Point_t> getLegalMovesCage(const vector<Token_t>&, Token_t);
vector<Point_t> getLegalMovesSquare(const vector<Token_t>&, Token_t);
bool isOccupied(const vector<Token_t>&, Point_t);
Move_t takeHuman ( Token_t tiger, const vector<Token_t>& tokens, Point_t goal );
bool checkOpen (const vector<Token_t>& tokens, Point_t pt);
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
        move = humanFunction(tokens);
    }
    return move;
}
inline Move_t humanFunction(const vector<Token_t>& tokens ) {
    Move_t m;
    Token_t token;
    Point_t p;
    //DIAGONAL MOVEMNET
    // 1:UP RIGHT
    // 2:DOWN LEFT
    // 3:DOWN RIGHT
    // 4:UP LEFT
    //HORIZONTAL
    // 1:RIGHT 2:LEFT
    //VERTICLE
    // 1:UP 2:DOWN
    //PHASE 1 AND 2
    if(HUMAN_PROGRESSION_ROW>3) {
        token = checkProtected(tokens);
        //CHECK PROTECTED
        bool BADMOVE = true;
        while(BADMOVE) {
            if(!checkSameToken(token,findTiger(tokens)))
            {
                m = protect(tokens,token);
            }
            else if(!checkSameToken( takeDiag(tokens).token,tokens[0])) {
                m = takeDiag(tokens);
            }
            //MOVE FURTHEST
            else {
                m = getFurthestPiece(tokens);
            }
            if(!checkBadMove(tokens,m)) {
                BADMOVE = false;
            }
        }
        ILLEGALTOKENS.clear();
        updateProgressionRow(tokens);
    }
    return m;
}
inline bool checkLegalToken(Token_t token) {
    for(Token_t t: ILLEGALTOKENS) {
        if(checkSameToken(t,token)) {
            return false;
        }
    }
    return true;
}

bool checkBadMove(vector<Token_t> tokens, Move_t m) {
    //2 bools for easier debugging
    bool selfSac = checkSelfSacrifice(tokens,m.token,m.destination);
    bool sac = checkSacrifice(tokens,m.token,m.destination);
    if(selfSac || sac) {
        ILLEGALTOKENS.push_back(m.token);
        return true;
    }
    return false;

}
//checkCapture Takes in the tokens, the peice moving and the location its moving to
// Returns true if the move results in self sacrifice
bool checkSelfSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin()+1);
    bool sac = false;
    Point_t p = mirror(newLocation, human.location);
    if(p.col == tiger.location.col && p.row == tiger.location.row) {
        sac = true;
    }
    return sac;
}
inline bool checkSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
    bool sacrifice = false;
    Token_t tigerToken = findTiger(tokens);
    Token_t midpointLocation;
    midpointLocation.location.col = (tigerToken.location.col + human.location.col) / 2;
    midpointLocation.location.row = (tigerToken.location.row + human.location.row) / 2;

    if (dist(tigerToken.location, human.location) <= sqrt(8)) {
        if (onDiag(tigerToken) && onDiag(human)) {
            if (onDiag(midpointLocation)) {
                for (auto & token : tokens) {
                    if (token.location == midpointLocation.location) {
                        sacrifice = true;
                    }
                }
            }
        }
        else if (tigerToken.location.row == human.location.row || tigerToken.location.col == human.location.col && dist(tigerToken.location, human.location) == 2) {
            for (auto & token : tokens) {
                if (token.location == midpointLocation.location) {
                    sacrifice = true;
                }
            }
        }
    }
    return sacrifice;

}
//finds a piece that can take
Move_t takeDiag(vector<Token_t> tokens) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Move_t m;
    m.token = tiger;
    m.destination = tiger.location;
    vector<Point_t> diagSpots = availableDiag(tokens);
    for(Point_t p: diagSpots) {
        for(Token_t t: tokens) {
            if(t.location.col == p.col && t.location.row == p.row+1 && checkLegalToken(t)) {
                m.token = t;
                m.destination = p;
                return m;
            }
        }
    }
    return m;
}
//Returns a vector with the the open diagonal spots on the progression row
vector<Point_t> availableDiag(vector<Token_t> tokens) {
    vector<Point_t> diagRows;

    int index =0;
    int availDiags =2;
    bool validDiag= true;
    for(Point_t p: DIAGONAL_COORDINATES) {
        if(p.row == HUMAN_PROGRESSION_ROW) {
            for(Token_t t: tokens) {
                if(t.location == p) {
                    validDiag = false;
                }
            }
            if(validDiag)
                diagRows.push_back(p);
            validDiag = true;
        }
    }
    return diagRows;
}
void updateProgressionRow(vector<Token_t> tokens) {
    if(HUMAN_PROGRESSION_ROW == 8) {
        cout << "DEBUG";
    }
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    int count =0;
    for(Token_t t: tokens) {
        if(t.location.row == HUMAN_PROGRESSION_ROW) {
            count++;
        }
    }
    if(count ==9) {
        HUMAN_PROGRESSION_ROW--;
        cout << "---------------------------" << endl;
        cout << "---------------------------" << endl;
        cout << "---------------------------" << endl;
        cout << "---------------------------" << endl;
        cout << "---------------------------" << endl;
        cout << HUMAN_PROGRESSION_ROW << endl;
    }
}
//returns the furthest piece from tiger moving up
//This needs to go through
Move_t getFurthestPiece(vector<Token_t> tokens) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Token_t furthestPiece = tiger;
    Move_t m;
    m.token =tiger;
    m.destination = tiger.location;
    //TODO SHIT Code
    //TODO need to fix this assumes that the first 9 elements are at the top
    //TODO EIther need to sort the tokens vector or cahnge this logic
    for(int i=0; i  <9; i++) {
        if(tokens[i].location.row == HUMAN_PROGRESSION_ROW+1) {
            if(dist(tokens[i].location,tiger.location) > dist(furthestPiece.location,tiger.location) && checkLegalToken(tokens[i]) ) {
                furthestPiece= tokens[i];
            }
        }
    }
    m = moveVert(furthestPiece,1);
    return m;
}
Move_t protect(vector<Token_t> tokens, Token_t vulnPiece) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Move_t m;
    m.token = tiger;
    m.destination = tiger.location;
    if(checkRowVulnrability(tokens,vulnPiece)) {
        // //Fixing row vuln by moving token behind up.
        for(Token_t t: tokens)
        {
            if(t.location.col == vulnPiece.location.col && t.location.row == vulnPiece.location.row+2) {
                m.destination = t.location;
                m.destination.row-=1;
                m.token = t;
            }
        }
    }
    return m;
}
//Finds a token that is vulnrable to a vertical jump
//If it doesnt find it RETURNS THE TIGER
Token_t checkProtected(vector<Token_t> tokens) {
    Token_t tiger = tokens[0];
    //Finding a token with rowVulnrability
    for(Token_t t: tokens) {
        if(checkColumnDanger(tokens,t)) {
            return t;
        }
        if(checkRowVulnrability(tokens,t)) {
            return t;
        }
    }
    return tiger;
}
//returns false if there is a token protecting the selected token
bool checkRowVulnrability(vector<Token_t> tokens, Token_t piece) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    for(int i=0;i < tokens.size(); i++) {
        if(checkSameToken(tokens[i],piece)) {
            tokens.erase(tokens.begin()+i);
        }
    }
    for(Token_t t: tokens) {
        if(checkLegalToken(t)) {
            if((t.location.col == piece.location.col && t.location.row == piece.location.row+1) ||
                (t.location.col == piece.location.col && t.location.row == piece.location.row-1)) {
                return false;
                }
        }
    }
    return true;
}
bool checkColumnDanger(vector<Token_t> tokens, Token_t piece) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    for(Token_t t: tokens) {
        if(fabs(tiger.location.col - t.location.col == 1) && t.location.row == tiger.location.row) {
            Point_t p= mirror(t.location, tiger.location);
        }
    }
    return true;
}
double dist(Point_t p1, Point_t p2) {
    return sqrt(pow(p1.row - p2.row,2) + pow(p1.col - p2.col,2));
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

inline Move_t tigerFunction(const vector<Token_t>& tokens) {
    Move_t move;
    Token_t tigerToken = tokens[0];
    move.token = tigerToken;
    if (TIGERMOVECOUNT <= 7) {
        if (TIGERMOVECOUNT == 8) {
            move.destination.col = (++tigerToken.location.col);
            move.destination.row = (++tigerToken.location.row);
        }
        else if (TIGERMOVECOUNT == 1) {
            move.destination.col = (--tigerToken.location.col);
            move.destination.row = (++tigerToken.location.row);
        }
        else if (TIGERMOVECOUNT == 2 || TIGERMOVECOUNT == 3) {
            move.destination.col = (++tigerToken.location.col);
            move.destination.row = (++tigerToken.location.row);
        }

        if (onDiag(tigerToken) && TIGERMOVECOUNT >= 4 && TIGERMOVECOUNT <= 5) {
            move.destination.col = (++tigerToken.location.col);
            move.destination.row = (++tigerToken.location.row);
        }
    }
    if ( TIGERMOVECOUNT >= 6) {
        move = moveVert(tigerToken, 2);
        if (isOccupied(tokens, move.destination)){
            if (checkOpen(tokens, move.destination)){
                Point_t mir = mirror(move.destination, tigerToken.location);
                move = takeHuman(tigerToken, tokens, mir);
            } else {
                move = moveVert(tigerToken, 1);
            }
        }

        if (!inBounds(move.destination)){

        }
    }


    TIGERMOVECOUNT++;
    return move;
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

Move_t takeHuman ( Token_t tiger, const vector<Token_t>& tokens, Point_t goal ) {
    Move_t move;
    move.token = tiger;
    move.destination = tiger.location;
    if ( !isOccupied(tokens, goal) ) {
        move.destination = goal;
        return move;
    }
    return move;
}

bool checkOpen (const vector<Token_t>& tokens, Point_t pt) {
    bool takeable = false;
    Token_t tigerToken = tokens[0];
    if (isOccupied(tokens, pt)) {
        Point_t mir = mirror(pt, tigerToken.location);

        if (!isOccupied(tokens, mir)) {
            takeable = true;
        }

    }
    return takeable;
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
    return moves;
}
Move_t moveDiag(Token_t item, int direction){
    Point_t location = item.location;
    Move_t newLocation;
    newLocation.token = item;
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

Move_t moveHorz(Token_t item, int direction){
    Point_t location = item.location;
    Move_t newLocation;
    newLocation.token = item;
    switch (direction){
        case 1: // right
            newLocation.destination.row = ++location.row;
            newLocation.destination.col = location.col;
            break;
        case 2: //left
            newLocation.destination.row = --location.row;
            newLocation.destination.col = location.col;
            break;
    }
    return newLocation;
}
Move_t moveVert(Token_t item, int direction){
    Point_t location = item.location;
    Move_t newLocation;
    newLocation.token = item;
    switch (direction){
        case 1: // up
            newLocation.destination.row = --location.row;
            newLocation.destination.col = location.col;
            break;
        case 2: // down
            newLocation.destination.row = ++location.row;
            newLocation.destination.col = location.col;
            break;
    }
    return newLocation;
}
//TODO doesnt really work since you have to return a token
// Token_t getHumanAt(vector<Token_t> tokens, Point_t p) {
//     for(Token_t t: tokens) {
//         if(t.location.row == p.row && t.location.col == p.col) {
//             return t;
//         }
//     }
//     return tokens[0];
// }

inline bool checkSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
    bool sacrifice = false;
    Token_t tigerToken = tokens[0];
    Token_t midpointLocation;
    midpointLocation.location.col = (tigerToken.location.col + human.location.col) / 2;
    midpointLocation.location.row = (tigerToken.location.row + human.location.row) / 2;

    if (dist(tigerToken.location, human.location) <= sqrt(8)) {
        if (onDiag(tigerToken) && onDiag(human)) {
            if (onDiag(midpointLocation)) {
                for (size_t i = 0; i < tokens.size(); i++) {
                    if (tokens.at(i).location == midpointLocation.location) {
                        sacrifice = true;
                    }
                }
            }
        }
        else if (tigerToken.location.row == human.location.row || tigerToken.location.col == human.location.col && dist(tigerToken.location, human.location) == 2) {
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens.at(i).location == midpointLocation.location) {
                    sacrifice = true;
                }
            }
        }
        return sacrifice;
    }
}

bool inBounds(Point_t pt){
    for (int i = 0; i < CAGE_COORDINATES.size(); i++){
        if (pt.col == CAGE_COORDINATES[i].col &&
            pt.row == CAGE_COORDINATES[i].row){
            return true;
        }
    }

    if (pt.col > 8 || pt.col < 0 || pt.row > 12 || pt.row < 4) {
        return false;
    }

    return true;
}

pair<bool, Move_t> singleScan(vector<Token_t> tokens){
    Token_t tigerToken = tokens[0];
    pair<bool, Move_t> movesReturn;
    Point_t tempMove;
    movesReturn.second.token = tigerToken;
    double tolerance = 0.0001;

    if (onDiag(tigerToken)){
        for (int i = 0; i < DIAGONAL_COORDINATES.size(); i++){
            if (abs(dist(tigerToken.location, DIAGONAL_COORDINATES[i]) - sqrt(2)) < tolerance) {

            }
        }
    }

    return movesReturn;
}