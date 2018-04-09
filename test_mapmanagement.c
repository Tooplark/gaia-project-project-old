#include "mapmanagement.h"
#include <assert.h>
 
void test_rotate() {
	tile test_tile = malloc(sizeof(tile));
	test_tile->planet_count = 5;
	test_tile->planet_x = malloc(5 * sizeof(int));
	test_tile->planet_y = malloc(5 * sizeof(int));
	test_tile->planet_planet = malloc(5 * sizeof(planet));

	memcpy(test_tile->planet_x, {0, 0, 1, 1, 2}, 5 * sizeof(int));
	memcpy(test_tile->planet_y, {3, 4, 2, 4, 0}, 5 * sizeof(int));
	memcpy(test_tile->planet_types, {transdim, transdim, earthlike, 
			white, brown}, 5 * sizeof(planet));

	/* After rotating it by 2, the planet coordinates should be
	 * (3,0), (2,0), (3,1), (1,1), (4,2).
	 */
	rotate_tile(test_tile, 2);
	assert(test_tile->planet_x[0] == 3);
	assert(test_tile->planet_y[0] == 0);
	assert(test_tile->planet_x[1] == 2);
	assert(test_tile->planet_y[1] == 0);
	assert(test_tile->planet_x[2] == 3);
	assert(test_tile->planet_y[2] == 1);
	assert(test_tile->planet_x[3] == 1);
	assert(test_tile->planet_y[3] == 1);
	assert(test_tile->planet_x[4] == 4);
	assert(test_tile->planet_y[4] == 2);
//		3, 1, 2, 3, 3, 7, 4, 3, 6};

}

void test_add_tile_to_map() {
	/* We create a 5x5 map and a tile with 8 planets on it. 
	 * We add the tile to the map, and test that the planets appear
	 * in the correct locations.
	 */
	int** map = malloc(25 * sizeof(int));
	int test_tile[24] = {0, 3, 8, 0, 4, 8, 1, 2, 4, 1, 4, 5, 2, 0, 0,
		3, 1, 2, 3, 3, 7, 4, 3, 6};
	add_tile_to_map(map, test_tile, 3, 3);
	assert();
}

void test_fill_map() {
}

void main (int argc, char **argv) {
	
}
