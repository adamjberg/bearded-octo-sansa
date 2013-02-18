/*
 * GameInfo.h
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#ifndef GAMEINFO_H_
#define GAMEINFO_H_
#include "Game.h"

struct GameInfo {
	unsigned int score;
	unsigned int currency;
	unsigned int highest_score;
	unsigned int invadedAlien;
	char* playerName;
	int gameLvl;
	int start;
	bool isEnd;
	bool isWin;
	bool startButton;
};

void initGameInfo();
void loadGameInfo(char*);
void updateScoreMoney(int, int);

#endif /* GAMEINFO_H_ */
