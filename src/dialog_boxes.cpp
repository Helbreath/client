//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "helbreath.h"

void helbreath::SetupDialogBox(int dialog, short X, short Y, short background, int backFrame, int titleTxtFrame, bool trans)
{
    short sizeX = m_pSprite[background]->brush[backFrame].szx;
    short sizeY = m_pSprite[background]->brush[backFrame].szy;
    m_dialogBoxes[dialog].SetupDialog(dialog, X, Y, sizeX, sizeY, background, backFrame, titleTxtFrame, trans);
}

void helbreath::SetupDialogBoxes()
{
    //Character-Info Dialog(F5)
    SetupDialogBox(DIALOG_CHARACTER, 50, 78, SPRID_INTERFACE_ND_TEXT, 0, -1, true);
    //m_dialogBoxes[1].SetupDialog(DIALOG_CHARACTER, 30, 30, 270, 376);
    m_dialogBoxes[1].SetupHandlers(GAMEFUNCT(DrawDialogBox_Character), GAMEFUNCT(DlgBoxClick_Character),
        GAMEFUNCT(DlgBoxDoubleClick_Character), GAMEFUNCT(bItemDrop_Character));

    //Inventory Dialog(F6)
    SetupDialogBox(DIALOG_INVENTORY, 472, 297, SPRID_INTERFACE_ND_INVENTORY, 0, -1, true);
    //m_dialogBoxes[2].SetupDialog(DIALOG_INVENTORY, 380, 210, 225, 185);
    m_dialogBoxes[2].SetupHandlers(GAMEFUNCT(DrawDialogBox_Inventory), GAMEFUNCT(DlgBoxClick_Inventory),
        GAMEFUNCT(DlgBoxDoubleClick_Inventory), GAMEFUNCT(bItemDrop_Inventory));

    //Magic Circle Dialog(F7)
    SetupDialogBox(DIALOG_MAGIC, 429, 113, SPRID_INTERFACE_ND_GAME1, 1, 7, true);
    //m_dialogBoxes[3].SetupDialog(DIALOG_MAGIC, 337, 57, 258, 328);
    m_dialogBoxes[3].SetupHandlers(GAMEFUNCT(DrawDialogBox_Magic), GAMEFUNCT(DlgBoxClick_Magic), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // Item drop confirmation
    SetupDialogBox(DIALOG_ITEMDROP, 0, 0, SPRID_INTERFACE_ND_GAME1, 2);
    //m_dialogBoxes[4].SetupDialog(DIALOG_ITEMDROP, 0, 0, 270, 105);
    m_dialogBoxes[4].SetupHandlers(GAMEFUNCT(DrawDialogBox_ItemDrop), GAMEFUNCT(DlgBoxClick_ItemDrop), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // Age <15 box !?!?!?
    m_dialogBoxes[5].SetupDialog(DIALOG_15AGEMSG, 0, 0, 310, 170);
    m_dialogBoxes[5].SetupHandlers(GAMEFUNCT(DrawDialogBox_15AgeMsg), GAMEFUNCT(DlgBoxClick_15AgeMsg), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // ** This is a battle area **
    SetupDialogBox(DIALOG_WARNINGMSG, 160, 2, SPRID_INTERFACE_ND_GAME4, 2);
    //m_dialogBoxes[6].SetupDialog(DIALOG_WARNINGMSG, 160, 2, 310, 170);
    m_dialogBoxes[6].SetupHandlers(GAMEFUNCT(DrawDialogBox_WarningMsg), GAMEFUNCT(DlgBoxClick_WarningMsg), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Guild Menu Dialog
    SetupDialogBox(DIALOG_GUILDMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2, 19);
    //	m_dialogBoxes[7].SetupDialog(DIALOG_GUILDMENU, 337, 57, 258, 339);
    m_dialogBoxes[7].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildMenu), GAMEFUNCT(DlgBoxClick_GuildMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Guild Operation Dialog
    SetupDialogBox(DIALOG_GUILDOPERATION, 337, 57, SPRID_INTERFACE_ND_GAME2, 0, 19);
    //	m_dialogBoxes[8].SetupDialog(DIALOG_GUILDOPERATION, 337, 57, 295, 346);
    m_dialogBoxes[8].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildOperation), GAMEFUNCT(DlgBoxClick_GuildOp), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Guide Map Dialog
    m_dialogBoxes[9].SetupDialog(DIALOG_GUIDEMAP, 672, 0, 128, 128);
    m_dialogBoxes[9].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuideMap), GAMEFUNCT(DlgBoxClick_GuideMap),
        GAMEFUNCT(DlgBoxDoubleClick_GuideMap), GAMEFUNCT(emptyfunc));

    //Chatting History Dialog(F9)
    SetupDialogBox(DIALOG_CHAT, 211, 374, SPRID_INTERFACE_ND_GAME2, 4, 22, true);
    //	m_dialogBoxes[10].SetupDialog(DIALOG_CHAT, 135, 273, 364, 162);
    m_dialogBoxes[10].SetupHandlers(GAMEFUNCT(DrawDialogBox_Chat),
        GAMEFUNCT(DlgBoxClick_Chat), GAMEFUNCT(DlgBoxDoubleClick_Chat), GAMEFUNCT(emptyfunc));

    //Sale Menu Dialog
    SetupDialogBox(DIALOG_SHOP, 70, 50, SPRID_INTERFACE_ND_GAME2, 2, 11);
    //	m_dialogBoxes[11].SetupDialog(DIALOG_SHOP, 70, 50, 258, 339);
    m_dialogBoxes[11].SetupHandlers(GAMEFUNCT(DrawDialogBox_Shop), GAMEFUNCT(DlgBoxClick_Shop), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Level-Up Setting Dialog
    SetupDialogBox(DIALOG_LEVELUPSETTING, 0, 0, SPRID_INTERFACE_ND_GAME2, 0, 2);
    //	m_dialogBoxes[12].SetupDialog(DIALOG_LEVELUPSETTING, 0, 0, 258, 339);
    m_dialogBoxes[12].SetupHandlers(GAMEFUNCT(DrawDialogBox_LevelUpSetting), GAMEFUNCT(DlgBoxClick_LevelUpSettings), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //City Hall Menu Dialog
    SetupDialogBox(DIALOG_CITYHALLMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2, 18);
    //	m_dialogBoxes[13].SetupDialog(DIALOG_CITYHALLMENU, 337, 57, 258, 339);
    m_dialogBoxes[13].SetupHandlers(GAMEFUNCT(DrawDialogBox_CityHallMenu), GAMEFUNCT(DlgBoxClick_CityhallMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Bank Dialog
    SetupDialogBox(DIALOG_BANK, 60, 50, SPRID_INTERFACE_ND_GAME2, 2, 21);
    //	m_dialogBoxes[14].SetupDialog(DIALOG_BANK , 60, 50, 258, 339);
    m_dialogBoxes[14].SetupHandlers(GAMEFUNCT(DrawDialogBox_Bank), GAMEFUNCT(DlgBoxClick_Bank), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_Bank));
    m_dialogBoxes[14].sV1 = 13;

    //Skill Menu(F8)
    SetupDialogBox(DIALOG_SKILL, 440, 81, SPRID_INTERFACE_ND_GAME2, 0, 1);
    //	m_dialogBoxes[15].SetupDialog(DIALOG_SKILL, 337, 57, 258, 339);
    m_dialogBoxes[15].SetupHandlers(GAMEFUNCT(DrawDialogBox_Skill), GAMEFUNCT(DlgBoxClick_Skill), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Magic Shop Menu
    SetupDialogBox(DIALOG_MAGICSHOP, 30, 30, SPRID_INTERFACE_ND_GAME4, 1, 14);
    //	m_dialogBoxes[16].SetupDialog(DIALOG_MAGICSHOP, 30, 30, 304, 328);
    m_dialogBoxes[16].SetupHandlers(GAMEFUNCT(DrawDialogBox_MagicShop), GAMEFUNCT(DlgBoxClick_MagicShop), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Dialog items drop external screen
    SetupDialogBox(DIALOG_QUERYDROPITEMAMOUNT, 0, 0, SPRID_INTERFACE_ND_GAME2, 5);
    //	m_dialogBoxes[17].SetupDialog(DIALOG_QUERYDROPITEMAMOUNT, 0, 0, 215, 87);
    m_dialogBoxes[17].SetupHandlers(GAMEFUNCT(DrawDialogBox_QueryDropItemAmount), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Text Dialog
    SetupDialogBox(DIALOG_TEXT, 20, 65, SPRID_INTERFACE_ND_GAME2, 0);
    m_dialogBoxes[18].SetupHandlers(GAMEFUNCT(DrawDialogBox_Text), GAMEFUNCT(DlgBoxClick_Text), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //System Menu Dialog(F12)
    SetupDialogBox(DIALOG_SYSMENU, 452, 131, SPRID_INTERFACE_ND_GAME1, 0, 6);
    //	m_dialogBoxes[19].SetupDialog(DIALOG_SYSMENU, 337, 107, 258, 268);
    m_dialogBoxes[19].SetupHandlers(GAMEFUNCT(DrawDialogBox_SysMenu), GAMEFUNCT(DlgBoxClick_SysMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //NpcActionQuery Dialog
    SetupDialogBox(DIALOG_NPCACTIONQUERY, 237, 57, SPRID_INTERFACE_ND_GAME2, 5);
    //	m_dialogBoxes[20].SetupDialog(DIALOG_NPCACTIONQUERY, 237, 57, 252, 87);
    m_dialogBoxes[20].SetupHandlers(GAMEFUNCT(DrawDialogBox_NpcActionQuery), GAMEFUNCT(DlgBoxClick_NpcActionQuery), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //NpcTalk Dialog
    SetupDialogBox(DIALOG_NPCTALK, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[21].SetupDialog(DIALOG_NPCTALK, 337, 57, 258, 339);
    m_dialogBoxes[21].SetupHandlers(GAMEFUNCT(DrawDialogBox_NpcTalk), GAMEFUNCT(DlgBoxClick_NpcTalk), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Map
    m_dialogBoxes[22].SetupDialog(DIALOG_MAP, 336 + 120, 88, 270, 346); // Map Fixed xRisenx?
    m_dialogBoxes[22].SetupHandlers(GAMEFUNCT(DrawDialogBox_Map), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //ItemSellorRepair Dialog
    SetupDialogBox(DIALOG_SELLORREPAIRITEM, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[23].SetupDialog(DIALOG_SELLORREPAIRITEM, 337, 57, 258, 339);
    m_dialogBoxes[23].SetupHandlers(GAMEFUNCT(DrawDialogBox_SellorRepairItem), GAMEFUNCT(DlgBoxClick_ItemSellorRepair), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Fishing Dialog
    SetupDialogBox(DIALOG_FISHING, 193, 241, SPRID_INTERFACE_ND_GAME1, 2);
    //	m_dialogBoxes[24].SetupDialog(DIALOG_FISHING, 193, 241, 263, 100);
    m_dialogBoxes[24].SetupHandlers(GAMEFUNCT(DrawDialogBox_Fishing), GAMEFUNCT(DlgBoxClick_Fish), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //ShutDownMsg Dialog
    SetupDialogBox(DIALOG_SHUTDOWNMSG, 162, 40, SPRID_INTERFACE_ND_GAME4, 2);
    //	m_dialogBoxes[25].SetupDialog(DIALOG_SHUTDOWNMSG, 162, 40, 315, 171);
    m_dialogBoxes[25].SetupHandlers(GAMEFUNCT(DrawDialogBox_ShutDownMsg), GAMEFUNCT(DlgBoxClick_ShutDownMsg), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //SkillDlg Dialog
    m_dialogBoxes[26].SetupDialog(DIALOG_SKILLDLG, 100, 60, 258, 339);
    m_dialogBoxes[26].SetupHandlers(GAMEFUNCT(DrawDialogBox_SkillDlg), GAMEFUNCT(DlgBoxClick_SkillDlg), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_SkillDialog));

    //Exchange Dialog
    SetupDialogBox(DIALOG_EXCHANGE, 100, 30, SPRID_INTERFACE_ND_NEWEXCHANGE, 0);
    //	m_dialogBoxes[27].SetupDialog(DIALOG_EXCHANGE, 100, 30, 520, 357);
    m_dialogBoxes[27].SetupHandlers(GAMEFUNCT(DrawDialogBox_Exchange), GAMEFUNCT(DlgBoxClick_Exchange), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_ExchangeDialog));

    //Quest Dialog
    SetupDialogBox(DIALOG_QUEST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2, 4);
    //	m_dialogBoxes[28].SetupDialog(DIALOG_QUEST, 0, 0, 258, 339);
    m_dialogBoxes[28].SetupHandlers(GAMEFUNCT(DrawDialogBox_Quest), GAMEFUNCT(DlgBoxClick_Quest), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Gauge Pannel
    //m_dialogBoxes[29].SetupDialog(DIALOG_GAUGEPANEL, 0, 434, 157, 53);
    m_dialogBoxes[29].SetupDialog(DIALOG_GAUGEPANEL, 0, 597, 157, 53);
    m_dialogBoxes[29].SetupHandlers(GAMEFUNCT(DrawDialogBox_GaugePanel), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Icon Pannel
    //m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, 427, 640, 53);
    m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, 547, GetWidth(), 53); // 800x600 Resolution xRisenx
    m_dialogBoxes[30].SetupHandlers(GAMEFUNCT(DrawDialogBox_IconPanel), GAMEFUNCT(DlgBoxClick_IconPanel), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_IconPanel));

    //Sell List Dialog
    SetupDialogBox(DIALOG_SELLLIST, 170, 70, SPRID_INTERFACE_ND_GAME2, 2, 11);
    //	m_dialogBoxes[31].SetupDialog(DIALOG_SELLLIST, 170, 70, 258, 339);
    m_dialogBoxes[31].SetupHandlers(GAMEFUNCT(DrawDialogBox_SellList), GAMEFUNCT(DlgBoxClick_SellList), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_SellList));

    //Party Dialog
    SetupDialogBox(DIALOG_PARTY, 0, 0, SPRID_INTERFACE_ND_GAME2, 0, 3);
    //	m_dialogBoxes[32].SetupDialog(DIALOG_PARTY, 0, 0, 258, 339);
    m_dialogBoxes[32].SetupHandlers(GAMEFUNCT(DrawDialogBox_Party), GAMEFUNCT(DlgBoxClick_Party), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Crusade Job Dialog
    SetupDialogBox(DIALOG_CRUSADEJOB, 360, 65, SPRID_INTERFACE_ND_GAME2, 0);
    //	m_dialogBoxes[33].SetupDialog(DIALOG_CRUSADEJOB, 360, 65, 258, 339);
    m_dialogBoxes[33].SetupHandlers(GAMEFUNCT(DrawDialogBox_CrusadeJob), GAMEFUNCT(DlgBoxClick_CrusadeJob), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Item Upgrade Dialog
    SetupDialogBox(DIALOG_ITEMUPGRADE, 60, 50, SPRID_INTERFACE_ND_GAME2, 0, 5);
    //	m_dialogBoxes[34].SetupDialog(DIALOG_ITEMUPGRADE, 60, 50, 258, 339);
    m_dialogBoxes[34].SetupHandlers(GAMEFUNCT(DrawDialogBox_ItemUpgrade), GAMEFUNCT(DlgBoxClick_ItemUpgrade), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_ItemUpgrade));

    //Help Menu Dialog(F1)
    SetupDialogBox(DIALOG_HELP, 358, 65, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[35].SetupDialog(DIALOG_HELP, 358, 65, 258, 339);
    m_dialogBoxes[35].SetupHandlers(GAMEFUNCT(DrawDialogBox_Help), GAMEFUNCT(DlgBoxClick_Help), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Crusade Commander Dialog
    //SetupDialogBox(DIALOG_COMMANDER, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  15, true);
    m_dialogBoxes[36].SetupDialog(DIALOG_COMMANDER, 20, 20, 310, 400);
    m_dialogBoxes[36].SetupHandlers(GAMEFUNCT(DrawDialogBox_Commander), GAMEFUNCT(DlgBoxClick_Commander), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Crusade Constructor Dialog
    //SetupDialogBox(DIALOG_CONSTRUCTOR, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  16, true);
    m_dialogBoxes[37].SetupDialog(DIALOG_CONSTRUCTOR, 20, 20, 310, 400);
    m_dialogBoxes[37].SetupHandlers(GAMEFUNCT(DrawDialogBox_Constructor), GAMEFUNCT(DlgBoxClick_Constructor), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Crusade Soldier Dialog
    //SetupDialogBox(DIALOG_SOLDIER, 20, 20, SPRID_INTERFACE_ND_CRUSADE, 0,  17, true);
    m_dialogBoxes[38].SetupDialog(DIALOG_SOLDIER, 20, 20, 310, 400);
    m_dialogBoxes[38].SetupHandlers(GAMEFUNCT(DrawDialogBox_Soldier), GAMEFUNCT(DlgBoxClick_Soldier), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // Give item ???
    m_dialogBoxes[39].SetupDialog(39, 0, 0, 291, 413);

    // 3.51 Slates Dialog - Diuuude
    m_dialogBoxes[40].SetupDialog(DIALOG_SLATES, 100, 60, 258, 339);
    m_dialogBoxes[40].SetupHandlers(GAMEFUNCT(DrawDialogBox_Slates), GAMEFUNCT(DlgBoxClick_Slates), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_Slates));

    // Snoopy: Item exchange confirmation
    SetupDialogBox(DIALOG_CONFIRMEXCHANGE, 285, 200, SPRID_INTERFACE_ND_GAME1, 2);
    //	m_dialogBoxes[41].SetupDialog(DIALOG_CONFIRMEXCHANGE, 285, 200, 270, 105);
    m_dialogBoxes[41].SetupHandlers(GAMEFUNCT(DrawDialogBox_ConfirmExchange), GAMEFUNCT(DlgBoxClick_ConfirmExchange), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // MJ Stats Change DialogBox - Diuuude
    SetupDialogBox(DIALOG_CHANGESTATSMAJESTIC, 0, 0, SPRID_INTERFACE_ND_GAME2, 0, 2);
    //	m_dialogBoxes[42].SetupDialog(DIALOG_CHANGESTATSMAJESTIC, 0, 0, 258, 339);
    m_dialogBoxes[42].SetupHandlers(GAMEFUNCT(DrawDialogBox_ChangeStatsMajestic), GAMEFUNCT(DlgBoxClick_ChangeStatsMajestic), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // friends
    SetupDialogBox(DIALOG_FRIENDLIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[43].SetupDialog(DIALOG_FRIENDLIST, 0, 0, 258, 339);
    m_dialogBoxes[43].SetupHandlers(GAMEFUNCT(DrawDialogBox_FriendList), GAMEFUNCT(DlgBoxClick_FriendList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //guildquery (assign rank)
    SetupDialogBox(DIALOG_GUILDQUERY, 237, 57, SPRID_INTERFACE_ND_GAME2, 6);
    //	m_dialogBoxes[44].SetupDialog(DIALOG_GUILDQUERY, 237, 57, 252, 87);
    m_dialogBoxes[44].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildQuery), GAMEFUNCT(DlgBoxClick_GuildQuery), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // guild teleport request
    SetupDialogBox(DIALOG_GUILDSUMMONS, 185, 20, SPRID_INTERFACE_ND_GAME1, 2);
    //	m_dialogBoxes[49].SetupDialog(DIALOG_GUILDSUMMONS, 185, 20, 270, 105);
    m_dialogBoxes[49].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildSummons), GAMEFUNCT(DlgBoxClick_GuildSummons), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // Resurection
    SetupDialogBox(DIALOG_RESURECT, 185, 20, SPRID_INTERFACE_ND_GAME1, 2);
    //	m_dialogBoxes[50].SetupDialog(DIALOG_RESURECT, 185, 20, 270, 105);
    m_dialogBoxes[50].SetupHandlers(GAMEFUNCT(DrawDialogBox_Resurect), GAMEFUNCT(DlgBoxClick_Resurect), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Guild Hall Menu Dialog
    SetupDialogBox(DIALOG_CMDHALLMENU, 337, 57, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[51].SetupDialog(DIALOG_CMDHALLMENU, 337, 57, 258, 339);
    m_dialogBoxes[51].SetupHandlers(GAMEFUNCT(DrawDialogBox_CMDHallMenu), GAMEFUNCT(DlgBoxClick_CMDHallMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // DK Weapons Dialog
    SetupDialogBox(DIALOG_DKMENUWEAPONS, 358, 65, SPRID_INTERFACE_ND_GAME2, 2);
    //	m_dialogBoxes[53].SetupDialog(DIALOG_DKMENUWEAPONS, 358, 65, 258, 339);
    m_dialogBoxes[53].SetupHandlers(GAMEFUNCT(DrawDialogBox_DKMenuWeapons), GAMEFUNCT(DlgBoxClick_DKMenuWeapons), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Quest Helper
    m_dialogBoxes[55].SetupDialog(55, 530, 130, 110, 36);

    //GuildBank Dialog
    SetupDialogBox(DIALOG_GUILDBANK, 60, 50, SPRID_INTERFACE_ND_GAME2, 2, 21);
    //	m_dialogBoxes[56].SetupDialog(DIALOG_GUILDBANK, 60, 50, 258, 339);
    m_dialogBoxes[56].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildBank), GAMEFUNCT(DlgBoxClick_GuildBank), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_GuildBank));
    m_dialogBoxes[56].sV1 = 13;

    //Guild Dialog
    SetupDialogBox(DIALOG_GUILD, 30, 30, SPRID_INTERFACE_ND_GAME2, 2, 19);
    //	m_dialogBoxes[57].SetupDialog(DIALOG_GUILD, 30, 30, 258, 339);
    m_dialogBoxes[57].SetupHandlers(GAMEFUNCT(DrawDialogBox_Guild), GAMEFUNCT(DlgBoxClick_Guild), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    // Contribute to guild
    SetupDialogBox(DIALOG_GUILDCONTRIBUTE, 0, 0, SPRID_INTERFACE_ND_GAME2, 5);
    //	m_dialogBoxes[58].SetupDialog(DIALOG_GUILDCONTRIBUTE, 0, 0, 215, 87);
    m_dialogBoxes[58].SetupHandlers(GAMEFUNCT(DrawDialogBox_GuildContribute), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    SetupDialogBox(DIALOG_EXTENDEDSYSMENU, 300, 220, SPRID_INTERFACE_ND_GAME4, 3);
    m_dialogBoxes[59].SetupHandlers(GAMEFUNCT(DrawDialogBox_ExtendedSysMenu), GAMEFUNCT(DlgBoxClick_ExtendedSysMenu), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //SetupDialogBox(DIALOG_FRIENDLIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
//	m_dialogBoxes[43].SetupDialog(DIALOG_FRIENDLIST, 0, 0, 258, 339);
    //m_dialogBoxes[60].SetupHandlers(GAMEFUNCT(DrawDialogBox_FriendList), GAMEFUNCT(DlgBoxClick_FriendList));

    // Titles xRisenx
    //SetupDialogBox(DIALOG_TITLE, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
    //m_dialogBoxes[70].SetupHandlers(GAMEFUNCT(DrawDialogBox_Titles), GAMEFUNCT(DlgBoxClick_Titles));
    // Titles xRisenx

    SetupDialogBox(DIALOG_MUTELIST, 0, 0, SPRID_INTERFACE_ND_GAME2, 2);
    m_dialogBoxes[DIALOG_MUTELIST].SetupHandlers(GAMEFUNCT(DrawDialogBox_MuteList), GAMEFUNCT(DlgBoxClick_MuteList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //Quest Menu Dialog
    SetupDialogBox(DIALOG_QUESTS, 337, 57, SPRID_INTERFACE_ND_GAME2, 0, 4);
    m_dialogBoxes[DIALOG_QUESTS].SetupHandlers(GAMEFUNCT(DrawDialogBox_QuestList), GAMEFUNCT(DlgBoxClick_QuestList), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));
#ifdef TitleClient
    // Title
    SetupDialogBox(DIALOG_TITLE, 74, 133, SPRID_INTERFACE_ND_GAME2, 2);
    m_dialogBoxes[63].SetupHandlers(GAMEFUNCT(DrawDialogBox_Titles), GAMEFUNCT(DlgBoxClick_Titles), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));
#endif
    SetupDialogBox(DIALOG_MAILBOX, 30, 65, SPRID_INTERFACE_ND_GAME2, 2);
    m_dialogBoxes[DIALOG_MAILBOX].SetupHandlers(GAMEFUNCT(DrawDialogBox_Mailbox), GAMEFUNCT(DlgBoxClick_Mailbox),
        GAMEFUNCT(emptyfunc), GAMEFUNCT(ItemDrop_Mailbox));

    SetupDialogBox(DIALOG_YESNO, 184, 188, SPRID_INTERFACE_ND_GAME1, 2);
    m_dialogBoxes[DIALOG_YESNO].
        SetupHandlers(GAMEFUNCT(DrawDialogBox_YesNo), GAMEFUNCT(DlgBoxClick_YesNo), GAMEFUNCT(emptyfunc), GAMEFUNCT(emptyfunc));

    //New Top Bar xRisenx
    //m_dialogBoxes[30].SetupDialog(DIALOG_ICONPANEL, 0, 427, 640, 53);
    m_dialogBoxes[62].SetupDialog(DIALOG_TOPPANEL, 2, 2, 165, 48); // 800x600 Resolution xRisenx
    m_dialogBoxes[62].SetupHandlers(GAMEFUNCT(DrawDialogBox_TopPanel), GAMEFUNCT(DlgBoxClick_TopPanel), GAMEFUNCT(emptyfunc),
        GAMEFUNCT(bItemDrop_TopPanel));

    //Quest Menu Dialog

    for (int i = 0; i < MAXDIALOGBOX; i++)
    {
        m_dialogBoxes[i].SetMode(0); //Setup default buttons
    }
}


void helbreath::EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, const char * pString)
{
	int i;
	short sX, sY;

	switch (iBoxID)
	{
		case 11:
			if (m_bIsDialogEnabled[11] == false)
			{
				switch (cType)
				{
					case 0:
						break;
					default:
						_LoadShopMenuContents(cType);
						m_dialogBoxes[11].sV1 = cType;
						m_dialogBoxes[11].SetMode(0);
						m_dialogBoxes[11].sView = 0;
						m_dialogBoxes[11].bFlag = true;
						m_dialogBoxes[11].sV3 = 1;
						break;
				}
			}
			break;

		case 12: // levelup diag
			if (m_bIsDialogEnabled[12] == false)
			{
				m_dialogBoxes[12].m_X = m_dialogBoxes[1].m_X + 20;
				m_dialogBoxes[12].m_Y = m_dialogBoxes[1].m_Y + 20;
				//m_iLU_Point = m_iLevel*3 - (
				m_iLU_Point = m_iLevel * 7 - (
					(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
					- 70)
					//- 3 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
					- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
				m_dialogBoxes[12].sV1 = m_iLU_Point;
			}
			break;

		case 3: // Magic Dialog
			break;

		case 4:
			if (m_bIsDialogEnabled[4] == false)
			{
				m_dialogBoxes[4].sView = cType;
			}
			break;

		case 5:
			if (m_bIsDialogEnabled[5] == false)
			{
				m_dialogBoxes[5].sView = cType;
			}
			break;

		case 6:
			if (m_bIsDialogEnabled[6] == false)
			{
				m_dialogBoxes[6].sView = cType;
			}
			break;

		case 7:
			if (m_dialogBoxes[7].GetMode() == 1)
			{
				sX = m_dialogBoxes[7].m_X;
				sY = m_dialogBoxes[7].m_Y;
				EndInputString();
				StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
			}
			break;

		case 17: // demande quantité
			if (m_bIsDialogEnabled[17] == false)
			{
				m_dialogBoxes[iBoxID].SetMode(1);
				m_dialogBoxes[17].sView = cType;
				EndInputString();
				ZeroMemory(m_cAmountString, sizeof(m_cAmountString));
				wsprintfA(m_cAmountString, "%d", sV1);
				sX = m_dialogBoxes[17].m_X;
				sY = m_dialogBoxes[17].m_Y;
				StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
			}
			else
			{
				if (m_dialogBoxes[17].GetMode() == 1)
				{
					sX = m_dialogBoxes[17].m_X;
					sY = m_dialogBoxes[17].m_Y;
					EndInputString();
					StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
				}
			}
			break;

		case 18:
			if (m_bIsDialogEnabled[18] == false)
			{
				switch (cType)
				{
					case 0:
						m_dialogBoxes[18].SetMode(0);
						m_dialogBoxes[18].sView = 0;
						break;
					default:
						_LoadTextDlgContents(cType);
						m_dialogBoxes[18].SetMode(0);
						m_dialogBoxes[18].sView = 0;
						break;
				}
			}
			break;

		case 19:
			break;

		case 120:
			break;

		case 20: // Talk to npc or unicorn
			m_bIsItemDisabled[m_dialogBoxes[20].sV1] = false;
			if (m_bIsDialogEnabled[20] == false)
			{
				m_dialogBoxes[11].sV1 = m_dialogBoxes[11].sV2 = m_dialogBoxes[11].sV3 =
					m_dialogBoxes[11].sV4 = m_dialogBoxes[11].sV5 = m_dialogBoxes[11].sV6 = 0;
				m_dialogBoxes[20].SetMode(cType);
				m_dialogBoxes[20].sView = 0;
				m_dialogBoxes[20].sV1 = sV1;
				m_dialogBoxes[20].sV2 = sV2;
			}
			break;

		case 21:
			if (m_bIsDialogEnabled[21] == false)
			{
				m_dialogBoxes[21].SetMode(cType);
				m_dialogBoxes[21].sView = 0;
				m_dialogBoxes[21].sV1 = _iLoadTextDlgContents2(sV1 + 20);
				m_dialogBoxes[21].sV2 = sV1 + 20;
			}
			break;

		case 22:
			if (m_bIsDialogEnabled[22] == false)
			{
				m_dialogBoxes[22].sV1 = sV1;
				m_dialogBoxes[22].sV2 = sV2;

				m_dialogBoxes[22].sSizeX = 290;
				m_dialogBoxes[22].sSizeY = 290;
			}
			break;

		case 23:
			if (m_bIsDialogEnabled[23] == false)
			{
				m_dialogBoxes[23].SetMode(cType);
				m_dialogBoxes[23].sV1 = sV1;		// ItemID
				m_dialogBoxes[23].sV2 = sV2;
				if (cType == 2)
				{
					m_dialogBoxes[23].m_X = m_dialogBoxes[11].m_X;
					m_dialogBoxes[23].m_Y = m_dialogBoxes[11].m_Y;
				}
			}
			break;

		case 15:
			break;

		case 24:
			if (m_bIsDialogEnabled[24] == false)
			{
				m_dialogBoxes[24].SetMode(cType);
				m_dialogBoxes[24].sV1 = sV1;
				m_dialogBoxes[24].sV2 = sV2;
				m_bSkillUsingStatus = true;
			}
			break;

		case 25:
			if (m_bIsDialogEnabled[25] == false)
			{
				m_dialogBoxes[25].SetMode(cType);
				m_dialogBoxes[25].sV1 = sV1;
				m_dialogBoxes[25].sV2 = sV2;
			}
			break;

		case 26:
			switch (cType)
			{
				case 1:
				case 2: //
					if (m_bIsDialogEnabled[26] == false)
					{
						m_dialogBoxes[26].sV1 = -1;
						m_dialogBoxes[26].sV2 = -1;
						m_dialogBoxes[26].sV3 = -1;
						m_dialogBoxes[26].sV4 = -1;
						m_dialogBoxes[26].sV5 = -1;
						m_dialogBoxes[26].sV6 = -1;
						m_dialogBoxes[26].cStr[0] = 0;
						m_bSkillUsingStatus = true;
						m_dialogBoxes[26].sSizeX = 195;
						m_dialogBoxes[26].sSizeY = 215;
						m_dialogBoxes[26].SetMode(cType);
						DisableDialogBox(17);
						DisableDialogBox(20);
						DisableDialogBox(23);
					}
					break;

				case 3:	//
					if (m_bIsDialogEnabled[26] == false)
					{
						m_dialogBoxes[26].sView = 0;
						m_dialogBoxes[26].sV1 = -1;
						m_dialogBoxes[26].sV2 = -1;
						m_dialogBoxes[26].sV3 = -1;
						m_dialogBoxes[26].sV4 = -1;
						m_dialogBoxes[26].sV5 = -1;
						m_dialogBoxes[26].sV6 = -1;
						m_dialogBoxes[26].cStr[0] = 0;
						m_dialogBoxes[26].cStr[1] = 0;
						m_dialogBoxes[26].cStr[4] = 0;
						m_bSkillUsingStatus = true;
						_bCheckBuildItemStatus();
						//m_stDialogBoxInfo[26].sX = 0;
						//m_stDialogBoxInfo[26].sY = 0;
						m_dialogBoxes[26].sSizeX = 270;
						m_dialogBoxes[26].sSizeY = 381;
						m_dialogBoxes[26].SetMode(cType);
						DisableDialogBox(17);
						DisableDialogBox(20);
						DisableDialogBox(23);
					}
					break;

				case 6:
					if (m_bIsDialogEnabled[26] == false)
					{
						m_dialogBoxes[26].cStr[2] = sV1;
						m_dialogBoxes[26].cStr[3] = sV2;
						m_dialogBoxes[26].sSizeX = 270;
						m_dialogBoxes[26].sSizeY = 381;
						m_dialogBoxes[26].SetMode(cType);
						m_bSkillUsingStatus = true;
						_bCheckBuildItemStatus();
						DisableDialogBox(17);
						DisableDialogBox(20);
						DisableDialogBox(23);
					}
					break;
					// Crafting
				case 7:
				case 8:
					if (m_bIsDialogEnabled[26] == false)
					{
						m_dialogBoxes[26].sV1 = -1;
						m_dialogBoxes[26].sV2 = -1;
						m_dialogBoxes[26].sV3 = -1;
						m_dialogBoxes[26].sV4 = -1;
						m_dialogBoxes[26].sV5 = -1;
						m_dialogBoxes[26].sV6 = -1;
						m_dialogBoxes[26].cStr[0] = 0;
						m_dialogBoxes[26].cStr[1] = 0;
						m_bSkillUsingStatus = true;
						//_bCheckCraftItemStatus();
						m_dialogBoxes[26].sSizeX = 195;
						m_dialogBoxes[26].sSizeY = 215;
						m_dialogBoxes[26].SetMode(cType);
						DisableDialogBox(17);
						DisableDialogBox(20);
						DisableDialogBox(23);
					}
					break;
			}
			break;

		case 27: // Snoopy: 7 mar 06 (multitrade) case rewriten
			if (m_bIsDialogEnabled[27] == false)
			{
				m_dialogBoxes[27].SetMode(cType);
				for (i = 0; i < 8; i++)
				{
					ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
					ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
					m_stDialogBoxExchangeInfo[i].sV1 = -1;
					m_stDialogBoxExchangeInfo[i].sV2 = -1;
					m_stDialogBoxExchangeInfo[i].sV3 = -1;
					m_stDialogBoxExchangeInfo[i].sV4 = -1;
					m_stDialogBoxExchangeInfo[i].sV5 = -1;
					m_stDialogBoxExchangeInfo[i].sV6 = -1;
					m_stDialogBoxExchangeInfo[i].sV7 = -1;
					m_stDialogBoxExchangeInfo[i].dwV1 = 0;
				}
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
				DisableDialogBox(26);
			}
			break;

		case 41: // Snoopy: 7 mar 06 (MultiTrade) Confirmation dialog
			break;

		case 28:
			if (m_bIsDialogEnabled[28] == false)
			{
				m_dialogBoxes[28].m_X = m_dialogBoxes[1].m_X + 20;
				m_dialogBoxes[28].m_Y = m_dialogBoxes[1].m_Y + 20;
				m_dialogBoxes[28].SetMode(cType);
			}
			break;

		case 32:
			if (m_bIsDialogEnabled[32] == false)
			{
				m_dialogBoxes[32].m_X = m_dialogBoxes[1].m_X + 20;
				m_dialogBoxes[32].m_Y = m_dialogBoxes[1].m_Y + 20;
				m_dialogBoxes[32].SetMode(cType);
			}
			break;

		case 33:
			if ((m_iHP <= 0) || m_side == NEUTRAL) return;
			if (m_bIsDialogEnabled[33] == false)
			{
				m_dialogBoxes[33].m_X = 360;
				m_dialogBoxes[33].m_Y = 65;
				m_dialogBoxes[33].sV1 = sV1;
				m_dialogBoxes[33].SetMode(cType);
			}
			break;

		case 34:
			if (m_bIsDialogEnabled[iBoxID] == false)
			{
				m_dialogBoxes[iBoxID].sV1 = -1;
				m_dialogBoxes[iBoxID].dwV1 = 0;
				m_dialogBoxes[iBoxID].SetMode(5);
			}
			break;

		case 16:
			if (m_bIsDialogEnabled[iBoxID] == false)
			{
				if (m_cSkillMastery[SKILL_MAGIC] == 0)
				{
					DisableDialogBox(16);
					EnableDialogBox(21, 0, 480, 0);
					return;
				}
				else
				{
					m_dialogBoxes[iBoxID].SetMode(0);
					m_dialogBoxes[iBoxID].sView = 0;
				}
			}
			break;

		case 14:
			EndInputString();
			if (m_bIsDialogEnabled[iBoxID] == false)
			{
				m_dialogBoxes[iBoxID].SetMode(0);
				m_dialogBoxes[iBoxID].sView = 0;
				EnableDialogBox(2);
				DisableDialogBox(56);
			}
			break;

		case 40: // Slates
			if (m_bIsDialogEnabled[40] == false)
			{
				m_dialogBoxes[40].sView = 0;
				m_dialogBoxes[40].sV1 = -1;
				m_dialogBoxes[40].sV2 = -1;
				m_dialogBoxes[40].sV3 = -1;
				m_dialogBoxes[40].sV4 = -1;
				m_dialogBoxes[40].sV5 = -1;
				m_dialogBoxes[40].sV6 = -1;
				m_dialogBoxes[40].cStr[0] = 0;
				m_dialogBoxes[40].cStr[1] = 0;
				m_dialogBoxes[40].cStr[4] = 0;

				m_dialogBoxes[40].sSizeX = 180;
				m_dialogBoxes[40].sSizeY = 183;
				m_dialogBoxes[40].SetMode(cType);
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
				DisableDialogBox(26);
			}
			break;

		case 42: // Diuuude: Change stats window
			if (m_bIsDialogEnabled[42] == false)
			{
				m_dialogBoxes[42].m_X = m_dialogBoxes[12].m_X + 10;
				m_dialogBoxes[42].m_Y = m_dialogBoxes[12].m_Y + 10;
				m_dialogBoxes[42].SetMode(0);
				m_dialogBoxes[42].sView = 0;
				m_bSkillUsingStatus = false;
			}
			break;

		case 43:
			if (m_bIsDialogEnabled[43] == false)
			{
				m_iFriendIndex = -1;
				m_dialogBoxes[43].sV1 = 0;
			}
			break;

		case 44: // guildquery
			if (m_bIsDialogEnabled[44])
				break;
			m_dialogBoxes[44].SetMode(cType);
			m_dialogBoxes[44].sV1 = sV1; // current rank
			if (pString)
				strcpyn(m_dialogBoxes[44].cStr, pString);
			break;

		case 49: // guild teleport request
			if (!m_bIsDialogEnabled[49])
			{
				m_dialogBoxes[49].m_X = 185;
				m_dialogBoxes[49].m_Y = 20;
				m_dialogBoxes[49].SetMode(0);
				m_dialogBoxes[49].sView = 0;
				GetOfficialMapName(GetMapName(sV1), m_dialogBoxes[49].cStr);
			}
			break;

		case 50: // Resurection
			if (m_bIsDialogEnabled[50] == false)
			{
				m_dialogBoxes[50].m_X = 185;
				m_dialogBoxes[50].m_Y = 20;
				m_dialogBoxes[50].SetMode(0);
				m_dialogBoxes[50].sView = 0;
				m_bSkillUsingStatus = false;
			}
			break;

		case 52: //New Shop
			break;
		case 53: //new bsmith
			break;

			//GuildBank
		case 56:
			EndInputString();

			if (m_bIsDialogEnabled[iBoxID] == false)
			{
				m_dialogBoxes[iBoxID].SetMode(0);
				m_dialogBoxes[iBoxID].sView = 0;
				if (m_guildBankVer == 0)
					m_guildBankIt = m_guildBankMap.end();
				else
					m_guildBankIt = m_guildBankMap.begin();
				EnableDialogBox(2);
				bSendCommand(MSGID_REQUEST_GUILDBANK);
				DisableDialogBox(14);
			}
			break;

		case 58:
			if (!m_bIsDialogEnabled[iBoxID])
			{
				m_dialogBoxes[iBoxID].SetMode(1);
				m_dialogBoxes[iBoxID].sView = cType;

				EndInputString();
				sX = m_dialogBoxes[iBoxID].m_X;
				sY = m_dialogBoxes[iBoxID].m_Y;
				ZeroMemory(m_cAmountString, sizeof(m_cAmountString));
				StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
			}
			else
			{
				// moving dialog
				if (m_dialogBoxes[iBoxID].GetMode() == 1)
				{
					sX = m_dialogBoxes[iBoxID].m_X;
					sY = m_dialogBoxes[iBoxID].m_Y;
					EndInputString();
					StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
				}
			}
			break;

		case DIALOG_MAILBOX:
			EndInputString();

			if (!m_bIsDialogEnabled[iBoxID])
			{
				m_dialogBoxes[iBoxID].SetMode(0);
				m_dialogBoxes[iBoxID].sView = 0;
				bSendCommand(MSGID_REQ_MAILBOX);
			}
			break;

		case DIALOG_YESNO:
			if (!m_bIsDialogEnabled[iBoxID])
				m_dialogBoxes[iBoxID].SetMode(cType);
			break;

		default:
			EndInputString();
			if (m_bIsDialogEnabled[iBoxID] == false)
			{
				m_dialogBoxes[iBoxID].SetMode(0);
				m_dialogBoxes[iBoxID].sView = 0;
			}
			break;
	}
	if (iBoxID != 30)
	{
		if (m_bIsDialogEnabled[iBoxID] == false)
			//{	if( m_dialogBoxes[iBoxID].m_Y > 400 ) m_dialogBoxes[iBoxID].m_Y = 410;
			//	if( m_dialogBoxes[iBoxID].m_X > 620 ) m_dialogBoxes[iBoxID].m_X = 620;
		{
			if (m_dialogBoxes[iBoxID].m_Y > 520) m_dialogBoxes[iBoxID].m_Y = 530;
			if (m_dialogBoxes[iBoxID].m_X > 780) m_dialogBoxes[iBoxID].m_X = 780;
			if ((m_dialogBoxes[iBoxID].m_X + m_dialogBoxes[iBoxID].sSizeX) < 10) m_dialogBoxes[iBoxID].m_X += 20;
			if ((m_dialogBoxes[iBoxID].m_Y + m_dialogBoxes[iBoxID].sSizeY) < 10) m_dialogBoxes[iBoxID].m_Y += 20;
		}
	}
	m_bIsDialogEnabled[iBoxID] = true;
	if (pString != 0) strcpy(m_dialogBoxes[iBoxID].cStr, pString);

	for (i = 0; i < MAXDIALOGBOX - 2; i++)
		if (m_cDialogBoxOrder[i] == iBoxID) m_cDialogBoxOrder[i] = 0;

	for (i = 1; i < MAXDIALOGBOX - 2; i++)
		if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
		{
			m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
			m_cDialogBoxOrder[i] = 0;
		}

	for (i = 0; i < MAXDIALOGBOX - 2; i++)
		if (m_cDialogBoxOrder[i] == 0)
		{
			m_cDialogBoxOrder[i] = iBoxID;
			return;
		}
}

void helbreath::DisableDialogBox(int iBoxID)
{
	int i;

	switch (iBoxID)
	{
		case 4:
			m_bIsItemDisabled[m_dialogBoxes[4].sView] = false;
			break;

		case 5:
			m_bIsItemDisabled[m_dialogBoxes[5].sView] = false;
			break;

		case 6:
			m_bIsItemDisabled[m_dialogBoxes[6].sView] = false;
			break;

		case 7:
			if (m_dialogBoxes[7].GetMode() == 1)
				EndInputString();
			m_dialogBoxes[7].SetMode(0);
			break;

		case 11:
			for (i = 0; i < MAXMENUITEMS; i++)
				if (m_pItemForSaleList[i] != 0)
				{
					delete m_pItemForSaleList[i];
					m_pItemForSaleList[i] = 0;
				}
			m_dialogBoxes[39].sV3 = 0;
			m_dialogBoxes[39].sV4 = 0;
			m_dialogBoxes[39].sV5 = 0;
			m_dialogBoxes[39].sV6 = 0;
			break;

		case 14:
			if (m_dialogBoxes[14].GetMode() < 0) return;
			break;

		case 17:
			if (m_dialogBoxes[17].GetMode() == 1)
			{
				EndInputString();
				if (m_dialogBoxes[17].sV3 != 1004) // leave item disabled if sending mail
					m_bIsItemDisabled[m_dialogBoxes[17].sView] = false;
			}
			break;

		case 20:
			m_bIsItemDisabled[m_dialogBoxes[20].sV1] = false;
			break;

		case 21:
			if (m_dialogBoxes[21].sV2 == 500)
			{
				bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GETMAGICABILITY, 0, 0, 0, 0, 0);
			}
			break;

		case 24:
			m_bSkillUsingStatus = false;
			break;

		case 26:
			if (m_dialogBoxes[26].sV1 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV1] = false;
			if (m_dialogBoxes[26].sV2 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV2] = false;
			if (m_dialogBoxes[26].sV3 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV3] = false;
			if (m_dialogBoxes[26].sV4 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV4] = false;
			if (m_dialogBoxes[26].sV5 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV5] = false;
			if (m_dialogBoxes[26].sV6 != -1) m_bIsItemDisabled[m_dialogBoxes[26].sV6] = false;
			m_bSkillUsingStatus = false;
			break;

		case 27: //Snoopy: 7 mar 06 (multiTrade) case rewriten
			for (i = 0; i < 8; i++)
			{
				ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
				ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
				m_stDialogBoxExchangeInfo[i].sV1 = -1;
				m_stDialogBoxExchangeInfo[i].sV2 = -1;
				m_stDialogBoxExchangeInfo[i].sV3 = -1;
				m_stDialogBoxExchangeInfo[i].sV4 = -1;
				m_stDialogBoxExchangeInfo[i].sV5 = -1;
				m_stDialogBoxExchangeInfo[i].sV6 = -1;
				m_stDialogBoxExchangeInfo[i].sV7 = -1;
				m_stDialogBoxExchangeInfo[i].dwV1 = 0;
				if (m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] == true)
					m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] = false;
			}
			break;


		case 31:
			for (i = 0; i < MAXSELLLIST; i++)
			{
				if (m_stSellItemList[i].iIndex != -1) m_bIsItemDisabled[m_stSellItemList[i].iIndex] = false;
				m_stSellItemList[i].iIndex = -1;
				m_stSellItemList[i].iAmount = 0;
			}
			break;

		case 34:
			if (m_dialogBoxes[34].sV1 != -1)
				m_bIsItemDisabled[m_dialogBoxes[34].sV1] = false;
			break;

		case 40:
			m_bIsItemDisabled[m_dialogBoxes[40].sV1] = false;
			m_bIsItemDisabled[m_dialogBoxes[40].sV2] = false;
			m_bIsItemDisabled[m_dialogBoxes[40].sV3] = false;
			m_bIsItemDisabled[m_dialogBoxes[40].sV4] = false;

			ZeroMemory(m_dialogBoxes[40].cStr, sizeof(m_dialogBoxes[40].cStr));
			ZeroMemory(m_dialogBoxes[40].cStr2, sizeof(m_dialogBoxes[40].cStr2));
			ZeroMemory(m_dialogBoxes[40].cStr3, sizeof(m_dialogBoxes[40].cStr3));
			ZeroMemory(m_dialogBoxes[40].cStr4, sizeof(m_dialogBoxes[40].cStr4));
			m_dialogBoxes[40].sV1 = -1;
			m_dialogBoxes[40].sV2 = -1;
			m_dialogBoxes[40].sV3 = -1;
			m_dialogBoxes[40].sV4 = -1;
			m_dialogBoxes[40].sV5 = -1;
			m_dialogBoxes[40].sV6 = -1;
			m_dialogBoxes[40].dwV1 = 0;
			m_dialogBoxes[40].dwV2 = 0;
			break;

		case 42:
			m_luStat[STAT_STR] = 0;
			m_luStat[STAT_VIT] = 0;
			m_luStat[STAT_DEX] = 0;
			m_luStat[STAT_INT] = 0;
			m_luStat[STAT_MAG] = 0;
			m_luStat[STAT_CHR] = 0;
			// Stats System xRisenx
			/*for (int i=0; i<TOTALLEVELUPPOINT; i++) {
				m_cStatChange[i] = 0;
			}
			break;*/
			//GuildBank
		case 56:
			if (m_dialogBoxes[56].GetMode() < 0) return;
			break;

		case DIALOG_GUILD:
			m_dialogBoxes[iBoxID].SetMode(0);
			if (m_dialogBoxes[iBoxID].GetMode() == 10)
			{
				EndInputString();
			}
			break;

		case DIALOG_GUILDCONTRIBUTE:
			if (m_dialogBoxes[iBoxID].GetMode() == 1)
			{
				EndInputString();
			}
			break;

		case DIALOG_MAILBOX:
			if (m_dialogBoxes[iBoxID].GetMode() == 1)
			{
				EndInputString();
			}
			break;
	}

	m_bIsDialogEnabled[iBoxID] = false;

	for (i = 0; i < MAXDIALOGBOX - 2; i++)
		if (m_cDialogBoxOrder[i] == iBoxID)
			m_cDialogBoxOrder[i] = 0;

	for (i = 1; i < MAXDIALOGBOX - 2; i++)
		if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
		{
			m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
			m_cDialogBoxOrder[i] = 0;
		}
}
