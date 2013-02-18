

/*
 * Game.c
 * The main models of tower defence game developed by group 5 in EECE 381 class.
 *
 */
#include "Game.h"


int swap;
//struct Cursor* mouse;
alt_up_pixel_buffer_dma_dev* pixel_buffer;
struct GameInfo info;
struct Frame gamePanel;
struct BulletCollection* weapon1_lvl1;
struct BulletCollection* weapon1_lvl2;
struct BulletCollection* weapon2_lvl1;
struct BulletCollection* weapon2_lvl2;
struct BulletCollection* weapon3_lvl1;
struct BulletCollection* weapon3_lvl2;
struct BulletCollection* weapon4_lvl1;
struct BulletCollection* weapon4_lvl2;
struct BulletCollection* weapon5_lvl1;
struct BulletCollection* weapon5_lvl2;

int lock;
int cameraX;
int cameraY;
int cameraZ;

struct AudioController* audioController;

alt_alarm alarm;

alt_up_ps2_dev *ps2;

alt_up_character_lcd_dev * char_lcd_dev;
/* Alternative version for 'free()'
 * This function is memory safe
 */
void safefree(void **pp)
{
    if (pp != NULL) {               /* safety check */
        free(*pp);                  /* deallocate chunk, note that free(NULL) is valid */
        *pp = NULL;                 /* reset original pointer */
    }
}

/*
 * System clock that run the animation for all objects in the environment
 */
alt_u32 my_alarm_callback (void* paras)
{
	if(lock == 1) return alt_ticks_per_second()/1000;
	int i;
	for(i = 0; i < ((struct Env*)paras)->size; i++) {
		if((*(((struct Env*)paras)->o+i))->isRunable == 1 ) {
			(*(((struct Env*)paras)->o+i))->animate(&((*(((struct Env*)paras)->o+i))->currImg),
						(*(((struct Env*)paras)->o+i))->x, (*(((struct Env*)paras)->o+i))->y, (*(((struct Env*)paras)->o+i))->scale, 1,
						*(((struct Env*)paras)->o+i), ((struct Env*)paras)->m);
			if((*(((struct Env*)paras)->o+i))->curr_frame < 40000000)
				(*(((struct Env*)paras)->o+i))->curr_frame++;
			else
				(*(((struct Env*)paras)->o+i))->curr_frame = 1;
			if((*(((struct Env*)paras)->o+i))->curr_frame > (*(((struct Env*)paras)->o+i))->end_frame)
				(*(((struct Env*)paras)->o+i))->action((void*)(((struct Env*)paras)->o[i]));
			if((*(((struct Env*)paras)->o+i))->curr_frame == (*(((struct Env*)paras)->o+i))->end_frame) {
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer,(*(((struct Env*)paras)->o+i))->x, (*(((struct Env*)paras)->o+i))->y, (*(((struct Env*)paras)->o+i))->x+(*(((struct Env*)paras)->o+i))->scale+10, (*(((struct Env*)paras)->o+i))->y+(*(((struct Env*)paras)->o+i))->scale+10, 0x0000, 0);
				setXY_noBound(*(((struct Env*)paras)->o+i), 150, 500);
			} else if((*(((struct Env*)paras)->o+i))->curr_frame == (*(((struct Env*)paras)->o+i))->end_frame+2) {
				(*(((struct Env*)paras)->o+i))->isRunable = 0;
			}
		}

	}
	if(((struct Env*)paras)->redraw) {
		redraw(((struct Env*)paras)->m);
		((struct Env*)paras)->redraw = false;
	}
	return alt_ticks_per_second()/15; //100ms
}
/*
 * this function clear the screen and set up pixel buffers for graphics
 */
void initVGA() {

	  // Set the background buffer address � Although we don�t use thebackground,
	  // they only provide a function to change the background buffer address, so
	  // we must set that, and then swap it to the foreground.
	  unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	  unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320*240*2);

	  alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr1);
	  // Swap background and foreground buffers
	  alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	  // Wait for the swap to complete
	  while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	  alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr2);
	  // Clear the screen
	  alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	  alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	 // alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF,1);
	   //Swap background and foreground buffers
	  	  alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	  	   //Wait for the swap to complete
	  	  while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	  //	  return pixel_buffer;
}

