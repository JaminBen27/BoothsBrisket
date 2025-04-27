/*
* Color.h
 *
 *  Created on: Dec 13, 2017
 *      Author: booth
 */

#ifndef COLOR_H_
#define COLOR_H_


struct Color{
    int R, G, B;

    Color(int r = 0, int g = 0, int b = 0){
        R = r;
        G = g;
        B = b; //black
    }

    void setColor(int r, int g, int b){
        R = r;
        G = g;
        B = b;
    }

    void setColor(Color c){
        R = c.R;
        G = c.G;
        B = c.B;
    }

    bool operator==(Color c){
        return R == c.R && G == c.G && B == c.B;
    }
};


const Color BACKGROUND(255,255,255);
const Color BLACK(0,0,0);
const Color red(220,20,20);
const Color GREEN(20,220,20);
const Color blue(20,20,220);

#endif /* COLOR_H_ */