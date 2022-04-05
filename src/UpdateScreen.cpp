//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "helbreath.h"
#include "buffer.h"
#include <algorithm>
#include <iostream>

#include "lan_eng.h"
#include <asio/ssl.hpp>

extern void MakeSprite(char * FileName, int iStart, short sCount, bool bAlphaEffect);
extern void MakeTileSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect);
extern void MakeEffectSpr(char * FileName, short sStart, short sCount, bool bAlphaEffect);

extern helbreath * game;

extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];

extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
extern int _tmp_iStatus;
extern short _tmp_sHeadApprValue, _tmp_sBodyApprValue, _tmp_sArmApprValue, _tmp_sLegApprValue;
extern char _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocuiStatus;
extern int iFocusApprColor;
extern short sFocusHeadApprValue, sFocusBodyApprValue, sFocusArmApprValue, sFocusLegApprValue;
extern int iHungerStatus;
extern bool m_bStuckHack;

struct load_data
{
    uint32_t id;
    uint32_t sprite_type;
    std::string name;
    uint32_t num;
    bool alpha;
    std::string label;
};
#define SPRITETYPE_SPRITE 1
#define SPRITETYPE_TILE 2
#define SPRITETYPE_EFFECT 3

std::queue<load_data> data_list;
int data_max = 0;

