#include <stdbool.h>
#include <stdlib.h>
/* A bunch of enums listing the types of planets and buildings and stuff. */
typedef enum planet_type_enum {
	brown,
	yellow,
	orange,
	red,
	earthlike,
	ice,
	metal,
	gaia,
	transdim
} planet_type;
	
/* We handle gaiaformers as buildings, since they can't coexist w/ buildings */
typedef enum building_enum {
	none,
	mine,
	trading,
	stronghold,
	research,
	institute,
	gaiaformer
} building;

/* A planet has a color and possibly a building belonging to a player. */
typedef struct planet {
	planet_type color;
	int owner;
	building b;
} *planet;

/* The map is made up of 7 or 10 hexagonal tiles, each containing 19 individual
 * hexes. After the hex tile rolls, we dump everything into an array.
 *
 * A hex can be an empty space hex, or it can contain a planet. Space hexes
 * can have satellites (up to one per player). The contents of planets are
 * handled in the planet struct.
 *
 * A hex also has an x and y coordinate (using the axial coordinate system,
 * mucho thankos to https://redblobgames.com/grids/hexagons).
 * Our hexagons are flat side on top, so y is vertical position and x is 
 * down and right.
 */
typedef struct hex {
	bool isSpace;
	union {
		bool satellites[4];
		planet p;
	};
} *hex;

/* I gave in and used a struct to store the contents of each tile, for clarity.
 * Planet_count is the number of planets on the tile.
 * Planet_x lists the x coordinates of each planet on the tile, planet_y for y.
 * Planet_types lists the types of planets on the tile.
 * The idea is that planet_x[1],planet_y[1], planet_types[1] will tell you
 * all of the information about one planet.
 */
typedef struct tile {
	int planet_count;
	int* planet_x;
	int* planet_y;
	int* planet_types;
}
// Never mind this junk, we're using a struct for tiles now.
// Keepin this here in case i change my mind again.
/* We define the contents of the tiles that make up the game board here.
 * That is, this is a concise specification of the locations of the planets
 * on each tile.
 * The format is an array of triples. The first two numbers in each triple 
 * are the axial coordinates of a planet, and the last is the planet type.
 *
 * 11, 12, and 13 are the 2-p-only back sides to 5, 6, and 7 respectively.
 */
/*
int tile1[18] = {0, 3, 1, 1, 2, 0, 2, 4, 3, 3, 1, 4, 3, 3, 2, 4, 1, 8};
int tile2[21] = {1, 1, 2, 1, 3, 0, 1, 4, 3, 2, 0, 6, 3, 1, 5, 3, 3, 8, 4, 1, 1};
int tile3[18] = {1, 2, 7, 1, 4, 4, 2, 0, 8, 2, 4, 1, 3, 2, 5, 4, 1, 6};
int tile4[18] = {0, 3, 5, 1, 3, 2, 2, 0, 6, 2, 1, 3, 3, 2, 0, 4, 2, 4};
*/
/* for now we're just gonna get 2-p running
int tile5[18] = {
int tile6[18] =
int tile7[18] =
int tile8[18] =
int tile9[18] =
int tile10[18] =
*/
/*
int tile11[15] = {1, 2, 7, 1, 4, 2, 2, 0, 5, 4, 0, 8, 4, 1, 3};
int tile12[15] = {2, 3, 7, 3, 0, 8, 3, 1, 4, 3, 3, 8, 4, 2, 1};
int tile13[15] = {0, 2, 8, 1, 3, 7, 2, 1, 7, 2, 4, 6, 3, 2, 0};
*/

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
	return map;
}

/* This creates and returns an array of 7 tiles corresponding to the tiles
 * used in the 2-player Gaia map.
 */
