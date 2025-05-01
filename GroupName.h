/*TODO
 *FIX failsafe being tigers position
 *FurthestPiece is poorly designd assumes tokens are sorted
 *Protect Column and protected row could probboly be merged
 *checkColumnDanger has a quintuple nest that could be refactored
 *checkHumanAt() logic would simplify code in alot of places
 *current bug is realted to checkProtectedRow RowBulnFix is not running so all the pieces are just movning up;
 *
 **/
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

static int TIGERMOVECOUNT = 1;
static int HUMAN_PROGRESSION_ROW = 10;
static vector<Move_t> SACMOVES;

//Game Phases
//GENERIC USEFUL FUNCTIONS
double dist(Point_t p1, Point_t p2);
Point_t mirror(Point_t pivot, Point_t  mirroredVal);
bool onDiag(Token_t);
bool inCage(Token_t);
Move_t moveDiag(Token_t, int);
Move_t moveHorz(Token_t, int);
Move_t moveVert(Token_t, int);
bool checkHumanAt(vector<Token_t> tokens, Point_t  p);
Token_t getHumanAt(vector<Token_t> tokens, Point_t  p);

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
vector<Move_t> getFurthestPieces(vector<Token_t> tokens,vector<Token_t>,vector<Token_t>);
bool checkRowVulnrability(vector<Token_t> tokens, Token_t piece);
bool checkColumnDanger(vector<Token_t> tokens, Token_t piece);
vector<Token_t> updateRowVulnrabilities(vector<Token_t> tokens,vector<Token_t>);
vector<Move_t> fixRowVuln(vector<Token_t> tokens, vector<Token_t> rowVulns);
vector<Token_t> sortDistanceFromTiger(vector<Token_t> tokens, Token_t tiger);

void updateProgressionRow(vector<Token_t> tokens);
vector<Point_t> availableDiag(vector<Token_t> tokens);
vector<Move_t> takeDiag(vector<Token_t> tokens);
bool checkBadMove(vector<Token_t> tokens, Move_t m);
bool checkLegalToken(Move_t);
void collectMoves(queue<Move_t>&, vector<Move_t>);

vector<Token_t> getFrontRow(vector<Token_t>);
vector<Token_t> getMiddleRow(vector<Token_t>);
vector<Token_t> getBackRow(vector<Token_t>);

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
pair<bool, Move_t> singleScan(vector<Token_t> tokens, Point_t pos);
pair<bool,Move_t> doubleScan(vector<Token_t> tokens);
Move_t moveToClosestHuman(vector<Token_t> tokens);

inline Move_t Move_BoothsBrisket(const vector<Token_t>& tokens, Color_t c) {
    if (c == RED) {
        return tigerFunction(tokens);
    }
    return humanFunction(tokens);
}

//Checks if a move is legal (i.e. move is in bounds & destination is unoccupied)
//Does not account for the cage
bool checkLegalMove(const vector<Token_t>& tokens, Move_t move) {
    for (Token_t t: tokens) {
        if (t.location == move.destination) {
            return false;
        }
    }
    if (move.destination.col < 0 || move.destination.col > 8) {
        return false;
    }
    if (move.destination.row < 4 || move.destination.row > 12) {
        return false;
    }
    return true;
}

Move_t pickRandom (const vector<Token_t>& tokens) {
    Move_t move;
    do {
        do {
            size_t i = rand() % tokens.size();
            move.token.color = tokens[i].color;
            move.token.location.row = tokens[i].location.row;
            move.token.location.col = tokens[i].location.col;
            move.destination.row = tokens[i].location.row;
            move.destination.col = tokens[i].location.col;
        } while (move.token.color == RED);

        int direction = rand() % 4;
        if (direction == 0) {
            move.destination.col = move.token.location.col + 1;
        } else if (direction == 1) {
            move.destination.col = move.token.location.col - 1;
        } else if (direction == 2) {
            move.destination.row = move.token.location.row + 1;
        } else {
            move.destination.row = move.token.location.row - 1;
        }
    } while (checkLegalMove(tokens, move) == false);
    return move;
}

