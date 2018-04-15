#include <stdio.h> //get ridda dis
#include <string.h>
#include "mapmanagement.h"
#include <stdbool.h>
#include <stdlib.h>

/* Constructor for planets. Nothing to see here. */
planet new_planet (){
	planet p = malloc(sizeof(struct planet));
	return p;
}

/* Constructor for hexes. Whee. */
hex new_hex () {
	hex h = malloc(sizeof(struct hex));
	return h;
}

/* The 3-4p map is very irregular. It has 20 columns in one direction, 18 in
 * another, and 17 in the third. We use the two shorter dimensions for our 
 * coordinate system.
 */
hex *initialize_map () {
	hex *map = malloc(18 * 17 * sizeof(hex));
	return map;
}

/* The 2p map has 15 x columns and 15 y columns. (7,7) is the center of 
 * the center tile. There are only 7 x 19 = 133 hexes used of the 225
 * we have room for, but that's not a lot of actual space wasted.
 */
hex *initialize_2p_map() {
	hex* map = malloc(15 * 15 * sizeof(hex));
//	printf("seg4\n");
	fill_2p_map(map);
//	printf("seg14\n");
	return map;
}

/* This creates and returns an array of 7 tiles corresponding to the tiles
 * used in the 2-player Gaia map.
 * TODO: make less grossugly?
 */
tile* create_2p_tiles() {
//	printf("seg6\n");
	int i;
	tile* tile_array = malloc(7 * sizeof(tile));
	for (i = 0; i < 7; i++) {
		tile_array[i] = malloc(sizeof(struct tile));
	}
	tile_array[0]->planet_count = 6;
	tile_array[1]->planet_count = 7;
	tile_array[2]->planet_count = 6;
	tile_array[3]->planet_count = 6;
	tile_array[4]->planet_count = 5;
	tile_array[5]->planet_count = 5;
	tile_array[6]->planet_count = 5;

//	printf("seg7\n");
	for(i=0; i < 7; i++) {
		tile_array[i]->planet_x = malloc(tile_array[i]->planet_count
				* sizeof(int));
		tile_array[i]->planet_y = malloc(tile_array[i]->planet_count
				* sizeof(int));
		tile_array[i]->planet_types = malloc(tile_array[i]->planet_count
				* sizeof(planet));
	}

//	printf("seg8\n");
	memcpy(tile_array[0]->planet_x, (int[]) {0, 1, 2, 3, 3, 4},
			6 * sizeof(int));
	memcpy(tile_array[0]->planet_y, (int[]) {3, 2, 4, 1, 3, 1}, 6 * sizeof(int));
	memcpy(tile_array[0]->planet_types, (int[]) {yellow, brown, red, 
		earthlike, orange, transdim}, 6 * sizeof(planet));

	memcpy(tile_array[1]->planet_x, (int[]) {1, 1, 1, 2, 3, 3, 4}, 7 * sizeof(int));
	memcpy(tile_array[1]->planet_y, (int[]) {1, 3, 4, 0, 1, 3, 1}, 7 * sizeof(int));
	memcpy(tile_array[1]->planet_types, (int[]) {orange, brown, red, metal, 
		white, transdim, yellow}, 7 * sizeof(planet));

	memcpy(tile_array[2]->planet_x, (int[]) {1, 1, 2, 2, 3, 4}, 6 * sizeof(int));
	memcpy(tile_array[2]->planet_y, (int[]) {2, 4, 0, 4, 2, 1}, 6 * sizeof(int));
	memcpy(tile_array[2]->planet_types, (int[]) {gaia, earthlike, transdim, 
		yellow, white, metal}, 6 * sizeof(planet));

	memcpy(tile_array[3]->planet_x, (int[]) {0, 1, 2, 2, 3, 4}, 6 * sizeof(int));
	memcpy(tile_array[3]->planet_y, (int[]) {3, 3, 0, 1, 2, 2}, 6 * sizeof(int));
	memcpy(tile_array[3]->planet_types, (int[]) {white, orange, metal, red, 
		brown, earthlike}, 6 * sizeof(planet));

	memcpy(tile_array[4]->planet_x, (int[]) {1, 1, 2, 4, 4}, 5 * sizeof(int));
	memcpy(tile_array[4]->planet_y, (int[]) {2, 4, 0, 0, 1}, 5 * sizeof(int));
	memcpy(tile_array[4]->planet_types, (int[]) {gaia, orange, white, transdim, red}, 5 * sizeof(planet));

	memcpy(tile_array[5]->planet_x, (int[]) {2, 3, 3, 3, 4}, 5 * sizeof(int));
	memcpy(tile_array[5]->planet_y, (int[]) {3, 0, 1, 3, 2}, 5 * sizeof(int));
	memcpy(tile_array[5]->planet_types, (int[]) {gaia, transdim, earthlike, 
		transdim, yellow}, 5 * sizeof(planet));

	memcpy(tile_array[6]->planet_x, (int[]) {0, 1, 2, 2, 3}, 5 * sizeof(int));
	memcpy(tile_array[6]->planet_y, (int[]) {2, 3, 1, 4, 2}, 5 * sizeof(int));
	memcpy(tile_array[6]->planet_types, (int[]) {transdim, gaia, gaia, metal, brown}, 5 * sizeof(planet));

//	printf("seg9\n");
	return tile_array;
}