void helbreath::create_load_list()
{
    uint32_t i = 0;
    data_list.push({ SPRID_INTERFACE_NEWMAPS1, SPRITETYPE_SPRITE, "Newmaps", 0, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_NEWMAPS2, SPRITETYPE_SPRITE, "Newmaps", 1, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_NEWMAPS3, SPRITETYPE_SPRITE, "Newmaps", 2, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_NEWMAPS4, SPRITETYPE_SPRITE, "Newmaps", 3, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_NEWMAPS5, SPRITETYPE_SPRITE, "Newmaps", 4, false, "Loading interface" });

    data_list.push({ SPRID_INTERFACE_ND_GAME1, SPRITETYPE_SPRITE, "GameDialog", 0, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_GAME2, SPRITETYPE_SPRITE, "GameDialog", 1, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_GAME3, SPRITETYPE_SPRITE, "GameDialog", 2, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_GAME4, SPRITETYPE_SPRITE, "GameDialog", 3, false, "Loading interface" });

    data_list.push({ SPRID_INTERFACE_ND_CRUSADE, SPRITETYPE_SPRITE, "GameDialog", 4, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_ICONPANNEL, SPRITETYPE_SPRITE, "GameDialog", 6, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_INVENTORY, SPRITETYPE_SPRITE, "GameDialog", 7, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_SELECTCHAR, SPRITETYPE_SPRITE, "GameDialog", 8, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_NEWCHAR, SPRITETYPE_SPRITE, "GameDialog", 9, false, "Loading interface" });
    //TODO current sprite does not contain newer interface
    //data_list.push({ SPRID_INTERFACE_ND_NEWEXCHANGE, SPRITETYPE_SPRITE, "GameDialog", 10, false, "Loading interface" });

    data_list.push({ SPRID_INTERFACE_ND_TEXT, SPRITETYPE_SPRITE, "DialogText", 0, false, "Loading interface" });
    data_list.push({ SPRID_INTERFACE_ND_BUTTON, SPRITETYPE_SPRITE, "DialogText", 1, false, "Loading interface" });

    auto make_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label) {
        for (uint32_t i = 0; i < sCount; i++)
            data_list.push({ i + iStart, SPRITETYPE_SPRITE, FileName, i, bAlphaEffect, label });
    };

    auto make_tile_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label) {
        for (uint32_t i = 0; i < sCount; i++)
            data_list.push({ i + iStart, SPRITETYPE_TILE, FileName, i, bAlphaEffect, label });
    };

    auto make_effect_sprite = [&](std::string FileName, uint32_t iStart, short sCount, bool bAlphaEffect, std::string label) {
        for (uint32_t i = 0; i < sCount; i++)
            data_list.push({ i + iStart, SPRITETYPE_EFFECT, FileName, i, bAlphaEffect, label });
    };

    make_sprite("Telescope", SPRID_INTERFACE_GUIDEMAP, 34, false, "Loading interface");
    make_sprite("Telescope2", SPRID_INTERFACE_GUIDEMAP + 35, 4, false, "Loading interface");
    make_sprite("monster", SPRID_INTERFACE_MONSTER, 1, false, "Loading interface");

    make_tile_sprite("maptiles1", 0, 32, true, "Loading map data");
    make_tile_sprite("structures1", 50, 20, true, "Loading map data");
    make_tile_sprite("Sinside1", 70, 27, false, "Loading map data");
    make_tile_sprite("Trees1", 100, 46, true, "Loading map data");
    make_tile_sprite("TreeShadows", 150, 46, true, "Loading map data");
    make_tile_sprite("objects1", 200, 10, true, "Loading map data");
    make_tile_sprite("objects2", 211, 5, true, "Loading map data");
    make_tile_sprite("objects3", 216, 4, true, "Loading map data");
    make_tile_sprite("objects4", 220, 2, true, "Loading map data");

    make_tile_sprite("Tile223-225", 223, 3, true, "Loading map data");
    make_tile_sprite("Tile226-229", 226, 4, true, "Loading map data");
    make_tile_sprite("objects5", 230, 9, true, "Loading map data");
    make_tile_sprite("objects6", 238, 4, true, "Loading map data");
    make_tile_sprite("objects7", 242, 7, true, "Loading map data");
    make_tile_sprite("maptiles2", 300, 15, true, "Loading map data");
    make_tile_sprite("maptiles4", 320, 10, true, "Loading map data");
    make_tile_sprite("maptiles5", 330, 19, true, "Loading map data");
    make_tile_sprite("maptiles6", 349, 4, true, "Loading map data");
    make_tile_sprite("maptiles353-361", 353, 9, true, "Loading map data");
    make_tile_sprite("Tile363-366", 363, 4, true, "Loading map data");
    make_tile_sprite("Tile367-367", 367, 1, true, "Loading map data");
    make_tile_sprite("Tile370-381", 370, 12, true, "Loading map data"); // Tile370~381
    make_tile_sprite("Tile382-387", 382, 6, true, "Loading map data");
    make_tile_sprite("Tile388-402", 388, 15, true, "Loading map data");

    make_tile_sprite("Tile403-405", 403, 3, true, "Loading map data");
    make_tile_sprite("Tile406-421", 406, 16, true, "Loading map data");
    make_tile_sprite("Tile422-429", 422, 8, true, "Loading map data");
    make_tile_sprite("Tile430-443", 430, 14, true, "Loading map data");
    make_tile_sprite("Tile444-444", 444, 1, true, "Loading map data");
    make_tile_sprite("Tile445-461", 445, 17, true, "Loading map data");
    make_tile_sprite("Tile462-473", 462, 12, true, "Loading map data");
    make_tile_sprite("Tile474-478", 474, 5, true, "Loading map data");
    make_tile_sprite("Tile479-488", 479, 10, true, "Loading map data");
    make_tile_sprite("Tile489-522", 489, 34, true, "Loading map data"); // Drunken City
    make_tile_sprite("Tile523-530", 523, 8, true, "Loading map data");  // Rampart
    make_tile_sprite("Tile531-540", 531, 10, true, "Loading map data"); // GodH + Pont
    make_tile_sprite("Tile541-545", 541, 5, true, "Loading map data");  // GodH

    make_sprite("itempack", SPRID_ITEMPACK_PIVOTPOINT + 1, 27, false, "Loading items");

    data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itempack", 17, false, "Loading items" });
    data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itempack", 18, false, "Loading items" });
    data_list.push({ SPRID_ITEMPACK_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" });

    make_sprite("itemground", SPRID_ITEMGROUND_PIVOTPOINT + 1, 19, false, "Loading items");

    data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itemground", 17, false, "Loading items" });
    data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itemground", 18, false, "Loading items" });
    data_list.push({ SPRID_ITEMGROUND_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itemground", 19, false, "Loading items" });

    make_sprite("item-dynamic", SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, false, "Loading items");

    // MALE
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 0, SPRITETYPE_SPRITE, "itemequipM", 0, false, "Loading items" });   // body
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 1, SPRITETYPE_SPRITE, "itemequipM", 1, false, "Loading items" });   // 1-swords
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 2, SPRITETYPE_SPRITE, "itemequipM", 2, false, "Loading items" });   // 2-bows
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 3, SPRITETYPE_SPRITE, "itemequipM", 3, false, "Loading items" });   // 3-shields
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 4, SPRITETYPE_SPRITE, "itemequipM", 4, false, "Loading items" });   // 4-tunics
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 5, SPRITETYPE_SPRITE, "itemequipM", 5, false, "Loading items" });   // 5-shoes
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 7, SPRITETYPE_SPRITE, "itemequipM", 6, false, "Loading items" });   // 6-berk
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 8, SPRITETYPE_SPRITE, "itemequipM", 7, false, "Loading items" });   // 7-hoses
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 9, SPRITETYPE_SPRITE, "itemequipM", 8, false, "Loading items" });   // 8-bodyarmor
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 15, SPRITETYPE_SPRITE, "itemequipM", 11, false, "Loading items" }); // Axe hammer
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 17, SPRITETYPE_SPRITE, "itemequipM", 12, false, "Loading items" }); // Wands
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 18, SPRITETYPE_SPRITE, "itemequipM", 9, false, "Loading items" });  // hair
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 19, SPRITETYPE_SPRITE, "itemequipM", 10, false, "Loading items" }); // undies
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "itemequipM", 13, false, "Loading items" }); // capes
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "itemequipM", 14, false, "Loading items" }); // helm

    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 16, SPRITETYPE_SPRITE, "itempack", 15, false, "Loading items" }); // Necks, Angels, Pendants
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 22, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" }); // Angels

    // FEMALE
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 40, SPRITETYPE_SPRITE, "itemequipW", 0, false, "Loading items" });  // body
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 41, SPRITETYPE_SPRITE, "itemequipW", 1, false, "Loading items" });  // 1-swords
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 42, SPRITETYPE_SPRITE, "itemequipW", 2, false, "Loading items" });  // 2-bows
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 43, SPRITETYPE_SPRITE, "itemequipW", 3, false, "Loading items" });  // 3-shields
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 45, SPRITETYPE_SPRITE, "itemequipW", 4, false, "Loading items" });  // 4-tunics
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 50, SPRITETYPE_SPRITE, "itemequipW", 5, false, "Loading items" });  // 5-shoes
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 51, SPRITETYPE_SPRITE, "itemequipW", 6, false, "Loading items" });  // 6-berk
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 52, SPRITETYPE_SPRITE, "itemequipW", 7, false, "Loading items" });  // 7-hoses
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 53, SPRITETYPE_SPRITE, "itemequipW", 8, false, "Loading items" });  // 8-bodyarmor
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 55, SPRITETYPE_SPRITE, "itemequipW", 11, false, "Loading items" }); // Axe hammer
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 57, SPRITETYPE_SPRITE, "itemequipW", 12, false, "Loading items" }); // Wands
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 58, SPRITETYPE_SPRITE, "itemequipW", 9, false, "Loading items" });  // hair
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 59, SPRITETYPE_SPRITE, "itemequipW", 10, false, "Loading items" }); // undies
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 60, SPRITETYPE_SPRITE, "itemequipW", 13, false, "Loading items" }); // capes
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 61, SPRITETYPE_SPRITE, "itemequipW", 14, false, "Loading items" }); // helm

    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 56, SPRITETYPE_SPRITE, "itempack", 15, false, "Loading items" }); // Necks, Angels, Pendants
    data_list.push({ SPRID_ITEMEQUIP_PIVOTPOINT + 62, SPRITETYPE_SPRITE, "itempack", 19, false, "Loading items" }); // Angels

    make_sprite("Bm", 500 + 15 * 8 * 0, 96, true, "Loading characters"); // Black Man (Type: 1)
    make_sprite("Wm", 500 + 15 * 8 * 1, 96, true, "Loading characters"); // White Man (Type: 2)
    make_sprite("Ym", 500 + 15 * 8 * 2, 96, true, "Loading characters"); // Yellow Man (Type: 3)
    make_sprite("Bw", 500 + 15 * 8 * 3, 96, true, "Loading characters"); // Black Woman (Type: 4)
    make_sprite("Ww", 500 + 15 * 8 * 4, 96, true, "Loading characters"); // White Woman (Type: 5)
    make_sprite("Yw", 500 + 15 * 8 * 5, 96, true, "Loading characters"); // Yellow Woman (Type: 6)

    make_sprite("TutelarAngel1", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, false, "Loading angels"); //(STR)
    make_sprite("TutelarAngel2", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, false, "Loading angels"); //(DEX)
    make_sprite("TutelarAngel3", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, false, "Loading angels"); //(INT)
    make_sprite("TutelarAngel4", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, false, "Loading angels"); //(MAG)

    make_sprite("slm", SPRID_MOB + 7 * 8 * 0, 40, true, "Loading npcs");    // Slime (Type: 10)
    make_sprite("ske", SPRID_MOB + 7 * 8 * 1, 40, true, "Loading npcs");    // Skeleton (Type: 11)
    make_sprite("Gol", SPRID_MOB + 7 * 8 * 2, 40, true, "Loading npcs");    // Stone-Golem (Type: 12)
    make_sprite("Cyc", SPRID_MOB + 7 * 8 * 3, 40, true, "Loading npcs");    // Cyclops (Type: 13)
    make_sprite("Orc", SPRID_MOB + 7 * 8 * 4, 40, true, "Loading npcs");    // Orc (Type: 14)
    make_sprite("Shopkpr", SPRID_MOB + 7 * 8 * 5, 8, true, "Loading npcs"); // ShopKeeper-Woman (Type: 15)
    make_sprite("Ant", SPRID_MOB + 7 * 8 * 6, 40, true, "Loading npcs");    //  Giant-Ant (Type: 16)
    make_sprite("Scp", SPRID_MOB + 7 * 8 * 7, 40, true, "Loading npcs");    //  Scorpion (Type: 17)
    make_sprite("Zom", SPRID_MOB + 7 * 8 * 8, 40, true, "Loading npcs");    //  Zombie (Type: 18)
    make_sprite("Gandlf", SPRID_MOB + 7 * 8 * 9, 8, true, "Loading npcs");
    make_sprite("Howard", SPRID_MOB + 7 * 8 * 10, 8, true, "Loading npcs");
    make_sprite("Guard", SPRID_MOB + 7 * 8 * 11, 40, true, "Loading npcs");  // Guard (Type: 21)
    make_sprite("Amp", SPRID_MOB + 7 * 8 * 12, 40, true, "Loading npcs");    // Amphis (Type: 22)
    make_sprite("Cla", SPRID_MOB + 7 * 8 * 13, 40, true, "Loading npcs");    // Clay-Golem (Type: 23)
    make_sprite("tom", SPRID_MOB + 7 * 8 * 14, 8, true, "Loading npcs");     // Tom (Type: 24)
    make_sprite("William", SPRID_MOB + 7 * 8 * 15, 8, true, "Loading npcs"); // William (Type: 25)

    make_sprite("Kennedy", SPRID_MOB + 7 * 8 * 16, 8, true, "Loading npcs");        // Kennedy (Type: 26)
    make_sprite("Helb", SPRID_MOB + 7 * 8 * 17, 40, true, "Loading npcs");          // Hellbound (Type: 27)
    make_sprite("Troll", SPRID_MOB + 7 * 8 * 18, 40, true, "Loading npcs");         // Troll (Type: 28)
    make_sprite("EnragedTroll", SPRID_MOB + 7 * 8 * 112, 40, true, "Loading npcs"); // Enraged Troll (Type: 122)
    make_sprite("Orge", SPRID_MOB + 7 * 8 * 19, 40, true, "Loading npcs");          // Orge (Type: 29)
    make_sprite("Liche", SPRID_MOB + 7 * 8 * 20, 40, true, "Loading npcs");         // Liche (Type: 30)
    make_sprite("Demon", SPRID_MOB + 7 * 8 * 21, 40, true, "Loading npcs");         // Demon (Type: 31)

    //TODO some dumbass made this - missing 40th (39) sprite
    //make_sprite("BlackDemon", SPRID_MOB + 7 * 8 * 95, 40, true, "Loading npcs");// Black Demon (Type: 105)

    make_sprite("Unicorn", SPRID_MOB + 7 * 8 * 22, 40, true, "Loading npcs");     // Unicorn (Type: 32)
    make_sprite("DarkUnicorn", SPRID_MOB + 7 * 8 * 94, 40, true, "Loading npcs"); // DarkUnicorn (Type: 104)
    make_sprite("WereWolf", SPRID_MOB + 7 * 8 * 23, 40, true, "Loading npcs");    // WereWolf (Type: 33)
    make_sprite("Dummy", SPRID_MOB + 7 * 8 * 24, 40, true, "Loading npcs");       // Dummy (Type: 34)

    for (i = 0; i < 40; i++)
        data_list.push({ SPRID_MOB + i + 7 * 8 * 25, SPRITETYPE_SPRITE, "Effect5", 0, false, "Loading npcs" });

    make_sprite("GT-Arrow", SPRID_MOB + 7 * 8 * 26, 40, true, "Loading npcs");        // Arrow-GuardTower (Type: 36)
    make_sprite("GT-Cannon", SPRID_MOB + 7 * 8 * 27, 40, true, "Loading npcs");       // Cannon-GuardTower (Type: 37)
    make_sprite("ManaCollector", SPRID_MOB + 7 * 8 * 28, 40, true, "Loading npcs");   // Mana Collector (Type: 38)
    make_sprite("Detector", SPRID_MOB + 7 * 8 * 29, 40, true, "Loading npcs");        // Detector (Type: 39)
    make_sprite("ESG", SPRID_MOB + 7 * 8 * 30, 40, true, "Loading npcs");             // ESG (Type: 40)
    make_sprite("GMG", SPRID_MOB + 7 * 8 * 31, 40, true, "Loading npcs");             // GMG (Type: 41)
    make_sprite("ManaStone", SPRID_MOB + 7 * 8 * 32, 40, true, "Loading npcs");       // ManaStone (Type: 42)
    make_sprite("LWB", SPRID_MOB + 7 * 8 * 33, 40, true, "Loading npcs");             // Light War Beetle (Type: 43)
    make_sprite("GHK", SPRID_MOB + 7 * 8 * 34, 40, true, "Loading npcs");             // God's Hand Knight (Type: 44)
    make_sprite("GHKABS", SPRID_MOB + 7 * 8 * 35, 40, true, "Loading npcs");          // God's Hand Knight with Armored Battle Steed (Type: 45)
    make_sprite("TK", SPRID_MOB + 7 * 8 * 36, 40, true, "Loading npcs");              // Temple Knight (Type: 46)
    make_sprite("BG", SPRID_MOB + 7 * 8 * 37, 40, true, "Loading npcs");              // Battle Golem (Type: 47)
    make_sprite("Stalker", SPRID_MOB + 7 * 8 * 38, 40, true, "Loading npcs");         // Stalker (Type: 48)
    make_sprite("Hellclaw", SPRID_MOB + 7 * 8 * 39, 40, true, "Loading npcs");        // Hellclaw (Type: 49)
    make_sprite("Tigerworm", SPRID_MOB + 7 * 8 * 40, 40, true, "Loading npcs");       // Tigerworm (Type: 50)
    make_sprite("Catapult", SPRID_MOB + 7 * 8 * 41, 40, true, "Loading npcs");        // Catapult (Type: 51)
    make_sprite("Gagoyle", SPRID_MOB + 7 * 8 * 42, 40, true, "Loading npcs");         // Gargoyle (Type: 52)
    make_sprite("EnragedGagoyle", SPRID_MOB + 7 * 8 * 115, 40, true, "Loading npcs"); // Enraged Gagoyle (Type: 125)
    make_sprite("Beholder", SPRID_MOB + 7 * 8 * 43, 40, true, "Loading npcs");        // Beholder (Type: 53)
    make_sprite("DarkElf", SPRID_MOB + 7 * 8 * 44, 40, true, "Loading npcs");         // Dark-Elf (Type: 54)
    make_sprite("Bunny", SPRID_MOB + 7 * 8 * 45, 40, true, "Loading npcs");           // Bunny (Type: 55)
    make_sprite("Cat", SPRID_MOB + 7 * 8 * 46, 40, true, "Loading npcs");             // Cat (Type: 56)
    make_sprite("GiantFrog", SPRID_MOB + 7 * 8 * 47, 40, true, "Loading npcs");       // GiantFrog (Type: 57)
    make_sprite("MTGiant", SPRID_MOB + 7 * 8 * 48, 40, true, "Loading npcs");         // Mountain Giant (Type: 58)
    make_sprite("Ettin", SPRID_MOB + 7 * 8 * 49, 40, true, "Loading npcs");           // Ettin (Type: 59)
    make_sprite("CanPlant", SPRID_MOB + 7 * 8 * 50, 40, true, "Loading npcs");        // Cannibal Plant (Type: 60)
    make_sprite("Rudolph", SPRID_MOB + 7 * 8 * 51, 40, true, "Loading npcs");         // Rudolph (Type: 61)
    make_sprite("DireBoar", SPRID_MOB + 7 * 8 * 52, 40, true, "Loading npcs");        // Boar (Type: 62)
    make_sprite("frost", SPRID_MOB + 7 * 8 * 53, 40, true, "Loading npcs");           // Frost (Type: 63)
    make_sprite("Crop", SPRID_MOB + 7 * 8 * 54, 40, true, "Loading npcs");            // Crop(Type: 64)
    make_sprite("IceGolem", SPRID_MOB + 7 * 8 * 55, 40, true, "Loading npcs");        // IceGolem (Type: 65)
    make_sprite("Wyvern", SPRID_MOB + 7 * 8 * 56, 24, true, "Loading npcs");          // Wyvern (Type: 66)
    make_sprite("McGaffin", SPRID_MOB + 7 * 8 * 57, 16, true, "Loading npcs");        // McGaffin (Type: 67)
    make_sprite("Perry", SPRID_MOB + 7 * 8 * 58, 16, true, "Loading npcs");           // Perry (Type: 68)
    make_sprite("Devlin", SPRID_MOB + 7 * 8 * 59, 16, true, "Loading npcs");          // Devlin (Type: 69)
    make_sprite("Barlog", SPRID_MOB + 7 * 8 * 60, 40, true, "Loading npcs");          // Barlog (Type: 70)
    make_sprite("Centaurus", SPRID_MOB + 7 * 8 * 61, 40, true, "Loading npcs");       // Centaurus (Type: 71)
    make_sprite("ClawTurtle", SPRID_MOB + 7 * 8 * 62, 40, true, "Loading npcs");      // Claw-Turtle (Type: 72)
    make_sprite("FireWyvern", SPRID_MOB + 7 * 8 * 63, 24, true, "Loading npcs");      // Fire-Wyvern (Type: 73)
    make_sprite("GiantCrayfish", SPRID_MOB + 7 * 8 * 64, 40, true, "Loading npcs");   // Giant-Crayfish (Type: 74)
    make_sprite("GiantLizard", SPRID_MOB + 7 * 8 * 65, 40, true, "Loading npcs");     // Giant-Lizard (Type: 75)
    make_sprite("DarkWyvern", SPRID_MOB + 7 * 8 * 98, 24, true, "Loading npcs");      // Dark-Wyvern (Type: 108)
    make_sprite("EarthWyvern", SPRID_MOB + 7 * 8 * 96, 24, true, "Loading npcs");     // Earth-Wyvern (Type: 106)
    make_sprite("GrassWyvern", SPRID_MOB + 7 * 8 * 99, 24, true, "Loading npcs");     // Grass-Wyvern (Type: 109)
    make_sprite("MetalWyvern", SPRID_MOB + 7 * 8 * 102, 24, true, "Loading npcs");    // Metal-Wyvern (Type: 112)
    make_sprite("SeaWyvern", SPRID_MOB + 7 * 8 * 101, 24, true, "Loading npcs");      // Sea-Wyvern (Type: 111)
    make_sprite("SkyWyvern", SPRID_MOB + 7 * 8 * 100, 24, true, "Loading npcs");      // Sky-Wyvern (Type: 110)
    make_sprite("VoidWyvern", SPRID_MOB + 7 * 8 * 103, 24, true, "Loading npcs");     // Void-Wyvern (Type: 113)

    //TODO: another retard made this (missing last sprite)
    //make_sprite("WindWyvern", SPRID_MOB + 7 * 8 * 97, 24, true, "Loading npcs");// Wind-Wyvern (Type: 107)

    make_sprite("GiantPlant", SPRID_MOB + 7 * 8 * 66, 40, true, "Loading npcs");       // Giant-Plant (Type: 76)
    make_sprite("MasterMageOrc", SPRID_MOB + 7 * 8 * 67, 40, true, "Loading npcs");    // MasterMage-Orc (Type: 77)
    make_sprite("Minotaurs", SPRID_MOB + 7 * 8 * 68, 40, true, "Loading npcs");        // Minotaurs (Type: 78)
    make_sprite("Nizie", SPRID_MOB + 7 * 8 * 69, 40, true, "Loading npcs");            // Nizie (Type: 79)
    make_sprite("Tentocle", SPRID_MOB + 7 * 8 * 70, 40, true, "Loading npcs");         // Tentocle (Type: 80)
    make_sprite("yspro", SPRID_MOB + 7 * 8 * 71, 32, true, "Loading npcs");            // Abaddon (Type: 81)
    make_sprite("Sorceress", SPRID_MOB + 7 * 8 * 72, 40, true, "Loading npcs");        // Sorceress (Type: 82)
    make_sprite("TPKnight", SPRID_MOB + 7 * 8 * 73, 40, true, "Loading npcs");         // TPKnight (Type: 83)
    make_sprite("ElfMaster", SPRID_MOB + 7 * 8 * 74, 40, true, "Loading npcs");        // ElfMaster (Type: 84)
    make_sprite("DarkKnight", SPRID_MOB + 7 * 8 * 75, 40, true, "Loading npcs");       // DarkKnight (Type: 85)
    make_sprite("HBTank", SPRID_MOB + 7 * 8 * 76, 32, true, "Loading npcs");           // HeavyBattleTank (Type: 86)
    make_sprite("CBTurret", SPRID_MOB + 7 * 8 * 77, 32, true, "Loading npcs");         // CBTurret (Type: 87)
    make_sprite("Babarian", SPRID_MOB + 7 * 8 * 78, 40, true, "Loading npcs");         // Babarian (Type: 88)
    make_sprite("ACannon", SPRID_MOB + 7 * 8 * 79, 32, true, "Loading npcs");          // ACannon (Type: 89)
    make_sprite("EternalDragon", SPRID_MOB + 7 * 8 * 104, 32, true, "Loading npcs");   // Eternal dragon (Type: 114)
    make_sprite("EnragedCyclops", SPRID_MOB + 7 * 8 * 113, 40, true, "Loading npcs");  // Enraged Cyclops (Type: 123)
    make_sprite("EnragedStalker", SPRID_MOB + 7 * 8 * 114, 40, true, "Loading npcs");  // Enraged Stalker (Type: 124)
    make_sprite("EnragedHellclaw", SPRID_MOB + 7 * 8 * 116, 40, true, "Loading npcs"); // Enraged Hellclaw (Type: 126)
    make_sprite("EnragedTW", SPRID_MOB + 7 * 8 * 117, 40, true, "Loading npcs");       // Enraged Tigerworm (Type: 127)

    make_sprite("Gail", SPRID_MOB + 7 * 8 * 80, 8, true, "Loading npcs");  // Gail (Type: 90)
    make_sprite("Gate", SPRID_MOB + 7 * 8 * 81, 24, true, "Loading npcs"); // Heldenian Gate (Type: 91)/**/
    make_sprite("Scarecrow", SPRID_MOB + 7 * 8 * 82, 40, true, "Loading npcs");
    make_sprite("Princess", SPRID_MOB + 7 * 8 * 92, 8, true, "Loading npcs"); // Princess 102

    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mpt", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mpt", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mpt", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mpt", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mpt", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mpt", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mpt", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mpt", i + 12 * 7, false, "Loading items" });

    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mhr", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mhr", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mhr", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mhr", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mhr", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mhr", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mhr", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mhr", i + 12 * 7, false, "Loading items" });

    make_sprite("MLArmor", SPRID_BODYARMOR_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MCMail", SPRID_BODYARMOR_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MSMail", SPRID_BODYARMOR_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MPMail", SPRID_BODYARMOR_M + 15 * 4, 12, true, "Loading items");
    //make_sprite( "MPMail",	SPRID_BODYARMOR_M + 15*25, 12, true, "Loading items");
    make_sprite("Mtunic", SPRID_BODYARMOR_M + 15 * 5, 12, true, "Loading items");
    make_sprite("MRobe1", SPRID_BODYARMOR_M + 15 * 6, 12, true, "Loading items");
    make_sprite("MSanta", SPRID_BODYARMOR_M + 15 * 7, 12, true, "Loading items");
    make_sprite("MHPMail1", SPRID_BODYARMOR_M + 15 * 8, 12, true, "Loading items");       //hero
    make_sprite("MHPMail2", SPRID_BODYARMOR_M + 15 * 9, 12, true, "Loading items");       //hero
    make_sprite("MHRobe1", SPRID_BODYARMOR_M + 15 * 10, 12, true, "Loading items");       //hero
    make_sprite("MHRobe2", SPRID_BODYARMOR_M + 15 * 11, 12, true, "Loading items");       //hero
    make_sprite("AncHeroArmorM", SPRID_BODYARMOR_M + 15 * 12, 12, true, "Loading items"); //hero // Black Ancient Hero Armor M
    make_sprite("AncHeroRobeM", SPRID_BODYARMOR_M + 15 * 13, 12, true, "Loading items");  //hero // Black Ancient Hero Robe M
    make_sprite("ALegRobeM", SPRID_BODYARMOR_M + 15 * 14, 12, true, "Loading items");     // Aresden Ancient Hero Robe M
    make_sprite("ELegRobeM", SPRID_BODYARMOR_M + 15 * 15, 12, true, "Loading items");     // Elvine Ancient Hero Robe M
    make_sprite("ALegArmorM", SPRID_BODYARMOR_M + 15 * 16, 12, true, "Loading items");    // Aresden Ancient Hero Armor M
    make_sprite("ELegArmorM", SPRID_BODYARMOR_M + 15 * 17, 12, true, "Loading items");    // Elvine Ancient Hero Armor M
    make_sprite("MShirt", SPRID_BERK_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHauberk", SPRID_BERK_M + 15 * 2, 12, true, "Loading items");
    //make_sprite( "MHauberk",	  SPRID_BERK_M + 15*25, 12, true, "Loading items");
    make_sprite("MHHauberk1", SPRID_BERK_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MHHauberk2", SPRID_BERK_M + 15 * 4, 12, true, "Loading items");
    make_sprite("AncHeroHauberkM", SPRID_BERK_M + 15 * 5, 12, true, "Loading items"); // Black Ancient Hero Hauberk M
    make_sprite("ALegHauberkM", SPRID_BERK_M + 15 * 6, 12, true, "Loading items");    // Aresden Ancient Hero Hauberk M
    make_sprite("ELegHauberkM", SPRID_BERK_M + 15 * 7, 12, true, "Loading items");    // Elvine Ancient Hero Hauberk M

    make_sprite("MTrouser", SPRID_LEGG_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHTrouser", SPRID_LEGG_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MCHoses", SPRID_LEGG_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MLeggings", SPRID_LEGG_M + 15 * 4, 12, true, "Loading items");
    //make_sprite( "MLeggings",SPRID_LEGG_M + 15*25, 12, true, "Loading items");
    make_sprite("MHLeggings1", SPRID_LEGG_M + 15 * 5, 12, true, "Loading items");   // hero
    make_sprite("MHLeggings2", SPRID_LEGG_M + 15 * 6, 12, true, "Loading items");   // hero
    make_sprite("AncHeroLegM", SPRID_LEGG_M + 15 * 7, 12, true, "Loading items");   // hero // Black Ancient Hero Leggings M
    make_sprite("ALegLeggingsM", SPRID_LEGG_M + 15 * 8, 12, true, "Loading items"); // Aresden Ancient Leggings M
    make_sprite("ELegLeggingsM", SPRID_LEGG_M + 15 * 9, 12, true, "Loading items"); // Elvine Ancient Leggings M

    make_sprite("MShoes", SPRID_BOOT_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MLBoots", SPRID_BOOT_M + 15 * 2, 12, true, "Loading items");
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 1, SPRITETYPE_SPRITE, "Msw", i + 56 * 0, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 2, SPRITETYPE_SPRITE, "Msw", i + 56 * 1, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 3, SPRITETYPE_SPRITE, "Msw", i + 56 * 2, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 4, SPRITETYPE_SPRITE, "Msw", i + 56 * 3, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 5, SPRITETYPE_SPRITE, "Msw", i + 56 * 4, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 6, SPRITETYPE_SPRITE, "Msw", i + 56 * 5, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 7, SPRITETYPE_SPRITE, "Msw", i + 56 * 6, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 8, SPRITETYPE_SPRITE, "Msw", i + 56 * 7, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 9, SPRITETYPE_SPRITE, "Msw", i + 56 * 8, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 10, SPRITETYPE_SPRITE, "Msw", i + 56 * 9, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 11, SPRITETYPE_SPRITE, "Msw", i + 56 * 10, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 12, SPRITETYPE_SPRITE, "Msw", i + 56 * 11, false, "Loading items" });

    make_sprite("Mswx", SPRID_WEAPON_M + 64 * 5, 56, true, "Loading items");
    make_sprite("Msw2", SPRID_WEAPON_M + 64 * 13, 56, true, "Loading items");
    make_sprite("Msw3", SPRID_WEAPON_M + 64 * 14, 56, true, "Loading items");
    make_sprite("MStormBringer", SPRID_WEAPON_M + 64 * 15, 56, true, "Loading items");
    make_sprite("MDarkExec", SPRID_WEAPON_M + 64 * 16, 56, true, "Loading items");
    make_sprite("MKlonessBlade", SPRID_WEAPON_M + 64 * 17, 56, true, "Loading items");
    make_sprite("MKlonessAstock", SPRID_WEAPON_M + 64 * 18, 56, true, "Loading items");
    make_sprite("MDebastator", SPRID_WEAPON_M + 64 * 19, 56, true, "Loading items");
    make_sprite("MAxe1", SPRID_WEAPON_M + 64 * 20, 56, true, "Loading items"); // Axe
    make_sprite("MAxe2", SPRID_WEAPON_M + 64 * 21, 56, true, "Loading items");
    make_sprite("MAxe3", SPRID_WEAPON_M + 64 * 22, 56, true, "Loading items");
    make_sprite("MAxe4", SPRID_WEAPON_M + 64 * 23, 56, true, "Loading items");
    make_sprite("MAxe5", SPRID_WEAPON_M + 64 * 24, 56, true, "Loading items");
    make_sprite("MPickAxe1", SPRID_WEAPON_M + 64 * 25, 56, true, "Loading items");
    make_sprite("MAxe6", SPRID_WEAPON_M + 64 * 26, 56, true, "Loading items");
    make_sprite("Mhoe", SPRID_WEAPON_M + 64 * 27, 56, true, "Loading items");
    make_sprite("MKlonessAxe", SPRID_WEAPON_M + 64 * 28, 56, true, "Loading items");
    make_sprite("MLightBlade", SPRID_WEAPON_M + 64 * 29, 56, true, "Loading items");
    make_sprite("MHammer", SPRID_WEAPON_M + 64 * 30, 56, true, "Loading items");
    make_sprite("MBHammer", SPRID_WEAPON_M + 64 * 31, 56, true, "Loading items");
    make_sprite("MBabHammer", SPRID_WEAPON_M + 64 * 32, 56, true, "Loading items");
    make_sprite("MBShadowSword", SPRID_WEAPON_M + 64 * 33, 56, true, "Loading items");
    make_sprite("MBerserkWand", SPRID_WEAPON_M + 64 * 34, 56, true, "Loading items");
    make_sprite("Mstaff1", SPRID_WEAPON_M + 64 * 35, 56, true, "Loading items"); // Staff
    make_sprite("Mstaff2", SPRID_WEAPON_M + 64 * 36, 56, true, "Loading items");
    make_sprite("MStaff3", SPRID_WEAPON_M + 64 * 37, 56, true, "Loading items");
    make_sprite("MReMagicWand", SPRID_WEAPON_M + 64 * 38, 56, true, "Loading items");
    make_sprite("MKlonessWand", SPRID_WEAPON_M + 64 * 39, 56, true, "Loading items");
    make_sprite("Staff4M", SPRID_WEAPON_M + 64 * 40, 56, true, "Loading items");
    // Bows 40 41 below
    make_sprite("MDirectBow", SPRID_WEAPON_M + 64 * 43, 56, true, "Loading items");
    make_sprite("MFireBow", SPRID_WEAPON_M + 64 * 44, 56, true, "Loading items");
    make_sprite("Mbo", SPRID_WEAPON_M + 64 * 41, 56, true, "Loading items");

    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_M + i + 64 * 42, SPRITETYPE_SPRITE, "Mbo", i + 56 * 1, false, "Loading items" });

    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 1, SPRITETYPE_SPRITE, "Msh", i + 7 * 0, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 2, SPRITETYPE_SPRITE, "Msh", i + 7 * 1, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 3, SPRITETYPE_SPRITE, "Msh", i + 7 * 2, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 4, SPRITETYPE_SPRITE, "Msh", i + 7 * 3, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 5, SPRITETYPE_SPRITE, "Msh", i + 7 * 4, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 6, SPRITETYPE_SPRITE, "Msh", i + 7 * 5, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 7, SPRITETYPE_SPRITE, "Msh", i + 7 * 6, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 8, SPRITETYPE_SPRITE, "Msh", i + 7 * 7, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_M + i + 8 * 9, SPRITETYPE_SPRITE, "Msh", i + 7 * 8, false, "Loading items" });

    make_sprite("Mmantle01", SPRID_MANTLE_M + 15 * 1, 12, true, "Loading items");
    make_sprite("Mmantle02", SPRID_MANTLE_M + 15 * 2, 12, true, "Loading items");
    make_sprite("Mmantle03", SPRID_MANTLE_M + 15 * 3, 12, true, "Loading items");
    make_sprite("Mmantle04", SPRID_MANTLE_M + 15 * 4, 12, true, "Loading items");
    make_sprite("Mmantle05", SPRID_MANTLE_M + 15 * 5, 12, true, "Loading items");
    make_sprite("Mmantle06", SPRID_MANTLE_M + 15 * 6, 12, true, "Loading items");
    make_sprite("Mmantle07", SPRID_MANTLE_M + 15 * 7, 12, true, "Loading items");
    make_sprite("Mmantle08", SPRID_MANTLE_M + 15 * 8, 12, true, "Loading items");
    make_sprite("AAncHeroCapeM", SPRID_MANTLE_M + 15 * 9, 12, true, "Loading items");  // Aresden Black Ancient Hero Cape M
    make_sprite("EAncHeroCapeM", SPRID_MANTLE_M + 15 * 10, 12, true, "Loading items"); // Elvine Black Ancient Hero Cape M
    make_sprite("MHelm1", SPRID_HEAD_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHelm2", SPRID_HEAD_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MHelm3", SPRID_HEAD_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MHelm4", SPRID_HEAD_M + 15 * 4, 12, true, "Loading items");
    make_sprite("NMHelm1", SPRID_HEAD_M + 15 * 5, 12, true, "Loading items");
    //make_sprite( "NMHelm1", SPRID_HEAD_M + 15*25, 12, true, "Loading items");
    make_sprite("NMHelm2", SPRID_HEAD_M + 15 * 6, 12, true, "Loading items");
    make_sprite("NMHelm3", SPRID_HEAD_M + 15 * 7, 12, true, "Loading items");
    make_sprite("NMHelm4", SPRID_HEAD_M + 15 * 8, 12, true, "Loading items");
    make_sprite("MHHelm1", SPRID_HEAD_M + 15 * 9, 12, true, "Loading items");
    make_sprite("MHHelm2", SPRID_HEAD_M + 15 * 10, 12, true, "Loading items");
    make_sprite("MHCap1", SPRID_HEAD_M + 15 * 11, 12, true, "Loading items");
    make_sprite("MHCap2", SPRID_HEAD_M + 15 * 12, 12, true, "Loading items");
    make_sprite("AncHeroHelmM", SPRID_HEAD_M + 15 * 13, 12, true, "Loading items"); // Black Ancient Hero Helm M
    make_sprite("AncHeroCapM", SPRID_HEAD_M + 15 * 14, 12, true, "Loading items");  // Black Ancient Hero Cap M
    make_sprite("ALegHelmM", SPRID_HEAD_M + 15 * 15, 12, true, "Loading items");    // Aresden Ancient Helm M
    make_sprite("ELegHelmM", SPRID_HEAD_M + 15 * 16, 12, true, "Loading items");    // Elvine Ancient Helm M
    make_sprite("ALegCapM", SPRID_HEAD_M + 15 * 17, 12, true, "Loading items");     // Aresden Ancient Cap M
    make_sprite("ELegCapM", SPRID_HEAD_M + 15 * 18, 12, true, "Loading items");     // Elvine Ancient Cap M

    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 0, SPRITETYPE_SPRITE, "Mpt", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 1, SPRITETYPE_SPRITE, "Mpt", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 2, SPRITETYPE_SPRITE, "Mpt", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 3, SPRITETYPE_SPRITE, "Mpt", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 4, SPRITETYPE_SPRITE, "Mpt", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 5, SPRITETYPE_SPRITE, "Mpt", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 6, SPRITETYPE_SPRITE, "Mpt", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_UNDIES_W + i + 15 * 7, SPRITETYPE_SPRITE, "Mpt", i + 12 * 7, false, "Loading items" });

    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 0, SPRITETYPE_SPRITE, "Mhr", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 1, SPRITETYPE_SPRITE, "Mhr", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 2, SPRITETYPE_SPRITE, "Mhr", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 3, SPRITETYPE_SPRITE, "Mhr", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 4, SPRITETYPE_SPRITE, "Mhr", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 5, SPRITETYPE_SPRITE, "Mhr", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 6, SPRITETYPE_SPRITE, "Mhr", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ SPRID_HAIR_W + i + 15 * 7, SPRITETYPE_SPRITE, "Mhr", i + 12 * 7, false, "Loading items" });

    make_sprite("WBodice1", SPRID_BODYARMOR_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WBodice2", SPRID_BODYARMOR_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WLArmor", SPRID_BODYARMOR_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WCMail", SPRID_BODYARMOR_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WSMail", SPRID_BODYARMOR_W + 15 * 5, 12, true, "Loading items");
    make_sprite("WPMail", SPRID_BODYARMOR_W + 15 * 6, 12, true, "Loading items");
    make_sprite("WRobe1", SPRID_BODYARMOR_W + 15 * 7, 12, true, "Loading items");
    make_sprite("WSanta", SPRID_BODYARMOR_W + 15 * 8, 12, true, "Loading items");
    make_sprite("WHPMail1", SPRID_BODYARMOR_W + 15 * 9, 12, true, "Loading items");       //hero
    make_sprite("WHPMail2", SPRID_BODYARMOR_W + 15 * 10, 12, true, "Loading items");      //hero
    make_sprite("WHRobe1", SPRID_BODYARMOR_W + 15 * 11, 12, true, "Loading items");       // hero
    make_sprite("WHRobe2", SPRID_BODYARMOR_W + 15 * 12, 12, true, "Loading items");       // hero
    make_sprite("AncHeroArmorW", SPRID_BODYARMOR_W + 15 * 13, 12, true, "Loading items"); //hero // Black Ancient Hero Armor W
    make_sprite("AncHeroRobeW", SPRID_BODYARMOR_W + 15 * 14, 12, true, "Loading items");  // hero // Black Ancient Hero Robe W
    make_sprite("ALegRobeW", SPRID_BODYARMOR_W + 15 * 15, 12, true, "Loading items");     // Aresden Ancient Robe W
    make_sprite("ELegRobeW", SPRID_BODYARMOR_W + 15 * 16, 12, true, "Loading items");     // Elvine Ancient Robe W
    make_sprite("ALegArmorW", SPRID_BODYARMOR_W + 15 * 17, 12, true, "Loading items");    // Aresden Ancient Armor W
    make_sprite("ELegArmorW", SPRID_BODYARMOR_W + 15 * 18, 12, true, "Loading items");    // Elvine Ancient Armor W

    make_sprite("WChemiss", SPRID_BERK_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WShirt", SPRID_BERK_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WHauberk", SPRID_BERK_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WHHauberk1", SPRID_BERK_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WHHauberk2", SPRID_BERK_W + 15 * 5, 12, true, "Loading items");
    make_sprite("AncHeroHauberkW", SPRID_BERK_W + 15 * 6, 12, true, "Loading items"); // Black Ancient Hero Hauberk W
    make_sprite("ALegHauberkW", SPRID_BERK_W + 15 * 7, 12, true, "Loading items");    // Aresden Ancient Hauberk W
    make_sprite("ELegHauberkW", SPRID_BERK_W + 15 * 8, 12, true, "Loading items");    // Elvine Ancient Hauberk W
    make_sprite("WSkirt", SPRID_LEGG_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WTrouser", SPRID_LEGG_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WHTrouser", SPRID_LEGG_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WCHoses", SPRID_LEGG_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WLeggings", SPRID_LEGG_W + 15 * 5, 12, true, "Loading items");
    make_sprite("WHLeggings1", SPRID_LEGG_W + 15 * 6, 12, true, "Loading items");
    make_sprite("WHLeggings2", SPRID_LEGG_W + 15 * 7, 12, true, "Loading items");
    make_sprite("AncHeroLegW", SPRID_LEGG_W + 15 * 8, 12, true, "Loading items");    // Black Ancient Hero Leggings W
    make_sprite("ALegLeggingsW", SPRID_LEGG_W + 15 * 9, 12, true, "Loading items");  // Aresden Ancient Leggings W
    make_sprite("ELegLeggingsW", SPRID_LEGG_W + 15 * 10, 12, true, "Loading items"); // Elvine Ancient Leggings W
    make_sprite("WShoes", SPRID_BOOT_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WLBoots", SPRID_BOOT_W + 15 * 2, 12, true, "Loading items");

    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 1, SPRITETYPE_SPRITE, "Wsw", i + 56 * 0, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 2, SPRITETYPE_SPRITE, "Wsw", i + 56 * 1, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 3, SPRITETYPE_SPRITE, "Wsw", i + 56 * 2, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 4, SPRITETYPE_SPRITE, "Wsw", i + 56 * 3, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 5, SPRITETYPE_SPRITE, "Wsw", i + 56 * 4, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 6, SPRITETYPE_SPRITE, "Wsw", i + 56 * 5, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 7, SPRITETYPE_SPRITE, "Wsw", i + 56 * 6, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 8, SPRITETYPE_SPRITE, "Wsw", i + 56 * 7, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 9, SPRITETYPE_SPRITE, "Wsw", i + 56 * 8, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 10, SPRITETYPE_SPRITE, "Wsw", i + 56 * 9, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 11, SPRITETYPE_SPRITE, "Wsw", i + 56 * 10, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 12, SPRITETYPE_SPRITE, "Wsw", i + 56 * 11, false, "Loading items" });

    make_sprite("Wswx", SPRID_WEAPON_W + 64 * 5, 56, true, "Loading items");
    make_sprite("Wsw2", SPRID_WEAPON_W + 64 * 13, 56, true, "Loading items");
    make_sprite("Wsw3", SPRID_WEAPON_W + 64 * 14, 56, true, "Loading items"); // TheVampire
    make_sprite("WStormBringer", SPRID_WEAPON_W + 64 * 15, 56, true, "Loading items");
    make_sprite("WDarkExec", SPRID_WEAPON_W + 64 * 16, 56, true, "Loading items");
    make_sprite("WKlonessBlade", SPRID_WEAPON_W + 64 * 17, 56, true, "Loading items");
    make_sprite("WKlonessAstock", SPRID_WEAPON_W + 64 * 18, 56, true, "Loading items");
    make_sprite("WDebastator", SPRID_WEAPON_W + 64 * 19, 56, true, "Loading items");
    make_sprite("WAxe1", SPRID_WEAPON_W + 64 * 20, 56, true, "Loading items"); // Axe
    make_sprite("WAxe2", SPRID_WEAPON_W + 64 * 21, 56, true, "Loading items");
    make_sprite("WAxe3", SPRID_WEAPON_W + 64 * 22, 56, true, "Loading items");
    make_sprite("WAxe4", SPRID_WEAPON_W + 64 * 23, 56, true, "Loading items");
    make_sprite("WAxe5", SPRID_WEAPON_W + 64 * 24, 56, true, "Loading items");
    make_sprite("WpickAxe1", SPRID_WEAPON_W + 64 * 25, 56, true, "Loading items");
    make_sprite("WAxe6", SPRID_WEAPON_W + 64 * 26, 56, true, "Loading items");
    make_sprite("Whoe", SPRID_WEAPON_W + 64 * 27, 56, true, "Loading items");
    make_sprite("WKlonessAxe", SPRID_WEAPON_W + 64 * 28, 56, true, "Loading items");
    make_sprite("WLightBlade", SPRID_WEAPON_W + 64 * 29, 56, true, "Loading items");
    make_sprite("WHammer", SPRID_WEAPON_W + 64 * 30, 56, true, "Loading items");
    make_sprite("WBHammer", SPRID_WEAPON_W + 64 * 31, 56, true, "Loading items");
    make_sprite("WBabHammer", SPRID_WEAPON_W + 64 * 32, 56, true, "Loading items");
    make_sprite("WBShadowSword", SPRID_WEAPON_W + 64 * 33, 56, true, "Loading items");
    make_sprite("WBerserkWand", SPRID_WEAPON_W + 64 * 34, 56, true, "Loading items");
    make_sprite("Wstaff1", SPRID_WEAPON_W + 64 * 35, 56, true, "Loading items");
    make_sprite("Wstaff2", SPRID_WEAPON_W + 64 * 36, 56, true, "Loading items");
    make_sprite("WStaff3", SPRID_WEAPON_W + 64 * 37, 56, true, "Loading items");
    make_sprite("WKlonessWand", SPRID_WEAPON_W + 64 * 39, 56, true, "Loading items");
    make_sprite("WReMagicWand", SPRID_WEAPON_W + 64 * 38, 56, true, "Loading items");
    make_sprite("Staff4W", SPRID_WEAPON_W + 64 * 40, 56, true, "Loading items");
    // bows 40 41 below
    make_sprite("WDirectBow", SPRID_WEAPON_W + 64 * 43, 56, true, "Loading items");
    make_sprite("WFireBow", SPRID_WEAPON_W + 64 * 44, 56, true, "Loading items");
    make_sprite("Wmantle01", SPRID_MANTLE_W + 15 * 1, 12, true, "Loading items");
    make_sprite("Wmantle02", SPRID_MANTLE_W + 15 * 2, 12, true, "Loading items");
    make_sprite("Wmantle03", SPRID_MANTLE_W + 15 * 3, 12, true, "Loading items");
    make_sprite("Wmantle04", SPRID_MANTLE_W + 15 * 4, 12, true, "Loading items");
    make_sprite("Wmantle05", SPRID_MANTLE_W + 15 * 5, 12, true, "Loading items");
    make_sprite("Wmantle06", SPRID_MANTLE_W + 15 * 6, 12, true, "Loading items");
    make_sprite("Wmantle07", SPRID_MANTLE_W + 15 * 7, 12, true, "Loading items");
    make_sprite("Wmantle08", SPRID_MANTLE_W + 15 * 8, 12, true, "Loading items");
    make_sprite("AAncHeroCapeW", SPRID_MANTLE_W + 15 * 9, 12, true, "Loading items");  // Aresden Black Ancient Hero Cape W
    make_sprite("EAncHeroCapeW", SPRID_MANTLE_W + 15 * 10, 12, true, "Loading items"); // Elvine Black Ancient Hero Cape W
    make_sprite("WHelm1", SPRID_HEAD_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WHelm4", SPRID_HEAD_W + 15 * 4, 12, true, "Loading items");
    make_sprite("NWHelm1", SPRID_HEAD_W + 15 * 5, 12, true, "Loading items");
    make_sprite("NWHelm2", SPRID_HEAD_W + 15 * 6, 12, true, "Loading items");
    make_sprite("NWHelm3", SPRID_HEAD_W + 15 * 7, 12, true, "Loading items");
    make_sprite("NWHelm4", SPRID_HEAD_W + 15 * 8, 12, true, "Loading items");
    make_sprite("WHHelm1", SPRID_HEAD_W + 15 * 9, 12, true, "Loading items");
    make_sprite("WHHelm2", SPRID_HEAD_W + 15 * 10, 12, true, "Loading items");
    make_sprite("WHCap1", SPRID_HEAD_W + 15 * 11, 12, true, "Loading items");
    make_sprite("WHCap2", SPRID_HEAD_W + 15 * 12, 12, true, "Loading items");
    make_sprite("AncHeroHelmW", SPRID_HEAD_W + 15 * 13, 12, true, "Loading items"); // Black Ancient Hero Helm W
    make_sprite("AncHeroCapW", SPRID_HEAD_W + 15 * 14, 12, true, "Loading items");  // Black Ancient Hero Cap W
    make_sprite("ALegHelmW", SPRID_HEAD_W + 15 * 15, 12, true, "Loading items");    // Aresden Ancient Helm M
    make_sprite("ELegHelmW", SPRID_HEAD_W + 15 * 16, 12, true, "Loading items");    // Elvine Ancient Helm M
    make_sprite("ALegCapW", SPRID_HEAD_W + 15 * 17, 12, true, "Loading items");     // Aresden Ancient Cap M
    make_sprite("ELegCapW", SPRID_HEAD_W + 15 * 18, 12, true, "Loading items");     // Elvine Ancient Cap M
    make_sprite("Wbo", SPRID_WEAPON_W + 64 * 41, 56, true, "Loading items");        // Bow

    for (i = 0; i < 56; i++)
        data_list.push({ SPRID_WEAPON_W + i + 64 * 42, SPRITETYPE_SPRITE, "Wbo", i + 56 * 1, false, "Loading items" });

    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 1, SPRITETYPE_SPRITE, "Wsh", i + 7 * 0, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 2, SPRITETYPE_SPRITE, "Wsh", i + 7 * 1, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 3, SPRITETYPE_SPRITE, "Wsh", i + 7 * 2, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 4, SPRITETYPE_SPRITE, "Wsh", i + 7 * 3, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 5, SPRITETYPE_SPRITE, "Wsh", i + 7 * 4, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 6, SPRITETYPE_SPRITE, "Wsh", i + 7 * 5, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 7, SPRITETYPE_SPRITE, "Wsh", i + 7 * 6, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 8, SPRITETYPE_SPRITE, "Wsh", i + 7 * 7, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ SPRID_SHIELD_W + i + 8 * 9, SPRITETYPE_SPRITE, "Wsh", i + 7 * 8, false, "Loading items" });

    make_effect_sprite("effect", 0, 10, false, "Loading effects");
    make_effect_sprite("effect2", 10, 3, false, "Loading effects");
    make_effect_sprite("effect3", 13, 6, false, "Loading effects");
    make_effect_sprite("effect4", 19, 5, false, "Loading effects");
    for (i = 0; i <= 6; i++)
        data_list.push({ i + 24, SPRITETYPE_EFFECT, "effect5", i + 1, false, "Loading effects" });
    make_effect_sprite("CruEffect1", 31, 9, false, "Loading effects");
    make_effect_sprite("effect6", 40, 5, false, "Loading effects");
    make_effect_sprite("effect7", 45, 12, false, "Loading effects");
    make_effect_sprite("effect8", 57, 9, false, "Loading effects");
    make_effect_sprite("effect9", 66, 21, false, "Loading effects");

    make_effect_sprite("effect10", 87, 2, false, "Loading effects");  // Effects Hero items
    make_effect_sprite("effect11", 89, 14, false, "Loading effects"); // Cancel, stormBlade, resu, GateHeldenian....etc
    make_effect_sprite("effect11s", 104, 1, false, "Loading effects");
    make_effect_sprite("yseffect2", 140, 8, false, "Loading effects"); // Abaddon's death
    make_effect_sprite("effect12", 148, 4, false, "Loading effects");  // Slates auras
    make_effect_sprite("yseffect3", 152, 16, false, "Loading effects");
    make_effect_sprite("yseffect4", 133, 7, false, "Loading effects"); // Abaddon's map thunder.
    make_effect_sprite("effects", 168, 1, false, "Loading effects");   // minimap ping

    //progressLabel = "Loading sounds";
    for (i = 1; i <= 8; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
        CSoundBuffer[i].loadFromFile(G_cTxt);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 1; i <= 50; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
        MSoundBuffer[i].loadFromFile(G_cTxt);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 1; i <= 15; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
        ESoundBuffer[i].loadFromFile(G_cTxt);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }
    for (i = 9; i <= 16; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
        CSoundBuffer[i].loadFromFile(G_cTxt);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 51; i <= 100; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
        MSoundBuffer[i].loadFromFile(G_cTxt);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 16; i <= 30; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
        ESoundBuffer[i].loadFromFile(G_cTxt);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }
    for (i = 17; i <= 24; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\C%d.wav", i);
        CSoundBuffer[i].loadFromFile(G_cTxt);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 101; i <= 156; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\M%d.wav", i);
        MSoundBuffer[i].loadFromFile(G_cTxt);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 31; i <= 54; i++)
    {
        sprintf(G_cTxt, "data\\sounds\\E%d.wav", i);
        ESoundBuffer[i].loadFromFile(G_cTxt);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }
    data_max = data_list.size();
}

