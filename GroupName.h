/* TODO
  * Add Flexpiece logic to shimmy
  * Code reverse shimmy
  */
 
 #include <vector>
 #include <cstdlib>
 #include <list>
 
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
 bool shimmy = false;
 bool reachedCorner = false;
 bool doTempo = false;
 bool inCorner = false;
 Move_t tigerMove = {{RED, {2, 4}}, {2,4}};
 Token_t cornerPiece = {RED, {-1,-1}};
 static Point_t gap = {-1, -1};
 static Point_t flex = {-1, -1};
 static int diagCount = 0;
 static int HUMAN_PROGRESSION_ROW = 10;
 static bool ENDGAME = false;
 static vector<Move_t> SACMOVES;
 enum DIRECTION {UP, DOWN, LEFT, RIGHT, NONE};
 
 //Game Phases
 //GENERIC USEFUL FUNCTIONS
 Move_t checkCageSpots(Move_t move, vector<Token_t> tokens);
 bool checkLegalMove(const vector<Token_t>& tokens, Move_t move);
 double dist(Point_t p1, Point_t p2);
 Point_t mirror(Point_t pivot, Point_t  mirroredVal);
 bool onDiag(Token_t);
 bool onDiag(Point_t);
 bool inCage(Point_t);
 Move_t moveDiag(Token_t, int);
 Move_t moveHorz(Token_t, int);
 Move_t moveVert(Token_t, int);
 bool checkHumanAt(vector<Token_t> tokens, Point_t  p);
 Token_t getHumanAt(vector<Token_t> tokens, Point_t  p);
 bool inBounds(Point_t pt);
 bool isSamePoint(Point_t p1, Point_t p2);
 void printPoint(Point_t p);
 void printMove(Move_t m);
 bool isTokenRight(Token_t tiger);
 int getQuadrantFactor(Token_t token);
 //------------------------------------------------------------------
 //------------------------------------------------------------------
 //------------------------------------------------------------------
 //HUMAN SPECIFIC FUNTIONS
 Move_t stalemate(vector<Token_t> tokens, Token_t token);
 bool giveDiag(vector<Token_t> tokens);
 vector<Move_t> fixTripleStack(vector<Token_t> tokens, vector<Token_t> stack);
 Move_t getShimmy(vector<Token_t> tokens);
 vector<Token_t> getTripleStackCol(vector<Token_t> tokens);
 Move_t getPhaseOneMove(vector<Token_t> tokens);
 vector<Move_t> getRowOneMoves(vector<Token_t> frontRow, Token_t tiger);
 Move_t pickRandom (const vector<Token_t> tokens);
 Move_t humanFunction(const vector<Token_t>& tokens );
 bool checkAdj(Token_t tiger, Point_t p);
 bool checkSelfSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation);
 bool checkSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation);
 bool checkSameToken(Token_t token1, Token_t token2);
 vector<Move_t> getFurthestPieces(vector<Token_t> tokens,vector<Token_t>,vector<Token_t>);
 bool checkRowVulnerability(vector<Token_t> tokens, Token_t piece);
 bool checkColumnDanger(vector<Token_t> tokens, Token_t piece);
 DIRECTION checkImmediateDanger(vector<Token_t> tokens);
 
 vector<Token_t> updateColVulnerabilities(vector<Token_t> tokens, vector<Token_t>);
 vector<Token_t> updateRowVulnerabilities(vector<Token_t> tokens,vector<Token_t>);
 vector<Move_t> fixRowVuln(vector<Token_t> tokens, vector<Token_t> rowVulns);
 vector<Move_t> fixColVuln(vector<Token_t> tokens, vector<Token_t> colVulns);
 
 vector<Token_t> sortDistanceFromTiger(vector<Token_t> tokens, Token_t tiger);
 
 void updateProgressionRow(vector<Token_t> tokens);
 vector<Point_t> availableDiag(vector<Token_t> tokens);
 vector<Move_t> takeDiag(vector<Token_t> tokens);
 bool checkBadMove(vector<Token_t> tokens, Move_t m);
 void collectMoves(queue<Move_t>&, vector<Move_t>);
 Token_t getAbsFurthest(vector<Token_t> tokens);
 Move_t tempo(vector<Token_t>);
 vector<Token_t> getFrontRow(vector<Token_t>);
 vector<Token_t> getMiddleRow(vector<Token_t>);
 vector<Token_t> getBackRow(vector<Token_t>);
 int  getHalf(Point_t p);
 vector<Move_t> protectImmediateDanger(vector<Token_t> tokens, DIRECTION d);
 inline DIRECTION checkImmediateDanger(vector<Token_t> tokens);
 //HUMAN END GAME FUNCTIONS
 Move_t getEndGameMove(vector<Token_t> tokens, queue<Move_t>& moveList);
 Move_t moveTo(Token_t t, Point_t p);
 vector<Token_t> getBoxHumans(vector<Token_t> tokens);
 vector<Move_t> getReplacementMoves(vector<Token_t> tokens);
 vector<Move_t> getBoxMoves(vector<Token_t> tokens,vector<Token_t> boxTokens,Token_t tiger);
 void proccessDiagonals(vector<Move_t>& moves);
 int getAmmo(vector<Token_t> tokens);
 
 //---------------------------------------------------------------------
 //---------------------------------------------------------------------
 //---------------------------------------------------------------------
 //TIGER SPECIFIC FUNCTIONS

/*
 * Description: picks a random move
 * Return: Move_t
 * Pre condtion: There are tokens on the board and a move that needs randomization
 * Post condition: returns a random move for the tiger
 */
 Move_t pickRandom(vector<Token_t> tokens, Move_t move);

/*
 * Description: Tiger logic controller
 * Return: Move_t
 * Pre condtion: It is the tiger's turn and the game is not over
 * Post condition: returns the move for the tiger
 */
 Move_t tigerFunction(const vector<Token_t>&);

