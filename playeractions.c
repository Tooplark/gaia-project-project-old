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

char* advtechlist[15] = {"PASSCOLORS", "PASSRS", "PASSFED", "TECHUPPT", "BUILDMINEPT", "BUILDTSPT", "TSPT", "FEDPT", "MINEPT", "SECTORPT", "GAIAPT", "SECTORORE", "ACT5C1Q", "ACT3R", "ACT3O"};
char* techlist[9] = {"1O1Pw", "4C", "1R1C", "1x1O1Q", "1x7Pt", "1xCOLORS->R", "BUILDGAIAPT", "4Pw", "BIG"};
char* boosterlist[10] = {"1O1R", "1O+2Pw", "2C1QIC", "3Jump2Pw", "1Dig2C", "1OPassTS", "4PwPassBIG", "1RPassRS", "1OPassMine", "4CPassGaia"};
char* roundlist[10] = {"2/SPADE", "2/MINE", "3/GAIA", "4/GAIA", "3/TP", "4/TP", "5/BIG", "5/BIG", "5/FED", "2/TECH"};
char* endlist[6] = {"SECTOR", "SATELLITE", "BUILDINGS", "FEDBUILDINGS", "GAIA", "COLORS"};
char* techtreelist[6] = {"TERRAFORMING", "NAVIGATION", "AI", "GAIAFORMING", "ECONOMY", "RESEARCH"};
char* federationlist[6] = {"6pt 2r", "7pt 2o", "7pt 6c", "8pt 1Q", "8pt 2Tok", "12pt"};
char* specialactions={"3Jump2Pw", "1Dig2C", "4Pw", "ACT5C1Q", "ACT3R", "ACT3O", "Inst+1QIC","SwapMineSH","BuildSpaceStation","RStoTS","TechUpLowest"};

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
}*adv_techtile;

/* Define the player's position on different VP tracks, techtrees, techtiles and federation tiles they possess, remaining buildings, and built planets
There are maximum 4 players.
 */
typedef struct player {
  int vp;
  int techtree[6]; /*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research*/
  int eogbonus[2];
  bool techtiles[9];
  bool covered_techtiles[9];
  bool adv_techtiles[6];
  bool federationtiles[12];
  int unflipped_federationtiles;
  int spec_actions[];
  int numMine;
  int numTrading;
  int numSH;
  int numResearch;
  int numAcademy;
  int numGaiaFormer;
  planet *planets;
  int ore;
  int credit;
  int pw[4];
  int QIC;
  int knowledge;
} player[4];

/*The game supply has a number of federation tiles and techtiles and advanced techtiles
 */
