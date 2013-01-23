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
struct GameInfo* initGameInfo() {
	struct GameInfo* g = (struct GameInfo*)malloc(sizeof(struct GameInfo));
	g->score = g->gameLvl = 0;
	return g;
}

void loadGameInfo(struct GameInfo* g, char* filename) {

}