/*
 * Description: gets a list of all legal points in the cage
 * Return: vector<Point_t>
 * Pre condtion: The game is active and the tiger is in the cage
 * Post condition: tells the tiger what cage moves are available
 */
 vector<Point_t> getLegalMovesCage(const vector<Token_t>&, Token_t);

/*
 * Description: Gets a list of legal board points
 * Return: vector<Point_t>
 * Pre condtion: There are tokens on the board
 * Post condition: tells the tiger what moves are on the board
 */
 vector<Point_t> getLegalMovesSquare(const vector<Token_t>&, Token_t);

/*
 * Description: checks to see if the point in front of the tiger is occupied by a human
 * Return: bool
 * Pre condtion: the tiger wants to move to a position
 * Post condition: returns true or false depending on if the space is filled
 */
 bool isOccupied(const vector<Token_t>&, Point_t);

/*
 * Description: Takes a human
 * Return: Move_t
 * Pre condtion: There is a human in front of the tiger that is unprotected
 * Post condition: takes the piece and moves the tiger past it
 */
 Move_t takeHuman ( Token_t tiger, const vector<Token_t>& tokens, Point_t goal );

/*
 * Description: checks if the space behind a piece is open
 * Return: bool
 * Pre condtion: there is an occupied piece in front of the tiger
 * Post condition: returns true or flase depending if the space is open
 */
 bool checkOpen (const vector<Token_t>& tokens, Point_t pt);

/*
 * Description: Does a single sweep checking for open pieces
 * Return: pair <bool, Move_t>
 * Pre condtion: The tiger is attempting to move/decide to move
 * Post condition: returns true if a takeable piece has been found and the move to take it
 */
 pair<bool, Move_t> singleScan(vector<Token_t> tokens, Point_t pos);

/*
 * Description: Does a single scan from each spot adjacent to the tiger
 * Return: pair<bool, Move_t>
 * Pre condtion: The tiger wants to move and there are no immediately available pieces
 * Post condition: returns true if a piece can be taken next turn and the move to get closer
 */
 pair<bool,Move_t> doubleScan(vector<Token_t> tokens);

/*
 * Description: Begins hunting down remaning pieces
 * Return: Move_t
 * Pre condtion: 3 pieces have been taken by the tiger
 * Post condition: returns a move that gets the tiger closer to taking a piece
 */
 Move_t huntingMode(vector<Token_t> tokens);

/*
 * Description: Moves the tiger close to center mass of humans
 * Return: Move_t
 * Pre condtion: the tiger is not actively hunting
 * Post condition: moves the tiger closer to center of humans
 */
 Move_t groupCenterBias(vector<Token_t> tokens);

