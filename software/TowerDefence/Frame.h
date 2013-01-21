/*
 * Frame.h
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#ifndef FRAME_H_
#define FRAME_H_
#include "Game.h"
#include "GameInfo.h"
struct Frame {
	int ltx, lty, rbx, rby;
	struct Object* super;
};

struct Frame* initFrame(int, int, int, int, int);
void drawFrame(struct Frame*, int);
void updateScoreFrame(struct Frame*, struct GameInfo*);
void updateControlFrame(struct Frame*);
void updateStatusFrame(struct Frame*, char*);
#endif /* FRAME_H_ */
