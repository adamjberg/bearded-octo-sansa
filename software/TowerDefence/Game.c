/*
 * Game.c
 * The main models of tower defence game developed by group 5 in EECE 381 class.
 * test
 */
#include "Game.h"

/*helper function to calculate the distance between two points */
float distance(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return sqrt((float)(dx*dx+dy*dy));
}

/*
 * System clock that run the animation for all objects in the environment
 */
alt_u32 my_alarm_callback (void* paras)
{
	int i;
	for(i = 0; i < ((struct Env*)paras)->size; i++) {
		(*(((struct Env*)paras)->o+i))->animate(pixel_buffer, &((*(((struct Env*)paras)->o+i))->currImg),
					(*(((struct Env*)paras)->o+i))->x, (*(((struct Env*)paras)->o+i))->y, (*(((struct Env*)paras)->o+i))->scale, 1);
	}

	backgroundAnimation(pixel_buffer, ((struct Env*)paras)->coord);

	return alt_ticks_per_second()/10; //100ms
}

void initVGA() {
	// Use the name of your pixel buffer DMA core
	  pixel_buffer =alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma_0");

	  // Set the background buffer address – Although we don’t use thebackground,
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
 * Load bitmap image from SD card.  There is a small issue on the color decoding.
 * file name is required to be upper-case and bitmap pixels has to be even size x even size
 */
bool loadSDImage(char* filename, int** destination) {
	int i, j, bytes = 0, offset = 0, size = 0, byte = 0;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if(file_pointer < 0) {
		return false;
	}

	//Start reading the bitmap header
	while(bytes < 10) {
		alt_up_sd_card_read(file_pointer);
		bytes++;
	}
	offset = alt_up_sd_card_read(file_pointer);
	printf("offset %d\n", offset);
	while(bytes < offset-2){
		if(bytes == 21) {
			size = alt_up_sd_card_read(file_pointer);
			printf("size: %d\n", size);
			*destination = (int*)malloc(size*size*sizeof(int));
		} else if(bytes == 27) {
			printf("bits: %d\n", alt_up_sd_card_read(file_pointer));
		} else if(bytes == 29) {
			printf("compression: %d\n", alt_up_sd_card_read(file_pointer));
		}
		else
			alt_up_sd_card_read(file_pointer);
		bytes++;
	}

	//Start reading the pixel data
	for(j = size-1; j >= 0; j--) {
		for(i = 0; i < size; i++) {
			byte = alt_up_sd_card_read(file_pointer)+alt_up_sd_card_read(file_pointer)*64;
			//printf("%x ", byte);
			*(*destination + j*size+i) = byte;
		}//printf("\n");
	}
	alt_up_sd_card_fclose(file_pointer);

	return true;
}

void backgroundImage() {

	int* pic;
	while(!loadSDImage("E.BMP", &pic));
	draw(pixel_buffer, 210, 200, (int*)pic, 64);
	free(pic);
}
/*
 * check the collision event between each objects in the environment
 */
void checkCollision(struct Env* e) {
	int i, j;
	for(i = 0; i < e->size; i++) {
		for(j = 0; j < e->size; j++) {
			if(i != j) {
				if(distance(e->o[i]->center_x, e->o[i]->center_y, e->o[j]->center_x, e->o[j]->center_y) <= e->o[i]->scale) {
					//e->o[i]->collide(e->o[i], e->o[j]);
					printf("collision!\n");
				}
			}
		}
	}
}
/*
 * Helper method that will print the string on the display
 */
void displayString(char* string, int x, int y) {
	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, string, x, y);
}
/*
 * Main Game Loop
 */
