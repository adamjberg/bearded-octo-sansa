/*
 * Bullet.c
 *
 *  Created on: 2013-01-24
 *      Author: Diana
 *  Edited on: 2013-01-25
 *	Author: Perry
 */

#include "Bullet.h"

/*
 *  Constructor of Bullet object
 */
struct Bullet* initBullet(int x, int y, int size, float damage_, struct animation* a, struct BulletCollection* bc){
	struct Bullet* b;
	b = (struct Bullet*)malloc(sizeof(struct Bullet));
	b->super = initObject((int)x, (int)y, size, a, (void*)b);
	b->super->collideRange = size;
	b->acc_x = 0;
	b->acc_y = 0;
	b->v_x = 0;
	b->v_y = 0;
	b->damage = damage_;
	b->next_x = x;
	b->next_y = y;
	b->frame = -1;
	b->image_changed_flag = 0;
	b->pBulCollection = bc;
	b->super->class = bullet;

	return b;
}

void setupBullet(struct Bullet* this, struct animation* explode, void* action, void* collide) {
	this->super->action = action;
	this->super->collide = collide;
	addStats(this->super, explode);
}
void killBullet(void* b)
{
	((struct Bullet*)b)->super->end_frame = ((struct Bullet*)b)->super->curr_frame + 5;
}

void resetBullet(struct Bullet* this, int scale) {
	this->v_y = this->v_x = 0;
	this->frame = 0;
	this->image_changed_flag = 0;
	this->super->scale = scale;
	this->super->collideRange = scale;
	this->super->startAnimated = 1;
	setStats(this->super, 0);
}
void bulletAction(void* self){
	if (((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag == 1)
	{
		if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 5)
		{
			setXY_noBound((struct Object*)self, 400, 400);
		}
		else if (((struct Bullet*)((struct Object*)self)->subObject)->frame >= 20)
		{
			removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);
			removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
			resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 1);

			// reload bullet that has collided with boundary or alien
			reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
		}
		((struct Bullet*)((struct Object*)self)->subObject)->frame++;

	}

	else
	{
		shootEnemy((struct Bullet*)((struct Object*)self)->subObject);
	}
}

void blackHoleAction(void* self){
	if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 30) {
			setXY_noBound((struct Object*)self, 400, 400);
	} else if(((struct Bullet*)((struct Object*)self)->subObject)->frame == 80) {
		resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 40);

		removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
		removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);

		reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
	}
	((struct Bullet*)((struct Object*)self)->subObject)->frame++;
}

void laserAction(void* self) {
	if (((struct Bullet*)((struct Object*)self)->subObject)->frame >= 4)
	{

		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, ((struct Object*)self)->x, ((struct Object*)self)->y, ((struct Bullet*)((struct Object*)self)->subObject)->init_x, ((struct Bullet*)((struct Object*)self)->subObject)->init_y, 0x0000,swap);
		//usleep(1000);
		setXY_noBound((struct Object*)self, 400, 400);
		removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
		removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);
		resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 1);
		// reload bullet that has collided with boundary or alien
		reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
	} else {
		((struct Bullet*)((struct Object*)self)->subObject)->next_x = ((struct Object*)self)->x +((struct Bullet*)((struct Object*)self)->subObject)->v_x;
		((struct Bullet*)((struct Object*)self)->subObject)->next_y = ((struct Object*)self)->y+ ((struct Bullet*)((struct Object*)self)->subObject)->v_y;
		if(((struct Bullet*)((struct Object*)self)->subObject)->next_x  > 245 ||
			((struct Bullet*)((struct Object*)self)->subObject)->next_y >= 235 ||
			((struct Bullet*)((struct Object*)self)->subObject)->next_x  <= 3 ||
			((struct Bullet*)((struct Object*)self)->subObject)->next_y <= 3) {
			((struct Bullet*)((struct Object*)self)->subObject)->v_x = ((struct Bullet*)((struct Object*)self)->subObject)->v_y = 0;
			((struct Bullet*)((struct Object*)self)->subObject)->frame = 4;
		} else {
			setXY_noBound((struct Object*)self, (int)(((struct Bullet*)((struct Object*)self)->subObject)->next_x) , (int)(((struct Bullet*)((struct Object*)self)->subObject)->next_y));
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, ((struct Object*)self)->x, ((struct Object*)self)->y,((struct Object*)self)->prev_x, ((struct Object*)self)->prev_y, 0xFF11,0);
			((struct Bullet*)((struct Object*)self)->subObject)->frame++;
			//usleep(1000);
		}
	}
}

