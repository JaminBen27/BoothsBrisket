/*
* Point.h
 *
 *  Created on: Dec 13, 2017
 *      Author: booth
 */

#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <ostream>
using namespace std;

struct Point{
    int x,y;

    Point(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    }

    double distance(Point p){
        return sqrt(pow(x - p.x, 2.0) + pow(y - p.y, 2.0));
    }

    Point operator+ (Point& p1){
        Point p(x+p1.x, y+p1.y);
        return p;
    }

    bool operator== (const Point& p) const{
        return x == p.x && y == p.y;
    }

    bool operator < (const Point& p) const{
        return x < p.x || (x == p.x && y < p.y);
    }

    Point midPoint(Point p){
        return Point((x + p.x)/2,(y + p.y)/2);
    }

};


struct comparePoint{
    bool operator() (const Point& p1, const Point& p2) const{
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    }
};

ostream& operator<<(ostream& out, Point p){
    out << "(" << p.x << "," << p.y << ")";
    return out;
}

#endif /* POINT_H_ */