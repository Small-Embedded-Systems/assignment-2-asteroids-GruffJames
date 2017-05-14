/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to usd them */
struct ship {
    vector_t  vec;
	float heading,headingChange;//nose direction
	point v;
	point pointA;//left
	point pointB;//right
	point pointC;//front	
	int fire;//true when firing
	float shields;
	float hitbox;
};

/* initial struts for building linked lists */

//the asteroids
typedef struct rock {
		point p;//the rocks coordinates
    point v;//the rocks velocity 
		float life,radius;//
    struct rock *next;
} node_rock;



void physics(void);

struct ship initShip(ship theShip);
//struct ship updateShip(ship theShip);

//the lasers
typedef struct laser {
	  point p;//the rocks coordinates
		point v;//the rocks velocity 
    float life,radius;
    struct laser *next;
} node_laser;

void initShip(void);
void initLasers(void);
void initRocks(void);

//node_laser *allocnodeLaser(void);
//node_rock *allocnodeRock(void);

//void freenodeLaser(node_laser *n);
//void freenodeRock(node_laser *n);

extern struct laser *activeLaser;
extern struct rock *activeRock;