inline Move_t humanFunction(const vector<Token_t>& tokens) {
    Move_t m;
    Token_t token;
    static bool earlyGame;          //First Half of Board
    static bool midGamer;           //Second Half of Board
    static bool lateGame;           //Cage

    vector<Token_t> rowVulnerabilities;    //Pieces that are vulnerable to a vertical jump
    vector<Token_t> colVulnerabilities;    //Pieces that are vulnerable to a horizontal jump

    vector<Token_t> frontLine;
    vector<Token_t> midLine;
    vector<Token_t> backLine;

    queue<Move_t> moveList;


    if (HUMAN_PROGRESSION_ROW>0) {
        //CHECK PROTECTED

        //current bug is realted to checkProtectedRow RowBulnFix is not running so all the pieces are just moving up;
        //token = checkProtectedRow(tokens);
        frontLine = getFrontRow(tokens);
        midLine = getMiddleRow(tokens);
        backLine = getBackRow(tokens);
        rowVulnerabilities = updateRowVulnrabilities(tokens, frontLine);
        vector<Move_t> temp;
        if(rowVulnerabilities.size() > 0) {
            temp = fixRowVuln(tokens,rowVulnerabilities);
            collectMoves(moveList,temp);
        }
            temp = takeDiag(tokens);
            collectMoves(moveList,temp);
        //MOVE FURTHEST
            temp = getFurthestPieces(tokens,midLine,backLine);
            collectMoves(moveList,temp);
        bool badMove = true;
        while(badMove && moveList.size() > 0) {
            m = moveList.front();
            moveList.pop();
            badMove = checkBadMove(tokens,m);
        }
        if (badMove) {
            return pickRandom(tokens);
        }
        SACMOVES.clear();
        updateProgressionRow(tokens);
    }

    return m;
}
vector<Move_t> fixRowVuln(vector<Token_t> tokens, vector<Token_t> rowVulns) {
    vector<Move_t> moves;
    Move_t m;
    for(Token_t t: rowVulns) {
        Point_t p = t.location;
        p.row +=2;
        if(checkHumanAt(tokens,p)) {
            Token_t protectionPiece = getHumanAt(tokens,p);
            m.token = protectionPiece;
            p.row--;
            m.destination = p;
            moves.push_back(m);
        }
    }
    return moves;
}
vector<Token_t> getFrontRow(vector<Token_t> tokens) {
    vector<Token_t> row;
    for(Token_t t: tokens) {
        if(t.location.row == HUMAN_PROGRESSION_ROW) {
            row.push_back(t);
        }
    }
    return row;
}
vector<Token_t> getMiddleRow(vector<Token_t> tokens) {
    vector<Token_t> row;
    for(Token_t t: tokens) {
        if(t.location.row == HUMAN_PROGRESSION_ROW+1) {
            row.push_back(t);
        }
    }
    return row;
}
vector<Token_t> getBackRow(vector<Token_t> tokens) {
    vector<Token_t> row;
    for(Token_t t: tokens) {
        if(t.location.row == HUMAN_PROGRESSION_ROW+2) {
            row.push_back(t);
        }
    }
    return row;
}
vector<Token_t> updateRowVulnrabilities(vector<Token_t> tokens,vector<Token_t> frontRow) {
    vector<Token_t> rowVulns;
    for(Token_t t: frontRow) {
        Point_t p = t.location;
        p.row++;
        if(!checkHumanAt(tokens,p)) {
            rowVulns.push_back(t);
        }
    }
    return rowVulns;
}
// vector<Token_t> updateColumnVulnrabilities(vector<Token_t> tokens,vector<Token_t> frontRow,) {
//     vector<Token_t> rowVulns;
//     for(Token_t t: frontRow) {
//         Point_t p = t.location;
//         if(!checkHumanAt(tokens,p)) {
//             rowVulns.push_back(t);
//         }
//     }
//     return rowVulns;
// }
inline bool checkLegalToken(Move_t move) {
    for(Move_t m: SACMOVES) {
        if(checkSameToken(m.token,move.token) &&
            m.destination.row == move.destination.row &&
            m.destination.col == move.destination.col) {
            return false;
        }
    }
    return true;
}

