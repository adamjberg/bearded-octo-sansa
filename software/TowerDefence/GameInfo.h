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
	int score;
	int currency;
	int highest_score;
	char* playerName;
	int gameLvl;
};

struct GameInfo* initGameInfo();
void loadGameInfo(struct GameInfo*, char*);

#endif /* GAMEINFO_H_ */