bool loadSDCard(alt_up_sd_card_dev* device) {
	if((device = alt_up_sd_card_open_dev("/dev/SD_Card")) != NULL) {
		if (alt_up_sd_card_is_Present()) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
				return true;
			} else {
				printf("Unknown file system.\n");
			}
		}
	}
	return false;
}

/*
 * Load bitmap image from SD card.  Fixed problem with color coding
 * file name is required to be upper-case and bitmap pixels has to be even size x even size
 */
bool loadSDImage(char* filename, int** destination) {
	int i, j, bytes = 0, offset = 0, size = 0, byte = 0;
	int a, b;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if(file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return false;
	}

	//Start reading the bitmap header
	while(bytes < 10) {
		if(alt_up_sd_card_read(file_pointer) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			return false;
		}
		bytes++;
	}
	if((offset = alt_up_sd_card_read(file_pointer))< 0) {
		alt_up_sd_card_fclose(file_pointer);
		return false;
	}
	while(bytes < offset-1){
		if(bytes == 21) {
			if((size = alt_up_sd_card_read(file_pointer))< 0) {
				alt_up_sd_card_fclose(file_pointer);
				return false;
			}
			*destination = (int*)malloc(size*size*sizeof(int));
		} else if( alt_up_sd_card_read(file_pointer) < 0) {
			safefree(destination);
			alt_up_sd_card_fclose(file_pointer);
			return false;
		}
		bytes++;
	}
	//Start reading the pixel data
	for(j = size-1; j >= 0; j--) {
		for(i = 0; i < size; i++) {
			a = alt_up_sd_card_read(file_pointer);
			b = alt_up_sd_card_read(file_pointer);
			if(a < 0 || b < 0) {
				free(*destination);
				*destination = NULL;
				printf("%s invalid at pixel[%d, %d]!\n", filename, i, j);
				alt_up_sd_card_fclose(file_pointer);
				return false;
			}
			byte = getColor555(b*256+a);
			*(*destination + j*size+i) = byte;
		}
	}
	alt_up_sd_card_fclose(file_pointer);
	return true;
}
/*
 * An abstract function that allows to load all images with same prefix
 *	index is the number of characters that are prefix
 */
struct animation* loadSDImageSeq(char* filename, int index, int size) {
	int i = 0, j = 0;
	swap = 0;
	char temp[30],buffer[30];
	char buff[20];
	char* prefix = (char*)malloc(sizeof(char)*(index+1));
	prefix[index] = '\0';
	int* ptr = NULL;

	strncpy(prefix, filename, index);
	struct animation* a = NULL;
	struct animation* b = NULL;
	while(i < size) {
		strcpy(temp, prefix);
		buffer[0] = 0;
		sprintf(buffer, "%d.BMP ", i);
		if(i < 10) {
			strncat(temp, buffer, 5);
			strcpy(buff, temp);
			for(j = 0; j < i%5; j++) {
				buff[index+5+j] = '.';
			} buff[index+5+j] = ' ';buff[index+6+j] = ' ';buff[index+7+j] = ' ';buff[index+8+j] = ' ';buff[index+9+j] = '\0';
		}else {
			strncat(temp, buffer, 6);
			strcpy(buff, temp);
			for(j = 0; j < i%5; j++) {
				buff[index+6+j] = '.';
			}
			buff[index+6+j] = ' ';buff[index+6+j+1] = ' ';buff[index+6+j+2] = ' ';buff[index+9+j] = '\0';
		}
		if(info.start == 1) {
			alt_up_char_buffer_string(char_buffer, buff, 48, 30); //show reading file name because we are in loading page
		}
		while(!loadSDImage(temp, &ptr)){
			printf("Loading File Error: %s\n", temp);
		}
		if(i == 0) {
			a = initAnimation(ptr, 1);
			imageInDisk(a);
		}
		else {
			b = initAnimation(ptr, 0);
			imageInDisk(b);
			addImage(a, b);
		}
		i++;
	}
	free(prefix); prefix = NULL;
	return a;
}
/*
 * Draw some static images on the background before the game is starting
 */
void backgroundImage() {

	int* pic = NULL;
	while(!loadSDImage("E.BMP", &pic));
	draw(210, 200, pic, 64);
	free(pic); pic = NULL;
}

