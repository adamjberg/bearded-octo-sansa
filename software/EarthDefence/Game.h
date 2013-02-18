/*
 * Game.h
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#ifndef GAME_H_
#define GAME_H_
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include "os/alt_syscall.h"
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "animation.h"
#include "Object.h"
#include <alt_types.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "hardCode.h"
#include "Env.h"
#include "Frame.h"
#include "SpaceShip.h"
#include "Alien.h"
#include "Bullet.h"
#include "Cursor.h"
#include "Collidable.h"
#include "UnitTest.h"
#include "QuardTree.h"
#include "BulletCollection.h"
#include "Button.h"
#include "TowerPlacer.h"
#include "CursorController.h"
#include "BitOperations.h"
#include "SwitchController.h"
#include "KeyController.h"
#include "Box3D.h"
#include "Node.h"
#include "Interface.h"
#include <system.h>
#include "HighScoreScreen.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_mouse.h"
#include "AudioController.h"
#include "altera_up_avalon_character_lcd.h"
#include "audio_ISR.h"
#define BOUND_X 250
#define BOUND_Y 240

#define GOALX 250-52
#define GOALY 240-60

alt_up_char_buffer_dev* char_buffer;
extern alt_up_pixel_buffer_dma_dev* pixel_buffer;

extern alt_up_character_lcd_dev * char_lcd_dev;
extern int swap;
extern int lock;
extern struct GameInfo info;
extern struct Frame gamePanel;
extern struct BulletCollection* weapon1_lvl1;
extern struct BulletCollection* weapon1_lvl2;
extern struct BulletCollection* weapon2_lvl1;
extern struct BulletCollection* weapon2_lvl2;
extern struct BulletCollection* weapon3_lvl1;
extern struct BulletCollection* weapon3_lvl2;
extern struct BulletCollection* weapon4_lvl1;
extern struct BulletCollection* weapon4_lvl2;
extern struct BulletCollection* weapon5_lvl1;
extern struct BulletCollection* weapon5_lvl2;
extern int cameraX;
extern int cameraY;
extern int cameraZ;
extern alt_up_ps2_dev *ps2;
extern struct AudioController* audioController;
bool loadSDImage(char*, int**);
void starAction(void*);
/* Alternative version for 'free()' */
void safefree(void**);
void promptSDcard(struct Env*, alt_up_sd_card_dev*);
void initWeapon(struct Collidable*, struct Env* p);
void checkStages(struct Alien**, int, struct Collidable*);
void mainMenu(struct Cursor*, struct CursorController*, struct Env*);
void gameSetup(struct Env*, struct animation**, struct Cursor*, struct animation*, struct animation*);
void endGame(struct Alien**, struct Collidable*, struct Env*, struct Cursor*, struct CursorController*, struct KeyController*);
void printhex(unsigned int);
#endif /* GAME_H_ */
