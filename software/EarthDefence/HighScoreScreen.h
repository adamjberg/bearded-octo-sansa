/*
 * HighScoreScreen.h
 *
 *  Created on: 2013-02-08
 *      Author: adam_000
 */

#ifndef HIGHSCORESCREEN_H_
#define HIGHSCORESCREEN_H_

#include "Game.h"

#define FILE_NAME "HIGH.TXT"
#define NUM_HIGH_SCORES_LENGTH 2
#define NAME_LENGTH 4
#define SCORE_LENGTH 6
#define MAX_NUM_HIGH_SCORES 20
#define CHARS_PER_LINE 9

void start_high_score_screen( unsigned int, struct KeyController* );
#endif /* HIGHSCORESCREEN_H_ */
