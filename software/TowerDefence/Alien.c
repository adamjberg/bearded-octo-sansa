/*
 * Alien.c
 *
 *  Created on: 2013-01-25
 *		Author: Perry
 */
#include "Alien.h"

// Constructor
struct Alien* initAlien( int pos_x, int pos_y, int size, struct animation* anim, struct animation* dead,
						char* new_name, float speed,
						float max_life, int money_val, struct Collidable* col )
{
	struct Alien* a = (struct Alien*) malloc (sizeof(struct Alien));
	a->super = initObject(pos_x, pos_y, size, anim, (void*)a);
	addStats(a->super,dead);
	a->super->action = alienAction;
	a->super->collide = alienCollide;

	// Speeds
	/*
	a->x_speed = a->init_x_speed = x_speed_new;
	a->y_speed = a->init_y_speed = y_speed_new;
*/
	a->speed = a->init_speed = speed;
	// Life
	a->max_life = max_life;
	a->scale = size;

	resetAlien(a, pos_x, pos_y);

	// Name
	a->name = new_name;
	a->bounty = money_val;
	a->score = 1;

	a->super->class = alien;
	a->super->action = alienAction;

	a->col = col;
	return a;
}
void resetAlien(struct Alien* this, int x, int y) {

	setXY(this->super, x, y);
	this->speed = this->init_speed;
	this->x_speed = 0;
	this->y_speed = 0;
	this->current_life = this->max_life;
	this->super->scale = this->scale;
	// Active/inactive flag
	this->isHit = false;
	this->dead = false;
	this->frame = 0;
	this->path_found = false;
	this->path = 0;
	this->super->scale = this->scale;
	// towerBlocked is for determining if alien is colliding with Tower
	this->towerBlock = false;
	this->towerBlock_move_x = false;
	this->towerBlock_move_y = false;

	int i;
	for(i = 0; i < 4; i++ )
		this->borderBlocked[i] = false;  // border collisions : 0 = left, 1 = top, 2 = right, 3 = bottom

	this->team_collide = false; // colliding with teammates (other aliens)

	setStats(this->super, 0);
}

// Destructor

void killAlien(void* a){
	((struct Alien*)a)->dead = true;
	((struct Alien*)a)->speed = 0;
	((struct Alien*)a)->x_speed = 0;
	((struct Alien*)a)->y_speed = 0;
	((struct Alien*)a)->super->isShowing = 0;
	((struct Alien*)a)->super->scale = 10;
	setStats(((struct Alien*)a)->super, 1);
	((struct Alien*)a)->super->end_frame = ((struct Alien*)a)->super->curr_frame + 10;
	((struct Alien*)a)->super->isShowing = 1;
}

// destructor plus effects
/*void explodeAlien(struct Collidable* c, void* o){

		// Stop alien from moving
		((struct Alien*)((struct Object*)o)->subObject)->x_speed = 0;
		((struct Alien*)((struct Object*)o)->subObject)->y_speed = 0;

		removeFromCollide(c, (struct Object*)o);
		killAlien( (struct Alien*)(((struct Object*)o)->subObject) );
}*/