/*
 * Description: Checks to see if we are trying to illegally cross into cage
 * Return: Move_t
 * Pre condtion: The tiger wants to move into the cage illegally
 * Post condition: returns a random legal move if the move is illegal
 */
 Move_t checkCageSpots(Move_t move, vector<Token_t> tokens);
 
 inline Move_t Move_BoothsBrisket(const vector<Token_t>& tokens, Color_t c) {

     if (c == RED) {
         return tigerFunction(tokens);
     }
 
     tigerMove.token.location = tigerMove.destination;
     tigerMove.destination = tokens.front().location;
 
     if (tokens.size() > 14) {
         return humanFunction(tokens);
     }
     if (cornerPiece.color == RED) {
         cornerPiece = getAbsFurthest(tokens);
     }

     return stalemate(tokens, cornerPiece);
 }
 
 Move_t getShimmy(vector<Token_t> tokens) {
     if (doTempo) {
         doTempo = false;
         return tempo(tokens);
     }
     Token_t tiger = tokens[0];
     tokens.erase(tokens.begin());
     Move_t move;
 
     if (dist(tiger.location, gap) > 2 && gap.col == flex.col) {
         move.token = {BLUE, gap};
         move.destination = move.token.location;
         move.token.location.row++;
         shimmy = false;
         return move;
     }
 
     //Set destination to pocket
     move.destination = gap;
     move.token = {BLUE, move.destination};
 
     //Pick closer diagonal to move to
     bool left = false;
     if (gap.col <= 4) {
         left = true;
     }
     if (onDiag(flex) == false && flex.col != gap.col) {
         doTempo = true;
         move.token = {BLUE, flex};
         move.destination = flex;
         if (left) {
             move.destination.col--;
             flex.col--;
         }
         else {
             move.destination.col++;
             flex.col++;
         }
         return move;
     }
 
     if (onDiag(move.destination)) {
         diagCount++;
         if (diagCount == 2) {
             diagCount = 0;
             shimmy = false;
         }
         move.token.location.row++;
         gap.row++;
         if (left) {
             gap.col++;
             move.token.location.col++;
         }
         else {
             gap.col--;
             move.token.location.col--;
         }
         return move;
     }
 
 
     if (left) {
         move.token.location.col--;
     }
     else {
         move.token.location.col++;
     }
     gap = move.token.location;
 
     return move;
 }
 
 Move_t diagonalTempo (vector<Token_t> tokens, vector<Token_t> backLine) {
     Token_t tiger = tokens[0];
     tokens.erase(tokens.begin());
     bool left = false;
 
     if (tiger.location.col <= 4) {
         left = true;
     }
     Move_t move;
     move.destination = backLine.front().location;
 
     while (onDiag(move.destination) == false) {
         if (left) {
             move.destination.col--;
         }
         else {
             move.destination.col++;
         }
     }
 
     move.token = {BLUE, move.destination};
     move.token.location.row--;
 
     return move;
 }
 
 inline Move_t humanFunction(const vector<Token_t>& tokens) {
     cout << "Human is thinking" << endl;
     Move_t m;
 
 
     queue<Move_t> moveList;
 
     updateProgressionRow(tokens);
     if(HUMAN_PROGRESSION_ROW == 3) {
          m = getEndGameMove(tokens,moveList);
          m = getEndGameMove(tokens,moveList);
     }
     else if (HUMAN_PROGRESSION_ROW>0) {
         m = getPhaseOneMove(tokens);
     }
     if (checkLegalMove(tokens, m)) {
         return m;
     }
     return pickRandom(tokens);
 }
 Move_t getPhaseOneMove(vector<Token_t> tokens) {
     Move_t m;
     Token_t token;
 
     //Stores possible moves that can be made
     queue<Move_t> moveList;
     vector<Move_t> temp;
 
     vector<Token_t> frontLine = getFrontRow(tokens);
     vector<Token_t> midLine = getMiddleRow(tokens);
     vector<Token_t> backLine = getBackRow(tokens);
     vector<Token_t> rowVulnerabilities = updateRowVulnerabilities(tokens, frontLine);
     vector<Token_t> colVulnerabilities = updateColVulnerabilities(tokens,frontLine);
 
     updateProgressionRow(tokens);
 
     //ROW 1 AVOID IMEDIATE DEATH
     if(HUMAN_PROGRESSION_ROW== 6) {
         cout << "PROG ROW 6";
     }
     if(HUMAN_PROGRESSION_ROW == 10) {
         temp = getRowOneMoves(midLine,tokens[0]);
         collectMoves(moveList,temp);
     }
     if(shimmy && HUMAN_PROGRESSION_ROW >= 8) {
         return getShimmy(tokens);
     }
     if (checkImmediateDanger(tokens) != NONE) {
         temp = (protectImmediateDanger(tokens, checkImmediateDanger(tokens)));
         collectMoves(moveList,temp);
     }
     vector<Token_t> stack;
     stack = getTripleStackCol(tokens);
     if(!stack.empty()) {
         temp = fixTripleStack(tokens,stack);
         collectMoves(moveList,temp);
 
         cout << "YAY IT WORKED";
     }
     if(!rowVulnerabilities.empty()) {
         temp = fixRowVuln(tokens,rowVulnerabilities);
         collectMoves(moveList,temp);
     }
     if(!colVulnerabilities.empty()) {
         temp = fixColVuln(tokens,colVulnerabilities);
         collectMoves(moveList,temp);
     }
     temp = takeDiag(tokens);
     collectMoves(moveList,temp);
     temp = getFurthestPieces(tokens,midLine,backLine);
     collectMoves(moveList,temp);
     bool badMove = true;
     while(badMove && moveList.size() > 0) {
         m = moveList.front();
         if(isSamePoint(m.destination,{10,3})) {
             cout << "debig";
         }
         if(isSamePoint(m.destination,{10,4})) {
             cout << "debig";
         }
         moveList.pop();
         badMove = checkBadMove(tokens,m);
     }
     if (badMove) {
         if (HUMAN_PROGRESSION_ROW < 6) {
             return diagonalTempo(tokens, backLine);
         }
         m =  tempo(tokens);
         shimmy = true;
         gap = tokens.front().location;
         flex = tokens.front().location;
         flex.row += 2;
     }
     return m;
 }

 vector<Move_t> getPhaseTwoMove(vector<Token_t> tokens) {
     vector<Move_t> moves;
     Move_t m;
     for(Token_t t: tokens) {
         if(t.location.row == 5 ) {
             if(t.location.col <= 2 || t.location.col >=6) {
                 m.token = t;
                 m.destination = {t.location.row-1,t.location.col};
                 moves.push_back(m);
             }
         }
     }
     if(moves.empty()) {
         moves = getReplacementMoves(tokens);
     }
     return moves;
 }
 bool giveDiag(vector<Token_t> tokens,Move_t m) {
     Token_t tiger = tokens.at(0);
     tokens.erase(tokens.begin());
     if(onDiag(tiger) && onDiag(m.token.location) && dist(tiger.location,m.token.location) == sqrt(2)) {
         return true;
     }
     return false;
 }
 vector<Move_t> fixTripleStack(vector<Token_t> tokens, vector<Token_t> stack){
     vector<Token_t> tripleStack;
     vector<Move_t> moves;
     Move_t m;
     //sort tripleStack
     while(tripleStack.size() <3) {
         int swapLoc = 0;
 
         Token_t max= stack.at(0);
         for(int i=0; i< stack.size(); i++) {
             if(stack.at(i).location.row < max.location.row) {
                 max = stack.at(i);
                 swap(stack.at(i),stack.at(swapLoc));
                 swapLoc= i;
             }
         }
         tripleStack.push_back(max);
         stack.erase(stack.begin()+swapLoc);
     }
     m.token = tripleStack.at(1);
     m.destination = tripleStack.at(1).location;
     if(!checkHumanAt(tokens, {m.token.location.row,m.token.location.col-1})) {
         m.destination.col --;
     }
     else if(!checkHumanAt(tokens, {m.token.location.row,m.token.location.col+1})){
         m.destination.col ++;
     }
     else {
         return moves;
     }
     moves.push_back(m);
     return moves;
 }
 vector<Token_t> getTripleStackCol(vector<Token_t> tokens) {
     tokens.erase(tokens.begin());
     int count= 0;
     int colCounter=0;
     vector<Token_t> stack;
     while(colCounter <= 8) {
         for(Token_t t: tokens) {
             if(t.location.col == colCounter) {
                 count++;
                 stack.push_back(t);
             }
             if(count == 3) {
                 return stack;
             }
         }
         stack.clear();
         count =0;
         colCounter++;
     }
     return stack;
 }
 
 vector<Move_t> getRowOneMoves(vector<Token_t> midRow, Token_t tiger) {
     bool right = isTokenRight(tiger);
     vector<Move_t> moves;
     Move_t m;
     for(Token_t t: midRow) {
         if(t.location.col >=6 && right || t.location.col <=2 && !right) {
             m.token = t;
             m.destination = t.location;
             m.destination.row --;
             moves.push_back(m);
         }
     }
     return moves;
 
 }
 Move_t getEndGameMove(vector<Token_t> tokens, queue<Move_t>& moveList) {
     Move_t m;
     ENDGAME = true;
     //PHASE 1
     vector<Token_t> boxHumans;
     vector<Move_t> temp;
     boxHumans = getBoxHumans(tokens);
     if(getAmmo(tokens) <=1 || !checkHumanAt(tokens,{4,4})) {
         temp = getReplacementMoves(tokens);
         collectMoves(moveList,temp);
     }
     temp = getBoxMoves(tokens,boxHumans,tokens[0]);
     collectMoves(moveList,temp);
 
     bool badMove = true;
     while(badMove && !moveList.empty()) {
         m = moveList.front();
         moveList.pop();
         badMove = checkBadMove(tokens,m);
         printMove(m);
     }
     if (badMove) {
         shimmy = true;
         return tempo(tokens);
     }
     if (checkLegalMove(tokens, m)) {
         return m;
     }
     else {
         cout << "BAD THING";
     }
     return m;
 }
 bool isTokenRight(Token_t tiger) {
     return tiger.location.col >= 4;
 }
 void printMove(Move_t m) {
     printPoint(m.token.location);
     cout << "->";
     printPoint(m.destination);
     cout << endl;
 }
 void printPoint(Point_t p) {
     cout << "(" << p.row << "," << p.col << ")";
 }
 //Returns a direction the tiger may jump in
 inline DIRECTION checkImmediateDanger(vector<Token_t> tokens) {
     Token_t tiger = tokens[0];
     tokens.erase(tokens.begin());
 
 
     Point_t up = {tiger.location.row - 1, tiger.location.col};
     Point_t down = {tiger.location.row + 1, tiger.location.col};
 
     Point_t left = {tiger.location.row, tiger.location.col - 1};
     Point_t right = {tiger.location.row, tiger.location.col + 1};
 
     for (Token_t t : tokens) {
         if (t.location == left) {
             return LEFT;
         }
         if (t.location == right) {
             return RIGHT;
         }
         if (t.location == up) {
             return UP;
         }
         if (t.location == down) {
             return DOWN;
         }
     }
     return NONE;
 }
 
 vector<Move_t> protectImmediateDanger(vector<Token_t> tokens, const DIRECTION d) {
     Token_t tiger = tokens[0];
     vector<Move_t> moves;
     Move_t move;
     move.token = {BLUE, tiger.location};
 
     switch (d) {
         case LEFT:
             move.token.location.col--;
         break;
         case RIGHT:
             move.token.location.col++;
         break;
         case DOWN:
             move.token.location.row++;
         break;
     }

         move.destination = mirror(move.token.location, tiger.location);
         move.token.location = move.destination;
         move.token.location.row++;
 
     if (checkLegalMove(tokens, move)) {
         moves.push_back(move);
     }
     return moves;
 }
 int getAmmo(vector<Token_t> tokens) {
     Point_t ammoSpots[] = { {4,5}, {4,3}, {5,4} };
     int count =0;
     for(Token_t t: tokens) {
         for(Point_t p: ammoSpots) {
             if(isSamePoint(t.location,p)) {
                 count++;
             }
         }
     }
     return count;
 }
 void proccessDiagonals(vector<Move_t>& moves) {
     Point_t goodDiags[] = {{2,6},{2,2},{0,4}};
     vector<Move_t> temp;
     for(int i=0; i < moves.size(); i++) {
         for(Point_t p: goodDiags) {
             if(isSamePoint(p,moves[i].token.location)) {
                 moves.erase(moves.begin()+i);
                 i--;
             }
             if(isSamePoint(p,moves[i].destination)) {
                 temp.push_back(moves.at(i));
             }
         }
     }
     for(Move_t t: moves) {
         temp.push_back(t);
     }
     moves = temp;
 }
 vector<Point_t> getAdjacentCageCords(Point_t p) {
     vector<Point_t> points;
     points.push_back({p.row+1,p.col+1});
     points.push_back({p.row-1,p.col-1});
     points.push_back({p.row+1,p.col-1});
     points.push_back({p.row-1,p.col+1});
     return points;
 
 
 }
 vector<Move_t> getBoxMoves(vector<Token_t> tokens,vector<Token_t> boxTokens,Token_t tiger) {
     vector<Move_t> possibleMoves;
     //check if tiger next to exit
     if(isSamePoint(tiger.location,{3,3}) || isSamePoint(tiger.location,{3,5})){
         possibleMoves.push_back(tempo(tokens));
         return possibleMoves;
     }
     for(Token_t t: boxTokens) {
         vector<Point_t> points = getAdjacentCageCords(t.location);
         for(Point_t p: points) {
             bool b = inCage(p);
             bool legal = checkLegalMove(tokens,{t,p});
             bool capture =isSamePoint( mirror(p,tiger.location),t.location);
             bool backwards = isSamePoint(p,{4,4});
             if(b && legal && !capture && !backwards) {
                 possibleMoves.push_back({t,p});
             }
         }
     }
     proccessDiagonals(possibleMoves);
     return possibleMoves;
 }
 bool isSamePoint(Point_t p1, Point_t p2) {
     return p1.col == p2.col && p1.row == p2.row;
 }
 vector<Token_t> getBoxHumans(vector<Token_t> tokens) {
     vector<Token_t> boxTokens;
     for(Token_t t: tokens) {
         for(Point_t p: CAGE_COORDINATES) {
             if(isSamePoint(p,t.location) && t.color == BLUE) {
                 boxTokens.push_back(t);
             }
         }
     }
     return boxTokens;
 }
 vector<Move_t> getReplacementMoves(vector<Token_t> tokens) {
     Point_t godSpot = {4,4};
     vector<Move_t> listMoves;
     Move_t m;
     if(!checkHumanAt(tokens,{4,4})) {
         for(Token_t t: tokens) {
             if(dist(t.location,godSpot) == 1) {
                 listMoves.push_back(moveTo(t,godSpot));
             }
         }
     }
     else {
         for(Token_t t: tokens) {
             if(dist(t.location,godSpot) == 2 || dist(t.location,godSpot) == 3 ||dist(t.location,godSpot) == 4) {
                 Move_t tempMove = moveTo(t,godSpot);
                 if(!checkHumanAt(tokens,tempMove.destination))
                     listMoves.push_back(moveTo(t,godSpot));
             }
         }
     }
     return listMoves;
 }
 int getQuadrantFactor(Token_t token) {
     Point_t p= token.location;
     if(p.row > 8 && p.col >=4) {
         return 1;
     }
     if(p.row > 8 && p.col < 4) {
         return -1;
     }
     if(p.row <= 8 && p.col >=4) {
         return -1;
     }
     if(p.row >= 8 && p.col < 4) {
         return +1;
     }
     cout << "ERROR IN GETQUADRANT" << endl;
     return 0;
 }
 Move_t moveTo(Token_t t, Point_t p) {
     Move_t m;
     m.token = t;
     if(onDiag(t) && onDiag(p) && dist(t.location,p) == sqrt(2)) {
         m.destination = p;
         return m;
     }
     if(onDiag(t) && onDiag(p) && dist(t.location,p) == sqrt(8)) {
 
         m.destination.row = p.row;
         m.destination.col = p.col -getQuadrantFactor(t);
         return m;
     }
     if(!inCage(t.location)) {
         if(t.location.row == p.row) {
             if(t.location.col > p.col) {
                 m.destination = {t.location.row,t.location.col-1};
             }
             else {
                 m.destination = {t.location.row,t.location.col+1};
             }
         }
         else {
             m.destination = {t.location.row-1,t.location.col};
         }
     }
     return m;
 }
 Move_t tempo(vector<Token_t> tokens) {
     cout << "Tempo: ";
     vector<Token_t> backRow = getBackRow(tokens);
     Token_t token = getAbsFurthest(tokens);
     if(ENDGAME) {
         vector<Move_t> replaceMoves = getReplacementMoves(tokens);
         if(replaceMoves.size()>0) {
             return replaceMoves[0];
         }
     }
     Move_t m;
     m.token = token;
     for(Token_t t: backRow) {
         if(!checkHumanAt(tokens,{t.location.row-1,t.location.col})) {
             m.token = t;
             m.destination = {t.location.row-1,t.location.col};
             return m;
         }
     }
     m.destination = token.location;
     m.destination.row++;
     return m;
 }
 Token_t getAbsFurthest(vector<Token_t> tokens) {
     Token_t tiger = tokens.at(0);
     tokens.erase(tokens.begin());
     Token_t maxDist = tiger;
     for(Token_t t: tokens) {
         if(dist(t.location,tiger.location) > dist(maxDist.location,tiger.location)) {
             maxDist = t;
         }
     }
     return maxDist;
 }
 int  getHalf(Point_t p) {
     Point_t temp = p;
     temp.col -=1;
     if(p.col <=4 && inBounds(temp)) {
         return -1;
     }
     if( p.col <=4 && !inBounds(temp)) {
         return 1;
     }
     temp.col +=2;
     if(p.col> 4 && inBounds(temp)) {
         return 1;
     }
     if(p.col > 4 && !inBounds(temp)) {
         return -1;
     }
     cout << "ERROR ThIS SHOULD NOT BE PRINTING(GETHALF)";
     return 0;
 }
 vector<Move_t> fixColVuln(vector<Token_t> tokens, vector<Token_t> colVulns) {
     vector<Move_t> moves;
     Move_t m;
     Point_t primary;
     Point_t secondary;
     for(Token_t t: colVulns) {
         primary = {t.location.row + 1, t.location.col + getHalf(t.location)};
         //primary.col += getHalf(primary);
         //primary.row +=1;
 
         secondary = {t.location.row + 1, t.location.col - getHalf(t.location)};
 
         if(checkHumanAt(tokens,primary)) {
             m.token = getHumanAt(tokens,primary);
             m.destination = {primary.row - 1, primary.col};
             moves.push_back(m);
         }
         if (checkHumanAt(tokens,secondary)) {
             m.token = getHumanAt(tokens,secondary);
             m.destination = {secondary.row - 1, secondary.col};
             moves.push_back(m);
         }
     }
     return moves;
 }
 
 vector<Token_t> updateColVulnerabilities(vector<Token_t> tokens,vector<Token_t> frontRow) {
     vector<Token_t> colVulns;
     for(Token_t t: frontRow) {
         Point_t left = {t.location.row, t.location.col  - 1};
         Point_t right = {t.location.row, t.location.col + 1};
         if (t.color == BLUE && checkHumanAt(tokens,left) == false && checkHumanAt(tokens,right) == false) {
             colVulns.push_back(t);
         }
     }
     return colVulns;
 }
 
 vector<Move_t> fixRowVuln(vector<Token_t> tokens, vector<Token_t> rowVulns) {
     vector<Move_t> moves;
     Move_t m;
     //Check every vulnerable token
     for(Token_t t: rowVulns) {
         Point_t p = t.location;
         p.row +=2;
         //check 2 spaces behind vulnerable piece and check if there is a human -> move it up if there is
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
     tokens.erase(tokens.begin());
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
 vector<Token_t> updateRowVulnerabilities(vector<Token_t> tokens, vector<Token_t> frontRow) {
     vector<Token_t> rowVulns;
     //Check every token in the front row (excluding tiger)
     for(Token_t t: frontRow) {
             Point_t front = {t.location.row - 1, t.location.col};
             Point_t back = {t.location.row + 1, t.location.col};
             if(!checkHumanAt(tokens,front) && !checkHumanAt(tokens,back)) {
                 rowVulns.push_back(t);
             }
     }
     return rowVulns;
 }
 
 //Takes a token and sends it to the corner
 Move_t goToCorner (vector<Token_t> tokens, Token_t token) {
     Move_t move;
     move.token = token;
     move.destination = token.location;
 
     if (move.token.location.col == 0 || move.token.location.col == 8) {
         if (move.token.location.row < 8) {
             move.destination.row--;
             cornerPiece.location.row--;
         }
         else {
             move.destination.row++;
             cornerPiece.location.row++;
         }
     }
     else {
         if (move.token.location.col < 4) {
             move.destination.col--;
             cornerPiece.location.col--;
         }
         else {
             move.destination.col++;
             cornerPiece.location.col++;
         }
     }
 
     if (isOccupied(tokens, move.destination)) {
         return pickRandom({{BLUE, move.destination}});
     }
 
     return move;
 }
 
 double distToCorner(Token_t token) {
     double shortestPath = 100;
     double d;
     vector<Point_t> corners = {{12,0},{12,8},{4,0},{4,8}};
     for (Point_t p : corners) {
         d = dist(token.location, p);
         if (d < shortestPath) {
             shortestPath = d;
         }
     }
 
     return shortestPath;
 }
 
 Move_t mirror() {
     int row = tigerMove.destination.row - tigerMove.token.location.row;
     int col = tigerMove.destination.col - tigerMove.token.location.col;
 
     Move_t move;
     move.token = cornerPiece;
     cornerPiece.location.row -= row;
     cornerPiece.location.col -= col;
     move.destination = cornerPiece.location;
 
     return move;
 
 }
 
 Move_t stalemate (vector<Token_t> tokens, Token_t token) {
     if (distToCorner(token) == 0) {
         reachedCorner = true;
     }
     if (reachedCorner == false || distToCorner(token) == 1) {
         return goToCorner(tokens, token);
     }
     if (dist(cornerPiece.location, tigerMove.destination) == 1) {
         return mirror();
     }
     Move_t move = pickRandom({token});
 
     while (checkSelfSacrifice(tokens, move.token, move.destination)) {
         cornerPiece = move.token;
         move = pickRandom({token});
     }

     return move;
 }
 
 
 bool checkBadMove(vector<Token_t> tokens, Move_t m) {
     //2 bools for easier debugging
 
     bool selfSac = checkSelfSacrifice(tokens,m.token,m.destination);
     bool sac = checkSacrifice(tokens,m.token,m.destination);
     bool gDiag = giveDiag(tokens,m);
     if(gDiag) {
         cout << "giveDiag Triggered";
     }
     if(selfSac || sac || gDiag || !checkLegalMove(tokens,m)) {
         return true;
     }
     return false;
 
 }
 //checkCapture Takes in the tokens, the peice moving and the location its moving to
 // Returns true if the move results in self sacrifice
 bool checkSelfSacrifice(vector<Token_t> tokens, Token_t human, Point_t newLocation) {
     Token_t tiger = tokens[0];
     tokens.erase(tokens.begin());
     bool sac = false;
     Point_t p = mirror(newLocation, human.location);
     if(p.col == tiger.location.col && p.row == tiger.location.row) {
         sac = true;
     }
     if(onDiag(tiger) && onDiag({BLUE,newLocation})) {
 
     }
     bool mir = checkHumanAt(tokens,mirror(newLocation,tiger.location));
     double d = dist(newLocation,tiger.location) == 1;
     if(!mir && d == 1) {
         if(tiger.location.row == newLocation.row) {
             sac = true;
         }
 
     }
     if(!mir && d == sqrt(2)) {
         if(onDiag(tiger) && onDiag({BLUE,newLocation})) {
             sac = true;
         }
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
             if(isSamePoint(tiger.location, {p.row-1,p.col})) {
                 if(dist(t.location,p) == sqrt(2) && onDiag(t)) {
                     m = moveTo(t,p);
                     moves.push_back(m);
                 }
             }
             else if(t.location.col == p.col && t.location.row == p.row+1) {
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
         //cout << "DEBUG";
     }
     Token_t tiger = tokens[0];
     tokens.erase(tokens.begin());
     int count =0;
     int secondCount =0;
     Point_t key = {2,4};
     if(HUMAN_PROGRESSION_ROW ==3 && inCage(tiger.location) && checkHumanAt(tokens,key)) {
         return;
     }
 
     for(Token_t t: tokens) {
         if(t.location.row == HUMAN_PROGRESSION_ROW) {
             count++;
         }
     }
     for(Token_t t: tokens) {
         if(t.location.row == HUMAN_PROGRESSION_ROW+1) {
             secondCount++;
         }
     }
     if(count ==9 && secondCount ==9) {
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
 
     Move_t move;
     double maxDist = 0;
     for (Token_t t: canadites) {
         if (dist(tiger.location, t.location) > maxDist) {
             maxDist = dist(tiger.location, t.location);
             move.token = t;
         }
     }
     move.destination = move.token.location;
     move.destination.row--;
 
     moves.push_back(move);
 
     return moves;
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
 //move dest ( 10,4), tigerPos ( 11,4)
 Point_t mirror(Point_t pivot, Point_t  mirroredVal) {
     Point_t m;
      m.row = pivot.row - (mirroredVal.row- pivot.row);
      m.col = pivot.col - (mirroredVal.col- pivot.col);
     return m;
 }
 
 bool checkHumanAt(vector<Token_t> tokens, Point_t  p) {
     for(Token_t t: tokens) {
         if (t.location.col == p.col && t.location.row == p.row) {
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
 
 //Checks if a move is legal (i.e. move is in bounds & destination is unoccupied)
 //Does not account for the cage
 bool checkLegalMove(const vector<Token_t>& tokens, Move_t move) {
     bool found = false;
     for (Token_t t: tokens) {
         if (t.location == move.destination) {
             return false;
         }
         if (t.location == move.token.location) {
             found = true;
         }
     }
     if (found == false) {
         return false;
     }
     for(Point_t p: CAGE_COORDINATES) {
         if(isSamePoint(move.destination,p)) {
             return true;
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
 
 //HUMANS RANDOM
 Move_t pickRandom (const vector<Token_t> tokens) {
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
         }
         else if (direction == 1) {
             move.destination.col = move.token.location.col - 1;
         }
         else if (direction == 2) {
             move.destination.row = move.token.location.row + 1;
         }
         else {
             move.destination.row = move.token.location.row - 1;
         }
     } while (checkLegalMove(tokens, move) == false);
     if (reachedCorner) {
         cornerPiece.location = move.destination;
     }
     return move;
 }
 //TIGER RANDOM
 Move_t pickRandom(vector<Token_t> tokens, Move_t move) {
     move.destination = move.token.location;
     do {
         if (rand() % 2 == 0) {
             if (rand() % 2 == 0) {
                 move.destination.col++;
             }
             else {
                 move.destination.col--;
             }
         }
         else {
             if (rand() % 2 == 0) {
                 move.destination.row++;
             }
             else {
                 move.destination.row--;
             }
         }
     } while (!checkLegalMove(tokens, move));
 
     return move;
 }
 inline Move_t tigerFunction(const vector<Token_t>& tokens) {
     Move_t move;
     Token_t tigerToken = tokens[0];
     move.token = tigerToken;
     int random = rand();
 
     // //BENS TESTING ALG
 
     //  vector<Point_t> p;
     // if(inCage(tigerToken.location)) {
     //     p = getLegalMovesCage(tokens,tigerToken);
     // }
     // else {
     //     p = getLegalMovesSquare(tokens,tigerToken);
     // }
     // random = random%p.size();
     // move.destination = p[random];
     // return move;
 
 
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
      if ((tokens.size() -1 > 14) && TIGERMOVECOUNT >= 6) {
          pair<bool, Move_t> scanning = singleScan(tokens, tigerToken.location);
          pair<bool, Move_t> secondScan = doubleScan(tokens);
          if (scanning.first == true) {
              move.destination = scanning.second.destination;
              if (isOccupied(tokens, move.destination)) {
                  if (checkOpen(tokens, move.destination)) {
                      Point_t mir = mirror(move.destination, tigerToken.location);
                      move = takeHuman(tigerToken, tokens, mir);
                  } else {
                      move = pickRandom(tokens, move);
                  }
              }
          } else if (secondScan.first == true) {
              move.destination = secondScan.second.destination;
          } else {
              move = groupCenterBias(tokens);
          }
      }
      else if (tokens.size() - 1 < 15) {
          move = huntingMode(tokens);
      }
      TIGERMOVECOUNT++;
      if ( inBounds(move.destination) && ! isOccupied(tokens, move.destination)) {
          move = checkCageSpots(move, tokens);
          return move;
      }
          move = pickRandom(tokens, move);
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
 
 bool onDiag(Point_t point) {
     for (const Point_t& diagPoint : DIAGONAL_COORDINATES){
         if (point.row == diagPoint.row) {
             if (point.col == diagPoint.col) {
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
 
         if ( inBounds(move.destination)) {
             return move;
         }
     }
     return move = pickRandom(tokens, move);
 }
 
 bool checkOpen (const vector<Token_t>& tokens, Point_t pt) {
     bool takeable = false;
     Token_t tigerToken = tokens[0];
     if (isOccupied(tokens, pt)) {
         Point_t mir = mirror(pt, tigerToken.location);
 
         if (!isOccupied(tokens, mir)) {
             if ( inBounds(mir) ) {
                 takeable = true;
             }
         }
 
     }
     return takeable;
 }
 
 bool inCage(Point_t token){
     for (const Point_t& cagePoint : CAGE_COORDINATES) {
         if (token.row == cagePoint.row) {
             if (token.col == cagePoint.col) {
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
     if (inCage(token.location)){
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
     //newLocation.destination =item.location;
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
 
 
 Move_t huntingMode(vector<Token_t> tokens) {
     Token_t tigerToken = tokens[0];
     Move_t move;
     move.token = tigerToken;
 
     // First check if any humans are directly capturable
     pair<bool, Move_t> scanning = singleScan(tokens, tigerToken.location);
     pair<bool, Move_t> secondScan = doubleScan(tokens);
 
     if (scanning.first) {
         move.destination = scanning.second.destination;
         if (isOccupied(tokens, move.destination)) {
             if (checkOpen(tokens, move.destination)) {
                 Point_t mir = mirror(move.destination, tigerToken.location);
                 move = takeHuman(tigerToken, tokens, mir);
                 return move;
             }
         }
         move = pickRandom(tokens, move);
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
 
         if (onDiag(tigerToken)) {
             Point_t potentialDiag = tigerToken.location;
             potentialDiag.row += (rowDiff > 0) ? 1 : -1;
             potentialDiag.col += (colDiff > 0) ? 1 : -1;
 
             Token_t tempToken;
             tempToken.location = potentialDiag;
 
             if (onDiag(tempToken)) {
                 destination = potentialDiag;
             } else {
                 if (abs(rowDiff) > abs(colDiff)) {
                     destination.row += (rowDiff > 0) ? 1 : -1;
                 } else {
                     destination.col += (colDiff > 0) ? 1 : -1;
                 }
             }
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
 
 Move_t groupCenterBias(vector<Token_t> tokens) {
     Move_t move;
     Token_t tigerToken = tokens[0];
     move.token = tigerToken;
 
     // First check for any capturing opportunities
     pair<bool, Move_t> scanning = singleScan(tokens, tigerToken.location);
     if (scanning.first) {
         move.destination = scanning.second.destination;
         if (isOccupied(tokens, move.destination)) {
             if (checkOpen(tokens, move.destination)) {
                 Point_t mir = mirror(move.destination, tigerToken.location);
                 move = takeHuman(tigerToken, tokens, mir);
                 return move;
             }
         } else if (inBounds(move.destination)) {
             return move;
         }
     }
 
     pair<bool, Move_t> secondScan = doubleScan(tokens);
     if (secondScan.first && inBounds(secondScan.second.destination)) {
         return secondScan.second;
     }
 
     vector<Token_t> humans = tokens;
     humans.erase(humans.begin());
 
     if (humans.empty()) {
         return pickRandom(tokens, move);
     }
 
     int avgRow = 0;
     int avgCol = 0;
 
     for (const Token_t& human : humans) {
         avgRow += human.location.row;
         avgCol += human.location.col;
     }
 
     avgRow = avgRow / humans.size();
     avgCol = avgCol / humans.size();
 
     Point_t centerPoint = {avgRow, avgCol};
     Point_t destination = tigerToken.location;
 
     if (centerPoint.col == tigerToken.location.col) {
         if (centerPoint.row < tigerToken.location.row) {
             destination.row--;
         } else {
             destination.row++;
         }
     } else if (centerPoint.row == tigerToken.location.row) {
         if (centerPoint.col < tigerToken.location.col) {
             destination.col--;
         } else {
             destination.col++;
         }
     } else {
         int rowDiff = centerPoint.row - tigerToken.location.row;
         int colDiff = centerPoint.col - tigerToken.location.col;
 
         if (onDiag(tigerToken)) {
             Point_t potentialDiag = tigerToken.location;
             potentialDiag.row += (rowDiff > 0) ? 1 : -1;
             potentialDiag.col += (colDiff > 0) ? 1 : -1;
 
             Token_t tempToken;
             tempToken.location = potentialDiag;
 
             if (onDiag(tempToken)) {
                 destination = potentialDiag;
             } else {
                 if (abs(rowDiff) > abs(colDiff)) {
                     destination.row += (rowDiff > 0) ? 1 : -1;
                 } else {
                     destination.col += (colDiff > 0) ? 1 : -1;
                 }
             }
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
         vector<Point_t> possibleMoves;
 
         Point_t vertMove = tigerToken.location;
         vertMove.row += (centerPoint.row > tigerToken.location.row) ? 1 : -1;
         if (inBounds(vertMove) && !isOccupied(tokens, vertMove)) {
             possibleMoves.push_back(vertMove);
         }
 
         Point_t horzMove = tigerToken.location;
         horzMove.col += (centerPoint.col > tigerToken.location.col) ? 1 : -1;
         if (inBounds(horzMove) && !isOccupied(tokens, horzMove)) {
             possibleMoves.push_back(horzMove);
         }
 
         if (onDiag(tigerToken)) {
             for (const Point_t& diagPoint : DIAGONAL_COORDINATES) {
                 if (abs(diagPoint.row - tigerToken.location.row) == 1 &&
                     abs(diagPoint.col - tigerToken.location.col) == 1) {
 
                     if (inBounds(diagPoint) && !isOccupied(tokens, diagPoint)) {
                         possibleMoves.push_back(diagPoint);
                     }
                 }
             }
         }
 
         if (!possibleMoves.empty()) {
             Point_t bestMove = possibleMoves[0];
             double bestDist = dist(bestMove, centerPoint);
 
             for (const Point_t& p : possibleMoves) {
                 double currentDist = dist(p, centerPoint);
                 if (currentDist < bestDist) {
                     bestDist = currentDist;
                     bestMove = p;
                 }
             }
 
             destination = bestMove;
         } else {
             return pickRandom(tokens, move);
         }
     }
 
     move.destination = destination;
 
     if (!inBounds(move.destination) || isOccupied(tokens, move.destination) ||
         dist(tigerToken.location, move.destination) > sqrt(2) + 0.0001) {
         return pickRandom(tokens, move);
     }
 
     move = checkCageSpots(move, tokens);
 
     return move;
 }
 
 Move_t checkCageSpots(Move_t move, vector<Token_t> tokens) {
     // A quick fix for some known bad moves near where the normal field
     // And cage intersect.
     Token_t tigerToken = tokens[0];
     Point_t tempPoint = {3,5};
     Point_t tempPoint2 = {5,5};
     Point_t tempPoint3 = {3,3};
     Point_t tempPoint4 = {5,3};
     Point_t tempPoint5 = {4,3};
     Point_t tempPoint6 = {4,5};
     if ((move.destination == tempPoint && tigerToken.location == tempPoint2) ||
         (move.destination == tempPoint2 && tigerToken.location == tempPoint)) {
         move = pickRandom(tokens, move);
         move = checkCageSpots(move, tokens);
         }

     if ((move.destination == tempPoint3 && tigerToken.location == tempPoint4) ||
         (move.destination == tempPoint4 && tigerToken.location == tempPoint3)) {
         move = pickRandom(tokens, move);
         move = checkCageSpots(move, tokens);
         }

     if ((move.destination == tempPoint3 && tigerToken.location == tempPoint5) ||
         (move.destination == tempPoint5 && tigerToken.location == tempPoint3)) {
         move = pickRandom(tokens, move);
         move = checkCageSpots(move, tokens);
         }

     if ((move.destination == tempPoint && tigerToken.location == tempPoint6) ||
         (move.destination == tempPoint6 && tigerToken.location == tempPoint)) {
         move = pickRandom(tokens, move);
         move = checkCageSpots(move, tokens);
         }
 
     return move;
 }