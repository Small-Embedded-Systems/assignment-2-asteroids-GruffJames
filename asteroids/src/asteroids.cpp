/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
int elapsedTime; 
int   score;
int   lives;
struct ship player;

float Dt = 0.015f;

Ticker model, view, controller,scoreTck;
void incrementScore(void);
void togglePaused(void);

int paused = 1;
int inPlay=0;
/* The single user button needs to have the PullUp resistor enabled */
//DigitalIn userbutton(P2_10,PullUp);
int main()
{

    init_DBuffer();
		initBitFlip();
		initShip();
		initLasers();
		initRocks();

    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
    scoreTck.attach( incrementScore, 1);
    
    lives = 5;
    paused = 1;
     //Pause to start 
	///**
    while( paused ){ // remember 1 is not pressed 
        //paused=1;
				//player.pointC.x+=20;
        wait_ms(100);
    }
//*/
	
    while(true) {
				while(lives>=0){
					inPlay=1;
				}	
				paused=1;
				inPlay=0;
				while(paused){
				
				}
				lives=5;
				score=0;
				elapsedTime=0;
    }
	}

void incrementScore(void){
	if(!paused){
		score++;
		elapsedTime++;
	}
}