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
 */
typedef struct hex*{
	bool isSpace;
	union {
		bool[4] satellites;
		planet p;
	}
} hex;

/* Constructor for planets. Nothing to see here. */
planet new_planet (planet_type c, int o, building b) {
	planet p = malloc(sizeof(struct planet));
	p.color = c;
	p.owner = o;
	p.building = b;
	return p;
}

/* Constructor for hexes. Whee. */
hex new_hex (bool isSpace, 
