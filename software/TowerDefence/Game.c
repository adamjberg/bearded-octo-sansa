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
 * Load bitmap image from SD card.  The pixel data in the file still need to be decoded
 * because it was encoded in the bitmap file structure for compression.
 */
bool loadSDImage(char* filename, int** destination) {
	int i, j, bytes = 0, offset = 0, size = 0, byte = 0;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if(file_pointer < 0) return false;

	//Start reading the bitmap header
	while(bytes < 10) {
		alt_up_sd_card_read(file_pointer);
		bytes++;
	}
	offset = alt_up_sd_card_read(file_pointer);
	while(bytes < offset){
		if(bytes == 21) {
			size = alt_up_sd_card_read(file_pointer);
			*destination = (int*)malloc(size*size*sizeof(int));
		}
		else
			alt_up_sd_card_read(file_pointer);
		bytes++;
	}

	//Start reading the pixel data
	for(i = size-1; i >= 0; i--) {
		for(j = 0; j < size; j+=2) {
			byte = alt_up_sd_card_read(file_pointer);
			*(destination + i*size+j) = byte;
			*(destination + (i)*size+(j+1)) = byte;
		}
	}
	/*for(i = size*size-1; i >=0; i-=2) {
		byte = alt_up_sd_card_read(file_pointer);
		*(*destination + i) = byte / 16;
		*(*destination + i+1) = byte %256;
		if(*(*destination + i) == 15) *(*destination + i) = 0x7C00;
		switch(*(*destination + i)) {
			case 15:
				*(*destination + i) = 0xFFFF; //white
				break;
			case 14:
				*(*destination + i) = 0x52AA;
				break;
			case 13:
				*(*destination + i) = 0xA554;
				break;
			case 12:
				*(*destination + i) = 0x0554;
				break;
			case 9:
				*(*destination + i) = 0xF81F; //pink
				break;
			case 8:
				*(*destination + i) = 0x07E0; //green
				break;
			case 7:
				*(*destination + i) = 0xBDF7; //light gray
				break;
			case 6:
				*(*destination + i) = 0x7BEF; //dark gray
				break;
			case 5:
				*(*destination + i) = 0x07ff; //Cyan
			case 4:
				*(*destination + i) = 0x001F; //Blue
				break;
			case 3:
				*(*destination + i) = 0xFFE0; //Yello
				break;
			case 2:
				*(*destination + i) = 0x07E0; //green
				break;
			case 1:
				*(*destination + i) = 0xB800;
				break;
			case 0:
				*(*destination + i) = 0x0000;
				break;
			default:
				break;
		}
		switch(*(*destination + i+1)) {
					case 15:
						*(*destination + i+1) = 0xFFFF; //white
						break;
					case 14:
						*(*destination + i+1) = 0x52AA;
						break;
					case 13:
						*(*destination + i+1) = 0xA554;
						break;
					case 12:
						*(*destination + i+1) = 0x0554;
						break;
					case 9:
						*(*destination + i+1) = 0xF81F; //pink
						break;
					case 8:
						*(*destination + i) = 0x07E0; //green
						break;
					case 7:
						*(*destination + i+1) = 0xBDF7; //light gray
						break;
					case 6:
						*(*destination + i+1) = 0x7BEF; //dark gray
						break;
					case 5:
						*(*destination + i+1) = 0x07FF; //Cyan
						break;
					case 4:
						*(*destination + i+1) = 0x001F; //Blue
						break;
					case 3:
						*(*destination + i+1) = 0xFFE0; //yellow
						break;
					case 2:
						*(*destination + i+1) = 0x07E0; //green
						break;
					case 1:
						*(*destination + i+1) = 0xB800;
						break;
					case 0:
						*(*destination + i+1) = 0x0000;
						break;
					default:
						break;
				}
	}*/
	return true;
}

void backgroundImage() {

	draw(pixel_buffer, 220, 210, (int*)earth, 64);
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
	char_buffer  = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	alt_up_sd_card_dev *device_reference = NULL;
	//int* pic;
	int frame = 25;

	while(!loadSDCard(device_reference)) {
		displayString("Please insert the SD card to start", frame, 30);
		frame++;
		if(frame > 60) frame = -5;
		usleep(500000);
	}alt_up_char_buffer_clear(char_buffer);

	struct GameInfo* info = initGameInfo();
	//loadSDImage("EARTH.BMP", &pic);
	initVGA();

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

  alt_alarm_start (&alarm,alt_ticks_per_second(),my_alarm_callback,(void*)p);
  int k = 0;
  while(1) {
	  //background stars
	  setXY(star, rand()%240, rand()%240);

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


