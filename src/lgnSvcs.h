#ifndef LGNSVCS_H
#define LGNSVCS_H
#include "NetMessages.h"

enum lgnSvcsCmds{
	/*CMD_LGNSVC_MAGEHAT,
	CMD_LGNSVC_BMAGEHAT,
	CMD_LGNSVC_WARRIORHELM,
	CMD_LGNSVC_BMAGEHELM,*/
	CMD_LGNSVC_TOARE,
	CMD_LGNSVC_TOELV,
	CMD_LGNSVC_MAJ20,
	CMD_LGNSVC_MAJ200,
	CMD_LGNSVC_GOLD10,
	CMD_LGNSVC_GOLD100,
	CMD_LGNSVC_GOLD250,
	CMD_LGNSVC_REP10,
	CMD_LGNSVC_REP100,
	CMD_LGNSVC_REP1000,
	CMD_LGNSVC_XPSLATE,
	CMD_LGNSVC_ZERKSLATE,
	CMD_LGNSVC_MPSLATE,
	CMD_LGNSVC_HPSLATE,
	CMD_LGNSVC_ZEM,
	CMD_LGNSVC_SOM,
	CMD_LGNSVC_SOX,
	
	// tokens start
	CMD_LGNSVC_TOKEN1,
	CMD_LGNSVC_TOKEN10,
	CMD_LGNSVC_TOKEN100,
	CMD_LGNSVC_TRADETOKEN1,
	CMD_LGNSVC_TRADETOKEN10,
	CMD_LGNSVC_TRADETOKEN100,
	// tokens end
	CMD_LGNSVC_SAPPRUNE,
	CMD_LGNSVC_RUBYRUNE,
	CMD_LGNSVC_EMERUNE,
	CMD_LGNSVC_DIARUNE,
	CMD_LGNSVC_MDMRUNE,
	CMD_LGNSVC_MMSRUNE,
	CMD_LGNSVC_MDFRUNE,
	CMD_LGNSVC_MRMRUNE,
	CMD_LGNSVC_RESETALL,
	CMD_LGNSVC_ADYEYELLOW,
	CMD_LGNSVC_ADYEVIOLET,
	CMD_LGNSVC_ADYEPINK,
	CMD_LGNSVC_ADYEORANGE,
	CMD_LGNSVC_ADYEWHITE,
	CMD_LGNSVC_ADYEBLACK,
	CMD_LGNSVC_WDYERED,
	CMD_LGNSVC_WDYEBROWN,
	CMD_LGNSVC_WDYEBLACK,
	CMD_LGNSVC_WDYEVIOLET,
	CMD_LGNSVC_WDYEYELLOW,
	// spells start
	CMD_LGNSVC_TRADESCAN,
	CMD_LGNSVC_TRADEMAGICSHIELD,
	CMD_LGNSVC_TRADEICESTORM,
	CMD_LGNSVC_TRADEMASSHEAL,
	CMD_LGNSVC_TRADESOTG,
	CMD_LGNSVC_TRADECOTG,
	CMD_LGNSVC_TRADELC,
	CMD_LGNSVC_TRADEMB,
	CMD_LGNSVC_TRADEFSW,
	CMD_LGNSVC_TRADEBSW,
	// spells end
	CMD_LGNSVC_MAX
};