/*
 * Helper method that will print the string on the display
 */
void displayString(char* string, int x, int y) {
	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, string, x, y);
}
void promptSDcard(struct Env* p, alt_up_sd_card_dev* device_reference) {
	/*
	 * Loading Screen if SD card is not presented
	 */

	int frame = 25;
	struct animation* b = initAnimation((int*)pacman01, 1);
	addImage(b, initAnimation((int*)pacman02, 0));
	addImage(b, initAnimation((int*)pacman03, 0));
	addImage(b, initAnimation((int*)pacman04, 0));
	struct Object *face;
	face= initObject(80, 120, 10, b, NULL);
	addToEnv(p, face);

	alt_alarm_start (&alarm,alt_ticks_per_second(),my_alarm_callback,(void*)p);


	while(!loadSDCard(device_reference)) {
		displayString("Please insert the SD card to start", frame, 30);
		frame++;
		setXY(face, face->x+4, face->y);
		if(face->x >245) face->x = 0;
		if(frame > 61) frame = 0;

		usleep(300000);
	}alt_up_char_buffer_clear(char_buffer);
	killAnimation(b);
	removeFromEnv(p, face);
	alt_alarm_stop(&alarm);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
}
/*
 * Main Game Loop
 */
int main()
{
	// Use the name of your pixel buffer DMA core
	pixel_buffer =alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma_0");

	initVGA();
	usleep(5000000);
	ps2 = alt_up_ps2_open_dev("/dev/ps2_0");
	ps2->timeout = 2000000;
		alt_up_ps2_clear_fifo(ps2);
		alt_up_ps2_init(ps2);

		unsigned char byte1;
		while(alt_up_ps2_read_data_byte(ps2, &byte1)!=0);

	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");
	alt_up_character_lcd_init (char_lcd_dev);

	char_buffer  = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	alt_up_sd_card_dev *device_reference = NULL;
	struct Env* p = initEnv();
	initGameInfo();

	struct Collidable* collisionChecker = initCollidable();
	addCollisionToEnv(p, collisionChecker);

	promptSDcard(p, device_reference);

	usleep(1000);
	alt_up_char_buffer_string(char_buffer, "Loading ", 40, 30);

	  unsigned end_time, start_time;
	  int count = 0; lock = 0;

	struct animation* starAnimation = loadSDImageSeq("ST0.BMP", 2, 8);
	struct animation* star2Animation = loadSDImageSeq("ST00.BMP", 3, 7);
	struct animation* alien0 = loadSDImageSeq("A100.BMP", 2, 2); //2 images where first 2 characters are prefix
	struct animation* alien1 = loadSDImageSeq("A000.BMP", 2, 15);
	struct animation* ship0 = loadSDImageSeq("S00.BMP", 2, 16);
	struct animation* ship1 = loadSDImageSeq("S10.BMP", 2, 27);
	struct animation* bossAnimate = loadSDImageSeq("BO00.BMP", 2, 28);
	struct animation* ship2 = loadSDImageSeq("S20.BMP", 2, 35);
	struct animation* ship3 = loadSDImageSeq("S30.BMP", 2, 30);
	struct animation* ship4 = loadSDImageSeq("S40.BMP", 2, 10);

	struct animation* explode1 = initAnimation((int*)explode01, 1);
	addImage(explode1, initAnimation((int*)explode02, 0));
	addImage(explode1, initAnimation((int*)explode03, 0));
	addImage(explode1, initAnimation((int*)explode04, 0));
	addImage(explode1, initAnimation((int*)explode05, 0));

	struct animation** shipAnimationCollection = (struct animation**)malloc(sizeof(struct animation*)*5);
	shipAnimationCollection[0] = ship0;
	shipAnimationCollection[1] = ship1;
	shipAnimationCollection[2] = ship2;
	shipAnimationCollection[3] = ship3;
	shipAnimationCollection[4] = ship4;

	initWeapon(collisionChecker, p);

	struct Cursor* mouse = initCursor(p, collisionChecker);
	addToEnv(p, mouse->super);
	addObjToCollide(collisionChecker, mouse->super);
	setCursor(p, mouse);

	struct KeyController* keyController = initKeyController();
	struct SwitchController* switchController = initSwitchController();
	struct CursorController* ctrl = initCursorController(mouse->super, switchController, keyController);

	alt_up_char_buffer_string(char_buffer, "Loading Sounds            ", 30, 30);
	audioController = initAudioController();
	loadSound( audioController, LOOP_ONE );
	loadSound( audioController, LASER_SOUND );
	alt_irq_register(AUDIO_IRQ, audioController, (void*) audio_ISR);
	alt_irq_enable(AUDIO_IRQ);
	play_background_loop( audioController, LOOP_ONE );
	enableAudioController( audioController );

	printhex(info.score);

	mainMenu(mouse, ctrl, p);

	disableAudioController(audioController);
	stop_background_loop(audioController);
	unloadSoundById(audioController, LASER_SOUND);
	unloadSoundById(audioController, LOOP_ONE);
	alt_up_char_buffer_string(char_buffer, "Loading Sounds           ", 30, 30);
	//loadSound(audioController, WIN_SOUND);
	//loadSound(audioController, LOSE_SOUND);
	loadSound( audioController, TOWER_UPGRADE_SOUND );
	loadSound( audioController, LOOP_TWO );
	play_background_loop(audioController, LOOP_TWO);
	enableAudioController( audioController );
	alt_up_char_buffer_clear(char_buffer);
	//usleep(1000);
	struct Alien* testAlienCollection[60];
	gameSetup(p, shipAnimationCollection, mouse, starAnimation, star2Animation);

	usleep(500000); //time delay for panel to be drawn
//
	char LPS[50]; float lps_;

	int n = 0;

	for(n = 0; n < 20; n++) {
		testAlienCollection[n] =initAlien(n, 10*n, 10, alien0, explode1, "IdontKnow", 1.4, 150, 500, collisionChecker);
		addToEnvNR(p, testAlienCollection[n]->super);
	}
	for(n = 0; n < 20; n++) {
		testAlienCollection[n+20] =initAlien(10*n, n, 10, alien1, explode1, "whatName", 1.4, 190, 850, collisionChecker);
		addToEnvNR(p, testAlienCollection[n+20]->super);
	}
	for(n = 0; n < 20; n++) {
		testAlienCollection[n+40] =initAlien(10*n, n, 20, bossAnimate, explode1, "IamBoss", 1.6, 800, 1500, collisionChecker);
		testAlienCollection[n+40]->score = 300;
		addToEnvNR(p, testAlienCollection[n+40]->super);
	}
	int stage = 0;
	/*
	 * Game Starts!!!!!!
	 */
	alt_alarm_start (&alarm,alt_ticks_per_second(),my_alarm_callback,(void*)p);

	int startTimer = 0;
	char second_row1[15];
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
	  sprintf(second_row1, "wave# %d  ", stage);
	  alt_up_character_lcd_string(char_lcd_dev, second_row1);

  while(1) {
	  alt_timestamp_start();
	  start_time = (unsigned)alt_timestamp();

/*-----------------------------------------------------------------------------------------------*/

	  checkCollision(collisionChecker); //a major function that check each collision happen between each object

	  updateCursorController(ctrl, 1);

	  count++;

	  if (startTimer > count)
		  info.startButton = false;
	  else {
		  if(stage == 7)
			info.isWin = true;
		  else if(startTimer == count){
			//play_background_loop(audioController, LOOP_TWO);
			enableAudioController( audioController );
		  }
	  }
	  if (info.startButton){
			disableAudioController(audioController);
			//stop_background_loop(audioController);
		    startTimer = count + 15000;
	  		checkStages(testAlienCollection, stage%7, collisionChecker);
			stage++;
			//if(stage > 6) stage = 0;
			info.startButton = false;
		  	  alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
		  	  sprintf(second_row1, "wave# %d  ", stage);
		  	  alt_up_character_lcd_string(char_lcd_dev, second_row1);
	  }

	  if(info.isEnd || info.isWin) {

			disableAudioController(audioController);
			stop_background_loop(audioController);
		  endGame(testAlienCollection, collisionChecker, p, mouse, ctrl, keyController);
	  }
/*-----------------------------------------------------------------------------------------------*/



	  end_time = (unsigned)alt_timestamp();
	  lps_ = (float)alt_timestamp_freq()/(float)(end_time - start_time);

	  sprintf(LPS, "The current LPS is %.2f", lps_);
	  alt_up_char_buffer_string(char_buffer, LPS, 3, 2);
  }
  return 0;
}

