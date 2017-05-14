/* Asteroids model */
#include <mbed.h>//delete

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <MMA7455.h>//delete

#include "model.h"
#include "utils.h"
#include "asteroids.h"

//the origin of the ship
static float centX,centY;

int started=0;//whether the game has started
//


  

//the coordinates of the screen
static int topSide=10;
static int leftSide=-10;
static int rightSide=480;
static int bottomSide=300;


//These provide the starting coordinates for the ship
float startX=150;float startY=200;

/**
	@brief this function initialises the ship when it spawns
*/
void initShip(void){
	player.pointA.x=startX; player.pointA.y=startY;
	player.pointB.x=startX; player.pointB.y=startY+16;
	player.pointC.x=startX+20; player.pointC.y=startY+8;
	player.v.x=0;player.v.y=0;
	player.heading=0;
	player.shields=5;
	player.hitbox=16;
}

/**
	@brief this wraps each side of the screen around so that if the ship goes off one side it reapears on the other
	@param ship, theShip the ship to be manipulated
  @return ship, theShip the ship that was manipulated
*/
struct ship shipWrap(ship theShip){
		if(theShip.pointA.x>rightSide||theShip.pointB.x>rightSide||theShip.pointC.x>rightSide){
			theShip.pointA.x-=470;
			theShip.pointB.x-=470;
			theShip.pointC.x-=470;
		}else if(theShip.pointA.x<leftSide||theShip.pointB.x<leftSide||theShip.pointC.x<leftSide){
			theShip.pointA.x+=470;
			theShip.pointB.x+=470;
			theShip.pointC.x+=470;
		}else if(theShip.pointA.y>bottomSide||theShip.pointB.y>bottomSide||theShip.pointC.y>bottomSide){
			theShip.pointA.y-=290;
			theShip.pointB.y-=290;
			theShip.pointC.y-=290;
		}else if(theShip.pointA.y<topSide||theShip.pointB.y<topSide||theShip.pointC.y<topSide){
			theShip.pointA.y+=290;
			theShip.pointB.y+=290;
			theShip.pointC.y+=290;
		}
		return theShip;
}
static float c;
static float s;
static float newX1,newX2,newX3,newY1,newY2,newY3;
/**
	@brief this function updates the position and heading of the ship based on its velocity and heading change
	@param ship, theShip the ship to be manipulated
  @return ship, theShip the ship that was manipulated
*/
struct ship shipRotateMove(ship theShip){
	theShip.heading+=theShip.headingChange;
	c=cos(theShip.headingChange);
	s=sin(theShip.headingChange);
	centX=(theShip.pointA.x+theShip.pointB.x+theShip.pointC.x)/3;
	centY=(theShip.pointA.y+theShip.pointB.y+theShip.pointC.y)/3;
	newX1=c*(theShip.pointA.x-centX)-s*(theShip.pointA.y-centY)+centX;
	newY1=s*(theShip.pointA.x-centX)+c*(theShip.pointA.y-centY)+centY;
	newX2=c*(theShip.pointB.x-centX)-s*(theShip.pointB.y-centY)+centX;
	newY2=s*(theShip.pointB.x-centX)+c*(theShip.pointB.y-centY)+centY;
	newX3=c*(theShip.pointC.x-centX)-s*(theShip.pointC.y-centY)+centX;
	newY3=s*(theShip.pointC.x-centX)+c*(theShip.pointC.y-centY)+centY;

	theShip.pointA.x=newX1+theShip.v.x;
	theShip.pointA.y=newY1+theShip.v.y;
	theShip.pointB.x=newX2+theShip.v.x;
	theShip.pointB.y=newY2+theShip.v.y;
	theShip.pointC.x=newX3+theShip.v.x;
	theShip.pointC.y=newY3+theShip.v.y;
	return theShip;
}


/**
	@brief This function contains all of the functions which manipulate the ship
	@param ship theShip, the ship to be manipulated
  @return ship theShip, the ship that was manipulated
*/
struct ship updateShip(ship theShip){
	theShip=shipWrap(theShip);
	theShip=shipRotateMove(theShip);
		return theShip;
}
struct laser *activeLaser = NULL;
struct rock *activeRock = NULL;

static const int heapSizeLaser = 500;
static const int heapSizeRock = 20;

static node_laser heapLaser[heapSizeLaser];
static node_rock heapRock[heapSizeLaser];

static node_laser *freeNodesLaser;
static node_rock *freeNodesRock;
/**
	@brief this function initialises the lasers linkedList
*/
void initLasers(void)
{
    int n;
    for( n=0 ; n<(heapSizeLaser-1) ; n++) {
        heapLaser[n].next = &heapLaser[n+1];
    }
    heapLaser[n].next = NULL;
    freeNodesLaser = &heapLaser[0];
}
/**
	@brief this function initialises the rocks linkedList
*/
void initRocks(void)
{
    int n;
    for( n=0 ; n<(heapSizeRock-1) ; n++) {
        heapRock[n].next = &heapRock[n+1];
    }
    heapRock[n].next = NULL;
    freeNodesRock = &heapRock[0];
}
/**
	@brief this function returns a free node in the laser linkedList
	@return node_laser node, this is the free node 
*/
node_laser *allocnodeLaser(void)
{
    node_laser *node = NULL;
    if( freeNodesLaser ) {
        node = freeNodesLaser;
        freeNodesLaser = freeNodesLaser->next;
    }
    return node;
}
/**
	@brief this function returns a free node in the rocks linkedList
	@return node_laser node, this is the free node 
*/
node_rock *allocNodeRock(void)
{
    node_rock *node = NULL;
    if( freeNodesRock ) {
        node = freeNodesRock;
        freeNodesRock = freeNodesRock->next;
    }
    return node;
}
/**
	@brief this function removes rocks from the rocks linkedlist
*/
void freeNodeRock(node_rock *n)
{
    n->next = freeNodesRock;
    freeNodesRock = n;
}
/**
	@brief this function initialises the lasers when they spawns
*/
void createLaser(struct laser *r)
{
    r->p.x = player.pointC.x;
    r->p.y = player.pointC.y;
    r->v.x =player.v.x+(50*cos(player.heading));
    r->v.y =player.v.y+(50*sin(player.heading));//50 is the speed
    r->life  =5; 
		r->radius =5;
}
/**
	@brief this function initialises the rocks when they spawns
*/
void createRock(struct rock *r)
{

    r->v.x =randrange(-30,30);
    r->v.y =randrange(-30,30);
    r->life = randrange(50,100);//delete??????? 
		r->radius = randrange(15,25);
	  r->p.x = randrange(r->radius+5,469-r->radius);
    r->p.y = randrange(r->radius+5,250-r->radius);
	
}

