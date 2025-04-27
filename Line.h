/*
* Line.h
 *
 *  Created on: Dec 13, 2017
 *      Author: booth
 */

#ifndef LINE_H_
#define LINE_H_

#include <cmath>
using namespace std;

#include "SDL_Plotter.h"
#include "Point.h"
#include "Color.h"

class Line{

private:
    Point p1, p2;
    Color color;
    int width;


public:
    Line(Point p1 = Point(0,0), Point p2 = Point(1,1), int width = 1, Color c = BLACK){
        this->p1 = p1;
        this->p2 = p2;
        this->width = width;
        color = c;
    }

    Point getP1(){return p1;}
    Point getP2(){return p2;}

    void setP1(Point p){
        p1 = p;
    }
    void setP2(Point p){
        p2 = p;
    }

    bool slope(double& m){
        bool hasSlope = p1.x != p2.x;

        if(hasSlope){
            m = 0;
            if(fabs(p1.y - p2.y) > 0.001){
                m = static_cast<double>(p1.y - p2.y)/(p1.x - p2.x);
            }
        }
        return hasSlope;
    }

    bool y_intercept(double& b){
        double m;
        bool hasIntercept = slope(m);

        if(hasIntercept){
            b = p1.y - m * p1.x;
        }

        return hasIntercept;
    }

    void draw(SDL_Plotter& g){
        double m, b;
        int y;

        if(slope(m)){
            y_intercept(b);
            for(int w = -width/2; w <= width/2; w++){
                for(int x = min(p1.x, p2.x); x <= max(p1.x, p2.x); x++){
                    y = m * x + b + w;
                    g.plotPixel(x, y, color.R, color.G, color.B);
                }
            }
        }
        else{
            for(int w = -width/2; w <= width/2; w++){
                for(int y = min(p1.y, p2.y); y <= max(p1.y, p2.y); y++){
                    g.plotPixel(p1.x+w, y, color.R, color.G, color.B);
                }
            }

        }
    }
};


#endif /* LINE_H_ */