void starAction(void* this) {
	 if(((struct Object*)this)->curr_frame == 10) {
		 setXY((struct Object*)this, rand()%210, rand()%210);
		 ((struct Object*)this)->curr_frame = 0;
	 }
}

void checkStages(struct Alien** aliens, int stage, struct Collidable* col) {
	int n;
	switch(stage) {
	case 0:
		for(n = 0; n < 20; n++) {
			resetAlien(aliens[n], n, 10*n);
			addObjToCollide(col, aliens[n]->super);
		}
		break;
	case 1:
		for(n = 0; n < 20; n++) {
			aliens[n+20]->score += 250;
			resetAlien(aliens[n+20], n, 10*n);
			addObjToCollide(col, aliens[n+20]->super);
		}
		break;
	case 2:
		for(n = 0; n < 15; n++) {
			resetAlien(aliens[n], n, 10*n);
			addObjToCollide(col, aliens[n]->super);
		}
		for(n = 0; n < 15; n++) {
			resetAlien(aliens[n+20], n*10, n);
			addObjToCollide(col, aliens[n+20]->super);
		}
		break;
	case 3:
		for(n = 0; n < 20; n++) {
			aliens[n]->score += 550;
			aliens[n]->bounty += 550;
			aliens[n]->max_life += aliens[n]->max_life/4;
			resetAlien(aliens[n], n, 10*n);
			addObjToCollide(col, aliens[n]->super);
		}
		for(n = 0; n < 5; n++) {
			aliens[n+20]->score += 1000;
			aliens[n+20]->bounty += 1000;
			aliens[n+20]->max_life += aliens[n+20]->max_life/4;
			resetAlien(aliens[n+20], n*10, n);
			addObjToCollide(col, aliens[n+20]->super);
		}
		break;
	case 4:
		for(n = 0; n < 10; n++) {
			resetAlien(aliens[n], n*10, n);
			addObjToCollide(col, aliens[n]->super);
		}
		for(n = 0; n < 20; n++) {
			resetAlien(aliens[n+20], n, 10*n);
			addObjToCollide(col, aliens[n+20]->super);
		}
		break;
	case 5:
		for(n = 0; n < 15; n++) {
			aliens[n]->score *= 1.6;
			aliens[n]->bounty *= 1.6;
			aliens[n]->init_speed += 1;
			aliens[n]->max_life += aliens[n]->max_life/4;
			resetAlien(aliens[n], n, 10*n);
			addObjToCollide(col, aliens[n]->super);
		}
		for(n = 0; n < 15; n++) {
			aliens[n]->score *= 2;
			aliens[n]->bounty *= 2;
			aliens[n+20]->init_speed += 1;
			aliens[n+20]->max_life += aliens[n+20]->max_life/4;
			resetAlien(aliens[n+20], 10*n, n);
			addObjToCollide(col, aliens[n+20]->super);
		}
		break;
	case 6:
		for(n = 0; n < 10; n++) {
			aliens[n]->score *= 2;
			aliens[n]->bounty *= 2;
			aliens[n+40]->init_speed += 1.5;
			aliens[n+40]->max_life += aliens[n+40]->max_life/2;
			resetAlien(aliens[n+40], n, 10*n);
			addObjToCollide(col, aliens[n+40]->super);
		}
		break;
	default:
		break;
	}
}

