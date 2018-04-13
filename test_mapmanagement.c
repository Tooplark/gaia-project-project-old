#include <string.h>
#include "mapmanagement.h"
#include <assert.h>
 
tile create_test_tile () {
	tile test_tile = malloc(sizeof(tile));
	test_tile->planet_count = 5;
	test_tile->planet_x = malloc(5 * sizeof(int));
	test_tile->planet_y = malloc(5 * sizeof(int));
	test_tile->planet_types = malloc(5 * sizeof(planet));

	int array1[5] = {0, 0, 1, 1, 2};
	int array2[5] = {3, 4, 2, 4, 0};
	planet_type array3[5] = {transdim, transdim, earthlike, white, brown};
	memcpy(test_tile->planet_x, array1, 5 * sizeof(int));
	memcpy(test_tile->planet_y, array2, 5 * sizeof(int));
	memcpy(test_tile->planet_types, array3, 5 * sizeof(planet));

	return test_tile;
}

void test_rotate_tile() {
	tile test_tile = create_test_tile();

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
	hex current_hex;
	hex* map = malloc(25 * sizeof(hex));
	tile test_tile = create_test_tile();
	add_tile_to_map(map, 5, test_tile, 2, 2);

	/* The center of the tile is at 2,2. Of the 25 spaces, 6 don't
	 * correspond to proper hexes: (0,0), (0,1), (1,0), (3,4), (4,3), 
	 * and (4,4). Those should be NULL; the rest should match test_tile.
	 * First we test the locations that should have planets.
	 * Planets should be at (0,3), (0,4), (1,2), (1,4), (2,0)
	 */
	current_hex = get_hex_at_coords(map, 5, 0, 3);
	assert(!current_hex->isSpace);
	assert(current_hex->planet->color == transdim);
	assert(current_hex->planet->owner == 0);
	assert(current_hex->planet->building == none);

	current_hex = get_hex_at_coords(map, 5, 0, 4);
	assert(!current_hex->isSpace);
	assert(current_hex->planet->color == transdim);
	assert(current_hex->planet->owner == 0);
	assert(current_hex->planet->building == none);

	current_hex = get_hex_at_coords(map, 5, 1, 2);
	assert(!current_hex->isSpace);
	assert(current_hex->planet->color == earthlike);
	assert(current_hex->planet->owner == 0);
	assert(current_hex->planet->building == none);

	current_hex = get_hex_at_coords(map, 5, 1, 4);
	assert(!current_hex->isSpace);
	assert(current_hex->planet->color == white);
	assert(current_hex->planet->owner == 0);
	assert(current_hex->planet->building == none);

	current_hex = get_hex_at_coords(map, 5, 2, 0);
	assert(!current_hex->isSpace);
	assert(current_hex->planet->color == brown);
	assert(current_hex->planet->owner == 0);
	assert(current_hex->planet->building == none);

	current_hex = get_hex_at_coords(map, 5, 0, 0);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 0, 1);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 1, 0);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 3, 4);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 4, 4);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 4, 3);
	assert(current_hex == NULL);

	current_hex = get_hex_at_coords(map, 5, 0, 2);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 1, 3);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 2, 1);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 2, 2);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 2, 3);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 2, 4);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 3, 0);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 3, 1);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 3, 2);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 3, 3);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 4, 0);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 4, 1);
	assert(current_hex->isSpace);

	current_hex = get_hex_at_coords(map, 5, 4, 2);
	assert(current_hex->isSpace);

}

void test_initialize_map() {
	hex* map = initialize_2p_map();
}

void main (int argc, char **argv) {
	test_rotate_tile();
	test_add_tile_to_map();
	test_initialize_map();
}
