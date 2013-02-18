/*
 * TowerPlacer.h
 *
 *  Created on: 2013-01-25
 *      Author: adam_000
 */

#ifndef TOWERPLACER_H_
#define TOWERPLACER_H_

#import "animation.h"
#import "SpaceShip.h"

struct TowerPlacer {
	struct SpaceShip* currTower;
	int active;
};

struct TowerPlacer* initTowerPlacer();
void stopTowerPlacement(struct TowerPlacer*);
int attemptPlaceTower(struct TowerPlacer*);
void killTowerPlacer(void);
void selectTower(struct TowerPlacer*, struct SpaceShip*);
void updateTowerPosition(struct TowerPlacer*, int, int);

#endif /* TOWERPLACER_H_ */
