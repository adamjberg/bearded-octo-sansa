#include "BulletCollection.h"
/*
 * Constructor of the BulletCollection object
 */
struct BulletCollection* initBulletCollection(struct Env* e, struct Collidable* col){
	struct BulletCollection* bc = (struct BulletCollection*)malloc(sizeof(struct BulletCollection));
	bc->env = e;
	bc->pStart = 0;
	bc->pEnd = 0;
	bc->collision_checker = col;
	bc->total_bullet_available = 0;
	return bc;
}

// shotGun
void initBulletToWeapon1(int level, struct animation* bulletImage, struct animation* explosion, struct BulletCollection* bc){
	int i;
	float dmg = 40;
	if (level == 1) {
		dmg = 20;
		bc->bc_fire = fire_tower1_level1;
	}else
		bc->bc_fire = fire_tower1_level2;
	for (i = 0; i < 25; i++){
		bc->bullets[i] = initBullet(100, 100, 1, dmg, bulletImage, bc);
		setupBullet(bc->bullets[i], explosion, bulletAction, bulletCollide);
	}
	bc->total_bullet_available = 25;
	bc->pEnd = 25;
}

// black hole
void initBulletToWeapon2(int level, struct animation* bulletImage, struct animation* explosion, struct BulletCollection* this){
	int i;
	for (i = 0; i < 25; i++){
		this->bullets[i] = initBullet(400, 400, 40, 0.0, bulletImage, this);
		setupBullet(this->bullets[i], explosion, blackHoleAction, blackHoleCollide);
	}
	this->total_bullet_available = 25;
	this->pEnd = 25;
	if (level == 1)
		this->bc_fire = fire_tower2_level1;
	else
		this->bc_fire = fire_tower2_level2;
}

// laser
void initBulletToWeapon3(int level, struct animation* bulletImage, struct animation* explosion, struct BulletCollection* this) {
	int i;
	float dmg = 40;
	if (level == 1)
		this->bc_fire = fire_tower3_level1;
	else {
		dmg = 80;
		this->bc_fire = fire_tower3_level2;
	}
	for(i = 0; i< 25; i++) {
		this->bullets[i] = initBullet(400, 400, 1, dmg, bulletImage, this);
		setupBullet(this->bullets[i], explosion, laserAction, laserCollide);
		this->bullets[i]->super->isShowing = 0;
	}
	this->total_bullet_available = 25;
	this->pEnd = 25;
}

// fire bomb
void initBulletToWeapon4(int level, struct animation* bulletImage, struct animation* explosion, struct BulletCollection* this) {
	int i;
	float dmg = 100;
	if (level == 1)
		this->bc_fire = fire_tower4_level1;
	else {
		dmg = 180;
		this->bc_fire = fire_tower4_level2;
	}
	for(i = 0; i< 25; i++) {
		this->bullets[i] = initBullet(400, 400, 40, dmg, bulletImage, this);
		setupBullet(this->bullets[i], explosion, fireAction, fireCollide);
		this->bullets[i]->super->collideRange = 80;
	}
	this->total_bullet_available = 25;
	this->pEnd = 25;
}

void initBulletToWeapon5(int level, struct animation* bulletImage, struct animation* explosion, struct BulletCollection* this){
	int i;
	for (i = 0; i < 25; i++){
		this->bullets[i] = initBullet(400, 400, 40, 0.0, bulletImage, this);
		setupBullet(this->bullets[i], explosion, freezeAction, freezeCollide);
	}
	this->total_bullet_available = 25;
	this->pEnd = 25;
	if (level == 1)
		this->bc_fire = fire_tower5_level1;
	else
		this->bc_fire = fire_tower5_level2;

}


void killBulletCollection(void* bc)
{
	((struct BulletCollection*)bc)->super->end_frame = ((struct Bullet*)bc)->super->curr_frame + 5;
	safefree(&bc);
}
/*
 * This is the helper function that initialize main bullet with position
 */
