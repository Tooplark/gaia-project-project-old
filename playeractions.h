#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*list of the advanced techtiles*/
char* advtechlist[15] = {"PASSCOLORS", "PASSRS", "PASSFED", "TECHUPPT", "BUILDMINEPT", "BUILDTSPT", "TSPT", "FEDPT", "MINEPT", "SECTORPT", "GAIAPT", "SECTORORE", "ACT5C1Q", "ACT3R", "ACT3O"};

/*list of the techtiles*/
char* techlist[9] = {"1O1Pw", "4C", "1R1C", "1x1O1Q", "1x7Pt", "1xCOLORS->R", "BUILDGAIAPT", "4Pw", "BIG"};

/*list of the boosters */
char* boosterlist[10] = {"1O1R", "1O+2Pw", "2C1QIC", "3Jump2Pw", "1Dig2C", "1OPassTS", "4PwPassBIG", "1RPassRS", "1OPassMine", "4CPassGaia"};

/*list of the round tiles */
char* roundlist[10] = {"2/SPADE", "2/MINE", "3/GAIA", "4/GAIA", "3/TS", "4/TS", "5/BIG", "5/BIG", "5/FED", "2/TECH"};

/*list of the ending bonuses */
char* endlist[6] = {"SECTOR", "SATELLITE", "BUILDINGS", "FEDBUILDINGS", "GAIA", "COLORS"};

/*list of the techtree */
char* techtreelist[6] = {"TERRAFORMING", "NAVIGATION", "AI", "GAIAFORMING", "ECONOMY", "RESEARCH"};

/*list of the federation bonuses */
char* federationlist[6] = {"6pt 2r", "7pt 2o", "7pt 6c", "8pt 1Q", "8pt 2Tok", "12pt"};

/*list of all special actions*/
char* specialactions={"3Jump2Pw", "1Dig2C", "4Pw", "ACT5C1Q", "ACT3R", "ACT3O", "Inst1QIC","SwapMineSH","BuildSpaceStation","RStoTS","TechUpLowest"};




/* Define the player's position on different VP tracks, techtrees, techtiles and federation tiles they possess, remaining buildings, and built planets
There are maximum 4 players.
TODO: add something about plantes
 */
typedef struct player {
  int vp;
  int techtree[6]; /*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research*/
  int eogbonus[2];
  bool techtiles[9];
  bool covered_techtiles[9];
  bool adv_techtiles[6];
  bool federationtiles[13];
  int unflipped_federationtiles;
  int spec_actions[];
  int numMine;
  int numTrading;
  int numSH;
  int numResearch;
  int numAcademy;
  int numGaiaFormer;
  /* planet *planets;*/
  int ore;
  int credit;
  int pw[4];
  int QIC;
  int knowledge;
} player[4];

/*Each techtile contains information about numbers available in the game supply and techtree it is under, 6 means that it is under all three
 */
typedef struct techtile {
int numTechtile;
int techtree;
} techtile[9];

/*Each advanced techtile contains information about whether it is available in game supply and the techtree it is on*/
typedef struct advtechtile {
bool available;
int techtree;
} advtechtile[6]

/*Constructor for player*/
player new_player();

/*Constructor for techtile*/
techtile new_techtile();

/*Constructor for advanced techtile*/
advtechtile new_advtechtile();

void charge_pw(int playerid, int pw);

void burn_pw (int playerid, int pw);

void convert_resource(int playerid, char *orgn_res, char *tgt_res, int tgt_res_num);

bool maxTechFilled(int tech);

bool maxTechFilled(int tech);

void hard_tech_up(int playerid, int tech);

void start_gaia_project(int playerid, planet p, int gaia_pw1, int gaia_pw2, int gaia_pw3);






