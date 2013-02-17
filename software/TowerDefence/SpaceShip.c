/*
 * SpaceShip.c
 *
 *  Created on: 2013-01-23
 *      Author: Diana
 */

#include "SpaceShip.h"

/*
 *  Constructor of SpaceShip object
 */
struct SpaceShip* initSpaceShip(int x, int y, int type, struct animation* a){
	struct SpaceShip* s =  (struct SpaceShip*)malloc(sizeof(struct SpaceShip));
	s->super = initObject(x, y, 20, a, (void*)s);
	s->super->class = ship;
	s->super->action = shipAction;
	s->super->collide = spaceShipCollide;
	s->ss_shoot = spaceShipShoot;
	s->level = 1;
	s->name = "dummyShip";
	s->type = type;
	setShipProperty(s, type);
	return s;
}

void setShipProperty(struct SpaceShip* this, int type) {
	switch(type) {
	case 0:
		this->price = 200;
		this->fire_rate = this->frame = 10;
		this->super->collideRange = 50;
		this->name = "Space Station";
		setWeapon(weapon1_lvl1, this);
		break;
	case 1:
		this->price = 2000;
		this->fire_rate = this->frame = 150;
		this->super->collideRange = 15;
		this->name = "Transport Ship";
		setWeapon(weapon2_lvl1, this);
		break;
	case 2:
		this->price = 1000;
		this->fire_rate = this->frame = 15;
		this->super->collideRange = 60;
		this->name = "Satellite";
		setWeapon(weapon3_lvl1, this);
		break;
	case 3:
		this->price = 3000;
		this->fire_rate = this->frame = 80;
		this->super->collideRange = 20;
		this->name = "Lethal Ship";
		setWeapon(weapon5_lvl1, this);
		break;
	case 4:
		this->price = 3500;
		this->fire_rate = this->frame = 80;
		this->super->collideRange = 20;
		this->name = "Bomber";
		setWeapon(weapon4_lvl1, this);
		break;
	default:
		break;
	}
}
/*
 *  Destructor of SpaceShip object -- when upgrading, etc
 */
void killSpaceShip(void* s){
	((struct SpaceShip*)s)->super->end_frame = ((struct SpaceShip*)s)->super->curr_frame + 5;
}

void shipAction(void* s) {
	((struct SpaceShip*)((struct Object*)s)->subObject)->frame++;
}
void setWeapon(struct BulletCollection* b, struct SpaceShip* self){
	self->weapon = b;
	self->damage = self->weapon->bullets[0]->damage;
}

void spaceShipCollide(void* self, void* obj, struct Collidable* col){
	//lock = 1;
	if (((struct Object*)obj)->class == 2){
		spaceShipShoot(((struct Object*)obj)->center_x, ((struct Object*)obj)->center_y, ((struct Alien*)((struct Object*)obj)->subObject)->x_speed, ((struct Alien*)((struct Object*)obj)->subObject)->y_speed, ((struct SpaceShip*)(((struct Object*)self)->subObject)));
	}
	//lock = 0;
}

void spaceShipShoot(int en_x, int en_y, float v_x, float v_y, struct SpaceShip* self){
	// number of frame == fire rate
	if (self->frame >= self->fire_rate){
		self->weapon->bc_fire(en_x, en_y, self->weapon, self);
		self->frame = 0;

	}
}

void upgrade(struct SpaceShip* this) {
	if(this->level == 1) {
		if(info.currency < this->price*3) {
			writeMSG("You do not have enough money to upgrade!");
			return;
		}
		playSoundById(audioController, TOWER_UPGRADE_SOUND, 0 );
		writeMSG("A ship is successfully upgraded!!");
		this->level = 2;
		info.currency -= this->price*3;
		updateScoreFrame();
	} else
		return;
	switch(this->type) {
		case 0:
			this->fire_rate = this->frame = 5;
			this->super->collideRange = 55;
			setWeapon(weapon1_lvl2, this);
			break;
		case 1:
			this->fire_rate = this->frame = 100;
			this->super->collideRange = 20;
			setWeapon(weapon2_lvl2, this);
			break;
		case 2:
			this->fire_rate = this->frame = 10;
			this->super->collideRange = 60;
			setWeapon(weapon3_lvl2, this);
			break;
		case 3:
			this->fire_rate = this->frame = 60;
			this->super->collideRange = 30;
			setWeapon(weapon5_lvl2, this);
			break;
		case 4:
			this->fire_rate = this->frame = 60;
			this->super->collideRange = 35;
			setWeapon(weapon4_lvl2, this);
			break;
		default:
			break;
		}
}