void fireAction(void* self) {
	if (((struct Object*)self)->scale > 4){
		if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 40) {
			setXY_noBound((struct Object*)self, 400, 400);
		} else if(((struct Bullet*)((struct Object*)self)->subObject)->frame == 80) {
			resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 40);

			removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
			removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);

			reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
		}
		((struct Bullet*)((struct Object*)self)->subObject)->frame++;
	} else {

		if (((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag == 1)
		{
			setXY_noBound((struct Object*)self, 400, 400);
			if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 20)
			{
				removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);
				removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
				resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 40);

				// reload bullet that has collided with boundary or alien
				reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
			}
			((struct Bullet*)((struct Object*)self)->subObject)->frame++;
		} else {
			shootEnemy((struct Bullet*)((struct Object*)self)->subObject);
		}
	}
}

void freezeAction(void* self){
	// for big bullet, remove after 30 frames
	if (((struct Object*)self)->scale > 4) {
		if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 30) {
			setXY_noBound((struct Object*)self, 400, 400);
		} else if(((struct Bullet*)((struct Object*)self)->subObject)->frame == 80) {
			resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 40);

			removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
			removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);

			reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
		}
			((struct Bullet*)((struct Object*)self)->subObject)->frame++;

	// small bullet, remove after 15 frames
	} else {
		if (((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag == 1)
		{
			if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 15)
				setXY_noBound((struct Object*)self, 400, 400);
			if (((struct Bullet*)((struct Object*)self)->subObject)->frame == 20)
			{
				removeFromCollide((struct Collidable*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->collision_checker, (struct Object*)self);
				removeFromEnvOnly((struct Env*)((struct BulletCollection*)((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection)->env, (struct Object*)self);
				resetBullet(((struct Bullet*)((struct Object*)self)->subObject), 40);

				// reload bullet that has collided with boundary or alien
				reloadBullet(((struct Bullet*)((struct Object*)self)->subObject)->pBulCollection, ((struct Bullet*)((struct Object*)self)->subObject), 49);
			}
			((struct Bullet*)((struct Object*)self)->subObject)->frame++;
		} else {
			shootEnemy((struct Bullet*)((struct Object*)self)->subObject);
		}
	}
}

void setBulletInitPosition(int x, int y, struct Bullet* bul)
{
	bul->init_x = x;
	bul->init_y = y;
	bul->next_x = x;
	bul->next_y = y;

	bul->super->prev_x = x;
	bul->super->prev_y = y;
	bul->super->animate_prev_x = x;
	bul->super->animate_prev_y = y;
	if(bul->super->scale > 2)
		setXY(bul->super, x-bul->super->scale/2, y-bul->super->scale/2);
	else
		setXY(bul->super, x, y);
}

void shootEnemy(struct Bullet* self)
{

		self->v_x = ((self->acc_x)/50.0) + self->v_x;
		self->v_y = ((self->acc_y)/50.0) + self->v_y;

		self->next_x = self->next_x + ((self->v_x));
		self->next_y = self->next_y + ((self->v_y));

		if(self->next_x > 245 || self->next_y >= 240 || self->next_x <= 0 || self->next_y <= 0) {
			self->image_changed_flag = 1;
			self->v_x = self->v_y = 0;
			self->next_x = self->next_y = 400;
			self->frame = 20;
		}
		setXY_noBound(self->super, (int)(self->next_x), (int)(self->next_y));
}

void bulletCollide(void* self, void* obj, struct Collidable* col)
{
	lock = 1;

	if (((struct Object*)obj)->class == 2 && ((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag == 0 )
	{
		((struct Object*)self)->isShowing = 0; //ensure animation is not drawing this bullet until stats is changed
		((struct Object*)self)->scale = 10;
		setStats((struct Object*)self, 1);
		((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag = 1;
		//clean the bullet picture before explode
		drawBox(((struct Object*)self)->animate_prev_x, ((struct Object*)self)->animate_prev_y, ((struct Object*)self)->animate_prev_x+1, ((struct Object*)self)->animate_prev_y+1, 0, NULL);
		//make sure the bullet stop moving before explode
		((struct Bullet*)((struct Object*)self)->subObject)->v_x = 0;
		((struct Bullet*)((struct Object*)self)->subObject)->v_y = 0;
		((struct Bullet*)((struct Object*)self)->subObject)->acc_x = 0;
		((struct Bullet*)((struct Object*)self)->subObject)->acc_y = 0;
		((struct Object*)self)->isShowing = 1; //release for animation

	} lock = 0;
}

void blackHoleCollide(void* self, void* obj, struct Collidable* col){
	lock = 1;
	if (((struct Object*)obj)->class == alien){
		if (((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag == 0){
			// if tower position is greater than (30, 30), move alien back by 30 pixels.
			// Otherwise, send enemy back to 0,0
			if (((struct Object*)self)->x <= 30 && ((struct Object*)self)->y <= 30){
				setXY((struct Object*)obj, 0, 0);
				drawBox(((struct Object*)self)->animate_prev_x, ((struct Object*)self)->animate_prev_y, ((struct Object*)self)->animate_prev_x+1, ((struct Object*)self)->animate_prev_y+1, 0, NULL);
			}
			else{
				setXY((struct Object*)obj, ((struct Object*)obj)->x - 30, ((struct Object*)obj)->y - 30);
				drawBox(((struct Object*)self)->animate_prev_x, ((struct Object*)self)->animate_prev_y, ((struct Object*)self)->animate_prev_x+1, ((struct Object*)self)->animate_prev_y+1, 0, NULL);
			}
			((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag = 0;
		} else {
			setXY((struct Object*)obj, 0, 0);
			drawBox(((struct Object*)self)->animate_prev_x, ((struct Object*)self)->animate_prev_y, ((struct Object*)self)->animate_prev_x+1, ((struct Object*)self)->animate_prev_y+1, 0, NULL);
			((struct Bullet*)((struct Object*)self)->subObject)->image_changed_flag = 0;
		}
	}
	lock = 0;
}

void laserCollide(void* this, void* obj, struct Collidable* col) {
}

void fireCollide(void* this, void* obj, struct Collidable* col) {
	 lock = 1;

	if (((struct Object*)obj)->class == alien && ((struct Bullet*)((struct Object*)this)->subObject)->image_changed_flag == 0){
		if (((struct Object*)this)->scale == 4){
			((struct Bullet*)((struct Object*)this)->subObject)->image_changed_flag = 1;
			//clean the bullet picture before removing from environment
			drawBox(((struct Object*)this)->animate_prev_x, ((struct Object*)this)->animate_prev_y, ((struct Object*)this)->animate_prev_x+1, ((struct Object*)this)->animate_prev_y+1, 0, NULL);
			//make sure the bullet stop moving before moved from the environment
			((struct Bullet*)((struct Object*)this)->subObject)->v_x = 0;
			((struct Bullet*)((struct Object*)this)->subObject)->v_y = 0;
			((struct Bullet*)((struct Object*)this)->subObject)->acc_x = 0;
			((struct Bullet*)((struct Object*)this)->subObject)->acc_y = 0;
		}
	}
	lock = 0;
}

void freezeCollide(void* this, void* obj, struct Collidable* col){

	if (((struct Object*)obj)->class == alien){
		if (((struct Object*)this)->scale > 4){
			// big bullets : freeze enemy for 30 frames, damage and release
			if (((struct Bullet*)((struct Object*)this)->subObject)->frame < 30)
			{
				((struct Bullet*)((struct Object*)this)->subObject)->enemy_v_x = ((struct Alien*)((struct Object*)obj)->subObject)->speed;
				((struct Alien*)((struct Object*)obj)->subObject)->speed = 0;

				if (((struct Object*)this)->scale == 1)
					printf("freezeCollide error\n");

			} else {
				((struct Bullet*)((struct Object*)this)->subObject)->damage = 0;
				((struct Alien*)((struct Object*)obj)->subObject)->speed = 1;
			}
		} else {
			// small bullets : freeze enemy for 10 frames, damage and release
			if (((struct Bullet*)((struct Object*)this)->subObject)->frame < 10)
			{
				((struct Bullet*)((struct Object*)this)->subObject)->image_changed_flag = 1;
				drawBox(((struct Object*)this)->animate_prev_x, ((struct Object*)this)->animate_prev_y, ((struct Object*)this)->animate_prev_x+1, ((struct Object*)this)->animate_prev_y+1, 0, NULL);
				((struct Alien*)((struct Object*)obj)->subObject)->speed = 0;
				((struct Bullet*)((struct Object*)this)->subObject)->v_x = 0;
				((struct Bullet*)((struct Object*)this)->subObject)->v_y = 0;
				((struct Bullet*)((struct Object*)this)->subObject)->acc_x = 0;
				((struct Bullet*)((struct Object*)this)->subObject)->acc_y = 0;
			} else {
				((struct Bullet*)((struct Object*)this)->subObject)->damage = 10.0;
				((struct Alien*)((struct Object*)obj)->subObject)->speed = 1;
			}
		}
	}
}

void laserAnimate(struct animation** curr, int x, int y, int size, int isForward, struct Object* obj, struct Cursor* m) {

}