void helbreath::UpdateScreen_OnLoading(bool bActive)
{
    draw_version = true;
    if (data_list.empty())
    {
        ChangeGameMode(GAMEMODE_ONCONNECTING);
        return;
    }

    UpdateScreen_OnLoading_Progress();

    std::string progress_label;

    int perform = 300; // (game->fps.getFPS() + 20) / 20;

    while (--perform)
    {
        if (!data_list.empty())
        {
            load_data item = data_list.front();
            data_list.pop();
            progress_label = item.label;
            if (item.sprite_type == SPRITETYPE_SPRITE)
            {
                m_pSprite[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
            }
            else if (item.sprite_type == SPRITETYPE_TILE)
            {
                m_pTileSpr[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
            }
            else if (item.sprite_type == SPRITETYPE_EFFECT)
            {
                m_pEffectSpr[item.id] = sprite::CreateSprite(item.name, item.num, item.alpha);
            }
        }
    }

    double percent = ((double(data_max) - data_list.size()) / double(data_max)) * 118;

    m_cLoading = percent;


    /*
    // mass convert
    mkdir("dump");
    for (int i = 0; i < MAXSPRITES; ++i)
    {
        if (m_pSprite[i])
        {
            m_pSprite[i]->_iOpenSprite();
            if (m_pSprite[i]->_localimage.getSize().x == 0)
            {
                std::cout << fmt::format("Failed to load: {} - {}\n", i, m_pSprite[i]->m_cPakFileName);
            }
            mkdir(fmt::format("dump/{}", i).c_str());
            for (int x = 0; x < m_pSprite[i]->m_iTotalFrame; ++x)
            {
                //mkdir(fmt::format("dump/{}/{}", i, x).c_str());
                std::string save = fmt::format("dump/{}/{}.png", i, x);
                //IntRect(m_stBrush[i].sx, m_stBrush[i].sy, m_stBrush[i].szx, m_stBrush[i].szy)
                sf::RenderTexture temp;
                temp.create(m_pSprite[i]->m_stBrush[x].szx, m_pSprite[i]->m_stBrush[x].szy);
                m_pSprite[i]->sprite[x].setPosition(m_pSprite[i]->m_stBrush[x].pvx, m_pSprite[i]->m_stBrush[x].pvy);
                temp.draw(m_pSprite[i]->sprite[x]);
                temp.display();
                //std::cout << fmt::format("{} Size: {} x {} - Pivot: ({}, {}) - Source: ({}, {})\n", save, m_pSprite[i]->m_stBrush[x].szx, m_pSprite[i]->m_stBrush[x].szy, m_pSprite[i]->m_stBrush[x].pvx, m_pSprite[i]->m_stBrush[x].pvy, m_pSprite[i]->m_stBrush[x].sx, m_pSprite[i]->m_stBrush[x].sy);
                temp.getTexture().copyToImage().saveToFile(save);
            }
            m_pSprite[i]->_localimage.copyToImage().saveToFile(fmt::format("dump/{}/master.png", i));
        }
    }*/
}

void helbreath::UpdateScreen_OnLoading_Progress()
{
    m_pSprite[SPRID_INTERFACE_ND_LOADING]->_iOpenSprite();
    int16_t bgx = screenwidth / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[0].szx / 2;
    int16_t bgy = screenheight / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[0].szy / 2;
    DrawNewDialogBox(SPRID_INTERFACE_ND_LOADING, bgx, bgy, 0, true);
    int16_t fgx = screenwidth / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[1].szx / 2;
    int16_t fgy = screenheight / 2 - m_pSprite[SPRID_INTERFACE_ND_LOADING]->brush[1].szy / 2;
    m_pSprite[SPRID_INTERFACE_ND_LOADING]->PutSpriteFastWidth(fgx + 426/2, fgy + 424/2, 1, m_cLoading, G_dwGlobalTime);
}

void helbreath::UpdateScreen_OnSelectCharacter()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB, cTotalChar;
    char cMIresult;
    uint64_t dwTime;
    static uint64_t dwCTime;

    int iMIbuttonNum = 0;

    dwTime = unixtime();
    sX = 0;
    sY = 0;
    cTotalChar = 0;

    //need better way to accomplish first pass data
    if (m_cGameModeCount == 0)
    {
        G_cSpriteAlphaDegree = 1;
        InitGameSettings();

        dwCTime = unixtime();
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100)
        m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 2:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus)
                    m_cCurFocus = 1;
                break;
            case 4:
                m_cCurFocus--;
                if (m_cCurFocus <= 0)
                    m_cCurFocus = m_cMaxFocus;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEscPressed == true)
    {
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        m_bEscPressed = false;
        return;
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;
        PlaySound('E', 14, 5);

        if (m_pCharList[m_cCurFocus - 1] != nullptr)
        {
            if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
            {
                player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
                if (m_Misc.bCheckValidString(player_name.c_str()) == true)
                {
                    m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                    m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                    ChangeGameMode(GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                    m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                    ZeroMemory(m_cMsg, sizeof(m_cMsg));
                    strcpy(m_cMsg, "33");
                    ZeroMemory(m_cMapName, sizeof(m_cMapName));
                    memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                    return;
                }
            }
        }
        else
        {
            ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
            return;
        }
    }

    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    UpdateScreen_OnSelectCharacter(sX, sY, msX, msY);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8)
        m_cMenuDir = 1;

    m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR]->DrawRGB(msX, msY, 0, dwTime);
    m_stMCursor.sCursorFrame = 0;

    if (cMIresult)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                if (m_cCurFocus != iMIbuttonNum)
                    m_cCurFocus = iMIbuttonNum;
                else
                {
                    if (m_pCharList[m_cCurFocus - 1] != nullptr)
                    {
                        if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                        {
                            player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                            m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
                            if (m_Misc.bCheckValidString(player_name.c_str()) == true)
                            {
                                m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                                m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                                ChangeGameMode(GAMEMODE_ONCONNECTING);
                                m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                                m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                                ZeroMemory(m_cMsg, sizeof(m_cMsg));
                                strcpy(m_cMsg, "33");
                                ZeroMemory(m_cMapName, sizeof(m_cMapName));
                                memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                                return;
                            }
                        }
                    }
                    else
                    {
                        _InitOnCreateNewCharacter();
                        ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                        return;
                    }
                }
                break;

            case 5:
                if (m_pCharList[m_cCurFocus - 1] != nullptr)
                {
                    if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                    {
                        player_name = m_pCharList[m_cCurFocus - 1]->m_cName;
                        m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                        if (m_Misc.bCheckValidString(player_name.c_str()) == true)
                        {
                            m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
                            m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
                            ChangeGameMode(GAMEMODE_ONCONNECTING);
                            m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                            m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
                            ZeroMemory(m_cMsg, sizeof(m_cMsg));
                            strcpy(m_cMsg, "33");
                            ZeroMemory(m_cMapName, sizeof(m_cMapName));
                            memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName.c_str(), 10);
                            return;
                        }
                    }
                }
                break;

            case 6:
                if (m_iTotalChar < 4)
                {
                    _InitOnCreateNewCharacter();
                    ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
                    return;
                }
                break;

            case 7:
                if ((m_pCharList[m_cCurFocus - 1] != nullptr) && (m_pCharList[m_cCurFocus - 1]->m_sLevel <= MAXDELETELEVEL))
                {
                    ChangeGameMode(GAMEMODE_ONQUERYDELETECHARACTER);
                    m_wEnterGameType = m_cCurFocus;
                    return;
                }
                break;

            case 8:
                ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD);
                return;

            case 9:
                isItemLoaded = false;
                ChangeGameMode(GAMEMODE_ONMAINMENU);
                return;
        }
    }
}

