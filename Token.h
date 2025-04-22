/*
 * Token.h
 *
 *  Created on: Dec 13, 2017
 *      Author: booth
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <iostream>
using namespace std;

#include "Color.h"
#include "Point.h"
#include "BearGame.h"
#include "SDL_Plotter.h"

const int TOKEN_SIZE = 20;

class Token{
    private:
	    Point location;
	    Point oldLocation;
	    Color color;
	    bool  attached, moving;
	    bool  active;

    public:

	    Token(Point p = Point(), Color c = BLACK){
	    		location = p;
	    		oldLocation = location;
	    		color = c;
	    		attached = false;
	    		moving = false;
	    		active = true;
	    }

	    void setActive(bool flag){
	    	    active = flag;
	    }

	    bool isActive(){
	    	    return active;
	    }

	    void revertLocation(){
	    	   location = oldLocation;
	    }

	    void setLocation(Point p ){
	    	    if(attached && !moving){
	    	    	    oldLocation = location;
	    	    	    moving = true;
	    	    }

	    	    if(p.y > 110){
	    	    	    location = p;
	    	    }
	    }

	    Point getLocation(){
	    	    return location;
	    }

	    Point getOldLocation(){
	    	    return oldLocation;
	    }

	   void setColor(Color c){
		   color = c;
	   }

	   void draw(SDL_Plotter& g){
		   if(active){
			   for(int x = -TOKEN_SIZE; x < TOKEN_SIZE; x++){
				   for(int y = -TOKEN_SIZE; y < TOKEN_SIZE; y++){
					   if(sqrt(x*x + y*y) < TOKEN_SIZE){
						   g.plotPixel(location.x + x, location.y + y, color.R, color.G, color.B);
					   }
				   }
			   }
		   }
		   else{
			   //erase(g);
		   }
	   }

	   void erase(SDL_Plotter& g){
		   Color c = color;
		   setColor(BACKGROUND);
		   draw(g);
		   setColor(c);
	   }

	   void setAttached(bool flag){
		   attached = flag;
		   if(!attached){
			   moving = false;
		   }
	   }

	   bool isAttached(){
		   return attached;
	   }

	   bool isOn(Point p){
		   return (location.distance(p) <= TOKEN_SIZE);
	   }

	   void dumpState(){
		   if(color.R == 220)
			   cout << "RED:  ";
		   else
			   cout << "BLUE: ";
		   cout << "(" << (oldLocation.y-2*60)/60 << ", " << (oldLocation.x-2*60)/60 << ") ---> ";
		   cout << "(" << (location.y-2*60)/60 << ", " << (location.x-2*60)/60 << ")";
		   cout << endl;
	   }
};



#endif /* _TOKEN_H_ */