void alienAction(void* a) {

	// Stop alien from moving (prevents synchronization issue)
	if( ((struct Alien*)(((struct Object*)a)->subObject))->dead )
		return;

	//added 10 frames delay for alien to get damage from next bullet
	if(((struct Alien*)(((struct Object*)a)->subObject))->isHit) {
		((struct Alien*)(((struct Object*)a)->subObject))->frame++;
		if(((struct Alien*)(((struct Object*)a)->subObject))->frame == 5) {
			((struct Alien*)(((struct Object*)a)->subObject))->isHit = false;
			((struct Alien*)(((struct Object*)a)->subObject))->frame = 0;
		}
	}

	if( ((struct Alien*)(((struct Object*)a)->subObject))->path_found == false )
		alienPath( (struct Alien*)(((struct Object*)a)->subObject) ); // pick a path at random
	else
		followPath( (struct Alien*)(((struct Object*)a)->subObject) ); 	// Check predefined path



	int new_x;
	int new_y;



	// colliding with towers
	if( ((struct Alien*)(((struct Object*)a)->subObject))->towerBlock ) {
		// check if the collision with the tower results in a move in x
		if( ((struct Alien*)(((struct Object*)a)->subObject))->towerBlock_move_x == true ) {
		//	setXY((struct Object*)a, ((struct Object*)a)->prev_x, ((struct Object*)a)->prev_y  ); // step backwards
			setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 1, 0); // move in +x
		}
		// move in y
		if ( ((struct Alien*)(((struct Object*)a)->subObject))->towerBlock_move_y == true) {
		//	setXY((struct Object*)a, ((struct Object*)a)->prev_x, ((struct Object*)a)->prev_y  ); // step backwards
			setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 0, 1); // move in +y
		}

		((struct Alien*)(((struct Object*)a)->subObject))->towerBlock_move_x = false;
		((struct Alien*)(((struct Object*)a)->subObject))->towerBlock_move_y = false;
		((struct Alien*)(((struct Object*)a)->subObject))->towerBlock = false; // reset tower blocking

	}

	//TODO: Make sure following conditions do not apply when alien is killed but still in explosion effect

		// Check collisions with the left border
		if( (((struct Object*)a)->x ) <= 1 ) {
			setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 0, 1);
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[0] = true;

			// set all other border collision to false
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[1] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[2] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[3] = false;
		}
		else
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[0] = false;

		// Check collisions with the top border
		if( ((struct Object*)a)->y <= 5 ) {
			setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 1, 0);
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[1] = true;

			// set all other border collision to false
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[0] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[2] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[3] = false;

		}
		else
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[1] = false;

		// Check collisions with the bottom border
		if( ((struct Object*)a)->center_y +((struct Object*)a)->scale >= BOUND_Y-5) {
			setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 1, 0);
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[3] = true;

			// set all other border collision to false
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[1] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[2] = false;
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[0] = false;
		}
		else
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[3] = false;

		// Check collisions with the right border
		if( ((struct Object*)a)->center_x + ((struct Object*)a)->scale >= BOUND_X-10 ) {
				setAlienDirection(((struct Alien*)(((struct Object*)a)->subObject)), 0, 1);
				((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[2] = true;

				// set all other border collision to false
				((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[1] = false;
				((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[0] = false;
				((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[3] = false;
		}
		else
			((struct Alien*)(((struct Object*)a)->subObject))->borderBlocked[2] = false;



// check collisions with other aliens
/*
	if( ((struct Alien*)(((struct Object*)a)->subObject))->team_collide == true ) {
		int x_, y_;

		if((rand() %2) == 0) {
			x_ = 1;
			y_ = -1;
		}
		else {
			x_ = 1;
			y_ = 1;
		}
		setAlienDirection((struct Alien*)(((struct Object*)a)->subObject), x_, y_);
	}

	*/
	// Make the alien go forward
	new_x = ((struct Object*)a)->x
			+ ((struct Alien*)(((struct Object*)a)->subObject))->x_speed;

	new_y =  ((struct Object*)a)->y
			+ ((struct Alien*)(((struct Object*)a)->subObject))->y_speed;

	if( (new_x+5) > GOALX && (new_y+5) > GOALY -20 ) {
		info.invadedAlien++;
		if(info.invadedAlien > 50)
			info.isEnd = true;
		updateScoreFrame();
		removeFromCollide(((struct Alien*)(((struct Object*)a)->subObject))->col, (struct Object*)a);
		killAlien( (void*)(struct Alien*)(((struct Object*)a)->subObject));
	} else {
			// Apply it to the object
			setXY( 	((struct Object*)a) , new_x, new_y );
	}

	// toggle tower-collision boolean

		((struct Alien*)(((struct Object*)a)->subObject))->team_collide = false;
		((struct Alien*)(((struct Object*)a)->subObject))->towerBlock = false;
}


void alienCollide (void* this, void* obj, struct Collidable* c){

	int a_yMid = ((struct Object*)this)->center_x; // center
	int a_xMid = ((struct Object*)this)->center_y; // center
	int distx, disty;

if(((struct Object*)obj)->class == ship  ) {
	// midpoint of the tower
	int t_xMid = ((struct Object*)obj)->center_x;
	int t_yMid = ((struct Object*)obj)->center_y;

	//printf("collide\n");

	distx = t_xMid - a_xMid;
	disty = t_yMid - a_yMid;

	if(distx < disty){
		((struct Alien*)(((struct Object*)this)->subObject))->towerBlock_move_x = true; // move in x
		//((struct Alien*)(((struct Object*)this)->subObject))->towerBlock_move_y = false;
	}
	else {
		//((struct Alien*)(((struct Object*)this)->subObject))->towerBlock_move_x = false;
		((struct Alien*)(((struct Object*)this)->subObject))->towerBlock_move_y = true; // move in y
	}

	((struct Alien*)(((struct Object*)this)->subObject))->towerBlock = true;
}

	// Collide with bullets
	if(((struct Object*)obj)->class == bullet  && ((struct Alien*)(((struct Object*)this)->subObject))->dead == false
			&& ((struct Alien*)(((struct Object*)this)->subObject))->isHit == false) {
		// decrease the alien's life
		// TODO: Added time delay or response time for alien to get damage;
		//preventing alien get damage twice or more from the same bullet
		((struct Alien*)(((struct Object*)this)->subObject))->isHit = true;
		((struct Alien*)(((struct Object*)this)->subObject))->current_life -= ((struct Bullet*)((struct Object*)obj)->subObject)->damage;
		if(((struct Alien*)(((struct Object*)this)->subObject))->current_life <= 0) {
			updateScoreMoney(((struct Alien*)(((struct Object*)this)->subObject))->score,((struct Alien*)(((struct Object*)this)->subObject))->bounty);
			removeFromCollide(c, (struct Object*)this);
			killAlien((void*)((struct Alien*)(((struct Object*)this)->subObject)));
		}
	}

	/*
	// Collide with other aliens
	if(((struct Object*)obj)->class == alien ) {
		((struct Alien*)(((struct Object*)this)->subObject))->team_collide = true;
	}
	*/

}

void alienPath(struct Alien* a){
// Simple predefined paths
	int newpath = ( (rand() %5) )+ 1; // 5 different paths
	a->path_found = true;
	a->path = newpath;
//	printf("path %d\n", newpath);
}

void setAlienDirection(struct Alien* this, int x_dir, int y_dir) {
	this->x_speed = x_dir*this->speed;
	this->y_speed = y_dir*this->speed;
}

void followPath(struct Alien* a) {

	int xdir, ydir;

	if(	a->path == 1) {
		// Force alien to go diagonally
		setAlienDirection(a, 1, 1);

	}

	else if( a->path == 2) {
		// Force alien to follow top border then right border
		setAlienDirection(a, 1, 0);
	}

	else if( a->path == 3) {
		// Force alien to follow left border then bottom border
		setAlienDirection(a, 0, 1);
	}

	else if(a->path == 4) {
		// go right 120, down 120, right 120, down 120

		if(a->super->x < 120) {
			xdir = 1;
			ydir = 0;
		}
		if(a->super->x >= 120 ){
			xdir = 0;
			ydir = 1;
		}

		setAlienDirection(a, xdir, ydir);

	}

	else if(a->path == 5) {
		// go down 120, right 120, down 120, right 120

		if(a->super->x < 120) {
			xdir = 0;
			ydir = 1;
		}
		if(a->super->x >= 120 ){
			xdir = 1;
			ydir = 0;
		}

		setAlienDirection(a, xdir, ydir);
	}
}