bool checkBadMove(vector<Token_t> tokens, Move_t m) {
    //2 bools for easier debugging
    bool selfSac = checkSelfSacrifice(tokens,m.token,m.destination);
    bool sac = checkSacrifice(tokens,m.token,m.destination);
    if(selfSac || sac || !checkLegalMove(tokens,m)) {
        SACMOVES.push_back(m);
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
    Token_t tigerToken = tokens[0];
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
vector<Move_t> takeDiag(vector<Token_t> tokens) {
    vector<Move_t> moves;
    Token_t tiger = tokens[0];
    tokens.erase(tokens.begin());
    Move_t m;
    m.token = tiger;
    m.destination = tiger.location;
    vector<Point_t> diagSpots = availableDiag(tokens);
    for(Point_t p: diagSpots) {
        for(Token_t t: tokens) {
            if(t.location.col == p.col && t.location.row == p.row+1) {
                m.token = t;
                m.destination = p;
                moves.push_back(m);
            }
        }
    }
    return moves;
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
vector<Move_t> getFurthestPieces(vector<Token_t> tokens,vector<Token_t> midRow,vector<Token_t> backRow ) {
    Token_t tiger = tokens[0];
    Token_t furthestPiece = tiger;
    Move_t m;
    vector<Token_t> canadites;
    vector<Move_t> moves;

    tokens.erase(tokens.begin());

    for(Token_t t: midRow) {
        Point_t p = t.location;
        p.row+=1;
        if(checkHumanAt(tokens,p)) {
            canadites.push_back(t);
        }
    }
    canadites = sortDistanceFromTiger(canadites,tiger);
    //TODO GET FURTHEST CALCULATOR
    //SKIPPED FOR NOW FOR TIME SAKE
    for(Token_t t : canadites) {
        m.token = t;
        m.destination = t.location;
        m.destination.row = m.destination.row-1;
        moves.push_back(m);
    }
    return moves;
}
vector<Token_t> sortDistanceFromTiger(vector<Token_t> tokens, Token_t tiger) {
    vector<Token_t> list;
    while(tokens.size() >0) {
        int index;
        Token_t maxDist = tiger;
        for(int i=0; i < tokens.size(); i++) {
            if(dist(tokens.at(i).location, tiger.location) > dist(maxDist.location,tiger.location)) {
                maxDist = tokens.at(i);
                index = i;
            }
        }
        list.push_back(maxDist);
        tokens.erase(tokens.begin()+index);
    }
    return list;
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
    int random = rand();

    //Temporary fix for picking left or right randomly
    if (random % 2 == 0){
        random = 1;
    } else {
        random = 4;
    }
    if (TIGERMOVECOUNT <= 5) {

        if (TIGERMOVECOUNT == 1) {
            move = moveDiag(tigerToken, 4);
        }
        else if (TIGERMOVECOUNT == 2){
            move = moveDiag(tigerToken, 1);
        }
        else if (TIGERMOVECOUNT >= 3 && TIGERMOVECOUNT <= 5) {
            move = moveDiag(tigerToken, random);
        }
    }
    if ((tokens.size() - 1 > 9) && TIGERMOVECOUNT >= 6) {
        pair<bool, Move_t> scanning = singleScan(tokens, tigerToken.location);
        pair<bool, Move_t> secondScan = doubleScan(tokens);
        if (scanning.first == true) {
            cout << "found target" << endl;
            move.destination = scanning.second.destination;
            if (isOccupied(tokens, move.destination)) {
                if (checkOpen(tokens, move.destination)) {
                    Point_t mir = mirror(move.destination, tigerToken.location);
                    move = takeHuman(tigerToken, tokens, mir);
                    if (inBounds(move.destination)) {
                        cout << "AHHHHHHHHHHHHHHHHHH" << endl;
                    }
                    cout << "die die die" << endl;
                } else {
                    move = moveVert(tigerToken, 1);
                }
            }
        } else if (secondScan.first == true) {
            cout << "Hunting" << endl;
            move.destination = secondScan.second.destination;
        } else {
            if (isOccupied(tokens, moveVert(tigerToken, 2).destination)) {
                move = moveVert(tigerToken, 1);
            } else {
                move = moveVert(tigerToken, 2);
            }
        }
    } else if (tokens.size() - 1 < 10){
        move = moveToClosestHuman(tokens);
    }
    TIGERMOVECOUNT++;
    if ( inBounds(move.destination)) {
        return move;
    }
    else {
        move = moveVert(tigerToken, 1);
        return move;
    }
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
        case 1: // down right
            newLocation.destination.row = location.row + 1;
            newLocation.destination.col = location.col + 1;
            break;
        case 2: // up left
            newLocation.destination.row = location.row - 1;
            newLocation.destination.col = location.col - 1;
            break;
        case 3: // up right
            newLocation.destination.row = location.row - 1;
            newLocation.destination.col = location.col + 1;
            break;
        case 4: // down left
            newLocation.destination.row = location.row + 1;
            newLocation.destination.col = location.col - 1;
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
// Token_t checkHumanAt(vector<Token_t> tokens, Point_t p) {
//     for(Token_t t: tokens) {
//         if(t.location.row == p.row && t.location.col == p.col) {
//             return t;
//         }
//     }
//     return tokens[0];
// }


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

pair<bool, Move_t> singleScan(vector<Token_t> tokens, Point_t pos){
    Token_t tigerToken;
    tigerToken.location = pos;
    pair<bool, Move_t> movesReturn;
    Point_t tempMove;
    movesReturn.second.token = tigerToken;
    double tolerance = 0.0001;

    if (onDiag(tigerToken)) {
        for (int i = 0; i < DIAGONAL_COORDINATES.size(); i++) {
            if (abs(dist(tigerToken.location,
                         DIAGONAL_COORDINATES[i]) - sqrt(2)) < tolerance) {
                if (checkOpen(tokens, DIAGONAL_COORDINATES[i])) {
                    movesReturn.first = true;
                    movesReturn.second.destination = DIAGONAL_COORDINATES[i];
                    if (inBounds(movesReturn.second.destination)) {
                        return movesReturn;
                    }
                }
            }
        }
    }

    //Check Right for takeable
    tempMove = tigerToken.location;
    tempMove.col++;
    if(checkOpen(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            return movesReturn;
        }
    }

    //Check Left for takeable
    tempMove = tigerToken.location;
    tempMove.col--;
    if(checkOpen(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            return movesReturn;
        }
    }

    //Check Up for takeable
    tempMove = tigerToken.location;
    tempMove.row++;
    if(checkOpen(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            return movesReturn;
        }
    }

    //Check Down for takeable
    tempMove = tigerToken.location;
    tempMove.row--;
    if(checkOpen(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            return movesReturn;
        }
    }

    movesReturn.first = false;
    movesReturn.second.destination = tigerToken.location;
    return movesReturn;
}

pair<bool,Move_t> doubleScan(vector<Token_t> tokens){
    //TODO: THIS LOGIC MAKES NO SENSE, AND I AM VERY TIRED, GOODNIGHT
    Token_t tigerToken = tokens[0];
    pair<bool, Move_t> movesReturn;
    pair<bool, Move_t> scanResult;
    Point_t tempMove;
    movesReturn.second.token = tigerToken;
    double tolerance = 0.0001;

    if (onDiag(tigerToken)){
        for (int i = 0; i < DIAGONAL_COORDINATES.size(); i++){
            if (abs(dist(tigerToken.location,
                         DIAGONAL_COORDINATES[i]) - sqrt(2)) < tolerance) {
                if (! isOccupied(tokens, DIAGONAL_COORDINATES[i])){
                    movesReturn.first = true;
                    movesReturn.second.destination = DIAGONAL_COORDINATES[i];
                    if(inBounds(movesReturn.second.destination)){
                        scanResult = singleScan(tokens, movesReturn.second.destination);
                        if ( scanResult.first ) {
                            return movesReturn;
                        }
                    }
                }
            }
        }
    }

    //Check Right for takeable
    tempMove = tigerToken.location;
    tempMove.col++;
    if(! isOccupied(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            scanResult = singleScan(tokens, movesReturn.second.destination);
            if ( scanResult.first ) {
                return movesReturn;
            }
        }
    }

    //Check Left for takeable
    tempMove = tigerToken.location;
    tempMove.col--;
    if(! isOccupied(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            scanResult = singleScan(tokens, movesReturn.second.destination);
            if ( scanResult.first ) {
                return movesReturn;
            }
        }
    }

    //Check Up for takeable
    tempMove = tigerToken.location;
    tempMove.row++;
    if(! isOccupied(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            scanResult = singleScan(tokens, movesReturn.second.destination);
            if ( scanResult.first ) {
                return movesReturn;
            }
        }
    }

    //Check Down for takeable
    tempMove = tigerToken.location;
    tempMove.row--;
    if(! isOccupied(tokens, tempMove)){
        movesReturn.first = true;
        movesReturn.second.destination = tempMove;
        if(inBounds(movesReturn.second.destination)){
            scanResult = singleScan(tokens, movesReturn.second.destination);
            if ( scanResult.first ) {
                return movesReturn;
            }
        }
    }

    movesReturn.first = false;
    movesReturn.second.destination = tigerToken.location;
    return movesReturn;
}

bool checkHumanAt(vector<Token_t> tokens, Point_t  p) {
    for(Token_t t: tokens) {
        if(t.location.col == p.col && t.location.row == p.row) {
            return true;
        }
    }
    return false;
}
Token_t getHumanAt(vector<Token_t> tokens, Point_t  p) {
    if(checkHumanAt(tokens,p)) {
        for(Token_t t: tokens) {
            if(t.location.col == p.col && t.location.row == p.row) {
                return t;
            }
        }
    }
    else {
        cout << "ERROR: HUMAN DOESNT EXIST";
    }
    return tokens[0];

}
void collectMoves(queue<Move_t>& q, vector<Move_t> moves) {
    for(Move_t m: moves) {
        q.push(m);
    }
}

Move_t moveToClosestHuman(vector<Token_t> tokens) {
    Token_t tigerToken = tokens[0];
    Move_t move;
    move.token = tigerToken;

    // First check if any humans are directly capturable
    pair<bool, Move_t> scanning = singleScan(tokens, tigerToken.location);
    pair<bool, Move_t> secondScan = doubleScan(tokens);

    if (scanning.first) {
        return scanning.second;
    } else if (secondScan.first) {
        return secondScan.second;
    }

    // Find the closest human
    int closestIndex = 1;
    double closestDistance = dist(tigerToken.location, tokens[1].location);

    for (int i = 2; i < tokens.size(); i++) {
        double currentDist = dist(tigerToken.location, tokens[i].location);
        if (currentDist < closestDistance) {
            closestIndex = i;
            closestDistance = currentDist;
        }
    }

    // Get the closest human token
    Token_t closestHuman = tokens[closestIndex];
    Point_t destination = tigerToken.location;

    if (closestHuman.location.col == tigerToken.location.col) {
        if (closestHuman.location.row < tigerToken.location.row) {
            // Human is above tiger
            destination.row--;
        } else {
            // Human is below tiger
            destination.row++;
        }
    } else if (closestHuman.location.row == tigerToken.location.row) {
        if (closestHuman.location.col < tigerToken.location.col) {
            // Human is to the left
            destination.col--;
        } else {
            // Human is to the right
            destination.col++;
        }
    } else {
        // We need to move diagonally if possible, or choose the best direction
        int rowDiff = closestHuman.location.row - tigerToken.location.row;
        int colDiff = closestHuman.location.col - tigerToken.location.col;

        if (onDiag(tigerToken) && onDiag(closestHuman)) {
            destination.row += (rowDiff > 0) ? 1 : -1;
            destination.col += (colDiff > 0) ? 1 : -1;
        } else {
            if (abs(rowDiff) > abs(colDiff)) {
                destination.row += (rowDiff > 0) ? 1 : -1;
            } else {
                destination.col += (colDiff > 0) ? 1 : -1;
            }
        }
    }

    // Ensure the destination is valid
    if (!inBounds(destination) || isOccupied(tokens, destination)) {
        // If the preferred move isn't valid, try alternatives
        vector<Point_t> possibleMoves;

        Point_t vertMove = tigerToken.location;
        vertMove.row += (closestHuman.location.row > tigerToken.location.row) ? 1 : -1;
        if (inBounds(vertMove) && !isOccupied(tokens, vertMove)) {
            possibleMoves.push_back(vertMove);
        }

        Point_t horzMove = tigerToken.location;
        horzMove.col += (closestHuman.location.col > tigerToken.location.col) ? 1 : -1;
        if (inBounds(horzMove) && !isOccupied(tokens, horzMove)) {
            possibleMoves.push_back(horzMove);
        }

        if (onDiag(tigerToken)) {
            for (int rowDir = -1; rowDir <= 1; rowDir += 2) {
                for (int colDir = -1; colDir <= 1; colDir += 2) {
                    Point_t diagMove = tigerToken.location;
                    diagMove.row += rowDir;
                    diagMove.col += colDir;
                    if (inBounds(diagMove) && !isOccupied(tokens, diagMove)) {
                        Token_t tempToken;
                        tempToken.location = diagMove;

                        if (onDiag(tempToken)) {
                            possibleMoves.push_back(diagMove);
                        }
                    }
                }
            }
        }

        // Choose the move that gets us closest to the human
        if (!possibleMoves.empty()) {
            Point_t bestMove = possibleMoves[0];
            double bestDist = dist(bestMove, closestHuman.location);

            for (const Point_t& p : possibleMoves) {
                double currentDist = dist(p, closestHuman.location);
                if (currentDist < bestDist) {
                    bestDist = currentDist;
                    bestMove = p;
                }
            }

            destination = bestMove;
        }
    }

    move.destination = destination;
    return move;
}