void helbreath::UpdateScreen_OnConnecting()
{
    short sX, sY, msX, msY, msZ;
    char cLB, cRB, cMB;
    uint32_t dwTime = unixtime();
    static uint32_t dwMTime, dwCTime;

    if (m_cGameModeCount == 0)
    {
        draw_version = true;
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = dwMTime = unixtime();

        if (conn == nullptr)
        {
            perform_connect();
        }
        else if (conn->get_state() == websocketpp::session::state::value::open)
        {
            ConnectionEstablishHandler(SERVERTYPE_LOG);
            return;
        }
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 1000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            if (conn)
            {
                conn->close(websocketpp::close::status::going_away, "cancel");
            }
        }
        m_bEscPressed = false;
        return;
    }

    if ((dwTime - dwMTime) > 150) dwMTime = dwTime;

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    m_bIsHideLocalCursor = true;
    switch (m_cMsg[0])
    {
//         case '2':
//             _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
//             break;

        case '3':
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
            break;

        case '4':
            // Change Password
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
            break;

        case '5':
            //DIRECTX m_DDraw.ClearBackB4();
            break;
    }
    m_bIsHideLocalCursor = false;

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162+80,125+80,2); // xRisenx added x+80 and y+80
    wsprintfA(G_cTxt, "Connecting to Server... (%d)", (dwTime - m_dwTime) / 1000);
    PutString_SprFont(172 + 35 + 80, 190 + 80, G_cTxt, 7,0,0); // xRisenx added x+80 and y+80



    // 	if ((dwTime - m_dwTime) > 7000)
    // 	{
    // 		PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING1);//"Press ESC key during long time of no"  // xRisenx added x+80 and y+80
    // 		PutAlignedString(180+80, 463+80, 195+45+80, UPDATE_SCREEN_ON_CONNECTING2);//"connection and return to the main menu."  // xRisenx added x+80 and y+80
    // 	}
    // 	else PutAlignedString(180+80, 463+80, 195+30+80, UPDATE_SCREEN_ON_CONNECTING3);//"  Connecting to server. Please wait..."  // xRisenx added x+80 and y+80
    //DrawVersion();
    ////DIRECTX m_dInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB, &cMB);//DIRECTX
    //m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);
    m_stMCursor.sCursorFrame = 8;

    //	if (//DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void helbreath::UpdateScreen_OnWaitInitData()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    uint64_t dwTime = unixtime();

    msX = m_stMCursor.sX;
    msY = m_stMCursor.sY;
    msZ = m_stMCursor.sZ;
    cLB = (m_stMCursor.LB == true) ? 1 : 0;
    cRB = (m_stMCursor.RB == true) ? 1 : 0;
    cMB = (m_stMCursor.MB == true) ? 1 : 0;

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100)
        m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 7000)
        {
            isItemLoaded = false;
            ChangeGameMode(GAMEMODE_ONMAINMENU);
            socketmode(0);
        }
        m_bEscPressed = false;
        return;
    }

    ////DIRECTX m_DDraw.ClearBackB4();//DIRECTX
    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162 + 80, 125 + 80, 2); // xRisenx added x+80 and y+80

    wsprintfA(G_cTxt, "Waiting for response... %dsec", (dwTime - m_dwTime) / 1000);
    PutString_SprFont(172 + 44 + 80, 190 + 80, G_cTxt, 7, 0, 0); // xRisenx added x+80 and y+80
    if ((dwTime - m_dwTime) > 7000)
    {
        PutAlignedString(174 + 80, 467 + 80, 190 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA1); //"Press ESC key during long time of no" // xRisenx added x+80 and y+80
        PutAlignedString(174 + 80, 467 + 80, 190 + 45 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA2); //"connection and return to the main menu." // xRisenx added x+80 and y+80
    }
    else
        PutAlignedString(174 + 80, 467 + 80, 195 + 30 + 80, UPDATE_SCREEN_ON_WAIT_INIT_DATA3); //  Connecting to server. Please wait..." // xRisenx added x+80 and y+80

    m_stMCursor.sCursorFrame = 8;
}

