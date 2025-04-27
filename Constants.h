/*
* constants.h
 *
 *  Created on: Apr 20, 2018
 *      Author: booth
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const bool TESTING = false;
const bool TESTING1 = true;
const bool TESTING2 = false;

const int TIGER = 0;
const int MIN_MEN = 1;
const int MAX_MEN = 18;
const int NUM_TOKENS = 19;  //token ZERO is  the Bear
const int H_LINES  = 8;
const int V_LINES = 8;
const int LINE_WIDTH = 3;
const int LINE_SPACING = 60;
const int TOP_PADDING = 2;
const int BOTTOM_PADDING = 5;//2
const int SIDE_PADDING = 2;
const int SQUARE_WIDTH = LINE_SPACING;
const int UPPER_DIAMOND = 4;
const int X_OFFSET = LINE_SPACING * SIDE_PADDING;
const int Y_OFFSET = LINE_SPACING * TOP_PADDING;

const int ROWS = LINE_SPACING * (TOP_PADDING + UPPER_DIAMOND + H_LINES + BOTTOM_PADDING);
const int COLUMNS = LINE_SPACING * (SIDE_PADDING + V_LINES + SIDE_PADDING);

const int GRID_ROW = 13;
const int GRID_COL = 9;

const int BAD_MOVE  =  0;
const int GOOD_MOVE =  1;
const int JUMP_MOVE = -1;

const int TOKEN_SIZE = 25;

const int DOT_SIZE = 15;

const int MAX_MOVES = 1000;

enum Color_t {RED, BLUE};

const int START_DOT_ROW = 946 - LINE_SPACING;
const int START_DOT_COL = 15;
const int DOT_SPACE = 30;

const int START_BAR_COL = 340;
const int MAX_SAME_MOVE = 10;

struct Point_t{
    int row, col;
};

bool operator==(const Point_t &lhs, const Point_t &rhs){
    return (rhs.row == lhs.row && rhs.col == lhs.col);
}

struct Token_t{
    Color_t color;
    Point_t location;
};

bool operator==(const Token_t &lhs, const Token_t &rhs){
    return (rhs.location == lhs.location && rhs.color == lhs.color);
}


struct Move_t{
    Token_t token;
    Point_t destination;
};

bool operator == (const Move_t& lhs, const Move_t& rhs){
    return (lhs.token == rhs.token) && (lhs.destination == rhs.destination);
}

#endif /* CONSTANTS_H_ */