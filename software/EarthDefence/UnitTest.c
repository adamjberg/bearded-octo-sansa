#include "UnitTest.h"

/*
 * For SpaceShip Class
 *//*
void testKillSpaceShip(struct animation* a, struct Env* env, struct Collidable* collisionChecker, struct Frame* gamePanel) {
	struct SpaceShip** shipCollection = (struct SpaceShip**)malloc(sizeof(struct SpaceShip*)*30);
	int i = 0;
	for(i = 0; i < 30; i++) {
		shipCollection[i] = initSpaceShip(i*5,i*5, 20, 1, 1, 20.0, a);
		addObjToCollide(collisionChecker, shipCollection[i]->super);
		addToEnv(env, shipCollection[i]->super);
	}
	clearMBoard(gamePanel);
	writeMSG("Successful added all spaceships!");
	usleep(15000000); //wait 10 seconds

	for(i = 0; i < 30; i++) {
		removeFromCollide(collisionChecker, shipCollection[i]->super);
		killSpaceShip((void*)shipCollection[i]);
	}

	  clearMBoard(gamePanel);
	  writeMSG("Successful removed all spaceships!");
}
*/
/*
 * For QuardTree Class
 */
/*
void testInBoundary(struct Frame* gamePanel) {

	clearMBoard(gamePanel);
	struct Range* r = initRange(20, 20, 20, 30);
	struct Object* obj = initObject(50, 60, 10, NULL, NULL);
	struct Object* obj1 = initObject(40, 40, 20, NULL, NULL);
	struct Object* obj2 = initObject(0, 0, 10, NULL, NULL);
	struct Object* obj3 = initObject(0, 0, 30, NULL, NULL);
	if(!inBoundary(r, obj)) {
		writeMSG("obj test boundary successful!");
	} if(inBoundary(r, obj1))
		writeMSG("obj1 test boundary successful!");
	if(!inBoundary(r, obj2)) {
			writeMSG("ob2 test boundary successful!");
		} if(inBoundary(r, obj3))
			writeMSG( "obj3 test boundary successful!");

}

void testInsertTree() {
	int i;
	struct QuardTree* root = initQuardTree(initRange(0, 0, 240, 240));
	for(i = 0; i < 30; i++) {
		insertObj(root, initObject(rand()%240, rand()%240, rand()%30, NULL, NULL));
	}
}

void testQueryTree() {
	int i;
	struct QuardTree* root = initQuardTree(initRange(0, 0, 240, 240));
	struct Object** list = (struct Object**)malloc(sizeof(struct Object*)*30);
	int size = 0;
	for(i = 0; i < 30; i++) {
		insertObj(root, initObject(rand()%240, rand()%240, rand()%30, NULL, NULL));
	}
	size = query(root, initRange(30, 80, 20, 30), list);
	printf("%d objects collides with range\n", size);
	for(i = 0; i < size; i++) {
		printf("(%d, %d) collides with range with size %d\n", list[i]->x, list[i]->y, list[i]->scale);
	}
	clearTree(root);
}
*/
