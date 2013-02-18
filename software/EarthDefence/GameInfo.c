/*
 * GameInfo.c
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#include "GameInfo.h"
/*
 * Constructor
 */
void initGameInfo() {
	info.score = info.gameLvl = 0;
	info.highest_score = 0;
	info.currency = 20000;
	info.invadedAlien = 0;
	info.start = 1;
	info.isEnd = false;
	info.isWin = false;
	info.startButton = false;
}/*
void setObserver(struct Frame* obs) {
	info.observer = obs;
}*/
void loadGameInfo(char* filename) {

}

void updateScoreMoney(int addScore, int income) {
	info.score += addScore;
	info.currency += income;
	updateScoreFrame();

	printhex(info.score);

	  char second_row2[15];
	  alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 0);
	  sprintf(second_row2, "$$ %d  ", info.currency);
	  alt_up_character_lcd_string(char_lcd_dev, second_row2);
}