/* Rotates the specified tile r times. A single rotation, in this case, means
 * turning the tile once clockwise. r should be between 0 and 5; nothing should 
 * break if outside this range, though. NO PROMISES.
 * The tile coordinates are (0,0) through (4,4). We need to rotate around the
 * center hex (2,2). This means our first step is to subtract 2 from each
 * coordinate so we're dealing with (-2,-2) through (2,2).
 * Rotating hex coordinates has two parts: exchanging x,y,z, and flipping the
 * signs. An odd number of rotations flips the signs of all the coordinates;
 * one or four rotations swaps x->y->z, and two or five rotations swaps 
 * x->z->y. Zero or six rotations, obviously, does nothing.
 * TODO: make sure this is clockwise and not counterclockwise
 */
void rotate_tile(tile tile, int r) {
	int temp_x, temp_y, temp_z, i;
	for(i=0; i < tile->planet_count; i++){
/*		printf("Checking tile %d:\n",i);
		printf("planet_x pointer %p\n", (void *) tile->planet_x);
		printf("x coordinate %d\n",tile->planet_x[i]);
		printf("y coordinate %d\n");
*/		temp_x = tile->planet_x[i] - 2;
		temp_y = tile->planet_y[i] - 2;	
//		printf("bluh3\n");
		if (r % 2 == 1) {
			temp_x = -temp_x;
			temp_y = -temp_y;
		}
		if (r % 3 == 2) {
			temp_z = temp_y;
			temp_y = temp_x;
			temp_x = -temp_z -temp_y;
		}
		else if (r % 3 == 1) {
			temp_z = temp_x;
			temp_x = temp_y;
			temp_y = -temp_z -temp_x;
		}
//		printf("bluh2\n");
		tile->planet_x[i] = temp_x + 2;
		tile->planet_y[i] = temp_y + 2;
	}
}

/* This is a subroutine for fill_map that puts the details of the specified
 * map tile into the map (that is, setting the space and planet fields).
 * It will place the tile so that the center is in the position specified by
 * x and y.
 * The spaces occupied by the tile will be all spaces within 2 of the center;
 * that is, all spaces (a,b) where |a-x|, |b-y|, and |a+b-x-y| are at most 2.
 *
 */
void add_tile_to_map(hex* map, int map_width, tile tile, int x, int y){

	int j, k;
	hex current_hex;
	planet current_planet;

//	printf("addtileseg x=%d y=%d\n", x, y);
	/* Set all spaces touched by this tile to empty space
	 */
	for (j = -2; j < 3; j++) {
		for (k = -2; k < 3; k++) {
			if (- j - k <= 2 && j + k <= 2) {
				current_hex = new_hex();
				current_hex->isSpace = true;
//				printf("\tadding to %d, %d (actual location %d)\n", x+j, y+k, (x+j)*map_width + y + k);
				map[(x + j) * map_width + y + k] =
					current_hex;
			}
		}
	}

//	printf("seg10\n");
	/* Fill in the planets 
	 */
	for (k = 0; k < tile->planet_count; k++) {
		current_hex = new_hex();
					
		current_hex->isSpace = false;
		current_planet = new_planet();
		current_planet->color = tile->planet_types[k];
		current_planet->owner = 0;
		current_planet->building = none;
		current_hex->planet = current_planet;
/*		printf("planetseg at %d,%d (%d)\n", x + tile->planet_x[k] - 2,
				y + tile->planet_y[k] - 2, (x - 2 +
					tile->planet_x[k]) * map_width + 
				y + tile->planet_y[k] - 2);
*/		map[(x + tile->planet_x [k] - 2) * map_width + y + 
			tile->planet_y[k] - 2] = current_hex;
	}
//	printf("seg11\n");
}

void fill_2p_map(hex* map) {
	int i;
	int tile_order[7];
	int tile_rotations[7];

	/* Generates a random permutation of the tiles, and a list of 
	 * random rotations for them.
	 *
	srand(time(NULL));
	rando_perm(7, tile_order, 7);
	for(i=0; i<7; i++) tile_rotations[i] = rand() % 6;
	*/
	
	// Non-random setup here, for temporary / testing purposes: 
	
	for (i=0; i < 7; i++) {
		tile_order[i] = i;
		tile_rotations[i] = 0;
	}
	

	tile* tile_array = create_2p_tiles();
	
	/* The center space of the center tile is 7,7. The other tile centers
	 * are off by 3 in one direction, and 2 in another. That gives 
	 * (starting with the 'left' tile and going clockwise) (2,9), (5,4),
	 * (10,2), (12,5), (9, 10), (4,12).
	 */
	//TODO: generalize this?
//	printf("seg5\n");
	int centers[14] = {7,7, 2,9, 5,4, 10,2, 12,5, 9,10, 4,12};
	for (i=0; i < 7; i++) {
//		printf("i=%dseg\n",i);
		rotate_tile(tile_array[tile_order[i]], tile_rotations[i]);
//		printf("loopseg%d\n", i);
		add_tile_to_map(map, 15, tile_array[tile_order[i]], 
				centers[2*i], centers[2*i+1]);	
	}
//	printf("seg15\n");
}

hex get_hex_at_coords(hex* map, int width, int x, int y) {
	return map[x * width + y];
}