tile* create_2p_tiles() {
	int i;
	tile tile_array[7];
	tile_array[0].planet_count = 6;
	tile_array[1].planet_count = 7;
	tile_array[2].planet_count = 6;
	tile_array[3].planet_count = 6;
	tile_array[4].planet_count = 5;
	tile_array[5].planet_count = 5;
	tile_array[6].planet_count = 5;

	for(i=0; i < 7; i++) {
		tile_array[i].planet_x = malloc(tile_array[i].planet_count
				* sizeof(int));
		tile_array[i].planet_y = malloc(tile_array[i].planet_count
				* sizeof(int));
		tile_array[i].planet_types = malloc(tile_array[i].planet_count
				* sizeof(int));
	}

	tile_array[0].planet_x = {0, 1, 2, 3, 3, 4};
	tile_array[0].planet_y = {3, 2, 4, 1, 3, 1};
	tile_array[0].planet_types = {yellow, brown, red, 
		earthlike, orange, transdim};

	tile_array[1].planet_x = {1, 1, 1, 2, 3, 3, 4};
	tile_array[1].planet_y = {1, 3, 4, 0, 1, 3, 1};
	tile_array[1].planet_types = {orange, brown, red, metal, 
		white, transdim, yellow};

	tile_array[2].planet_x = {1, 1, 2, 2, 3, 4};
	tile_array[2].planet_y = {2, 4, 0, 4, 2, 1};
	tile_array[2].planet_types = {gaia, earthlike, transdim, 
		yellow, white, metal};

	tile_array[3].planet_x = {0, 1, 2, 2, 3, 4};
	tile_array[3].planet_y = {3, 3, 0, 1, 2, 2};
	tile_array[3].planet_types = {white, orange, metal, red, 
		brown, earthlike};

	tile_array[4].planet_x = {1, 1, 2, 4, 4};
	tile_array[4].planet_y = {2, 4, 0, 0, 1};
	tile_array[4].planet_types = {gaia, orange, white, transdim, red};

	tile_array[5].planet_x = {2, 3, 3, 3, 4};
	tile_array[5].planet_y = {3, 0, 1, 3, 2};
	tile_array[5].planet_types = {gaia, transdim, earthlike, 
		transdim, yellow};

	tile_array[6].planet_x = {0, 1, 2, 2, 3};
	tile_array[6].planet_y = {2, 3, 1, 4, 2};
	tile_array[6].planet_types = {transdim, gaia, gaia, metal, brown};

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
void rotate_tile(tile* tile, int r) {
	int temp_x, temp_y, temp_z, i;
	for(i=0; i < tile.planet_count; i++){
		temp_x = tile.planet_x[i] - 2;
		temp_y = tile.planet_y[i] - 2;	
		if (r % 2 = 1) {
			temp_x = -temp_x;
			temp_y = -temp_y;
		}
		if (r % 3 = 1) {
			temp_z = temp_y;
			temp_y = temp_x;
			temp_x = -temp_z -temp_y;
		}
		else if (r % 3 = 2) {
			temp_z = temp_x;
			temp_x = temp_y;
			temp_y = -temp_z -temp_x;
		}
		tile.planet_x[i] = temp_x + 2;
		tile.planet_y[i] = temp_y + 2;
	}
}

/* This is a subroutine for fill_map that puts the details of the specified
 * map tile into the map (that is, setting the space and planet fields).
 * It will place the tile so that the center is in the position specified by
 * tile_center_x and tile_center_y.
 * The spaces occupied by the tile will be all spaces within 2 of the center;
 * that is, all spaces (a,b) where |a-x|, |b-y|, and |a+b-x-y| are at most 2.
 */
void add_tile_to_map(hex* map, int* tile, int tile_center_x, int tile_center_y){
	/* i, j, and k are the coordinates of the hexes we want to modify.
	 * We loop through i and j, then check that k satisfies |k-z|=0.
	 * If it doesn't satisfy that, then that hex isn't in the tile.
	 * k and z are just 
	 */
	//TODO delete k and z if this works
	int i, j, k, z;
	for (i = -2; i <= 2; i++) {
		for (j = -2; j <= 2; j++) {
			z = tile_center_x + tile_center_y;
			k = i + j;
			if (z - k <= 2 && k - z <= 2) {
				//TODO actually do stuff
				printf("doing stuff at (%d,%d)\n", 
					tile_center_x + i, tile_center_y + j);
			}
		}
	}
}

void fill_map_two(hex* map) {
	int i;
	int tile_order[7];
	int tile_rotations[7];

	/* eventually this will be a random permutation; for now it's 
	 * always the same.
	 */
//	srand(time(NULL));
//	rando_perm(7, tile_order, 7);
//	for(i=0; i<7; i++) tile_rotations[i] = rand() % 6;
	for (i=0; i < 5; i++) {
		tile_order[i] = i;
		tile_rotations[i] = i;
	}
	//this gross stuff is to use the 2-p-only tiles, 11 12 13.
	for (i=5; i < 7; i++) {
		tile_order[i] = i+6;
		tile_rotations[i] = i % 6;
	}

	tile* tile_array = create_2p_tiles();
	
	/* The center space of the center tile is 7,7. The other tile centers
	 * are off by 3 in one direction, and 2 in another. That gives 
	 * (starting with the 'left' tile and going clockwise) (2,9), (5,4),
	 * (10,2), (14,5), (9, 10), (4,12).
	 */
	//TODO: generalize this?
	int[14] centers = {7,7, 2,9, 5,4, 10,2, 14,5, 9,10, 4,12};
	for (i=0; i < 7; i++) {
		rotate_tile(tile_array[tile_order[i]], tile_rotations[i]);
		add_tile_to_map(map, tile_array[tile_order[i]], 
				centers[2*i], centers[2*i+1]);	
	}
	
}