int main()
{
	initVGA();

	char_buffer  = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	alt_up_sd_card_dev *device_reference = NULL;
	int frame = 25;

	while(!loadSDCard(device_reference)) {
		displayString("Please insert the SD card to start", frame, 30);
		frame++;
		if(frame > 60) frame = -5;
		usleep(500000);
	}alt_up_char_buffer_clear(char_buffer);

	struct GameInfo* info = initGameInfo();

	alt_alarm alarm;
	struct Env* p = initEnv();

	backgroundImage();

  struct animation* a = initAnimation((int*)explode11, 1);
  addImage(a, initAnimation((int*)explode12, 0));
  addImage(a, initAnimation((int*)explode13, 0));
  addImage(a, initAnimation((int*)explode14, 0));
  struct Object *tower = initObject(100, 100, 10, a);

  struct animation* b = initAnimation((int*)pacman01, 1);
  addImage(b, initAnimation((int*)pacman02, 0));
  addImage(b, initAnimation((int*)pacman03, 0));
  addImage(b, initAnimation((int*)pacman04, 0));
  struct Object *face = initObject(160, 120, 10, b);

  struct animation* c = initAnimation((int*)explode11, 1);
  addImage(c, initAnimation((int*)explode12, 0));
  addImage(c, initAnimation((int*)explode13, 0));
  addImage(c, initAnimation((int*)explode14, 0));
  struct Object *ani = initObject(150, 50, 10, c);

  struct animation* explode = initAnimation((int*)explode01, 1);
  addImage(explode, initAnimation((int*)explode02, 0));
  addImage(explode, initAnimation((int*)explode03, 0));
  addImage(explode, initAnimation((int*)explode04, 0));
  addImage(explode, initAnimation((int*)explode05, 0));
  struct Object *explosion = initObject(60, 150, 10, explode);


	struct animation* starAnimation = initAnimation((int*)star, 1);
	struct Object* star = initObject(100, 150, 1, starAnimation);
	addToEnv(p, star);

	struct animation* starAnimation1 = initAnimation((int*)explode01, 1);
	addImage(starAnimation1, initAnimation((int*)explode02, 0));
		struct Object* star1 = initObject(150, 180, 10, starAnimation1);
		addToEnv(p, star1);

	int* pic0, pic1, pic2, pic3;
	loadSDImage("A1.BMP", &pic0);
	loadSDImage("A1.BMP", &pic1);
	loadSDImage("A0.BMP", &pic2);
	loadSDImage("A0.BMP", &pic3);
	struct animation* alien0 = initAnimation(pic0, 1);
	addImage(alien0, initAnimation(pic1, 0));
	addImage(alien0, initAnimation(pic2, 0));
	addImage(alien0, initAnimation(pic3, 0));
	struct Object* alien01 = initObject(50, 50, 10, alien0);
	addToEnv(p, alien01);
  addToEnv(p, explosion);
  addToEnv(p, tower);
  addToEnv(p, face);
  addToEnv(p, ani);

  alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 250, 0, 320, 240, 0x7BEF, 0);
  alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 253, 3, 317, 237, 0xBDF7, 0);
  struct Frame* scoreBoard = initFrame(255, 5, 315, 20, 1);
  addToEnv(p, scoreBoard->super);
  updateScoreFrame(scoreBoard, info);

  struct Frame* statusBoard = initFrame(255, 180, 315, 235, 3);
  addToEnv(p, statusBoard->super);
  updateStatusFrame(statusBoard, "Warning!! Pacman is approaching!!!");
  int* ship0, ship1, ship2, ship3, ship4, ship5, ship6, ship7, ship8, ship9;
  loadSDImage("S1.BMP", &ship0);
  loadSDImage("S1_B.BMP", &ship1);
  loadSDImage("S2.BMP", &ship2);
  loadSDImage("S2_B.BMP", &ship9);
  loadSDImage("S3.BMP", &ship3);
  loadSDImage("S3_B.BMP", &ship4);
  loadSDImage("S4.BMP", &ship5);
  loadSDImage("S4_B.BMP", &ship6);
  loadSDImage("S5.BMP", &ship7);
  loadSDImage("S5_B.BMP", &ship8);
  draw(pixel_buffer, 10, 10, ship0, 10);
  draw(pixel_buffer, 20, 10, ship1, 20);
  draw(pixel_buffer, 40, 10, ship2, 10);
  draw(pixel_buffer, 50, 10, ship9, 20);
  draw(pixel_buffer, 70, 10, ship3, 10);
  draw(pixel_buffer, 80, 10, ship4, 20);
  draw(pixel_buffer, 100, 10, ship5, 10);
  draw(pixel_buffer, 110, 10, ship6, 20);
  draw(pixel_buffer, 130, 10, ship7, 10);
  draw(pixel_buffer, 140, 10, ship8, 20);
  alt_alarm_start (&alarm,alt_ticks_per_second(),my_alarm_callback,(void*)p);
  int k = 0;
  while(1) {
	  //background stars
	  setXY(star, rand()%220, rand()%220);

	  checkCollision(p);

	  //Game Processing
	  setXY(face, face->x+1, face->y);
	  if(face->x > 240) face->x = 10;
	  if(k == 200) removeFromEnv(p, tower);
	  info->score++;
	  updateScoreFrame(scoreBoard, info);
	  //nothing running right now..thus slowing down the game speed a bit
	  usleep(100000);
	  k++;
  }
  return 0;
}


