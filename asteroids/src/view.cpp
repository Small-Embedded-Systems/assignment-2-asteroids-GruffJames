/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

/*
	Here is the const colors used in the program
*/

const colour_t background = rgb(0,51,102); /* Midnight Blue */
const colour_t col1 = rgb(3,3,255); /* Midnight Blue */
const colour_t col2back = rgb(65,0,117); /* Midnight Blue */
const colour_t col3 = rgb(3,251,255); /* Midnight Blue */

/*
	Here is the BitMap used for the laser
*/
#define laserlaser_width 10
#define laserlaser_height 10
static unsigned char laserlaser_bits[] = {
   0x48, 0x00, 0xb6, 0x01, 0x4a, 0x01, 0xb5, 0x02, 0x7a, 0x01, 0x7a, 0x01,
   0xb5, 0x02, 0x4a, 0x01, 0xb6, 0x01, 0x48, 0x00 };	 

uint8_t flippedLaser[sizeof(laserlaser_bits)];
 /**
	@brief  This funciton reverses each byte in the laser bitmap, so that it can be shown properly
 */
void initBitFlip(void){
	for( int i=0; i< sizeof(laserlaser_bits); i++) {
				flippedLaser[i]=laserlaser_bits[i];
				flippedLaser[i] = (flippedLaser[i] & 0xF0) >> 4 | (flippedLaser[i] & 0x0F) << 4;
				flippedLaser[i] = (flippedLaser[i] & 0xCC) >> 2 | (flippedLaser[i] & 0x33) << 2;
				flippedLaser[i] = (flippedLaser[i] & 0xAA) >> 1 | (flippedLaser[i] & 0x55) << 1;

	}
}
/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}
//swaps the buffer
void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}
/**
	@brief  This function draws the ship and its shields to the screen buffer
	@param  ship theShip is the ship being drawn
*/
static float centX;
static float centY;
void drawShipNew(ship theShip){
				graphics->drawTriangle(theShip.pointA.x,theShip.pointA.y,
													   theShip.pointB.x,theShip.pointB.y,
												  	 theShip.pointC.x,theShip.pointC.y,
												   	 BLUE);	
				//here the ships origin is calculated 
				centX=(player.pointA.x+player.pointB.x+player.pointC.x)/3;
				centY=(player.pointA.y+player.pointB.y+player.pointC.y)/3;
			
				//here the ships shields are drawn from the origin
				graphics->drawCircle(centX,centY, player.shields, col3);
	
				//graphics->drawCircle(centX,centY, player.hitbox, RED);//this is the hitbox for the ship
}

/*
	This function draws the lasers to the screen buffer
*/
/**
	@brief  This function draws the lasers to the screen bufferr
	@param  struct laser *l is a pointer to the laser linkedlist
*/
void drawparticlesLaser(struct laser *l)
{

    while(l) {
				graphics->drawBitmap(l->p.x-l->radius,l->p.y-l->radius,flippedLaser,laserlaser_width,laserlaser_height,col3);
        l = l->next;					
    }
}
/**
	@brief  This function draws the rocks to the screen bufferr
	@param  struct laser *r is a pointer to the rocks linkedlist
*/
void drawparticlesRock(struct rock *r)
{
    while(r) {
        graphics->fillCircle(r->p.x,r->p.y, r->radius, col1);
        r = r->next;			
    }
}
/**
	@brief  This function draws the scorebar at the top of the screen
*/
void drawScoreBar(){
	graphics->fillRect(0,0,500,20,col3);
	graphics->setCursor(5,7);
	graphics->setTextColor(BLACK);
	graphics->printf("Ship Score :%5d Total Score :%5d Lives left :%2d Shields strength :%2f ",score,elapsedTime,lives,player.shields);

}
/**
	@brief  This draws the games start, pause and game over menus
*/
void drawMenu(){
				graphics->setCursor(110,14);
				graphics->setTextColor(col3);
				graphics->setTextSize(5);
				graphics->printf("ASTEROIDS");
				if(inPlay){
					graphics->setTextSize(2);
					graphics->setCursor(45,180);
					graphics->printf("Score : %5d Total Score :%5d\n Lives Left :%2d",score,elapsedTime,lives);
					graphics->setCursor(70,230);
					graphics->printf("Press select to resume");
					graphics->setTextSize(1);
				}else if(lives<0){
					graphics->setTextSize(3);
					graphics->setCursor(125,100);
					graphics->printf("GAME OVER");					
					graphics->setTextSize(2);
					graphics->setCursor(25,180);
					graphics->printf("Total Score :%5d",elapsedTime);
					graphics->setCursor(70,220);
					graphics->printf("Press select to start again");
					graphics->setTextSize(1);
				}else{
					graphics->setTextSize(2);
					graphics->setCursor(125,200);
					graphics->printf("Press select to play");
					graphics->setTextSize(1);
		}
}
/**
	@brief  This function is attacted to a ticker to be run each cycle. It contains all of the functions that draw to the screen.
*/
void draw(void)
{
	graphics->fillScreen(col2back);
	if(!paused){
			drawShipNew(player);
			drawparticlesLaser(activeLaser);
			drawparticlesRock(activeRock);
			drawScoreBar();
	}else{
			drawMenu();
	}
	swap_DBuffer();
}