/**
	@brief this function removes lasers from the linkedList if there range is <0
	@param struct laser *l is the laser being tested
	@return l is the next laser in the linkedList if the one tested's range is <0 otherwise it is the same laser entered at the start
*/
	struct laser *pruneLaser(struct laser *l){
	if(l){
		if( l->life<=0 ) {
			l->p.x=-10;
			struct laser *expiredLaser = l;
			l = l->next;
			expiredLaser->next=NULL;
		}
	}
	return l;
}
/**
	@brief this function updates the lasers position.
	@param struct laser *l, is a pointer to the laser linkedlist being updated
	@param struct rock *r, is a pointer to the rocks linkedlist being compared
*/
void updateLaser(struct laser *l,struct rock *r)
{
	static float equa,xMinusX,yMinusY,combRdSq;
    for( ; l ; l = l->next ) {
        l->p.x += l->v.x * Dt; 
        l->p.y += l->v.y * Dt;

			//checks for collisions
				for( ; r ; r = r->next ) {
					xMinusX=r->p.x-l->p.x;
					yMinusY=r->p.y-l->p.y;			
					combRdSq=(r->radius+l->radius)*(r->radius+l->radius);
					equa=((xMinusX*xMinusX)+(yMinusY*yMinusY));
					if(equa <=combRdSq){
						l->life = 0;
						r->life = 0;
					}else{
					}
				}			

        //sides
					if(l->p.x>rightSide+l->radius){
						l->p.x-=490+l->radius;
					}else if(l->p.x<leftSide-l->radius){
						l->p.x+=490+l->radius;
					}else if(l->p.y>bottomSide+l->radius){
						l->p.y-=310+l->radius;
					}else if(l->p.y<topSide-l->radius){
						l->p.y+=310+l->radius;
					}	
					l->life -= Dt;
					pruneLaser(l);
					}		
    }
/**
	@brief this function updates the lasers position.
	@param struct rock *r, is a pointer to the rock being updated
*/
void updateRock(struct rock *r){
		static float equa,xMinusX,yMinusY,combRdSq1,combRdSq2;
    for( ; r ; r = r->next ) {

        r->p.x += r->v.x * Dt; 
        r->p.y += r->v.y * Dt;

				//checks for collisions between the ship and rocks
				centX=(player.pointA.x+player.pointB.x+player.pointC.x)/3;
				centY=(player.pointA.y+player.pointB.y+player.pointC.y)/3;
				xMinusX=r->p.x-centX;
				yMinusY=r->p.y-centY;				
				combRdSq1=(r->radius+player.shields)*(r->radius+player.shields);
				combRdSq2=(r->radius+player.hitbox)*(r->radius+player.hitbox);			
				equa=((xMinusX*xMinusX)+(yMinusY*yMinusY));
				if(equa <=combRdSq1){
					if(equa <=combRdSq2){
						initShip();
						score=0;
						lives--;
					}
					r->life = 0;
					player.shields-=10;
					if(player.shields<0){
						player.shields=0;
					}
				}
				//wraps the rocks round the screen
					if(r->p.x>rightSide+r->radius){
						r->p.x-=490+r->radius;
					}else if(r->p.x<leftSide-r->radius){
						r->p.x+=490+r->radius;
					}else if(r->p.y>bottomSide+r->radius){
						r->p.y-=310+r->radius;
					}else if(r->p.y<topSide-r->radius){
						r->p.y+=310+r->radius;
					}		
				
					r->life -= Dt;					
        if( r->next->life<=0 ) {
            struct rock *expiredRock = r->next;
            r->next = r->next->next;
            freeNodeRock(expiredRock);
			}
		}
	}

/**
	@brief  This function creates and updates the lasers and rocks
*/
void runRocksLasers(void){
				//creates rocks
				struct rock *sparkRock = allocNodeRock();
        if(sparkRock) {
							sparkRock->next = activeRock;
							activeRock = sparkRock;
							createRock(sparkRock);						
        }
				//updates the rocks
        updateRock(activeRock);//a
				//creates lasers if fired
				if(player.fire){
					struct laser *sparkLaser = allocnodeLaser();
        if(sparkLaser) {
								sparkLaser->next = activeLaser;
								activeLaser = sparkLaser;
								createLaser(sparkLaser);
        }
			}
			player.fire=0;
			//updates the lasers
			updateLaser(activeLaser,activeRock);
}



/**
	@brief  This function is attacted to a ticker to be run each cycle. It contains all of the functions that update the positions of the ship, lasers and rocks.
*/
void physics(void){
		if(!paused){
					player=updateShip(player);
					runRocksLasers();
					if(player.shields<22){//regenerates the shields
						player.shields+=0.08;
					}
	}
}




