/* Game state */

extern int elapsedTime; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */
extern int  paused;        /* lives remaining */
extern int  inPlay;        /* lives remaining */
extern bool  alive;        /* lives remaining */


extern struct ship player;

extern struct rock *asteroids; /* array of rocks / pointer to linked-list */
extern struct laser *shots;  /* array of missiles / pointer to linked-list */

extern const float Dt; /* Time step for physics, needed for consistent motion */
