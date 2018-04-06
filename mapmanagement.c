/* A bunch of enums listing the types of planets and buildings and stuff. */
typedef enum planet_type {
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
typedef enum building {
	none,
	mine,
	trading,
	stronghold,
	research,
	institute,
	gaiaformer
} building;

/* A planet has a color and possibly a building belonging to a player. */
typedef struct planet* {
	planet_type color;
	int owner;
	building b;
} planet;

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
typedef struct hex*{
	bool isSpace;
	union {
		bool[4] satellites;
		planet p;
	}
} hex;

/* Constructor for planets. Nothing to see here. */
planet new_planet {
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
hex* initialize_map () {
	int i;
	hex* map = malloc(18 * 17 * sizeof(hex));
	for (i=0; i < 18*17; i++) map[i] = NULL;
	return map;
}

/* The 2p map has 15 x columns and 15 y columns. 8,8 is the center of 
 * the center tile. There are only 7 x 19 = 133 hexes used of the 225
 * we have room for, but that's not a lot of actual space wasted.
 */

hex* initialize_map_2() {
	int i;
	hex* map = malloc(225 * sizeof(hex));
	for (i = 0; i < 225; i++) map[i] = NULL;
	return map;
}
