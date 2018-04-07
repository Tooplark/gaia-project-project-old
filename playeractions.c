#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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
- (Charge pw)
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
There are maximum 4 players.
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
  int pw[4];
  int gaiaformer;
  int QIC;
  int knowledge;
} player[4];

/*Charging power: 
if there is enough power in pw1, then pw is transfered from pw1 to pw2
 */
void charge_pw(int playerid, int pw){
  int pw1 = player[playerid].pw[1];
  int pw2 = player[playerid].pw[2];
  int pw3 = player[playerid].pw[3];
  int vp = player[playerid].vp;
  int vploss = pw - 1;
  if (pw1 > 0)
    if (pw1 >= pw)
      {pw2 = pw2 + pw;
      pw1 = pw1 - pw;
      vp = vp - vploss;
      }
    else if (pw2 + pw1 >= pw)
      {pw2 = pw2 + 2 * pw1 - pw;
	pw1 = 0;
	pw3 = pw3 + pw;
	vp = vp - vploss;
      }
    else
      {pw3 = pw3 + pw1 + pw2;
	vp = vp + 1 - pw1 - pw2;
	pw1 = 0;
	pw2 = 0;
      }
  else if (pw2 > 0)
    if (pw2 >= pw)
      {pw3 = pw3 + pw;
	pw2 = pw2 - pw;
	vp = vp - vploss;
      }
    else
      {pw3 = pw3 + pw2;
	vp = vp + 1 - pw2
	pw2 = 0;
      }
  else
    printf("No more power can be charged");
}

/*To burn power, the player has to have enough in bowl 2
 */
void burn_pw (int playerid, int pw){
  int pw2 = player[playerid].pw[2];
  int pw3 = player[playerid].pw[3];
  if (pw2 >= pw * 2)
    {pw3 = pw3 + pw;
      pw2 = pw2 - pw * 2;
    }
  else
    printf ("There isn't enough power to burn");
}

/*Converting resources
Possible *orgn_res: pw[3], ore, QIC, k
Possible *tgt_res: ore, c, QIC, k, +pw
tgt_res_num is the number of target resources that we want
 */
void convert_resource(int playerid, char *orgn_res, char *tgt_res, int tgt_res_num){
  int pw3 = player[playerid].pw[3];
  int pw1 = player[playerid].pw[1];
  int ore = player[playerid].ore;
  int c = player[playerid].c;
  int QIC = player[playerid].QIC;
  int k = player[playerid].k;
  if (strcmp(*orgn_res, "pw") == 0)
    if (pw3 >= tgt_res_num && strcmp(*tgt_res, "c") == 0)
	{pw3 = pw3 - tgt_res_num;
	  pw1 = pw1 + tgt_res_num;
	  c = c + tgt_res_num;
	}
    else if (pw3 >= 3 * tgt_res_num && strcmp(*tgt_res, "ore") == 0)
      {pw3 = pw3 - 3 * tgt_res_num;
	pw1 = pw1 + tgt_res_num;
	ore = ore + tgt_res_num;
      }
    else if (pw3 >= 4 * tgt_res_num)
      {pw3 = pw3 - 4 * tgt_res_num;
	pw1 = pw1 + 4 * tgt_res_num;
      if (strcmp(*tgt_res, "QIC") == 0)
	  QIC = QIC + tgt_res_num;
      else if (strcmp(*tgt_res,"k") == 0)
	k = k + tgt_res_num;
      }
    else
      printf ("There is not enough power to do conversion");
  else if (strcmp(*orgn_res, "ore") == 0 && ore >= tgt_res_num)
    if (strcmp(*tgt_res,"c") == 0)
      {ore = ore - tgt_res_num;
	c = c + tgt_res_num;
      }
    else if (strcmp(*tgt_res,"+pw") == 0)
      {ore = ore - tgt_res_num;
	pw1 = pw1 + tgt_res_num;
      }
    else
      printf ("Illegal conversion from ore to target resource or not enough ore");
  else if (strcmp(*orgn_res, "QIC") == 0 && QIC >= tgt_res_num && strcmp(*tgt_res,"ore") == 0)
    {QIC = QIC - tgt_res_num;
      ore = ore + tgt_res_num;
    }
  else if (strcmp(*orgn_res,"k") == 0 && k >= tgt_res_num && strcmp(*tgt_res,"c")==0)
    {k = k - tgt_res_num;
      c = c + tgt_res_num;
    }
  else
    printf ("Illegal conversion or not enough origin resource");
}

/*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research
 */
void tech_up(int playerid, int tech){
  if (player[playerid].techtree[tech] != 6)
    ++player[playerid].techtree[tech];
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