void mainMenu(struct Cursor* mouse, struct CursorController* ctrl, struct Env* p) {
	cameraX = 160; cameraY = 120;
	cameraZ = 320;
	alt_alarm_start (&alarm,alt_ticks_per_second(),my_alarm_callback,(void*)p);

	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "Have FUN :)", 10, 5);

		struct Box3D* box = initBox3D(200, 180, 20, 40, 0xCBAD);
		box->animate = animateBox0;
		struct Box3D* box1 = initBox3D(50, 80, 10, 20, 0xFF00);
		box1->animate = animateBox1;
		struct Box3D* box2 = initBox3D(250, 60, 5, 10, 0x0BA0);
		box2->animate = animateBox2;
		struct Frame* mainpage = initMainMenu(p, box, box1, box2, mouse);
		setFrameToCursor(mouse, mainpage);
		int k = 1;
		int i;
		int* introPage;
		while(!loadSDImage("INTR.BMP", &introPage));
		while(info.start == 1){
			//Swap background and foreground buffers
	//	  	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		  	//Wait for the swap to complete
		//  	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
		  //	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		  	//draw_nontransparent(mainpage->x, mainpage->y, mainpage->currImg->image,140, mouse);
		  //	mainpage->currImg = mainpage->currImg->next;
		//	i++;


		  	animateBox0(box);
		  	if(animateBox1(box1)) {
		  		lock = 1;
		  		alt_up_char_buffer_clear(char_buffer);
		  		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		  		struct HighScore* highScores[MAX_NUM_HIGH_SCORES];
		  		int numHighScores;
		  		while((numHighScores =read_high_scores(highScores)) == -1) {
		  			printf("reading high score fail\n");
		  		}
		  		show_high_scores(highScores, numHighScores);

		  		while(!isBitSet( byte1, 0 )) {
		  			updateCursorController(ctrl, 0);
				  	IOWR_8DIRECT(LEDR_BASE, 0, k);
				  	usleep(10000);
				  	if(k > 65536) k = 1;
				  	else
				  		k = k << 1;
		  		}
		  		for(i = 0; i < MAX_NUM_HIGH_SCORES; i++) {
		  			safefree(&highScores[i]);
		  		}
		  		box1->endAnimate = false;
		  		box1->x = 50;
		  		box1->y = 80;
		  		box1->z = 10;
		  		box1->vy = box1->vz = 0;
		  		alt_up_char_buffer_clear(char_buffer);
		  		alt_up_char_buffer_string(char_buffer, "Have FUN :)", 10, 5);
		  		updateBox3D(box1);
		  		lock = 0;
			  	for(i = 0; i < mouse->curr_page->buttons_size; i++) {
		  			drawTxtButton(mouse->curr_page->buttons[i], mouse);
		  		}
		  	}
		  if(animateBox2(box2)) {
			  lock = 1;
			  alt_up_char_buffer_clear(char_buffer);
			  alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

			  draw(40, 0, introPage, 240);
			  while(!isBitSet( byte1, 0 )) {
			  	updateCursorController(ctrl, 0);
			  	IOWR_8DIRECT(LEDR_BASE, 0, k);
			  	usleep(10000);
			  	if(k > 65536) k = 1;
			  	else
			  		k = k << 1;
			  	}
			  box2->endAnimate = false;
			  box2->x = 280;
			  box2->y = 60;
			  box2->z = 5;
			  box2->vy = box2->vz = 0;
			  alt_up_char_buffer_clear(char_buffer);
			  alt_up_char_buffer_string(char_buffer, "Have FUN :)", 10, 5);
			  alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
			  updateBox3D(box2);
			  lock = 0;
			  for(i = 0; i < mouse->curr_page->buttons_size; i++) {
			  	drawTxtButton(mouse->curr_page->buttons[i], mouse);
			  }
		  }
		  	drawBox3D(box, 0, 0xCBAD);
		  	drawBox3D(box1, 0, 0xFF00);
		  	drawBox3D(box2, 0, 0x0BA0);
		  	 updateCursorController(ctrl, 7);
		  	 IOWR_8DIRECT(LEDR_BASE, 0, k);
		  	 usleep(6000);
		  	 if(k > 65536) k = 1;
		  	 else
		  		 k = k << 1;
			if(isKeyDown(3)){
					if(cameraX > 5)
						cameraX-=5;
					updateBox3D(box);
					updateBox3D(box1);
					updateBox3D(box2);
				/*	setXY(mouse->curr_page->buttons[0]->super, cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z));
				  	drawTxtButton(mouse->curr_page->buttons[0], mouse);
				  	setXY(mouse->curr_page->buttons[1]->super, cameraX + (box1->x-cameraX)*cameraZ/(cameraZ+box1->z), cameraY - (cameraY-box1->y)*cameraZ/(cameraZ+box1->z));
				  	drawTxtButton(mouse->curr_page->buttons[1], mouse);
				  	setXY(mouse->curr_page->buttons[2]->super, cameraX + (box2->x-cameraX)*cameraZ/(cameraZ+box2->z), cameraY - (cameraY-box2->y)*cameraZ/(cameraZ+box2->z));
				  	drawTxtButton(mouse->curr_page->buttons[2], mouse);*/
				}
				if(isKeyDown(2)){
					if(cameraX < 315)
						cameraX+=5;
					updateBox3D(box);
					updateBox3D(box1);
					updateBox3D(box2);
				/*	setXY(mouse->curr_page->buttons[0]->super, cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z));
					drawTxtButton(mouse->curr_page->buttons[0], mouse);
					setXY(mouse->curr_page->buttons[1]->super, cameraX + (box1->x-cameraX)*cameraZ/(cameraZ+box1->z), cameraY - (cameraY-box1->y)*cameraZ/(cameraZ+box1->z));
					drawTxtButton(mouse->curr_page->buttons[1], mouse);
					setXY(mouse->curr_page->buttons[2]->super, cameraX + (box2->x-cameraX)*cameraZ/(cameraZ+box2->z), cameraY - (cameraY-box2->y)*cameraZ/(cameraZ+box2->z));
					drawTxtButton(mouse->curr_page->buttons[2], mouse);*/

				}
				if(isKeyDown(1)){
					if(cameraY < 235)
						cameraY+=5;
					updateBox3D(box);
					updateBox3D(box1);
					updateBox3D(box2);
			/*		setXY(mouse->curr_page->buttons[0]->super, cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z));
					drawTxtButton(mouse->curr_page->buttons[0], mouse);
					setXY(mouse->curr_page->buttons[1]->super, cameraX + (box1->x-cameraX)*cameraZ/(cameraZ+box1->z), cameraY - (cameraY-box1->y)*cameraZ/(cameraZ+box1->z));
					drawTxtButton(mouse->curr_page->buttons[1], mouse);
					setXY(mouse->curr_page->buttons[2]->super, cameraX + (box2->x-cameraX)*cameraZ/(cameraZ+box2->z), cameraY - (cameraY-box2->y)*cameraZ/(cameraZ+box2->z));
					drawTxtButton(mouse->curr_page->buttons[2], mouse);*/
					//setXY(startButton->super, cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z));

				}
				if(isKeyDown(0)){
					if(cameraY > 5)
						cameraY-=5;
					updateBox3D(box);
					updateBox3D(box1);
					updateBox3D(box2);
				/*	setXY(mouse->curr_page->buttons[0]->super, cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z));
					drawTxtButton(mouse->curr_page->buttons[0], mouse);
					setXY(mouse->curr_page->buttons[1]->super, cameraX + (box1->x-cameraX)*cameraZ/(cameraZ+box1->z), cameraY - (cameraY-box1->y)*cameraZ/(cameraZ+box1->z));
					drawTxtButton(mouse->curr_page->buttons[1], mouse);
					setXY(mouse->curr_page->buttons[2]->super, cameraX + (box2->x-cameraX)*cameraZ/(cameraZ+box2->z), cameraY - (cameraY-box2->y)*cameraZ/(cameraZ+box2->z));
					drawTxtButton(mouse->curr_page->buttons[2], mouse);*/

				}
				usleep(5000);
		}
		//killAnimation(mainpage->stats[0], mainpage->scale);
		//killObject(mainpage);
		free(introPage);
		introPage = NULL;
		killBox(box);
		killBox(box1);
		killBox(box2);
		removeFrame(mainpage);
	//	usleep(5000);
		alt_up_char_buffer_clear(char_buffer); //remove menu characters

		swap = 0; //start using only one buffer

		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		IOWR_8DIRECT(LEDR_BASE, 0, 0);
		alt_alarm_stop(&alarm);

}

