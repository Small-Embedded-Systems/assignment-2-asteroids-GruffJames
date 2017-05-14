/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "utils.h"

/* Joystick 5-way switch
*/
enum position { left=0,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
//pause button
DigitalIn pauseButton(P2_10,PullUp);

//vars used for accelerating the ship
static float aX,aY;
static float addVel=1;
//the ships speed limit
static float speedLimit=2;

void speedLimiter(void);

/**
	@brief This function is used to operate the joystick. If the button is held down it is true whether or not it is released
	@return bool true when pressed false when not
*/
bool joyStickMeth(position p) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};
        //initially all 1s: nothing pressed
	state = joystick[p].read();
  if ((savedState[p] == 0 )){// &&(state == 1)) ) {
		result = true;
	}
	savedState[p] = state;
	return result;
}
/**
	@brief This function is used to operate the pause button. If the button is held down it must be released before true can be returned
	@return bool true when pressed false when not
*/
bool pauseMeth(){
	bool result = false;
	uint32_t state;
	static uint32_t savedState = 1;
        //initially all 1s: nothing pressed
	state = pauseButton.read();
  if ((savedState == 0 &&(state == 1)) ) {
		result = true;
	}
	savedState = state;
	return result;
}

/**
	@brief This function operates the controls. It can rotate and accelerate the ship. it can also pause the game
*/
void controls(void){
		if(!paused){
					if(joyStickMeth(centre)){
						player.fire=1;
						player.v.x=0;
						player.v.y=0;
					}else	if(joyStickMeth(left)){;//rotate clockwise
						player.headingChange=radians(1.5);
					}else	if(joyStickMeth(right)){//rotate anti-clockwise
						player.headingChange=-radians(1.5);
					}else if(joyStickMeth(up)){//accelerate forwards 
						aX=addVel*cos(player.heading);
						aY=addVel*sin(player.heading);						
						player.v.x=player.v.x+aX;
						player.v.y=player.v.y+aY;					
					}else if(joyStickMeth(down)){//accelerate backwards 
						aX=addVel*cos(player.heading);
						aY=addVel*sin(player.heading);						
						player.v.x=player.v.x-aX;
						player.v.y=player.v.y-aY;
					}else if(pauseMeth()){//pauses the game
						paused=1;						
					}else{
						player.headingChange=0;//stops the ship spinning indefinitely 
					}
						speedLimiter();//limit the speed

	}else{
		if(pauseMeth()){//unpauses the game
			paused=0;
		}
	}
}
/**
	@brief This function set a speed limit to the velocity of the ship 
*/
void speedLimiter(void){
					//x velocity
					if(player.v.x>=speedLimit){
						player.v.x=speedLimit;
					}else if(player.v.x<=-speedLimit){
						player.v.x=-speedLimit;
					}
					//y velocity
					if(player.v.y>=speedLimit){
						player.v.y=speedLimit;
					}else if(player.v.y<=-speedLimit){
						player.v.y=-speedLimit;
					}
}