bool launchMainBullet(struct BulletCollection* self, struct SpaceShip* ss) {
	if(self->pStart > 49 || self->total_bullet_available < 1) {
		printf("load bullet error\n");
		return false;
	}
	setBulletInitPosition(ss->super->center_x, ss->super->center_y, self->bullets[self->pStart]);
	addToEnv(self->env, self->bullets[self->pStart]->super);
	addObjToCollide(self->collision_checker, self->bullets[self->pStart]->super);
	self->total_bullet_available--;
	return true;
}

/*
 * This is helper function that initialize secondary bullets (for weapon that have 2 diff kind of bullets)
 * REQUIREMENT:
 *  - image for secondary bullet has to be in stats[1];
 * - if you want to use for other types of bullet, remember to change: collideRange, scale
 */
bool launchSecondaryBullet(struct BulletCollection* this, struct SpaceShip* ss){
	if (this->pStart > 49 || this->total_bullet_available < 1){
		printf("load secondary bullet error\n");
		return false;
	}
	setStats(this->bullets[this->pStart]->super, 1);
	this->bullets[this->pStart]->super->scale = 4;
	this->bullets[this->pStart]->super->collideRange = 4;
	setBulletInitPosition(ss->super->center_x, ss->super->center_y, this->bullets[this->pStart]);
	addToEnv(this->env, this->bullets[this->pStart]->super);
	addObjToCollide(this->collision_checker, this->bullets[this->pStart]->super);
	this->total_bullet_available--;
	return true;
}

/*
 * This is the helper function that increase the index of bullet
 */
void nextBullet(struct BulletCollection* self) {
	if (self->pStart < 49)
		self->pStart++;
	else
		self->pStart = 0;
}

/*
 * fire_tower1_level1
 * this function will prepare bullets before shooting,
 * by calculating acceleration needed, set the initial position and shoots the bullet
 */
void fire_tower1_level1(int en_x, int en_y, struct BulletCollection* self, struct SpaceShip* ss){

	if(!launchMainBullet(self, ss)) return;
	setBulletAcceleration((en_x), (en_y), self->bullets[self->pStart]);
	nextBullet(self);
	//printf("pStart: %d, pEnd: %d, bullets left: %d\n", self->pStart, self->pEnd, self->total_bullet_available);
}

/*
 * fire_tower1_level2	: this function is available weapon for tower type 1 to upgrade
 * bullet's behavior	: 3 bullets will be shot at once, bullet1 will go directly to the enemy
 *						: bullet2 will be shot at -45deg from bullet1, bullet3 will be shot at +45deg from bullet1.
 */
void fire_tower1_level2(int en_x, int en_y, struct BulletCollection* self, struct SpaceShip* ss){
	if(self->total_bullet_available > 3){

		// set up Bullet1
		int bullet1_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		setBulletAcceleration(en_x, en_y, self->bullets[bullet1_index]);
		nextBullet(self);

		// set up Bullet2
		int bullet2_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		self->bullets[bullet2_index]->acc_x = (self->bullets[bullet1_index]->acc_x * 0.7071) + (self->bullets[bullet1_index]->acc_y * 0.7071);
		self->bullets[bullet2_index]->acc_y = (self->bullets[bullet1_index]->acc_x * -0.7071) + (self->bullets[bullet1_index]->acc_y * 0.7071);
		nextBullet(self);

		//set up Bullet3
		int bullet3_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		self->bullets[bullet3_index]->acc_x = (self->bullets[bullet1_index]->acc_x * 0.7071) - (self->bullets[bullet1_index]->acc_y * 0.7071);
		self->bullets[bullet3_index]->acc_y = (self->bullets[bullet1_index]->acc_x * 0.7071) + (self->bullets[bullet1_index]->acc_y * 0.7071);
		nextBullet(self);
	}
}

/*
 * fire_tower2_level1 :
 * if tower is located on position with x and y greater than 30,
 * this weapon will send enemy back by 30 pixels.
 * Otherwise, it will send enemy back to (0,0)
 */
void fire_tower2_level1(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	if(!launchMainBullet(this, ss)) return;
	this->bullets[this->pStart]->image_changed_flag = 0; // using this variable as level. this means level1
	nextBullet(this);
}

/*
 * fire_tower2_level2 :
 * this weapon will send enemy back to (0,0)
 */