void helbreath::UpdateScreen_OnQuit()
{
    short msX, msY, msZ;
    char cLB, cRB, cMB;
    char cMIresult;
    int iMIbuttonNum;

    uint64_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        m_bEnterPressed = false;
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 120)
        m_cGameModeCount = 120;

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        ChangeGameMode(GAMEMODE_NULL);
        io_service_.stop();
        window.close();
        return;
    }
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 328, 83, 1, true);

    m_stMCursor.sCursorFrame = 0;

    if ((cMIresult) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(GAMEMODE_NULL);
        io_service_.stop();
        window.close();
        return;
    }
}

void helbreath::UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus)
{
    int iYear, iMonth, iDay, iHour, iMinute;
    __int64 iTemp1, iTemp2;
    char cTotalChar = 0;
    uint64_t dwTime = unixtime();
    sY = 10;

    iTemp1 = 0;
    iTemp2 = 0;
    iYear = iMonth = iDay = iHour = iMinute = 0;

    if (selectedchar)
    {
        cTotalChar++;
        switch (selectedchar->m_sSex)
        {
            case 1:
                _tmp_sOwnerType = 1;
                break;
            case 2:
                _tmp_sOwnerType = 4;
                break;
        }
        _tmp_sOwnerType += selectedchar->m_sSkinCol - 1;
        _tmp_cDir = m_cMenuDir;
        _tmp_sAppr1 = selectedchar->m_sAppr1;
        _tmp_sAppr2 = selectedchar->m_sAppr2;
        _tmp_sAppr3 = selectedchar->m_sAppr3;
        _tmp_sAppr4 = selectedchar->m_sAppr4;
        _tmp_iApprColor = selectedchar->m_iApprColor;
        _tmp_sHeadApprValue = selectedchar->m_sHeadApprValue; // Re-Coding Sprite xRisenx
        _tmp_sBodyApprValue = selectedchar->m_sBodyApprValue; // Re-Coding Sprite xRisenx
        _tmp_sArmApprValue = selectedchar->m_sArmApprValue;   // Re-Coding Sprite xRisenx
        _tmp_sLegApprValue = selectedchar->m_sLegApprValue;   // Re-Coding Sprite xRisenx
        ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
        memcpy(_tmp_cName, selectedchar->m_cName.c_str(), 10);
        _tmp_cAction = OBJECTMOVE;
        _tmp_cFrame = m_cMenuFrame;

        if (selectedchar->m_sSex != 0)
        {
            if (m_Misc.bCheckValidString(selectedchar->m_cName.c_str()) == true)
            {
                m_pEffectSpr[0]->PutTransSprite(sX + 36, sY + 50 + 28, 1, dwTime);
                setRenderTarget(DS_CS, true, Color(0, 0, 0, 0));
                DrawObject_OnMove_ForMenu(0, 0, sX + 36, sY + 50 + 28, false, dwTime, 0, 0);
                charselect.display();
                setRenderTarget(DS_VISIBLE);
                int _sLevel = selectedchar->m_sLevel;
                sprintf(G_cTxt, "Name: %s", selectedchar->m_cName.c_str());
                PutString(sX + 46 + 96, sY + 160, G_cTxt, Color(255, 255, 255)); //25,35,25);	// sY = 14

                sprintf(G_cTxt, "Level: %d", _sLevel);
                PutString(sX + 46 + 96, sY + 160 + 17, G_cTxt, Color(255, 255, 255)); //25,35,25);

                sprintf(G_cTxt, "%I64d", selectedchar->m_iExp);                                     //TODO: won't be here for ui eventually
                PutString(sX - 10 + 96, sY + 179 - 9 + 118 + 19 - 2, G_cTxt, Color(255, 255, 255)); //25,35,25);
            }
            iTemp2 = selectedchar->m_iYear * 1000000 + selectedchar->m_iMonth * 60000 + selectedchar->m_iDay * 1700 + selectedchar->m_iHour * 70 + selectedchar->m_iMinute;
            if (iTemp1 < iTemp2)
            {
                iYear = selectedchar->m_iYear;
                iMonth = selectedchar->m_iMonth;
                iDay = selectedchar->m_iDay;
                iHour = selectedchar->m_iHour;
                iMinute = selectedchar->m_iMinute;
                iTemp1 = iTemp2;
            }
        }
    }

    if (iYear != 0)
    {
        wsprintfA(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
        PutAlignedString(98 + 80 - 40, 357 + 80 - 40, 415 + 10 + 120 - 40, G_cTxt);
    }

    if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0)
        PutAlignedString(129 + 80, 321 + 80, 456 + 120, MSG_WORLDNAME1); //"ABADDON Server"
    else if (strcmp(m_cWorldServerName, "WS2") == 0)
        PutAlignedString(129, 321, 456, MSG_WORLDNAME2); //"APOCALYPSE Server"
}