typedef struct supply {
  
} supply;

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
      {pw2 += pw;
      pw1 -= pw;
      vp -= vploss;
      }
    else if (pw2 + pw1 >= pw)
      {pw2 = pw2 + 2 * pw1 - pw;
	pw1 = 0;
	pw3 += pw;
	vp -= vploss;
      }
    else
      {pw3 = pw3 + pw1 + pw2;
	vp = vp + 1 - pw1 - pw2;
	pw1 = 0;
	pw2 = 0;
      }
  else if (pw2 > 0)
    if (pw2 >= pw)
      {pw3 += pw;
	pw2 -= pw;
	vp -= vploss;
      }
    else
      {pw3 += pw2;
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
    {pw3 += pw;
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
	{pw3 -= tgt_res_num;
	  pw1 += tgt_res_num;
	  c += tgt_res_num;
	}
    else if (pw3 >= 3 * tgt_res_num && strcmp(*tgt_res, "ore") == 0)
      {pw3 = pw3 - 3 * tgt_res_num;
	pw1 += tgt_res_num;
	ore += tgt_res_num;
      }
    else if (pw3 >= 4 * tgt_res_num)
      {pw3 -= 4 * tgt_res_num;
	pw1 += 4 * tgt_res_num;
      if (strcmp(*tgt_res, "QIC") == 0)
	  QIC += tgt_res_num;
      else if (strcmp(*tgt_res,"k") == 0)
	k += tgt_res_num;
      }
    else
      printf ("There is not enough power to do conversion");
  else if (strcmp(*orgn_res, "ore") == 0 && ore >= tgt_res_num)
    if (strcmp(*tgt_res,"c") == 0)
      {ore -= tgt_res_num;
	c += tgt_res_num;
      }
    else if (strcmp(*tgt_res,"+pw") == 0)
      {ore -= tgt_res_num;
	pw1 += tgt_res_num;
      }
    else
      printf ("Illegal conversion from ore to target resource or not enough ore");
  else if (strcmp(*orgn_res, "QIC") == 0 && QIC >= tgt_res_num && strcmp(*tgt_res,"ore") == 0)
    {QIC -= tgt_res_num;
      ore += tgt_res_num;
    }
  else if (strcmp(*orgn_res,"k") == 0 && k >= tgt_res_num && strcmp(*tgt_res,"c")==0)
    {k -= tgt_res_num;
      c += tgt_res_num;
    }
  else
    printf ("Illegal conversion or not enough origin resource");
}

/*Helper function: given a player id, return all other player id's*/
int *otherPlayers(int playerid){
  static int otherPlayers[3];
  if (playerid == 3)
    {for (i=0; i < 3; i++) {
	otherPlayers[i]=i;
      }
    }
  else
    {for (i=0; i < 4; ) {
	if (i < playerid)
	  {
	    otherPlayers[i]=i;
	    i++;
	  }
	else if (i == playerid)
	  i++;
	else
	  {otherPlayers[i-1]=i;
	    i++;
	  }
      }
    }
  return otherPlayers;
}

/*Helper function: check whether any other player is on the 6th level of a given tech*/
bool maxTechFilled(int playerid, int tech){
  int *otherPlayers = otherPlayers(playerid);
  i = 0;
  int currentPlayer = otherPlayers[i];
  while (currentPlayer < 4){
    if (player[currentPlayer].techtree[tech] !=6)
      {
	i++;
	int currentPlayer = otherPlayers[i];
      }
    else if (player[currentPlayer].techtree[tech] == 6)
	return true; 
  }
  return false;
}

/*0:terraforming, 1:navigation, 2:ai, 3:gaiaforming, 4:economy, 5:research
This will be called in hard_tech_up and when gaining a techtile
Returns the new level the player is on in the relevant tech
Adds every immediate resources here.
 */
int tech_up(int playerid, int tech){
  int tech_level = player[playerid].techtree[tech];
  int ore = player[playerid].ore;
  int QIC = player[playerid].QIC;
  int k = player[playerid].k;
  int GaiaFormer = player[playerid].GaiaFormer;
  int pw1 = player[playerid].pw[1];
  int unflipped_fedtiles = player[playerid].unflipped_federationtiles;
  if (tech_level == 5)
    if (!maxTechFilled(playerid,tech) && unflipped_fedtiles > 0)
      {++tech_level;
	maxTechAction(playerid,tech); /*to be written: give the maxtech benefits*/
	return 6;
      }
    else
      {
      printf ("Conditions are not satisfied to max up on techtree");
      return 5;
      }
  else if (tech_level < 5)
    {++tech_level;
      if (tech_level == 4)
      	charge_power(playerid,3);
      if (tech == 0 && (tech_level == 2 || tech_level == 5))
	  ore += 2;
      else if (tech == 1 && (tech_level == 2 || tech_level == 4))
	  QIC += 1;
      else if (tech == 2)
	{if (tech_level == 2 || tech_level == 3)
	  QIC += 1;
	  else if (tech_level == 4 || tech_level == 5)
	    QIC +=2;
	}
      else if (tech == 3)
	{if (tech_level == 2 || tech_level == 4 || tech_level == 5)
	    GaiaFormer += 1;
	  else if (tech_level == 3)
	    pw1 += 3;
	}
      return tech_level;
    }
  else
    {
      printf("Already maxed out on this techtree");
    return 6;
    }
}

/*hard tech-up: spend four knowledge to go up on tech*/
void hard_tech_up(int playerid, int tech){
  if (player[playerid].k >= 4)
    {player[playerid].k = player[playerid].k - 4;
      tech_up (playerid,tech);
    }
  else
    printf ("There is not enough knowledge to tech up");
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
