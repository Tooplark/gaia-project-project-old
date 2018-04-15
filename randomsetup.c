#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void rando_perm(int range, int* array, int size);
void gen_array(int range, int* array, int size);
void renumber(int range, int* array, int size);
void main(int argc, char* argv[]);
void test();
void beef();

char* advtechlist[15] = {"PASSCOLORS", "PASSRS", "PASSFED", "TECHUPPT", "BUILDMINEPT", "BUILDTSPT", "MINEPT", "TSPT", "FEDPT", "SECTORPT", "GAIAPT", "SECTORORE", "ACT5C1Q", "ACT3R", "ACT3O"};
char* techlist[9] = {"1O1Pw", "4C", "1R1C", "1x1O1Q", "1x7Pt", "1xCOLORS->R", "BUILDGAIAPT", "4Pw", "BIG"};
char* boosterlist[10] = {"1O1R", "1O2Tok", "2C1Q", "3Jump2Pw", "1Dig2C", "4CPassGaia", "4PwPassBIG", "1RPassRS", "1OPassMine", "1OPassTS"};
char* roundlist[10] = {"2/SPADE", "2/MINE", "3/GAIA", "4/GAIA", "3/TP", "4/TP", "5/BIG", "5/BIG", "5/FED", "2/TECH"};
char* endlist[6] = {"SECTOR", "SATELLITE", "BUILDINGS", "FEDBUILDINGS", "GAIA", "COLORS"};
char* techtreelist[6] = {"TERRAFORMING", "NAVIGATION", "AI", "GAIAFORMING", "ECONOMY", "RESEARCH"};
char* federationlist[6] = {"6pt 2r", "7pt 2o", "7pt 6c", "8pt 1Q", "8pt 2Tok", "12pt"};

/* Sanity debugging function for printing int arrays. Prints the contents
 * of array, beginning at array[start] and stopping at array[end].
 */
void printarr(int * array, int start, int end, char* label){
	int i;
	printf("%s: [", label);
	for (i=start; i< end; i++)
		printf("%d, ", array[i]);
	printf("%d]\n", array[i]);
	return;
}

void main(int argc, char* argv[]) {
	/* This bit does input parsing.
	 * -p sets number of players. Default is 4.
	 */
	int players = 4;
	if (argc == 1){
		printf("Defaulting to a 4-player game.\n");
	}
	else {
		if (argv[1][0] == '-'){
			switch (argv[1][1]) {
				case 'p': 
					players = strtol(argv[2], NULL, 10);  
					if (players > 0 && players < 5){
						printf("Starting a %d-player game.\n", players);
						break;
					}
				default: 
					printf("oh no, a bad happen\n");
					return;
			}
		}
	}
	beef(players);
//	test();
	return;
}

void test(){
	int array[4] = {2, 4, 2, 0};
	renumber(5, array, 3);
//	printarr(array, 0, 2, "Test A");
	assert(array[0] == 2);
	assert(array[1] == 5);
	assert(array[2] == 3);

	gen_array(8, array, 4);
	int otherarray[4];
	int i;
	for(i=0; i<4; i++) otherarray[i] = array[i];
	renumber(8, array, 4);
	printarr(otherarray, 0, 3, "Random array:");
	printarr(array, 0, 3, "Result:");
}

void beef(int players) {
	/* Eventual options:
	 * -s: specify random seed
	 * -p: number of players
	 * -r factions: random factions
	 * -r starts: random starts
	 * a bunch of options for manually specifying some parts of the setup
	 */
	// for now ignore all args
	
	int tiles[10]; //7 if 1-2 players
	int rotation[10]; //7 if 1-2 players
	int boosters[7]; //TODO: 3 + number of players
	int tech[6];
	int advtech[6];
	int round[6];
	int end[2];
	int randomfed;
	int i;

	int tilecount = players < 3 ? 7 : 10;
	int boostercount = 3 + players;

	srand(time(NULL));

	rando_perm(tilecount, tiles, tilecount);
	rando_perm(10, boosters, boostercount);
	rando_perm(9, tech, 6);
	rando_perm(15, advtech, 6);
	rando_perm(10, round, 6);

	for(i=0;i<tilecount;i++) {
		rotation[i] = rand() % 6;
		tiles[i]++; //tiles should be 1-indexed for clarity.
	}

	end[0] = rand() % 6;
	end[1] = rand() % 5;
	if (end[0] <= end[1]) end[1]++;

	randomfed = rand() % 6;

	printarr(tiles, 0, tilecount-1, "TILES\n");
	printarr(rotation, 0, tilecount-1, "ROTATIONS\n");
	
	for(i=0; i<6; i++) {
		printf("\nTrack %-12s gets %-11s on top, %-11s on bottom", 
				techtreelist[i], advtechlist[advtech[i]], 
				techlist[tech[i]]);
	}

	printf("\n\nTERRAFORMING step 5 reward: %s", federationlist[randomfed]);

	printf("\n\nBoosters are ");
	for(i=0; i<boostercount-1; i++) {
		printf("%s, ", boosterlist[boosters[i]]);
	}
	printf("%s\n\n", boosterlist[boosters[i]]);

	printf("Round bonuses are ");
	for(i=0; i<5; i++) {
		printf("%s, ", roundlist[round[i]]);
	}
	printf("%s\n\n", roundlist[round[i]]);

	printf("Final scoring is %s and %s\n", 
			endlist[end[0]], endlist[end[1]]);
}
	
/* Given a range and an array of integers, this helper converts the array
 * from a pre-permutation to a permutation.
 * By pre-permutation I mean that the array specifies a sequence of values
 * that determine a permutation by telling youto pick the kth remaining tile
 * for the next slot in the permutation.
 * That is, if we want 3 letters from {a, b, c, d, e}, then the array [1 3 1]
 * would first tell us to pick the second remaining letter (b), then the
 * fourth remaining letter (e), then the second (c). The output would be [1 4 2]
 * The input array is valid as long as array[i] is less than range - i.
 * If the input is not valid, eventually we'll do something. <-TODO
 * This is not clever, it's O(range * size).
 */
	void renumber(int range, int* array, int size) {
		int i,j,k;
		int* unused = malloc(sizeof(int) * range);

		//Fill up an array with the values that can be taken 
		for (i=0;i<range;i++) {
			unused[i] = i;
		}
		//Unused is an array: [0, 1, ... range-1].

//		printarr(array, 0, size-1, "Array");
//		printarr(unused, 0, range-1, "Unused");

		/* Remove the array[j]th value by counting how many values
		 * have been taken.
		 */
		for (j=0;j<size;j++) {
			k = 0;
//			printf("i: ");
			for (i=0; i<array[j]; i++) {
//				printf("%d ", i);
				//Skip already-taken values
				while(unused[k] == -1) k++;
				k++;
			}
//			printf("\n");
			while(unused[k] == -1) k++;
			array[j] = unused[k];
			unused[k] = -1;
//			printarr(unused, 0, range-1, "Unused Loop 1");
		}
}

void gen_array(int range, int* array, int size) {
	int i;
	for(i=0; i<size; i++) {
		array[i] = rand() % (range - i);
	}		
}

void rando_perm(int range, int* array, int size) {
	gen_array(range, array, size);
	renumber(range, array, size);
}