void gameSetup(struct Env* p, struct animation** shipAnimationCollection, struct Cursor* mouse, struct animation* starAnimation, struct animation* star2Animation) {
	backgroundImage();
	//restart the timer

	gamePanel = *initGamePanel(250, 0, 320, 240, p, shipAnimationCollection);
	setPanelToCursor(mouse, &gamePanel);

	struct Object* star1 = initObject(100, 150, 10, starAnimation, NULL);
	star1->action = starAction;
	addToEnv(p, star1);
	struct Object* star2 = initObject(10, 60, 10, star2Animation, NULL);
	star2->action = starAction;
	addToEnv(p, star2);
	info.isEnd = false;
}

void endGame(struct Alien** aliens, struct Collidable* col, struct Env* env, struct Cursor* mouse, struct CursorController* ctrl, struct KeyController* keyController) {
	int i, j;
	int* endImage;
	int soundToPlay;
	if(info.isWin)
	{
		//soundToPlay = WIN_SOUND;
		while(!loadSDImage("WIN.BMP", &endImage));
	}
	else
	{
		//soundToPlay = LOSE_SOUND;
		while(!loadSDImage("END.BMP", &endImage));
	}
	unloadSoundById( audioController, LOSE_LIFE_SOUND );
	unloadSoundById( audioController, TOWER_UPGRADE_SOUND );
	//loadSound(audioController, soundToPlay);
	for(i = 0; i < 60; i++) {
		removeFromCollide(col, aliens[i]->super);
		resetAlien(aliens[i], 0, 0);
		aliens[i]->super->isRunable = 0;
	}
	i = 0;
	int size = env->size;
	for(j = size-1; j >= 0; j--) {
		if(env->o[j]->class != cursor)
			removeFromEnv(env, env->o[j]);
	}

	setXY(mouse->super, 300, 200);
	clearPanelFromCursor(mouse);

//	playSoundById(audioController, soundToPlay, 0);

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	drawRec(0, 0, endImage, 240, 240);

	start_high_score_screen( info.score, keyController );
	while(1){
		//lets hold here for now
		waveLEDR();
	}
}
void initWeapon(struct Collidable* collisionChecker, struct Env* p) {
	struct animation* bulletImage = initAnimation(star, 1);
	struct animation* bullet4Image = loadSDImageSeq("W00.BMP", 2, 30); // fire bomb
	struct animation* bullet2Image = loadSDImageSeq("I0.BMP", 1, 16); // black hole
	struct animation* bullet5Image = loadSDImageSeq("W50.BMP", 2, 2); // freezing bomb

	struct animation* explode0 = initAnimation((int*)explode11, 1);
	addImage(explode0, initAnimation((int*)explode12, 0));
	addImage(explode0, initAnimation((int*)explode13, 0));
	addImage(explode0, initAnimation((int*)explode14, 0));

	int* fireballs = NULL;
	while(!loadSDImage("FB2.BMP", &fireballs)) {
		printf("Loading FB2.BMP error\n");
	}
	struct animation* secondaryBullet4Image = initAnimation(fireballs, 1);

	int* snowballs = NULL;
	while(!loadSDImage("SF2.BMP", &snowballs)){
		printf("Loading SF2.BMP error\n");
	}
	struct animation* secondaryBullet5Image = initAnimation(snowballs, 1);

	weapon1_lvl1 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon1(1, bulletImage, explode0, weapon1_lvl1);

	weapon1_lvl2 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon1(2, bulletImage, explode0, weapon1_lvl2);

	weapon2_lvl1 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon2(1, bullet2Image, explode0, weapon2_lvl1);

	weapon2_lvl2 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon2(2, bullet2Image, explode0, weapon2_lvl2);

	weapon3_lvl1 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon3(1, bulletImage, explode0, weapon3_lvl1);

	weapon3_lvl2 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon3(2, bulletImage, explode0, weapon3_lvl2);

	weapon4_lvl1 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon4(1, bullet4Image, explode0, weapon4_lvl1);

	weapon4_lvl2 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon4(2, bullet4Image, secondaryBullet4Image, weapon4_lvl2);

	weapon5_lvl1 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon5(1, bullet5Image, explode0, weapon5_lvl1);

	weapon5_lvl2 = initBulletCollection(p, collisionChecker);
	initBulletToWeapon5(2, bullet5Image, secondaryBullet5Image, weapon5_lvl2);

}

void printhex(unsigned int score){
const unsigned char segs [16] = {
		0XC0, 0Xf9, 0XA4, 0XB0, 0X99, 0X92,0X82,0XF8,0X80,0X90,0X88, 0X83, 0Xc6, 0XA1, 0X86, 0X8e
};


		IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE,segs[score%10]);


		IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE,segs[(score/10)%10]);

		IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE,segs[(score/100)%10]);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE,segs[(score/1000)%10]);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE,segs[(score/10000)%10]);
		//IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE,0x92);
}
