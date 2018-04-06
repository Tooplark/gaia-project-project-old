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


/* We define the contents of the tiles that make up the game board here.
 * That is, this is a concise specification of the locations of the planets
 * on each tile.
 * The format is an array of triples. The first two numbers in each triple 
 * are the axial coordinates of a planet, and the last is the planet type.
 *
 * 11, 12, and 13 are the 2-p-only back sides to 5, 6, and 7 respectively.
 */
int tile1[18] = {0, 3, 1, 1, 2, 0, 2, 4, 3, 3, 1, 4, 3, 3, 2, 4, 1, 8};
int tile2[21] = {1, 1, 2, 1, 3, 0, 1, 4, 3, 2, 0, 6, 3, 1, 5, 3, 3, 8, 4, 1, 1};
int tile3[18] = {1, 2, 7, 1, 4, 4, 2, 0, 8, 2, 4, 1, 3, 2, 5, 4, 1, 6};
int tile4[18] = {0, 3, 5, 1, 3, 2, 2, 0, 6, 2, 1, 3, 3, 2, 0, 4, 2, 4};
/* for now we're just gonna get 2-p running
int tile5[18] = {
int tile6[18] =
int tile7[18] =
int tile8[18] =
int tile9[18] =
int tile10[18] =
*/
int tile11[15] = {1, 2, 7, 1, 4, 2, 2, 0, 5, 4, 0, 8, 4, 1, 3};
int tile12[15] = {2, 3, 7, 3, 0, 8, 3, 1, 4, 3, 3, 8, 4, 2, 1};
int tile13[15] = {0, 2, 8, 1, 3, 7, 2, 1, 7, 2, 4, 6, 3, 2, 0};

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
hex initialize_map () {
	hex map = malloc(18 * 17 * sizeof(hex));
	return map;
}

/* The 2p map has 15 x columns and 15 y columns. 7,7 is the center of 
 * the center tile. There are only 7 x 19 = 133 hexes used of the 225
 * we have room for, but that's not a lot of actual space wasted.
 */

hex initialize_map_two() {
	hex map = malloc(225 * sizeof(hex));
	return map;
}

void fill_map_two(int** new_map) {
	int tile_array[7];
	int tile_rotations[7];
//	rando_perm(7, tile_array, 7);
	/* eventually this will be a random permutation; for now it's 
	 * always the same.
	 */
	int i;
	for (i=0; i < 5; i++) {
		tile_array[i] = i;
	}
	//this gross stuff is to use the 2-p-only tiles, 11 12 13.
	for (i=5; i < 7; i++) {
		tile_array[i] = i+6;
	}
	
	/* The center space of the center tile is 7,7. The other tile centers
	 * are off by 3 in one direction, and 2 in another. That gives 
	 * (starting with the 'left' tile and going clockwise) (2,9), (5,4),
	 * (10,2), (14,5), (9, 10), (4,12).
	 */
	
	
}
