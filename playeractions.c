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
- Burn power
 */

/* Define the player's position on different VP tracks, techtrees, techtiles and federation tiles they possess, remaining buildings, and built planets
 */
typedef struct player {
  int vp;
  int terraforming;
  int navigation;
  int ai;
  int gaiaforming;
  int economy;
  int research;
  int eogbonus1;
  int eogbonus2;
  char *techtiles;
  char *federationtiles;
  int numMine;
  int numTrading;
  int numSH;
  int numResearch;
  int numInst;
  planet *planets;
} *player;

  void gain_techtile (char techtile) {
  }
