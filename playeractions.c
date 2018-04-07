#include <stdbool.h>
#include <stdlib.h>
#include 'mapmanagement.h'

/*To keep track, list of possible player actions:
- Terraform
- Gaiaform (turn Gaia planet habitable)
- Start Gaia project (turn Transdim planet habitable)
- Build mine
- Upgrade building
-- Upgrade mine to trading
-- Upgrade trading to stronghold
-- Upgrade trading to research (+gain techtile,+advance on tech-tree)
-- Upgrade research to institute (+ gain techtile,+advance on tech-tree)
- Go up on tech-tree
- Form a federation
- Power and QIC action
- Other special action

Free actions:
- Convert resources
-- 4pw to 1 QIC
-- 3pw to 1 ore
-- 1QIC to 1 ore
-- 4pw to 1k
-- 1pw to 1c
-- 1k to 1c
-- 1ore to 1c
-- 1ore to +1pw
- Burn power
 */



/*
A federation tyle should contain information on whether it is flipped or not
 */
typedef struct federationtile {
  char *name;
  bool flipped;
}*federationtile;

/* A standard techtile should contain information on whether it is covered or not, and if applicable, if the action on it has been taken
 */
typedef struct techtile {
  char *name;
  bool covered;
  bool action;
}*techtile;

/*An advanced techtile should contain information, if applicable, whether the action on it has been taken
 */
typedef struct adv_techtile{
  char *name;
  bool action;
}

/* Define the player's position on different VP tracks, techtrees, techtiles and federation tiles they possess, remaining buildings, and built planets
 */
typedef struct player {
  int vp;
  int techtree[6]; /*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research*/
  int eogbonus[2];
  techtile *techtiles;
  adv_techtile *adv_techtiles;
  federationtile *federationtiles;
  int numMine;
  int numTrading;
  int numSH;
  int numResearch;
  int numInst;
  planet *planets;
  int ore;
  int credit;
  int pw1[4];
  int gaiaformer;
  int QIC;
  int knowledge;
} *player;

/*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research
 */
void gain_research(int tech){
  if ( player->techtree[tech] != 6)
    ++player->techtree[tech];
  else
    printf("Already maxed out on this techtree");
}

/* When gaining techtile 
1. decrease the number of that techtile available in the supply
2. check what research branch it lets you go on
3. give opportunity to tech up if it's on the lower row
 */
  void gain_techtile (char techtile) {
  }

void gain_advanced_techtile(char adv_techtile){
}
