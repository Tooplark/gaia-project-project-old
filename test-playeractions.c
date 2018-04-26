#include "mapmanagement.h"
#include "playeractions.h"
#include <assert.h>
 
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