void helbreath::UpdateScreen_OnGame()
{
    short sVal, sDivX, sModX, sDivY, sModY, sPivotX, sPivotY, sVPXsave, sVPYsave;
    static int iUpdateRet = -1;
    static uint32_t perfVar = 0, perfCnt = 0;
    short absX, absY, tX, tY;
    uint32_t ItemColor;
    int i, iAmount;
    uint64_t dwTime = unixtime();
    static uint64_t dwPrevChatTime = 0;
    static int imX = 0, imY = 0;
    iUpdateRet = 1;
    if (m_cGameModeCount == 0)
    {
        m_bIsRedrawPDBGS = true;
        //DIRECTX m_DDraw.ClearBackB4();
        //DIRECTX m_DDraw.ResetFrameCnt();
        m_dwFPStime = m_dwCheckConnTime = m_dwCheckSprTime = m_dwCheckChatTime = dwTime;
        if (m_bMusicStat)
            StartBGM();
        //if(m_iLevel < 40) AddEventList(UPDATE_SCREEN_ONGAME12, 10);
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 20)
        m_cGameModeCount = 20;
    // ----------------------------------------------------

    //DIRECTX//DIRECTX m_dInput.UpdateMouseState();
    m_dwCurTime = unixtime();

    if (m_bEnterPressed)
    {
        m_bEnterPressed = false;

        if (iGetTopDialogBoxIndex() == 7 && m_bIsDialogEnabled[7] && m_dialogBoxes[7].GetMode() == 1)
        {
            if (strlen(m_cGuildName) == 0)
                return;
            if (strcmp(m_cGuildName, "NONE") != 0)
            {
                bSendCommand(MSGID_REQUEST_CREATENEWGUILD, MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_dialogBoxes[7].SetMode(2);
            }
        }
        else if (iGetTopDialogBoxIndex() == 17 && m_bIsDialogEnabled[17] && m_dialogBoxes[17].GetMode() == 1)
        {
            // Drop Item Query
            if (m_bSkillUsingStatus == true)
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if ((m_bIsDialogEnabled[20] == true) && ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10); //"You can not give or drop before the actual Item transaction."
                return;
            }

            if ((m_bIsDialogEnabled[4] == true) || (m_bIsDialogEnabled[23] == true) || (m_bIsDialogEnabled[26] == true))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if (strlen(m_cAmountString) == 0)
                return;
            iAmount = atoi(m_cAmountString);

            if ((int)(m_pItemList[m_dialogBoxes[17].sView]->m_dwCount) < iAmount)
            {
                iAmount = m_pItemList[m_dialogBoxes[17].sView]->m_dwCount;
            }

            if (iAmount != 0)
            {
                if ((int)(m_pItemList[m_dialogBoxes[17].sView]->m_dwCount) >= iAmount)
                {
                    if (m_dialogBoxes[17].sV1 != 0)
                    {
                        absX = abs(m_dialogBoxes[17].sV1 - m_sPlayerX);
                        absY = abs(m_dialogBoxes[17].sV2 - m_sPlayerY);

                        if ((absX == 0) && (absY == 0))
                            AddEventList(UPDATE_SCREEN_ONGAME5, 10);
                        else if ((absX <= 8) && (absY <= 8))
                        {
                            switch (m_dialogBoxes[17].sV3)
                            {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                case 6:
                                    EnableDialogBox(20, 1, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                    //m_dialogBoxes[20].CentreOverPoint(game->m_stMCursor.sX, game->m_stMCursor.sY);
                                    m_dialogBoxes[20].sV3 = iAmount;
                                    m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                    m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                    m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                    ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                    strcpy(m_dialogBoxes[20].cStr, m_dialogBoxes[17].cStr);
                                    break;

                                case 20:
                                    EnableDialogBox(20, 3, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                    //m_dialogBoxes[20].//CentreOverPoint(game->m_stMCursor.sX, game->m_stMCursor.sY);
                                    m_dialogBoxes[20].sV3 = iAmount;
                                    m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                    m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                    m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                    ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                    GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                    break;

                                case 15:
                                case 24:
                                    EnableDialogBox(20, 2, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                    //m_dialogBoxes[20].//CentreOverPoint(game->m_stMCursor.sX, game->m_stMCursor.sY);
                                    m_dialogBoxes[20].sV3 = iAmount;
                                    m_dialogBoxes[20].sV4 = m_wCommObjectID;

                                    ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                    GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                    break;
                                case NPC_PRINCESS:
                                    EnableDialogBox(20, 10, m_dialogBoxes[17].sView, m_dialogBoxes[17].sV3);
                                    //m_dialogBoxes[20].//CentreOverPoint(game->m_stMCursor.sX, game->m_stMCursor.sY);
                                    m_dialogBoxes[20].sV3 = iAmount;
                                    m_dialogBoxes[20].sV4 = m_wCommObjectID;
                                    m_dialogBoxes[20].sV5 = m_dialogBoxes[17].sV1;
                                    m_dialogBoxes[20].sV6 = m_dialogBoxes[17].sV2;

                                    ZeroMemory(m_dialogBoxes[20].cStr, sizeof(m_dialogBoxes[20].cStr));
                                    GetNpcName(m_dialogBoxes[17].sV3, m_dialogBoxes[20].cStr);
                                    break;
                                case 1000: // Trade stackable items
                                           // hum, d�j� affich�? , j'attends le retour et je d�sactive!
                                           /*m_stDialogBoxInfo[27].sV1 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSprite;
                                                   m_stDialogBoxInfo[27].sV2 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSpriteFrame;
                                                   m_stDialogBoxInfo[27].sV3 = iAmount;
                                                   m_stDialogBoxInfo[27].sV4 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_cItemColor;*/

                                    if (m_stDialogBoxExchangeInfo[0].sV1 == -1)
                                        m_stDialogBoxExchangeInfo[0].sItemID = m_dialogBoxes[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)
                                        m_stDialogBoxExchangeInfo[1].sItemID = m_dialogBoxes[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)
                                        m_stDialogBoxExchangeInfo[2].sItemID = m_dialogBoxes[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)
                                        m_stDialogBoxExchangeInfo[3].sItemID = m_dialogBoxes[17].sV4;
                                    else
                                        return; // Impossible case, tested at function beginning

                                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_SETEXCHANGEITEM, 0, m_dialogBoxes[17].sV4, iAmount, 0, 0);
                                    break;

                                case 1001:
                                    // Sell List
                                    for (i = 0; i < MAXSELLLIST; i++)
                                        if (m_stSellItemList[i].iIndex == -1)
                                        {
                                            m_stSellItemList[i].iIndex = m_dialogBoxes[17].sV4;
                                            m_stSellItemList[i].iAmount = iAmount;
                                            m_bIsItemDisabled[m_dialogBoxes[17].sV4] = true;
                                            break;
                                        }
                                    if (i == MAXSELLLIST)
                                        AddEventList(UPDATE_SCREEN_ONGAME6, 10);
                                    break;

                                case 1002:
                                    if (_iGetBankItemCount() < MAXBANKITEMS)
                                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, iAmount, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4); //v1.4
                                    else
                                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                                    break;

                                case 1003: // guild bank
                                    if (m_guildBankMap.size() < maxGWHItems[m_guild.m_upgrades[GU_WAREHOUSE]])
                                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, m_dialogBoxes[39].sV1, iAmount, m_dialogBoxes[39].sV5, m_dialogBoxes[39].sV6, m_pItemList[m_dialogBoxes[39].sV1]->m_cName, m_dialogBoxes[39].sV4);
                                    else
                                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                                    break;

                                case 1004: // mail item send
                                    if (m_dialogBoxes[DIALOG_MAILBOX].vvec.size() < MAX_MAIL_ITEMS)
                                    {
                                        MailItemSend item;
                                        item.index = m_dialogBoxes[17].sView;
                                        item.count = iAmount;
                                        m_dialogBoxes[DIALOG_MAILBOX].vvec.push_back(item);
                                    }
                                    else
                                    {
                                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY10, 10);
                                    }
                                    break;

                                default:
                                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, (char)(m_dialogBoxes[17].sView), iAmount, m_dialogBoxes[17].sV1, m_dialogBoxes[17].sV2, m_pItemList[m_dialogBoxes[17].sView]->m_cName);
                                    break;
                            }
                            m_bIsItemDisabled[m_dialogBoxes[17].sView] = true;
                        }
                        else
                            AddEventList(UPDATE_SCREEN_ONGAME7, 10);
                    }
                    else
                    {
                        if (iAmount <= 0)
                        {
                            AddEventList(UPDATE_SCREEN_ONGAME8, 10);
                        }
                        else
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_ITEMDROP, 0, m_dialogBoxes[17].sView, iAmount, 0, m_pItemList[m_dialogBoxes[17].sView]->m_cName);
                            m_bIsItemDisabled[m_dialogBoxes[17].sView] = true;
                        }
                    }
                }
                else
                {
                    AddEventList(UPDATE_SCREEN_ONGAME9, 10); //"You entered more quantity that you can carry."
                }
            }
            DisableDialogBox(17);
        }
        else if (iGetTopDialogBoxIndex() == 58 && m_bIsDialogEnabled[58] && m_dialogBoxes[58].GetMode() == 1)
        {

            // contribute gold/maj to guild

            if (m_bSkillUsingStatus)
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if ((m_bIsDialogEnabled[20] == true) && ((m_dialogBoxes[20].GetMode() == 1) || (m_dialogBoxes[20].GetMode() == 2)))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10); //"You can not give or drop before the actual Item transaction."
                return;
            }

            if ((m_bIsDialogEnabled[4] == true) || (m_bIsDialogEnabled[23] == true) || (m_bIsDialogEnabled[26] == true))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if (strlen(m_cAmountString) == 0)
                return;
            iAmount = atoi(m_cAmountString);

            switch (m_dialogBoxes[58].sView)
            {
                case 0: // Gold
                    if (GetGold() < iAmount)
                        iAmount = GetGold();
                    break;
                case 1: // Maj
                    if (m_iGizonItemUpgradeLeft < iAmount)
                        iAmount = m_iGizonItemUpgradeLeft;
                    break;
                case 2: // Cont
                    if (m_iContribution < iAmount)
                        iAmount = m_iContribution;
                    break;
            }

            if (iAmount <= 0)
            {
                AddEventList(UPDATE_SCREEN_ONGAME8, 10);
            }
            else
            {
                bSendCommand(MSGID_GUILDCONTRIBUTE, 0, 0, m_dialogBoxes[58].sView, iAmount, 0, m_pItemList[m_dialogBoxes[58].sView]->m_cName);
            }
            DisableDialogBox(58);
        }
        else if (iGetTopDialogBoxIndex() == DIALOG_GUILD && m_bIsDialogEnabled[DIALOG_GUILD] &&
            m_dialogBoxes[DIALOG_GUILD].GetMode() == 10)
        {
        }
        else if (iGetTopDialogBoxIndex() == DIALOG_MAILBOX && m_bIsDialogEnabled[DIALOG_MAILBOX] &&
            m_dialogBoxes[DIALOG_MAILBOX].GetMode() >= 1 && m_dialogBoxes[DIALOG_MAILBOX].GetMode() < 5)
        {
        }
        else
        {

        }
    }

    sVPXsave = m_sViewPointX;
    sVPYsave = m_sViewPointY;

    if ((m_iCameraShakingDegree > 0) && (iUpdateRet != 0))
    {
        m_sViewPointX += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);
        m_sViewPointY += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);
        m_iCameraShakingDegree--;
        if (m_iCameraShakingDegree <= 0)
            m_iCameraShakingDegree = 0;
    }
    sPivotX = m_pMapData->m_sPivotX;
    sPivotY = m_pMapData->m_sPivotY;
    sVal = m_sViewPointX - (sPivotX * 32);
    sDivX = sVal / 32;
    sModX = sVal % 32;
    sVal = m_sViewPointY - (sPivotY * 32);
    sDivY = sVal / 32;
    sModY = sVal % 32;
    if (fullscreenswap)
        iUpdateRet = 1;
    if (iUpdateRet != 0)
        DrawBackground(sDivX, sModX, sDivY, sModY);

    bg.display();
    const sf::Texture & t = bg.getTexture();
    sf::Sprite sprite = sf::Sprite(t);
    sprite.setPosition(0 - sModX, 0 - sModY);
    visible.draw(sprite);

    if (iUpdateRet != 0)
        DrawEffectLights();
    if (iUpdateRet != 0)
        DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, game->m_stMCursor.sX, game->m_stMCursor.sY);

    if (m_showGrid)
    {
        float ix, iy, iyMax = {};
        int16_t indexY = sDivY + m_pMapData->m_sPivotY;
        for (iy = -sModY; iy < GetHeight() + 32; iy += 32)
        {
            int16_t indexX = sDivX + m_pMapData->m_sPivotX;
            for (ix = -sModX; ix < GetWidth() + 32; ix += 32)
            {
                {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(ix - 16, iy - 16), Color(127, 127, 0, 127)),
                        sf::Vertex(sf::Vector2f(ix - 16, iy + 16), Color(127, 127, 0, 127)) };
                    visible.draw(line, 2, sf::Lines);
                }
                {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(ix - 16, iy - 16), Color(127, 127, 0, 127)),
                        sf::Vertex(sf::Vector2f(ix + 16, iy - 16), Color(127, 127, 0, 127)) };
                    visible.draw(line, 2, sf::Lines);
                }

                //                 char text[20];
                //                 sprintf(text, "(%d,%d)\n%d-%d", (m_sViewPointX + ix) / 32, (m_sViewPointY + iy) / 32, m_pMapData->m_tile[indexX][indexY].m_sObjectSprite, m_pMapData->m_tile[indexX][indexY].m_sObjectSpriteFrame);
                //
                //                 _text.setFont(_font.at("default"));
                //                 _text.setString(text);
                //                 _text.setFillColor(Color(255, 0, 0, 180));
                //                 _text.setPosition(ix - 16, iy - 8);
                //                 _text.setOutlineColor(Color(0, 0, 0, 128));
                //                 _text.setOutlineThickness(1);
                //                 _text.setCharacterSize(10);
                //                 visible.draw(_text);
                indexX++;
            }
            indexY++;
        }
    }

    if (iUpdateRet != 0)
    {
        DrawEffects();
        DrawWeatherEffects();
        DrawChatMsgs(-100, 0, GetWidth(), GetHeight());
        WeatherObjectFrameCounter();
    }

    if (m_cMapIndex == 26) //Snoopy: Add Apocalypse map effect (fires in inferniaA)
    {
        m_pEffectSpr[89]->PutTransSprite(1296 - m_sViewPointX, 1283 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1520 - m_sViewPointX, 1123 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1488 - m_sViewPointX, 3971 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[93]->PutTransSprite(2574 - m_sViewPointX, 3677 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[93]->PutTransSprite(3018 - m_sViewPointX, 3973 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
    }
    else if (m_cMapIndex == 27) //Add Apocalypse map effect (fires in inferniaB)
    {
        m_pEffectSpr[89]->PutTransSprite(1293 - m_sViewPointX, 3657 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(944 - m_sViewPointX, 3881 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1325 - m_sViewPointX, 4137 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
        m_pEffectSpr[89]->PutTransSprite(1648 - m_sViewPointX, 3913 - m_sViewPointY, _tmp_iEffectFrame % 12, 0);
    }
    //Snoopy: Add Apocalypse Gate and apocalypse map effects (if no Gate, m_iGatePositX will be -1...
    if ((m_iGatePositX >= m_sViewPointX / 32) && (m_iGatePositX <= m_sViewPointX / 32 + 20) && (m_iGatePositY >= m_sViewPointY / 32) && (m_iGatePositY <= m_sViewPointY / 32 + 15))
    {
        m_pEffectSpr[101]->PutTransSprite(m_iGatePositX * 32 - m_sViewPointX - 96, m_iGatePositY * 32 - m_sViewPointY - 69, _tmp_iEffectFrame % 30, dwTime);
    }

    if ((iUpdateRet != 0))
    {
        if ((m_bIsDialogEnabled[7] && m_dialogBoxes[7].GetMode() == 1) ||
            (m_bIsDialogEnabled[17] && m_dialogBoxes[17].GetMode() == 1) ||
            (m_bIsDialogEnabled[58] && m_dialogBoxes[58].GetMode() == 1) ||
            (m_bIsDialogEnabled[DIALOG_MAILBOX] && m_dialogBoxes[DIALOG_MAILBOX].GetMode() == 1) ||
            (m_bIsDialogEnabled[DIALOG_GUILD] && m_dialogBoxes[DIALOG_GUILD].GetMode() == 10))
            //{}else //DIRECTX m_DDraw.DrawShadowBox(0, 413, 639, 429);
        {
        }
        //         else //DIRECTX m_DDraw.DrawShadowBox(0, 529, 799, 546); // 800x600 Resolution xRisenx
        //             ShowReceivedString();
    }

    if (iUpdateRet != 0)
        ShowEventList(m_dwCurTime);

    if ((iUpdateRet != 0) && (m_stMCursor.cSelectedObjectType == SELECTEDOBJTYPE_ITEM) &&
        (m_pItemList[m_stMCursor.sSelectedObjectID] != 0))
    {
        ItemColor = m_pItemList[m_stMCursor.sSelectedObjectID]->m_ItemColor;
        if (ItemColor != 0)
        {
            if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_LHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_RHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == EQUIPPOS_TWOHAND))
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(game->m_stMCursor.sX - m_stMCursor.sDistX, game->m_stMCursor.sY - m_stMCursor.sDistY,
                    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                    ItemColor,
                    dwTime);
            }
            else
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteRGB(game->m_stMCursor.sX - m_stMCursor.sDistX, game->m_stMCursor.sY - m_stMCursor.sDistY,
                    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                    ItemColor,
                    dwTime);
            }
        }
        else
            m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->PutSpriteFast(game->m_stMCursor.sX - m_stMCursor.sDistX, game->m_stMCursor.sY - m_stMCursor.sDistY,
                m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame, dwTime);

        if (isItemLoaded == false)
        {
            _LoadItemContents(); // Item Display - ShadowEvil
            isItemLoaded = true;
        }
        isCommon = isUncommon = isRare = isEpic = isLegendary = false;

        int iLoc;
        std::vector<string> * lines = GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID]);

        iLoc = 0;

        std::vector<string>::iterator iter = lines->begin(), end = lines->end();

        uint32_t maxWidth = 0;

        while (iter != end)
        {
            if (iter->size() > maxWidth)
                maxWidth = iter->size();

            ++iter;
        }

        //DIRECTX m_DDraw.DrawShadowBox(game->m_stMCursor.sX - 3, game->m_stMCursor.sY + 25 - 1, game->m_stMCursor.sX + maxWidth * 5.5, game->m_stMCursor.sY + 25+1 + 13*lines->size());

        iter = lines->begin();

        PutString(m_stMCursor.sX, m_stMCursor.sY + 25, iter->c_str(), m_itemColor, false, 1);
        iLoc += 13;
        ++iter;

        while (iter != end)
        {
            PutString(m_stMCursor.sX, m_stMCursor.sY + 25 + iLoc, iter->c_str(), Color(150, 150, 150), false, 1);
            iLoc += 13;
            ++iter;
        }

        // This is handled inside the GetItemName() function.
        //if ( iLoc == 13 )
        //{
        //	iLoc = 0;
        //	for ( int iTmp = 0 ; iTmp < MAXITEMS ; iTmp++ )
        //	{
        //		if( m_pItemList[iTmp] != NULL )
        //		{
        //			if( strcmp(m_pItemList[iTmp]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName ) == 0 )
        //				iLoc++;
        //		}
        //	}
        //	if( iLoc > 1 )
        //	{
        //		wsprintfA(G_cTxt, MSG_TOTAL_NUMBER, iLoc);
        //		PutString(game->m_stMCursor.sX, game->m_stMCursor.sY + 40, G_cTxt,video::SColor(255,150,150,150), FALSE, 1);
        //	}
        //}
    }

    //Snoopy: Add Apocalypse map effect (druncncity bubbles)
    if (iUpdateRet && m_cMapIndex == 25)
        //{	bAddNewEffect(13, m_sViewPointX + rand() %640, m_sViewPointY + rand() %480, 0, 0, -1*(rand() %80), 1);
    {
        bAddNewEffect(13, m_sViewPointX + rand() % GetWidth(), m_sViewPointY + rand() % GetHeight(), 0, 0, -1 * (rand() % 80), 1); // 800x600 Resolution xRisenx
        DrawDruncncity();
    }

    if (iUpdateRet)
    {
        if (m_displayQuest && m_stQuest.sQuestType)
            DrawQuestHelper();

        if (m_bIsHeldenianMode && m_bIsHeldenianMap)
            DrawHeldenianStats();

        if (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)
            DrawAstoriaStats();

        if (m_bSoccerMode && memcmp(m_cCurLocation, "fight", 5) == 0)
            DrawSoccerInfo();

        DrawTopMsg();
    }

