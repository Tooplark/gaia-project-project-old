#include <stdbool.h>
#include <stdlib.h>
/* A bunch of enums listing the types of planets and buildings and stuff. */
typedef enum planet_type_enum {
	brown,
	yellow,
	orange,
	red,
	earthlike,
	white,
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
	building building;
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
		planet planet;
	};
} *hex;

typedef struct tile {
	int planet_count;
	int* planet_x;
	int* planet_y;
	planet_type* planet_types;
} * tile;

/* Constructor for planets. Nothing to see here. */
planet new_planet ();

/* Constructor for hexes. Whee. */
hex new_hex ();

/* The 3-4p map is very irregular. It has 20 columns in one direction, 18 in
 * another, and 17 in the third. We use the two shorter dimensions for our 
 * coordinate system.
 */
hex *initialize_map ();

/* The 2p map has 15 x columns and 15 y columns. 7,7 is the center of 
 * the center tile. There are only 7 x 19 = 133 hexes used of the 225
 * we have room for, but that's not a lot of actual space wasted.
 */
hex *initialize_2p_map();

tile* create_2p_tiles();

void rotate_tile(tile tile, int r);

/* This is a subroutine for fill_map that puts the details of the specified
 * map tile into the map (that is, setting the space and planet fields).
 * It will place the tile so that the center is in the position specified by
 * tile_center_x and tile_center_y.
 * The spaces occupied by the tile will be all spaces within 2 of the center;
 * that is, all spaces (a,b) where |a-x|, |b-y|, and |a+b-x-y| are at most 2.
 */
void add_tile_to_map(hex* map, int map_width, tile tile, int x, int y);

void fill_2p_map(hex* map);
