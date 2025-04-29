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
static int HUMAN_PROGRESSION_ROW=10;
//GENERIC USEFUL FUNCTIONS
double dist(Point_t p1, Point_t p2);
Point_t mirror(Point_t pivot, Point_t  mirroredVal);
bool onDiag(Token_t);
bool inCage(Token_t);
Move_t moveDiag(Point_t, int);
Move_t moveHorz(Point_t, int);
Move_t moveVert(Point_t, int);
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//HUMAN SPECIFIC FUNTIONS
Move_t humanFunction(const vector<Token_t>& tokens );
bool checkAdj(Token_t tiger, Point_t p);
bool checkCapture(vector<Token_t> tokens, Token_t human, Point_t newLocation);
bool checkSameToken(Token_t token1, Token_t token2);
int getProgColumn(vector<Token_t>);
Token_t getFurthestPiece(vector<Token_t> tokens);
bool checkRowVulnrability(vector<Token_t> tokens, Token_t piece);
Token_t checkProtected(vector<Token_t> tokens);
Move_t protect(vector<Token_t> tokens, Token_t vulnPiece);
void updateProgressionRow(vector<Token_t> tokens);
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//TIGER SPECIFIC FUNCTIONS
Move_t tigerFunction(const vector<Token_t>&);
Token_t findTiger(const vector<Token_t>&);
vector<Point_t> getLegalMovesCage(const vector<Token_t>&, Token_t);
vector<Point_t> getLegalMovesSquare(const vector<Token_t>&, Token_t);
bool isOccupied(const vector<Token_t>&, Point_t);

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
        if(!checkSameToken(token,findTiger(tokens)))
        {
            m = protect(tokens,token);
        }
       //MOVE FURTHEST
        else {
            token = getFurthestPiece(tokens);
            p.col =token.location.col;
            p.row =token.location.row-1;
            m.token = token;
            m.destination =p;
        }
        updateProgressionRow(tokens);
    }
    return m;
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
Token_t getFurthestPiece(vector<Token_t> tokens) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Token_t furthestPiece = tiger;
    for(int i=0; i  <9; i++) {
        if(tokens[i].location.row == HUMAN_PROGRESSION_ROW+1) {
            if(dist(tokens[i].location,tiger.location) > dist(furthestPiece.location,tiger.location) ) {
                furthestPiece= tokens[i];
            }
        }
    }
    return furthestPiece;
}
Move_t protect(vector<Token_t> tokens, Token_t vulnPiece) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Move_t m;
    m.token = tiger;
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
Token_t checkProtected(vector<Token_t> tokens) {
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    //Finding a token with rowVulnrability
    for(Token_t t: tokens) {
        if(checkRowVulnrability(tokens,t)) {
            return t;
        }
    }
    return tiger;
}
bool checkRowVulnrability(vector<Token_t> tokens, Token_t piece) {
    for(int i=0;i < tokens.size(); i++) {
        if(checkSameToken(tokens[i],piece)) {
            tokens.erase(tokens.begin()+i);
        }
        for(Token_t t: tokens) {
            if((t.location.col == piece.location.col && t.location.row == piece.location.row+1) ||
                (t.location.col == piece.location.col && t.location.row == piece.location.row-1)) {
                return false;
            }
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
    move.token = tigerToken;
    if(TIGERMOVECOUNT%2 ==0) {
        move.destination.col = (++tigerToken.location.col);
        move.destination.row = (++tigerToken.location.row);
    }
    else {
        move.destination.col = (--tigerToken.location.col);
        move.destination.row = (--tigerToken.location.row);
    }
    // if (TIGERMOVECOUNT <= 8){
    //     if (TIGERMOVECOUNT == 0) {
    //         move.destination.col = (++tigerToken.location.col);
    //         move.destination.row = (++tigerToken.location.row);
    //     }
    //     else if (TIGERMOVECOUNT == 1) {
    //         move.destination.col = (--tigerToken.location.col);
    //         move.destination.row = (++tigerToken.location.row);
    //     }
    //     else if (TIGERMOVECOUNT == 2 || TIGERMOVECOUNT == 3) {
    //         move.destination.col = (++tigerToken.location.col);
    //         move.destination.row = (++tigerToken.location.row);
    //     }

        // if (onDiag(tigerToken)) {
        //
        // }
    //

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

double pDist(Point_t a, Point_t b) {
    return sqrt(pow(a.col - b.col, 2) + pow(a.row - b.row, 2));
}

inline bool checkSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
    bool sacrifice = false;
    Token_t tigerToken = findTiger(tokens);
    Token_t midpointLocation;
    midpointLocation.location.col = (tigerToken.location.col + human.location.col) / 2;
    midpointLocation.location.row = (tigerToken.location.row + human.location.row) / 2;

    if (pDist(tigerToken.location, human.location) <= sqrt(8)) {
        if (onDiag(tigerToken) && onDiag(human)) {
            if (onDiag(midpointLocation)) {
                for (size_t i = 0; i < tokens.size(); i++) {
                    if (tokens.at(i).location == midpointLocation.location) {
                        sacrifice = true;
                    }
                }
            }
        }
        else if (tigerToken.location.row == human.location.row || tigerToken.location.col == human.location.col && pDist(tigerToken.location, human.location) == 2) {
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens.at(i).location == midpointLocation.location) {
                    sacrifice = true;
                }
            }
        }
        return sacrifice;
    }
}

