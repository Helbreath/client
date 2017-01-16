#ifndef DNTSVCS_H
#define DNTSVCS_H
#include "NetMessages.h"

enum DntSvcsCmds{
	/*CMD_DNTSVC_MAGEHAT,
	CMD_DNTSVC_BMAGEHAT,
	CMD_DNTSVC_WARRIORHELM,
	CMD_DNTSVC_BMAGEHELM,*/
	CMD_DNTSVC_TOARE,
	CMD_DNTSVC_TOELV,
	CMD_DNTSVC_MAJ20,
	CMD_DNTSVC_MAJ240,
	CMD_DNTSVC_GOLD10,
	CMD_DNTSVC_GOLD100,
	//CMD_LGNSVC_GOLD250,
	CMD_DNTSVC_REP10,
	CMD_DNTSVC_REP100,
	//CMD_LGNSVC_REP1000,
	CMD_DNTSVC_XPSLATE,
	CMD_DNTSVC_ZERKSLATE,
	CMD_DNTSVC_MPSLATE,
	CMD_DNTSVC_HPSLATE,
	CMD_DNTSVC_ZEM,
	CMD_DNTSVC_SOM,
	CMD_DNTSVC_SOX,
	// tokens start
	CMD_DNTSVC_TOKEN1,
	CMD_DNTSVC_TOKEN10,
	CMD_DNTSVC_TOKEN100,
	CMD_DNTSVC_TRADETOKEN1,
	CMD_DNTSVC_TRADETOKEN10,
	CMD_DNTSVC_TRADETOKEN100,
	// tokens end
	// spells start
	CMD_DNTSVC_TRADECANCEL,
	CMD_DNTSVC_TRADEIMC,
	CMD_DNTSVC_TRADEICESTORM,
	CMD_DNTSVC_TRADEMFS,
	CMD_DNTSVC_TRADEBS,
	CMD_DNTSVC_TRADEESW,
	// spells end
	CMD_DNTSVC_MAX
};

const struct DntPtsSvc{
	char * desc;
	char * name;
	unsigned long price;
	unsigned short cmd;
} DntPtsSvcs[] = {
	/*{"MP14 Indestr. Golden Wizard Hat", "FantasyHat MP14", 25, CMD_DNTSVC_MAGEHAT},
	{"HP14 Indestr. Golden Wizard Hat", "FantasyHat HP14", 25, CMD_DNTSVC_BMAGEHAT},
	{"HP14 Indestr. Golden Winged Helm", "FantasyHelm HP14", 25, CMD_DNTSVC_WARRIORHELM},
	{"MP14 Indestr. Golden Winged Helm", "FantasyHelm MP14", 25, CMD_DNTSVC_BMAGEHELM},*/
	{"Town Change (to Aresden)", "TC(Are)", 10, CMD_DNTSVC_TOARE},
	{"Town Change (to Elvine)", "TC(Elv)", 10, CMD_DNTSVC_TOELV},
	{"2 Majestic points", "2 Maj", 1, CMD_DNTSVC_MAJ20},
	{"20 Majestic points", "20 Maj", 10, CMD_DNTSVC_MAJ240},
	{"10,000 Gold", "10k Gold", 1, CMD_DNTSVC_GOLD10},
	{"100,000 Gold", "100k Gold", 10, CMD_DNTSVC_GOLD100},
	{"10 Reputations points", "10 Rep", 1, CMD_DNTSVC_REP10},
	{"100 Reputations points", "100 Rep", 10, CMD_DNTSVC_REP100},
	{"Experience Slate", "XP Slate", 2, CMD_DNTSVC_XPSLATE},
	{"Berserk Slate", "Zerk Slate", 2, CMD_DNTSVC_ZERKSLATE},
	{"Mana Slate", "MP Slate", 4, CMD_DNTSVC_MPSLATE},
	{"Health Slate", "HP Slate", 6, CMD_DNTSVC_HPSLATE},
	{"Zemstone of Sacrifice", "Zem", 2, CMD_DNTSVC_ZEM},
	{"Stone of Merien", "SoM", 2, CMD_DNTSVC_SOM},
	{"Stone of Xelima", "SoX", 2, CMD_DNTSVC_SOX},
	{"Take 1 Donation Token", "1 Donation Token", 1, CMD_DNTSVC_TOKEN1},
	{"Take 10 Donation Token", "10 Donation Token", 10, CMD_DNTSVC_TOKEN10},
	{"Take 100 Donation Token", "100 Donation Token", 100, CMD_DNTSVC_TOKEN100},
	{"Trade Back 1 Donation Token", "Trade 1 Donation Token Back", 1, CMD_DNTSVC_TRADETOKEN1},
	{"Trade Back 10 Donation Token", "Trade 10 Donation Token Back", 10, CMD_DNTSVC_TRADETOKEN10},
	{"Trade Back 100 Donation Token", "Trade 100 Donation Token Back", 100, CMD_DNTSVC_TRADETOKEN100},
	{"Unlearn Cancel. and convert to manual", "Unlearn Cancel", 10, CMD_DNTSVC_TRADECANCEL},
	{"Unlearn I.M.C and convert to manual", "Unlearn I.M.C", 10, CMD_DNTSVC_TRADEIMC},
	{"Unlearn I.S and convert to manual", "Unlearn IceStorm", 10, CMD_DNTSVC_TRADEICESTORM},
	{"Unlearn M.F.S and convert to manual", "Unlearn MassFireStrike", 10, CMD_DNTSVC_TRADEMFS},
	{"Unlearn B.S.W and convert to manual", "Unlearn BloodyShockW", 10, CMD_DNTSVC_TRADEBS},
	{"Unlearn E.S.W and convert to manual", "Unlearn E.S.W", 10, CMD_DNTSVC_TRADEESW},
	{"","",0,0}
};

#endif // LGNSVCS_H