/*
const struct lgnPtsSvc{
	char * desc;
	char * name;
	unsigned long price;
	unsigned short cmd;
} lgnPtsSvcs[] = {
	/ *{"MP14 Indestr. Golden Wizard Hat", "FantasyHat MP14", 25, CMD_LGNSVC_MAGEHAT},
	{"HP14 Indestr. Golden Wizard Hat", "FantasyHat HP14", 25, CMD_LGNSVC_BMAGEHAT},
	{"HP14 Indestr. Golden Winged Helm", "FantasyHelm HP14", 25, CMD_LGNSVC_WARRIORHELM},
	{"MP14 Indestr. Golden Winged Helm", "FantasyHelm MP14", 25, CMD_LGNSVC_BMAGEHELM},* /
	{"Town Change (to Aresden)", "TC(Are)", 10, CMD_LGNSVC_TOARE},
	{"Town Change (to Elvine)", "TC(Elv)", 10, CMD_LGNSVC_TOELV},
	{"20 Majestic points", "2 Maj", 2, CMD_LGNSVC_MAJ20},
	{"240 Majestic points", "20 Maj", 12, CMD_LGNSVC_MAJ200},
	{"10,000 Gold", "10k Gold", 5, CMD_LGNSVC_GOLD10},
	{"100,000 Gold", "100k Gold", 25, CMD_LGNSVC_GOLD100},
	{"250,000 Gold", "250k Gold", 45, CMD_LGNSVC_GOLD250},
	{"10 Reputations points", "10 Rep", 1, CMD_LGNSVC_REP10},
	{"100 Reputations points", "100 Rep", 8, CMD_LGNSVC_REP100},
	{"1000 Reputations points", "1000 Rep", 25, CMD_LGNSVC_REP1000},
	{"Experience Slate", "XP Slate", 1, CMD_LGNSVC_XPSLATE},
	{"Berserk Slate", "Zerk Slate", 1, CMD_LGNSVC_ZERKSLATE},
	{"Mana Slate", "MP Slate", 2, CMD_LGNSVC_MPSLATE},
	{"Health Slate", "HP Slate", 3, CMD_LGNSVC_HPSLATE},
	{"Zemstone of Sacrifice", "Zem", 1, CMD_LGNSVC_ZEM},
	{"Stone of Merien", "SoM", 1, CMD_LGNSVC_SOM},
	{"Stone of Xelima", "SoX", 1, CMD_LGNSVC_SOX},
	{"Take 1 Fantasy Token", "1 Fantasy Token", 1, CMD_LGNSVC_TOKEN1},
	{"Take 10 Fantasy Token", "10 Fantasy Token", 10, CMD_LGNSVC_TOKEN10},
	{"Take 100 Fantasy Token", "100 Fantasy Token", 100, CMD_LGNSVC_TOKEN100},
	{"Trade Back 1 Fantasy Token", "Trade 1 Fantasy Token Back", 1, CMD_LGNSVC_TRADETOKEN1},
	{"Trade Back 10 Fantasy Token", "Trade 10 Fantasy Token Back", 10, CMD_LGNSVC_TRADETOKEN10},
	{"Trade Back 100 Fantasy Token", "Trade 100 Fantasy Token Back", 100, CMD_LGNSVC_TRADETOKEN100},
	{"Magic Sapphire Rune", "Used for Magic Sapphire", 2, CMD_LGNSVC_SAPPRUNE},
	{"Magic Ruby Rune", "Used for Magic Ruby", 2, CMD_LGNSVC_RUBYRUNE},
	{"Magic Emerald Rune", "Used for Magic Emerald", 2, CMD_LGNSVC_EMERUNE},
	{"Magic Diamond Rune", "Used for Magic Diamond", 2, CMD_LGNSVC_DIARUNE},
	{"Master Magic DM Rune", "Used for Magic Necklace (DM120)", 5, CMD_LGNSVC_MDMRUNE},
	{"Master Magic MS Rune", "Used for Magic Necklace (MS30)", 5, CMD_LGNSVC_MMSRUNE},
	{"Master Magic DF Rune", "Used for Magic Necklace (DF400)", 5, CMD_LGNSVC_MDFRUNE},
	{"Master Magic RM Rune", "Used for Magic Necklace (RM400)", 5, CMD_LGNSVC_MRMRUNE},
	{"Reset Scroll (ALL)", "Reset all stats back to 10", 20, CMD_LGNSVC_RESETALL},
	{"Armor Dye Yellow", "Dye 1 armor piece Yellow", 5, CMD_LGNSVC_ADYEYELLOW},
	{"Armor Dye Violet", "Dye 1 armor piece Violet", 5, CMD_LGNSVC_ADYEVIOLET},
	{"Armor Dye Pink", "Dye 1 armor piece Pink", 5, CMD_LGNSVC_ADYEPINK},
	{"Armor Dye Orange", "Dye 1 armor piece Orange", 10, CMD_LGNSVC_ADYEORANGE},
	{"Armor Dye White", "Dye 1 armor piece White", 10, CMD_LGNSVC_ADYEWHITE},
	{"Armor Dye Black", "Dye 1 armor piece Black", 10, CMD_LGNSVC_ADYEBLACK},
	{"Weapon Dye Red", "Dye 1 weapon Red", 12, CMD_LGNSVC_WDYERED},
	{"Weapon Dye Brown", "Dye 1 weapon Brown", 12, CMD_LGNSVC_WDYEBROWN},
	{"Weapon Dye Black", "Dye 1 weapon Black", 20, CMD_LGNSVC_WDYEBLACK},
	{"Weapon Dye Violet", "Dye 1 weapon Violet", 20, CMD_LGNSVC_WDYEVIOLET},
	{"Weapon Dye Yellow", "Dye 1 weapon Yellow", 20, CMD_LGNSVC_WDYEYELLOW},
	{"Unlearn Scan. and convert to manual", "Unlearn Scan", 10, CMD_LGNSVC_TRADESCAN},
	{"Unlearn M.S and convert to manual", "Unlearn Magic-Shield", 10, CMD_LGNSVC_TRADEMAGICSHIELD},
	{"Unlearn I.S and convert to manual", "Unlearn Ice-Storm", 10, CMD_LGNSVC_TRADEICESTORM},
	{"Unlearn M.H and convert to manual", "Unlearn Mass-Heal", 10, CMD_LGNSVC_TRADEMASSHEAL},
	{"Unlearn S.O.T.G and convert to manual", "Unlearn S.O.T.G", 10, CMD_LGNSVC_TRADESOTG},
	{"Unlearn C.O.T.G and convert to manual", "Unlearn C.O.T.G", 10, CMD_LGNSVC_TRADECOTG},
	{"Unlearn L.C and convert to manual", "Unlearn L.C", 10, CMD_LGNSVC_TRADELC},
	{"Unlearn M.B and convert to manual", "Unlearn M.B", 10, CMD_LGNSVC_TRADEMB},
	{"Unlearn F.S.W and convert to manual", "Unlearn F.S.W", 10, CMD_LGNSVC_TRADEFSW},
	{"Unlearn B.S.W and convert to manual", "Unlearn B.S.W", 10, CMD_LGNSVC_TRADEBSW},
	{"","",0,0}
};
*/

#endif // LGNSVCS_H