void fire_tower2_level2(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	if(!launchMainBullet(this, ss)) return;
	this->bullets[this->pStart]->image_changed_flag = 1; // this means level 2
	nextBullet(this);
}

void fire_tower3_level1(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	if(!launchMainBullet(this, ss)) return;
	setBulletVelocity((en_x), (en_y), this->bullets[this->pStart]);
	nextBullet(this);
}
void fire_tower3_level2(int en_x, int en_y, struct BulletCollection* self, struct SpaceShip* ss){
	if(self->total_bullet_available > 3){

		// set up Bullet1
		int bullet1_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		setBulletVelocity((en_x), (en_y), self->bullets[self->pStart]);
		nextBullet(self);

		// set up Bullet2
		int bullet2_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		self->bullets[bullet2_index]->v_x = (self->bullets[bullet1_index]->v_x * 0.7071) + (self->bullets[bullet1_index]->v_y * 0.7071);
		self->bullets[bullet2_index]->v_y = (self->bullets[bullet1_index]->v_x * -0.7071) + (self->bullets[bullet1_index]->v_y * 0.7071);
		nextBullet(self);

		//set up Bullet3
		int bullet3_index = self->pStart;
		if(!launchMainBullet(self, ss)) return;
		self->bullets[bullet3_index]->v_x = (self->bullets[bullet1_index]->v_x * 0.7071) - (self->bullets[bullet1_index]->v_y * 0.7071);
		self->bullets[bullet3_index]->v_y = (self->bullets[bullet1_index]->v_x * 0.7071) + (self->bullets[bullet1_index]->v_y * 0.7071);
		nextBullet(self);
	}
}
void fire_tower4_level1(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	if(!launchMainBullet(this, ss)) return;
	nextBullet(this);
}
void fire_tower4_level2(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){

	if (this->total_bullet_available > 7){

		if(!launchMainBullet(this, ss)) return;
		this->bullets[this->pStart]->super->collideRange = 50;
		nextBullet(this);

		// shoot in all directions at a time
		if(!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = 0;
		this->bullets[this->pStart]->acc_y = -50;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = 0;
		this->bullets[this->pStart]->acc_y = 50;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = 50;
		this->bullets[this->pStart]->acc_y = 0;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = -50;
		this->bullets[this->pStart]->acc_y = 0;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = 50;
		this->bullets[this->pStart]->acc_y = -50;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = 50;
		this->bullets[this->pStart]->acc_y = 50;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = -50;
		this->bullets[this->pStart]->acc_y = 50;
		nextBullet(this);

		if (!launchSecondaryBullet(this, ss)) return;
		this->bullets[this->pStart]->acc_x = -50;
		this->bullets[this->pStart]->acc_y = -50;
		nextBullet(this);
	}
}

/*
 * fire_tower5_level1 :
 * this weapon will freeze the enemy for 30 frames, damage and then release it
 */
void fire_tower5_level1(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	if(!launchMainBullet(this, ss)) return;
	nextBullet(this);
}

/*
 * fire_tower5_level2 :
 * this weapon will freeze enemy for 30 frames, damage and then release it
 * in the same time, it also shoots 6 other freezing bullets that will cause any alien to freeze for 10 frames.
 * */
void fire_tower5_level2(int en_x, int en_y, struct BulletCollection* this, struct SpaceShip* ss){
	fire_tower4_level2(en_x, en_y, this, ss);
}


void setBulletAcceleration(int en_x, int en_y, struct Bullet* bul)
{
		bul->acc_x = (float)(en_x  - bul->init_x);
		bul->acc_y = (float)(en_y - bul->init_y);
}

void setBulletVelocity(int en_x, int en_y, struct Bullet* bul)
{
		bul->v_x = (float)(en_x  - bul->init_x)/3;
		bul->v_y = (float)(en_y - bul->init_y)/3;
}

void reloadBullet(struct BulletCollection* this, struct Bullet* bul, int index) {
	if(this->pEnd > index || this->pEnd < 0) {
		printf("reload bullet error\n");
		return;
	}
	this->bullets[this->pEnd] = bul;
	this->total_bullet_available++;
	if (this->pEnd >= index)
		this->pEnd = 0;
	else
		this->pEnd++;
}