#ifdef _DEBUG
#ifdef MOUSEDEBUG
    if (iUpdateRet)
    {
        wsprintfA(G_cTxt, "M(%d,%d) T(%d,%d)", game->m_stMCursor.sX, game->m_stMCursor.sY, (m_sViewPointX + game->m_stMCursor.sX) / 32, (m_sViewPointY + game->m_stMCursor.sY) / 32);
        PutString(game->m_stMCursor.sX, game->m_stMCursor.sY + 30, G_cTxt, video::SColor(255, 255, 255, 255));
    }
#endif
#endif

    // LogOut process
    if (m_cLogOutCount > 0)
    {
        if ((dwTime - m_dwLogOutCountTime) > 1000) // Change to 1000 before releasing xRisenx
        {
            m_cLogOutCount--;
            m_dwLogOutCountTime = dwTime;
            wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME13, m_cLogOutCount);
            AddEventList(G_cTxt, 10);
        }
    }
    // ShadowEvil - Close Party Logout Process
    if (m_cLogOutCount <= 1 && m_cLogOutCount > 0)
    {
        m_iPartyStatus = 0;
        m_iTotalPartyMember = 0;
        ClearPartyMembers();
    }
    // Logout
    if (m_cLogOutCount == 0)
    {
        socketmode(0);
        isItemLoaded = false;
        conn->close(1000, "logout");
        m_bEscPressed = false;

        PlaySound('E', 14, 5);
        //DIRECTX		if (m_bSoundFlag) m_pESound[38]->bStop();
        if ((m_bSoundFlag) && (m_bMusicStat == true))
        {
            //DIRECTX			if (m_pBGM != NULL) m_pBGM->bStop();
        }
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
        return;
    }

    // Restart Process
    if (m_cRestartCount > 0)
    {
        if ((dwTime - m_dwRestartCountTime) > 1000)
        {
            m_cRestartCount--;
            m_dwRestartCountTime = dwTime;
            wsprintfA(G_cTxt, UPDATE_SCREEN_ONGAME14, m_cRestartCount); // "Restarting game...%d"
            AddEventList(G_cTxt, 10);
        }
    }
    // Restart
    if (m_cRestartCount == 0)
    {
        m_cRestartCount = -1;
        bSendCommand(MSGID_REQUEST_RESTART);
        return;
    }

    //Snoopy: Added diag "LEVEL UP" right down
    if (iUpdateRet && m_iLU_Point > 0)
        PutString_SprFont(GetWidth() - 80, GetHeight() - 90, "Level Up", 250, 250, 0); // 800x600 Resolution xRisenx added x+160 and y+120
                                                                                       //if (m_cGameModeCount < 6) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
    //if (m_cGameModeCount < 6)                                                          //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx
                                                                                       //if (m_cGameModeCount < 2) //DIRECTX m_DDraw.DrawShadowBox(0,0,639,479);
        //if (m_cGameModeCount < 2)                                                      //DIRECTX m_DDraw.DrawShadowBox(0,0,799,599); // 800x600 Resolution xRisenx

            //if( iUpdateRet == 0 ) m_iFrameCount++;
            //else m_iFrameCount+=256;

            if (iUpdateRet != 0 && (dwTime - lastFriendUpdate) < 6000 && (dwTime - lastFriendUpdate) > 4000)
                for (int f = 0; f < 13; f++)
                    if (friendsList[f].updated == false)
                        friendsList[f].online = false;

    perfVar++;
    if (iUpdateRet != 0)
    {
        if (m_adminLevel > 1)
        {
            //PutAlignedString(0, 639, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
            PutAlignedString(0, GetWidth() - 1, 0, "Ethereal", (m_GMFlags & GMFLAG_ETHEREAL) ? Color(50, 220, 50) : Color(220, 50, 50)); // 800x600 Resolution xRisenx

            if (m_adminLevel >= 4)
            {
                //PutAlignedString(0, 639, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                //PutAlignedString(0, 639, 30,  "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ?video::SColor(255,50,220,50) :video::SColor(255,220,50,50));
                PutAlignedString(0, GetWidth() - 1, 15, "No aggro", (m_GMFlags & GMFLAG_NOAGGRO) ? Color(50, 220, 50) : Color(220, 50, 50));      // 800x600 Resolution xRisenx
                PutAlignedString(0, GetWidth() - 1, 30, "Invincible", (m_GMFlags & GMFLAG_INVINCIBLE) ? Color(50, 220, 50) : Color(220, 50, 50)); // 800x600 Resolution xRisenx
            }
        }

        if (m_eventPot != -1)
        {
            wsprintfA(G_cTxt, "Total pot: %d", m_eventPot);
            //PutAlignedString(0, 639, 45,  G_cTxt,video::SColor(255,50,220,50));
            PutAlignedString(0, GetWidth() - 1, 45, G_cTxt, Color(50, 220, 50)); // 800x600 Resolution xRisenx
        }

        if (m_showTime)
        {
            wsprintfA(G_cTxt, "  %d", unixtime() / 1000 % 60);
            PutString(10, 100, G_cTxt, Color(255, 255, 255), false, 1);
        }

        // 		if( m_bShowFPS )
        // 		{
        // 			wsprintfA( G_cTxt, "FPS: %d", m_iFPS);
        // 			PutString( 10, 115, G_cTxt,video::SColor(255,255,255,255), FALSE, 1 );
        // 			wsprintfA( G_cTxt, "PC: %dk", perfCnt/1000 );
        // 			PutString( 10, 130, G_cTxt,video::SColor(255,255,255,255), FALSE, 1 );
        // 		}

#ifdef TitleClient
        // notify of title level up
        if (dwTime - dwTitleLevelUpTime < 5000)
        {
            //PutString_SprFont3(250, 200, "NEW TITLE ACQUIRED!", 200, 250, 2);
            //PutString_SprFont3(320-((strlen(cTitleLevelUpSubject)*7)/2), 215, cTitleLevelUpSubject, 2, 200, 250);
            PutString_SprFont3(300, 280, "NEW TITLE ACQUIRED!", 200, 250, 2);
            PutString_SprFont3(380 - ((strlen(cTitleLevelUpSubject) * 7) / 2), 295, cTitleLevelUpSubject, 2, 200, 250);
        }
#endif

        //if( //DIRECTX m_DDraw.iFlip() == DDERR_SURFACELOST ) RestoreSprites();
    }

    if (dwTime - m_dwFPStime > 1000)
    {
        m_dwFPStime = dwTime;
        m_iFPS = //DIRECTX m_DDraw.GetFrameCnt();
                 //DIRECTX m_DDraw.ResetFrameCnt();
            perfCnt = perfVar;
        perfVar = 0;
    }

    //if( dwTime - m_dwFPStime > 1000 )
    //{
    //	m_dwFPStime = dwTime;
    //	//m_iFPS = //DIRECTX m_DDraw.GetFrameCnt();
    //	m_iFPS = perfVar>>7;
    //	if ( m_iFPS < 10 ) m_iFPS += 6;
    //	//DIRECTX m_DDraw.ResetFrameCnt();
    //	perfCnt = perfVar;
    //	perfVar = 0;
    //}

    //if((dwTime - lastScreenUpdate) > 1)
    {
        iUpdateRet = m_pMapData->iObjectFrameCounter(player_name, m_sViewPointX, m_sViewPointY);
        lastScreenUpdate = dwTime;
    }
    //else iUpdateRet = 0;
    // Magic Icons xRisenx
    if (dwTime - m_dwAuraTime > 1000)
    {

        if (m_sZerk > 0)
        {
            m_sZerk--;
            if (m_sZerk == 0)
                m_bZerk = false;
        }
        if (m_sInv > 0)
        {
            m_sInv--;
            if (m_sInv == 0)
                m_bInv = false;
        }
        if (m_sPfm > 0)
        {
            m_sPfm--;
            if (m_sPfm == 0)
                m_bPfm = false;
        }
        if (m_sPfa > 0)
        {
            m_sPfa--;
            if (m_sPfa == 0)
                m_bPfa = false;
        }
        if (m_sShield > 0)
        {
            m_sShield--;
            if (m_sShield == 0)
                m_bShield = false;
        }
#ifdef MonsterBarClient
        if (m_sNpcBar == 0)
            bNpcBar = false;
        else
            m_sNpcBar--;
#endif
        m_dwAuraTime = dwTime;
    }

    if ((bEffectFrameCounter() == true) && (iUpdateRet == 0))
        iUpdateRet = -1;
    if (iUpdateRet == 2)
    {
        m_bCommandAvailable = true;
        m_dwCommandTime = 0;
    }
    CommandProcessor(game->m_stMCursor.sX, game->m_stMCursor.sY, ((sDivX + sPivotX) * 32 + sModX + game->m_stMCursor.sX - 17) / 32 + 1, ((sDivY + sPivotY) * 32 + sModY + game->m_stMCursor.sY - 17) / 32 + 1, game->m_stMCursor.LB, game->m_stMCursor.RB, game->m_stMCursor.MB);

    m_sViewPointX = sVPXsave;
    m_sViewPointY = sVPYsave;

    if (calcoldviewport)
    {
        if (iUpdateRet > 0)
            CalcViewPointOld();
    }
    else
        CalcViewPoint(dwTime);

    if (m_bIsObserverMode)
    {
        if ((dwTime - m_dwObserverCamTime) > 25)
        {
            m_dwObserverCamTime = dwTime;
            CalcViewPoint(dwTime);
            iUpdateRet = -1;
        }
    }

    if (iUpdateRet > 0)
    {
        if (m_bDrawFlagDir == false)
        {
            m_iDrawFlag++;
            if (m_iDrawFlag >= 25)
            {
                m_iDrawFlag = 25;
                m_bDrawFlagDir = true;
            }
        }
        else
        {
            m_iDrawFlag--;
            if (m_iDrawFlag < 0)
            {
                m_iDrawFlag = 0;
                m_bDrawFlagDir = false;
            }
        }
    }
    // iUpdateRet
}

