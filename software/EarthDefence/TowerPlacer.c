/*
 * TowerPlacer.c
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#import "TowerPlacer.h"

struct TowerPlacer* initTowerPlacer(void) {
	struct TowerPlacer* t = (struct TowerPlacer*)malloc(sizeof(struct TowerPlacer));
	t->active = 0;
	return t;
}

/**
 * Stops the tower placement phase
 * TODO: This should actually remove the current tower from the env because it
 * hasn't been placed yet.
 */
void stopTowerPlacement(struct TowerPlacer* towerPlacer) {
	if( towerPlacer->active == 0 ) {
		return;
	}
	towerPlacer->active = 0;
}

/**
 * This will be the function to check if a tower placement is valid and "place" the tower
 * at the current cursor position
 */
int attemptPlaceTower(struct TowerPlacer* towerPlacer) {
	towerPlacer->active = 0;
	towerPlacer->currTower = NULL;
	return 1;
}

/**
 * TODO: Memory cleanup should happen here
 */
void killTowerPlacer(void) {

}

/**
 * Function to call when a tower is selected from the side panel
 */
void selectTower(struct TowerPlacer* towerPlacer, struct SpaceShip* spaceShip) {
	if( towerPlacer->active == 1 )
		return;
	towerPlacer->currTower = spaceShip;
	towerPlacer->active = 1;
}

/**
 * Update function to reposition the currently selected tower
 */
void updateTowerPosition(struct TowerPlacer* towerPlacer, int x, int y) {
	if( towerPlacer->active == 0 )
		return;
	setXY( towerPlacer->currTower->super, x - 10, y-10 );
}