void helbreath::ShowReceivedString(bool bIsHide)
{
    ZeroMemory(G_cTxt2, sizeof(G_cTxt2));

    strcpy(G_cTxt2, m_pInputBuffer);
    if ((m_cEdit[0] != 0) && (strlen(m_pInputBuffer) + strlen(m_cEdit) + 1 <= m_inputMaxLen))
    {
        strcpy(G_cTxt2 + strlen(m_pInputBuffer), m_cEdit);
    }
    if (bIsHide == true)
    {
        for (unsigned char i = 0; i < strlen(G_cTxt2); i++)
            if (G_cTxt2[i] != 0)
                G_cTxt2[i] = '*';
    }

    if ((G_dwGlobalTime % 400) < 210)
        G_cTxt2[strlen(G_cTxt2)] = '_';

    if (m_iInputX2 == 0)
    {
        PutString(m_iInputX + 1, m_iInputY + 1, G_cTxt2, Color(255, 0, 0, 0));
        PutString(m_iInputX, m_iInputY + 1, G_cTxt2, Color(255, 0, 0, 0));
        PutString(m_iInputX + 1, m_iInputY, G_cTxt2, Color(255, 0, 0, 0));
        PutString(m_iInputX, m_iInputY, G_cTxt2, Color(255, 255, 255, 255));
    }
    else
    {
        PutAlignedString(m_iInputX + 1, m_iInputX2 + 1, m_iInputY + 1, G_cTxt2, Color(255, 0, 0, 0));
        PutAlignedString(m_iInputX, m_iInputX2, m_iInputY + 1, G_cTxt2, Color(255, 0, 0, 0));
        PutAlignedString(m_iInputX + 1, m_iInputX2 + 1, m_iInputY, G_cTxt2, Color(255, 0, 0, 0));
        PutAlignedString(m_iInputX, m_iInputX2, m_iInputY, G_cTxt2, Color(255, 255, 255, 255));
    }
}

void helbreath::ClearInputString()
{
    if (m_pInputBuffer != 0)
        ZeroMemory(m_pInputBuffer, sizeof(m_pInputBuffer));
    ZeroMemory(m_cEdit, sizeof(m_cEdit));
}

void helbreath::StartInputString(int left, int top, uint32_t len, char * pBuffer, bool bIsHide, int right)
{
    m_bInputStatus = true;
    m_iInputX = left;
    m_iInputY = top;
    m_iInputX2 = right;
    m_pInputBuffer = pBuffer;
    ZeroMemory(m_cEdit, sizeof(m_cEdit));
    m_inputMaxLen = len;
}

void helbreath::EndInputString()
{
    m_bInputStatus = false;
    int len = strlen(m_cEdit);
    if (len > 0)
    {
        m_cEdit[len] = 0;
        strcpy(m_pInputBuffer + strlen(m_pInputBuffer), m_cEdit);
        ZeroMemory(m_cEdit, sizeof(m_cEdit));
    }
}

void helbreath::ReceiveString(char * pString)
{
    strcpy(pString, m_pInputBuffer);
}

void helbreath::DrawVersion()
{
    std::string text = fmt::format("Version {}.{}.{}", UPPER_VERSION, LOWER_VERSION, PATCH_VERSION);
    PutFontStringSize("arya", 20, screenheight-20, text, Color(255, 255, 255, 255), 14);
}
