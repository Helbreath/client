
#include "helbreath.h"

#include "lan_eng.h"

// todo - make this a part of helbreath class?
extern char G_cSpriteAlphaDegree;

void helbreath::NotifyMsg_SetExchangeItem(char * pData)
{
    short * sp, sDir, sSprite, sSpriteFrame, sCurLife, sMaxLife, sPerformance;
    int * ip, iAmount, i;
    char * cp, cColor, cItemName[24], cCharName[12];
    uint32_t * dwp, dwAttribute;
    ZeroMemory(cItemName, sizeof(cItemName));
    ZeroMemory(cCharName, sizeof(cCharName));

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    sDir = *sp;
    cp += 2;
    sp = (short *)cp;
    sSprite = *sp;
    cp += 2;
    sp = (short *)cp;
    sSpriteFrame = *sp;
    cp += 2;
    ip = (int *)cp;
    iAmount = *ip;
    cp += 4;
    cColor = *cp;
    cp++;
    sp = (short *)cp;
    sCurLife = *sp;
    cp += 2;
    sp = (short *)cp;
    sMaxLife = *sp;
    cp += 2;
    sp = (short *)cp;
    sPerformance = *sp;
    cp += 2;
    memcpy(cItemName, cp, 20);
    cp += 20;
    memcpy(cCharName, cp, 10);
    cp += 10;
    dwp = (uint32_t *)cp;
    dwAttribute = *dwp;
    cp += 4;

    uint8_t sockets[MAXITEMSOCKETS];
    for (int j = 0; j < MAXITEMSOCKETS; j++)
        Pop(cp, sockets[j]);

    if (sDir >= 1000)  // Set the item I want to exchange
    {
        i = 0;
        while (m_stDialogBoxExchangeInfo[i].sV1 != -1)
        {
            i++;
            if (i >= 4) return; // Error situation
        }
    }
    else // Set the item he proposes me.
    {
        i = 4;
        while (m_stDialogBoxExchangeInfo[i].sV1 != -1)
        {
            i++;
            if (i >= 8) return; // Error situation
        }
    }
    m_stDialogBoxExchangeInfo[i].sV1 = sSprite;
    m_stDialogBoxExchangeInfo[i].sV2 = sSpriteFrame;
    m_stDialogBoxExchangeInfo[i].sV3 = iAmount;
    m_stDialogBoxExchangeInfo[i].sV4 = cColor;
    m_stDialogBoxExchangeInfo[i].sV5 = (int)sCurLife;
    m_stDialogBoxExchangeInfo[i].sV6 = (int)sMaxLife;
    m_stDialogBoxExchangeInfo[i].sV7 = (int)sPerformance;
    memcpy(m_stDialogBoxExchangeInfo[i].cStr1, cItemName, 20);
    memcpy(m_stDialogBoxExchangeInfo[i].cStr2, cCharName, 10);
    m_stDialogBoxExchangeInfo[i].dwV1 = dwAttribute;
    //if (i<4) m_stDialogBoxExchangeInfo[i].sItemID = sDir -1000;

    for (int j = 0; j < MAXITEMSOCKETS; j++)
        m_stDialogBoxExchangeInfo[i].sockets[j] = sockets[j];
}

void helbreath::NotifyMsg_DismissGuildApprove(char * pData)
{
    char * cp, cName[24];
    ZeroMemory(cName, sizeof(cName));

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    Pop(cp, cName, 20);

    ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
    m_guild.Reset();
    DisableDialogBox(57);
    m_iGuildRank = GUILDRANK_NONE;

    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 5);
}

void helbreath::NotifyMsg_DismissGuildReject(char * pData)
{
    char * cp, cName[21];
    ZeroMemory(cName, sizeof(cName));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    Pop(cp, cName, 20);

    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 6);
}

void helbreath::NotifyMsg_DownSkillIndexSet(char * pData)
{
    uint16_t * wp;
    short sSkillIndex;
    char * cp;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    wp = (uint16_t *)cp;
    sSkillIndex = (short)*wp;
    cp += 2;
    m_iDownSkillIndex = sSkillIndex;
    m_dialogBoxes[15].bFlag = false;
}

void helbreath::NotifyMsg_FishChance(char * pData)
{
    int iFishChance;
    char * cp;
    uint16_t * wp;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    wp = (uint16_t *)cp;
    iFishChance = (int)*wp;
    cp += 2;
    m_dialogBoxes[24].sV1 = iFishChance;
}

void helbreath::NotifyMsg_GuildDisbanded(char * pData)
{
    char * cp, cName[24], cLocation[12];
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);

    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;

    m_Misc.ReplaceString(cName, '_', ' ');


    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 7);

    ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
    m_guild.Reset();
    DisableDialogBox(57);
    m_iGuildRank = GUILDRANK_NONE;
}

void helbreath::NotifyMsg_WeatherChange(char * pData)
{
    char * cp;

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);

    SetWeatherStatus((Weather)*cp);
}

void helbreath::NotifyMsg_TimeChange(char * pData)
{
    char * cp;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    G_cSpriteAlphaDegree = *cp;

    switch (G_cSpriteAlphaDegree)
    {
        case 1:	PlaySound('E', 32, 0); break;
        case 2: PlaySound('E', 31, 0); break;
        case 3: // Special night with christmas bulbs
            if (m_weather > WEATHER_LIGHTSNOW && m_weather <= WEATHER_HEAVYSNOW)
                m_bIsXmas = true;
            else
                m_bIsXmas = false;

            PlaySound('E', 31, 0);
            G_cSpriteAlphaDegree = 2;
            break;
    }

    m_cGameModeCount = 1;
    m_bIsRedrawPDBGS = true;
}

void helbreath::NotifyMsg_RepairItemPrice(char * pData)
{
    char * cp, cName[21];
    uint32_t * dwp, wV1, wV2, wV3, wV4;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    dwp = (uint32_t *)cp;
    wV1 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV2 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV3 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV4 = *dwp;
    cp += 4;
    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;
    EnableDialogBox(23, 2, wV1, wV2);
    m_dialogBoxes[23].sV3 = wV3;
}

void helbreath::NotifyMsg_SellItemPrice(char * pData)
{
    char * cp, cName[21];
    uint32_t * dwp, wV1, wV2, wV3, wV4;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    dwp = (uint32_t *)cp;
    wV1 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV2 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV3 = *dwp;
    cp += 4;
    dwp = (uint32_t *)cp;
    wV4 = *dwp;
    cp += 4;
    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;
    EnableDialogBox(23, 1, wV1, wV2);
    m_dialogBoxes[23].sV3 = wV3;
    m_dialogBoxes[23].sV4 = wV4;
}

void helbreath::NotifyMsg_QueryDismissGuildPermission(char * pData)
{
    char * cp, cName[12];
    ZeroMemory(cName, sizeof(cName));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    memcpy(cName, cp, 10);
    cp += 10;
    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 2);
}

void helbreath::NotifyMsg_QueryJoinGuildPermission(char * pData)
{
    char * cp, cName[12];
    ZeroMemory(cName, sizeof(cName));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    memcpy(cName, cp, 10);
    cp += 10;
    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 1);
}

void helbreath::NotifyMsg_QuestContents(char * pData)
{
    short * sp;
    char * cp;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    m_stQuest.sWho = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sQuestType = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sContribution = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sTargetType = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sTargetCount = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sX = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sY = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.sRange = *sp;
    cp += 2;
    sp = (short *)cp;
    m_stQuest.bIsQuestCompleted = (bool)*sp;
    cp += 2;
    ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
    memcpy(m_stQuest.cTargetName, cp, 20);
    cp += 20;

    //AddEventList(NOTIFYMSG_QUEST_STARTED, 10);
}

void helbreath::NotifyMsg_PlayerProfile(char * pData)
{
    char * cp;
    char cTemp[500];
    int i;
    ZeroMemory(cTemp, sizeof(cTemp));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    strcpy(cTemp, cp);
    for (i = 0; i < 500; i++)
        if (cTemp[i] == '_') cTemp[i] = ' ';
    AddEventList(cTemp, 10);
}

void helbreath::NotifyMsg_NoticeMsg(char * pData)
{
    char * cp, cMsg[1000];
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    strcpy(cMsg, cp);
    AddEventList(cMsg, 10);
}

void helbreath::NotifyMsg_OpenExchageWindow(char * pData)
{
    short * sp, sDir, sSprite, sSpriteFrame, sCurLife, sMaxLife, sPerformance;
    int * ip, iAmount;
    char * cp, cColor, cItemName[24], cCharName[12];
    uint32_t * dwp, dwAttribute;
    ZeroMemory(cItemName, sizeof(cItemName));
    ZeroMemory(cCharName, sizeof(cCharName));

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    sDir = *sp;
    cp += 2;
    sp = (short *)cp;
    sSprite = *sp;
    cp += 2;
    sp = (short *)cp;
    sSpriteFrame = *sp;
    cp += 2;
    ip = (int *)cp;
    iAmount = *ip;
    cp += 4;
    cColor = *cp;
    cp++;
    sp = (short *)cp;
    sCurLife = *sp;
    cp += 2;
    sp = (short *)cp;
    sMaxLife = *sp;
    cp += 2;
    sp = (short *)cp;
    sPerformance = *sp;
    cp += 2;
    memcpy(cItemName, cp, 20);
    cp += 20;
    memcpy(cCharName, cp, 10);
    cp += 10;
    dwp = (uint32_t *)cp;
    dwAttribute = *dwp;
    cp += 4;

    uint8_t sockets[MAXITEMSOCKETS];
    for (int j = 0; j < MAXITEMSOCKETS; j++)
        Pop(cp, sockets[j]);

    EnableDialogBox(27, 1, 0, 0, 0);
    int i;
    if (sDir >= 1000)  // Set the item I want to exchange
    {
        i = 0;
        while (m_stDialogBoxExchangeInfo[i].sV1 != -1)
        {
            i++;
            if (i >= 4) return; // Error situation
        }
        if ((sDir > 1000) && (i == 0))
        {
            m_bIsItemDisabled[sDir - 1000] = true;
            m_stDialogBoxExchangeInfo[0].sItemID = sDir - 1000;
        }
    }
    else // Set the item he proposes me.
    {
        i = 4;
        while (m_stDialogBoxExchangeInfo[i].sV1 != -1)
        {
            i++;
            if (i >= 8) return; // Error situation
        }
    }
    m_stDialogBoxExchangeInfo[i].sV1 = sSprite;
    m_stDialogBoxExchangeInfo[i].sV2 = sSpriteFrame;
    m_stDialogBoxExchangeInfo[i].sV3 = iAmount;
    m_stDialogBoxExchangeInfo[i].sV4 = cColor;
    m_stDialogBoxExchangeInfo[i].sV5 = (int)sCurLife;
    m_stDialogBoxExchangeInfo[i].sV6 = (int)sMaxLife;
    m_stDialogBoxExchangeInfo[i].sV7 = (int)sPerformance;
    memcpy(m_stDialogBoxExchangeInfo[i].cStr1, cItemName, 20);
    memcpy(m_stDialogBoxExchangeInfo[i].cStr2, cCharName, 10);
    m_stDialogBoxExchangeInfo[i].dwV1 = dwAttribute;

    for (int j = 0; j < MAXITEMSOCKETS; j++)
        m_stDialogBoxExchangeInfo[i].sockets[j] = sockets[j];
}

void helbreath::NotifyMsg_JoinGuildApprove(char * pData)
{
    char * cp, cName[21];
    ZeroMemory(cName, sizeof(cName));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    memcpy(cName, cp, 20);
    cp += 20;
    short * sp = (short *)cp;
    m_iGuildRank = (GuildRank)*sp;
    cp += 2;

    ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
    strcpy(m_cGuildName, cName);
    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 3);
}

void helbreath::NotifyMsg_JoinGuildReject(char * pData)
{
    char * cp, cName[21];
    ZeroMemory(cName, sizeof(cName));
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    Pop(cp, cName, 20);

    EnableDialogBox(8, 0, 0, 0);
    _PutGuildOperationList(cName, 4);
}

void helbreath::NotifyMsgHandler(char * pData)
{
	uint32_t * dwp, dwTime, dwTemp;
	uint16_t * wp, wEventType;
	char * cp, cTemp[510], cTxt[120], cTempName[21];
	short * sp, sX, sY, sV1 = 0, sV2, sV3, sV4, sV5, sV6, sV7, sV8, sV9;
	int * ip, i, iV1, iV2, iV3, iV4;
	std::string str;

	dwTime = unixtime();

	wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	wEventType = *wp;

	switch (wEventType)
	{
		case NOTIFY_SOCCERBEGIN:
			m_bSoccerMode = true;
			SetTopMsg("Soccer event has been started.", 10);
			break;

		case NOTIFY_SOCCERVICTORY:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			ShowSoccerVictory(sV1);
			m_SoccerAresdenGoals = 0;
			m_SoccerElvineGoals = 0;
			m_bSoccerMode = false;
			SetTopMsg("Soccer event has been closed.", 10);
			break;

		case NOTIFY_SOCCER:
			NotifyMsg_Soccer(pData);
			break;

		case NOTIFY_GUILDCONTRIBUTERSP:
			GuildContributeRsp(pData + 6);
			break;

		case NOTIFY_LGNPTS:
			dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
			m_cash = *dwp;
			break;

		case NOTIFY_SLATE_BERSERK:		// reversed by Snoopy: 0x0BED
			AddEventList(MSG_NOTIFY_SLATE_BERSERK, 10);//"Berserk magic casted!"
			m_bUsingSlate = true;
			break;

		case NOTIFY_LOTERY_LOST:		// reversed by Snoopy: 0x0BEE:
			AddEventList(MSG_NOTIFY_LOTERY_LOST, 10);//"You draw a blank. Please try again next time.."
			break;

		case NOTIFY_TELEPORT_REJECTED:
			sp = (short *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *sp; sp++;
			sV2 = *sp; sp++;
			m_pMapData->m_tile[sV1][sV2].m_bIsTeleport = false;
			m_bIsTeleportRequested = false;
			ChangeGameMode(GAMEMODE_ONMAINGAME);
			AddEventList(NOTIFYMSG_TELEPORT_REJECTED, 10);
			break;

		case NOTIFY_0BEF:				// 0x0BEF: // Snoopy: Crash or closes the client? (Calls SE entry !)
			// I'm noot sure at all of this function's result, so let's quit game...

			break;
		case NOTIFY_EVENTILLUSION:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			if (sV1 == 1) AddEventList(NOTIFYMSG_EVENTILLUSIONON, 10);
			else AddEventList(NOTIFYMSG_EVENTILLUSIONOFF, 10);
			break;

		case NOTIFY_EVENTTP:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;

			if (sV2)
			{
				// donate event
				if (sV1 == 1)
					SetTopMsg(NOTIFYMSG_DONATEEVENTTPOPEN, 10);
				else
					SetTopMsg(NOTIFYMSG_DONATEEVENTTPCLOSE, 10);
			}
			else
			{
				if (sV1 == 1)
					AddEventList(NOTIFYMSG_EVENTTPOPEN, 10);
				else
					AddEventList(NOTIFYMSG_EVENTTPCLOSE, 10);
			}
			break;

		case NOTIFY_EVENTSPELL:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;
			sV3 = *cp;
			cp++;
			if (sV2 < MAXMAGICTYPE && sV2 >= 0)
			{
				if (sV1 == 1)
				{
					if (!sV3) m_magicDisabled[sV2] = true;
					fmt::format_to(cTemp, NOTIFYMSG_SPELLDISABLED, m_pMagicCfgList[sV2]->m_cName);
				}
				else
				{
					m_magicDisabled[sV2] = false;
					fmt::format_to(cTemp, NOTIFYMSG_SPELLENABLED, m_pMagicCfgList[sV2]->m_cName);
				}
				AddEventList(cTemp, 10);
			}
			break;

		case NOTIFY_EVENTARMOR:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;
			if (sV1 == 1)
			{
				if (!sV2) m_armorDisabled = true;
				AddEventList(NOTIFYMSG_ARMORDISABLED, 10);
			}
			else
			{
				m_armorDisabled = false;
				AddEventList(NOTIFYMSG_ARMORENABLED, 10);
			}
			break;

		case NOTIFY_EVENTSHIELD:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;
			if (sV1 == 1)
			{
				AddEventList(NOTIFYMSG_SHIELDDISABLED, 10);
				if (!sV2) m_shieldDisabled = true;
			}
			else
			{
				AddEventList(NOTIFYMSG_SHIELDENABLED, 10);
				m_shieldDisabled = false;
			}
			break;

		case NOTIFY_EVENTCHAT:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;
			if (sV1 == 1) AddEventList(NOTIFYMSG_CHATDISABLED, 10);
			else AddEventList(NOTIFYMSG_CHATENABLED, 10);
			break;

		case NOTIFY_EVENTPARTY:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sV1 = *cp;
			cp++;
			sV2 = *cp;
			cp++;
			if (sV1 == 1) AddEventList(NOTIFYMSG_PARTYDISABLED, 10);
			else AddEventList(NOTIFYMSG_PARTYENABLED, 10);
			break;

		case NOTIFY_EVENTRESET:
			m_shieldDisabled = false;
			m_armorDisabled = false;
			for (i = 0; i < MAXMAGICTYPE; i++)
				m_magicDisabled[i] = false;
			AddEventList(NOTIFYMSG_EVENTRESET, 10);
			break;

		case NOTIFY_CRAFTING_SUCCESS:	//reversed by Snoopy: 0x0BF0:
			m_iContribution -= m_iContributionPrice;
			m_iContributionPrice = 0;
			DisableDialogBox(25);
			AddEventList(NOTIFY_MSG_HANDLER42, 10);		// "Item manufacture success!"
			PlaySound('E', 23, 5);
			switch (m_sPlayerType)
			{
				case 1:
				case 2:
				case 3:
					PlaySound('C', 21, 0);
					break;
				case 4:
				case 5:
				case 6:
					PlaySound('C', 22, 0);
					break;
			}
			break;

		case NOTIFY_CRAFTING_FAIL:
			AddEventList(MSG_NOTIFY_CRAFTING_FAILED, 10);		// "Crafting failed"
			PlaySound('E', 24, 5);
			break;

		case NOTIFY_NOMATCHINGCRAFTING:
			AddEventList(MSG_NOTIFY_CRAFTING_NO_PART, 10);		// "There is not enough material"
			PlaySound('E', 24, 5);
			break;

		case NOTIFY_NO_CRAFT_CONTRIB:
			AddEventList(MSG_NOTIFY_CRAFTING_NO_CONTRIB, 10);	// "There is not enough Contribution Point"
			PlaySound('E', 24, 5);
			break;

		case NOTIFY_ANGELIC_STATS:		// reversed by Snoopy: 0x0BF2
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			m_angelStat[STAT_STR] = *ip;  // m_angelStat[STAT_STR]
			cp += 4;
			ip = (int *)cp;
			m_angelStat[STAT_INT] = *ip;  // m_angelStat[STAT_INT]
			cp += 4;
			ip = (int *)cp;
			m_angelStat[STAT_DEX] = *ip;  // m_angelStat[STAT_DEX]
			cp += 4;
			ip = (int *)cp;
			m_angelStat[STAT_MAG] = *ip;  // m_angelStat[STAT_MAG]
			break;

		case NOTIFY_ITEM_CANT_RELEASE:	// reversed by Snoopy: 0x0BF3
			AddEventList(MSG_NOTIFY_NOT_RELEASED, 10);//"Item cannot be released"
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ItemEquipHandler(*cp);
			break;

		case NOTIFY_ANGEL_FAILED:		// reversed by Snoopy: 0x0BF4
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			iV1 = *ip; // Error reason
			switch (iV1)
			{
				case 1: // "BFB9BBF3C4A120BECAC0BA20B9F6B1D7C0D4B4CFB4D92E20A4D02E2EA4D0" (Stolen bytes ?)
					AddEventList(MSG_NOTIFY_ANGEL_FAILED, 10); //"Impossible to get a Tutelary Angel." // Invented by Snoopy.
					break;
				case 2: //
					AddEventList(MSG_NOTIFY_ANGEL_MAJESTIC, 10);//"You need additional Majesty Points."
					break;
				case 3: //
					AddEventList(MSG_NOTIFY_ANGEL_LOW_LVL, 10); //"Only Majesty characters can receive Tutelary Angel"
					break;
			}
			break;

		case NOTIFY_ANGEL_RECEIVED:		// reversed by Snoopy: 0x0BF5:
			AddEventList("Gold has been decreased by 100.000", 10);
			AddEventList("Majestic points has been decreased by 100 points.", 10);
			AddEventList(MSG_NOTIFY_ANGEL_RECEIVED, 10);// "You have received the Tutelary Angel."
			break;

		case NOTIFY_SPELL_SKILL:		// reversed by Snoopy: 0x0BF6
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			for (i = 0; i < MAXMAGICTYPE; i++)
			{
				m_cMagicMastery[i] = *cp;
				cp++;
			}
			for (i = 0; i < MAXSKILLTYPE; i++)
			{
				m_cSkillMastery[i] = (unsigned char)*cp;
				if (m_pSkillCfgList[i] != 0)
					m_pSkillCfgList[i]->m_iLevel = (int)*cp;
				cp++;
			}
			break;

		case NOTIFY_APOCGATESTARTMSG: // Snoopy 0x0BD2
			SetTopMsg("The portal to the Apocalypse is opened.", 10);
			break;

		case NOTIFY_APOCGATEENDMSG: // Snoopy 0x0BD3
			SetTopMsg("The portal to the Apocalypse is closed.", 10);
			break;

		case NOTIFY_APOCGATEOPEN: // Snoopy ;  Case BD4 of switch 00454077
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			m_iGatePositX = *ip;
			cp += 4;
			ip = (int *)cp;
			m_iGatePositY = *ip;
			cp += 4;
			ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
			memcpy(m_cGateMapName, cp, 10);
			cp += 10;
			break;

		case NOTIFY_QUESTCOUNTER: // Snoopy;  Case BE2 of switch 00454077
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			m_stQuest.sCurrentCount = (short)*ip;
			cp += 4;
			break;
			//	// Defense Display in Action Bar xRisenx
		case NOTIFY_REQDEF: // Notify Defense xRisenx
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			m_iDefenseRatio = *ip;
			cp += 4;
			break;

		case NOTIFY_MONSTERCOUNT: // Snoopy ;  Case BE3 of switch 00454077
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;

			sV1 = *sp;
			sp++;
			sV2 = *sp;
			sp++;
			sV3 = *sp;

			cp += 6;
			fmt::format_to(cTxt, "Rest Monster :%d, Pos: %d, %d", sV1, sV2, sV3);
			AddEventList(cTxt, 10);
			break;

		case NOTIFY_APOCGATECLOSE: // Snoopy ;  Case BD5 of switch 00454077
			m_iGatePositX = m_iGatePositY = -1;
			ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
			break;

		case NOTIFY_APOCFORCERECALLPLAYERS: // Snoopy ;  Case BD7 of switch 00454077
			AddEventList("You are recalled by force, because the Apocalypse is started.", 10);
			break;

		case NOTIFY_ABADDONKILLED: // Snoopy ;  Case BD6 of switch 00454077
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;
			fmt::format_to(G_cTxt, "Abaddon is destroyed by %s", cTxt);
			AddEventList(G_cTxt, 10);
			break;

		case NOTIFY_RESURRECTPLAYER: // Case BE9 of switch 00454077
			EnableDialogBox(50, 0, 0, 0);
			break;

		case NOTIFY_HELDENIANEND:
			m_bIsHeldenianMode = false;
			SetTopMsg("Heldenian holy war has been closed.", 10);
			break;

		case NOTIFY_HELDENIANSTART:
			m_bIsHeldenianMode = true;
			m_bIsHeldenianMap = ((m_cMapIndex == 35) || (m_cMapIndex == 36) || (m_cMapIndex == 37));
			SetTopMsg("Heldenian has been started! Teleport to Heldenian field is available", 10);
			break;

		case NOTIFY_HELDENIANVICTORY:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			ShowHeldenianVictory(sV1);
			m_iHeldenianAresdenLeftTower = 0;
			m_iHeldenianElvineLeftTower = 0;
			m_iHeldenianAresdenFlags = 0;
			m_iHeldenianElvineFlags = 0;
			m_iHeldenianAresdenDead = 0;
			m_iHeldenianElvineDead = 0;
			m_iHeldenianAresdenKill = 0;
			break;

		case NOTIFY_HELDENIANCOUNT:
			NotifyMsg_Heldenian(pData);
			break;


			// Slates - Diuuude
		case NOTIFY_SLATE_CREATESUCCESS:	// 0x0BC1
			AddEventList(MSG_NOTIFY_SLATE_CREATESUCCESS, 10);
			break;
		case NOTIFY_SLATE_CREATEFAIL:		// 0x0BC2
			AddEventList(MSG_NOTIFY_SLATE_CREATEFAIL, 10);
			break;
		case NOTIFY_SLATE_INVINCIBLE:		// 0x0BD8
			AddEventList(MSG_NOTIFY_SLATE_INVINCIBLE, 10);
			m_bUsingSlate = true;
			break;
		case NOTIFY_SLATE_MANA:				// 0x0BD9
			AddEventList(MSG_NOTIFY_SLATE_MANA, 10);
			m_bUsingSlate = true;
			break;
		case NOTIFY_SLATE_EXP:				// 0x0BE0
			AddEventList(MSG_NOTIFY_SLATE_EXP, 10);
			m_bUsingSlate = true;
			break;
		case NOTIFY_SLATE_STATUS:			// 0x0BE1
			AddEventList(MSG_NOTIFY_SLATECLEAR, 10); // "The effect of the prophecy-slate is disappeared."
			m_bUsingSlate = false;
			break;

			// MJ Stats Change - Diuuude: Erreur, ici il s'agit de sorts et skills, le serveur comme la v351 sont aussi bugués !
		case NOTIFY_STATECHANGE_SUCCESS:	// 0x0BB5
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			for (i = 0; i < MAXMAGICTYPE; i++)
			{
				m_cMagicMastery[i] = *cp;
				cp++;
			}
			for (i = 0; i < MAXSKILLTYPE; i++)
			{
				m_cSkillMastery[i] = (unsigned char)*cp;
				if (m_pSkillCfgList[i] != 0)
					m_pSkillCfgList[i]->m_iLevel = (int)*cp;
				//else m_pSkillCfgList[i]->m_iLevel = 0;
				cp++;
			}
			// MJ Stats Change - Diuuude
			m_stat[STAT_STR] += m_luStat[STAT_STR];
			m_stat[STAT_VIT] += m_luStat[STAT_VIT];
			m_stat[STAT_DEX] += m_luStat[STAT_DEX];
			m_stat[STAT_INT] += m_luStat[STAT_INT];
			m_stat[STAT_MAG] += m_luStat[STAT_MAG];
			m_stat[STAT_CHR] += m_luStat[STAT_CHR];
			m_iLU_Point = m_iLevel * 7 - (
				//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
				(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
				- 70)
				- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
			AddEventList("Your stat has been changed.", 10); // "Your stat has been changed."
			break;

		case NOTIFY_LEVELUP: // 0x0B16
			NotifyMsg_LevelUp(pData);
			break;

		case NOTIFY_STATECHANGE_FAILED:		// 0x0BB6
			m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
			m_iLU_Point = m_iLevel * 7 - (
				//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
				(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
				- 70)
				- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			AddEventList("Your stat has not been changed.", 10);
			break;

		case NOTIFY_SETTING_FAILED: // 0x0BB4 -  Case BB4 of switch 00454077
			AddEventList("Your stat has not been changed.", 10);
			m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
			m_iLU_Point = m_iLevel * 7 - (
				//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - ( // Stats System xRisenx
				(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
				- 70)
				- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			// Stats System xRisenx
			//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
			break;

			// CLEROTH - LU
		case NOTIFY_SETTING_SUCCESS: // 0x0BB3 - envoie le niv et les stats
			NotifyMsg_SettingSuccess(pData);
			break;

		case NOTIFY_AGRICULTURENOAREA:		// 0x0BB2
			AddEventList(MSG_NOTIFY_AGRICULTURENOAREA, 10);
			break;
		case NOTIFY_AGRICULTURESKILLLIMIT:	// 0x0BB1
			AddEventList(MSG_NOTIFY_AGRICULTURESKILLLIMIT, 10);
			break;

		case NOTIFY_NOMOREAGRICULTURE:		// 0x0BB0
			AddEventList(MSG_NOTIFY_NOMOREAGRICULTURE, 10);
			break;
		case NOTIFY_MONSTEREVENT_POSITION:				// 0x0BAA
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			m_sMonsterID = (short)(*cp);
			cp++;
			sp = (short *)cp;
			m_sEventX = *sp;
			cp += 2;
			sp = (short *)cp;
			m_sEventY = *sp;
			cp += 2;
			m_dwMonsterEventTime = dwTime;
			break;

		case NOTIFY_RESPONSE_HUNTMODE:			// 0x0BA9
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			memcpy(m_cLocation, cp, 10);
			cp += 10;

			if (memcmp(m_cLocation, "are", 3) == 0)
				m_side = ARESDEN;
			else if (memcmp(m_cLocation, "elv", 3) == 0)
				m_side = ELVINE;
			/*else if (memcmp(m_cLocation, "ist", 3) == 0)
				m_side = ISTRIA;*/
			else
				m_side = NEUTRAL;

			AddEventList(MSG_GAMEMODE_CHANGED, 10);
			break;

		case NOTIFY_REQGUILDNAMEANSWER:	 //   0x0BA6
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			ZeroMemory(cTemp, sizeof(cTemp));
			memcpy(cTemp, cp, 20);
			cp += 20;

			ZeroMemory(m_stGuildName[sV2].cGuildName, sizeof(m_stGuildName[sV2].cGuildName));
			strcpy(m_stGuildName[sV2].cGuildName, cTemp);
			m_stGuildName[sV2].iGuildRank = sV1;
			for (i = 0; i < 20; i++) if (m_stGuildName[sV2].cGuildName[i] == '_') m_stGuildName[sV2].cGuildName[i] = ' ';
			break;

		case NOTIFY_FORCERECALLTIME: // 0x0BA7
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			if ((int)(sV1 / 20) > 0)
				fmt::format_to(G_cTxt, NOTIFY_MSG_FORCERECALLTIME1, (int)(sV1 / 20));
			else
				fmt::format_to(G_cTxt, NOTIFY_MSG_FORCERECALLTIME2);
			AddEventList(G_cTxt, 10);
			break;

		case NOTIFY_GIZONITEMUPGRADELEFT: // 0x0BA4// Item upgrade is possible.
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			m_iGizonItemUpgradeLeft = sV1;
			dwp = (uint32_t *)cp;
			switch (*dwp)
			{
				case 1: //
					AddEventList(NOTIFY_MSG_HANDLER_GIZONITEMUPGRADELEFT1, 10);
					break;
			}
			//fmt::format_to(G_cTxt,"majesty: %d", m_iGizonItemUpgradeLeft);
			//DebugLog(G_cTxt);
			cp += 4;
			break;

		case NOTIFY_GIZONEITEMCHANGE: // 0x0BA5
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			m_pItemList[sV1]->m_cItemType = *cp;
			cp++;
			wp = (uint16_t *)cp;
			m_pItemList[sV1]->m_wCurLifeSpan = *wp;
			cp += 2;
			sp = (short *)cp;
			m_pItemList[sV1]->m_sSprite = *sp;
			cp += 2;
			sp = (short *)cp;
			m_pItemList[sV1]->m_sSpriteFrame = *sp;
			cp += 2;
			dwp = (uint32_t *)cp;
			m_pItemList[sV1]->m_ItemColor = *dwp;
			cp += 4;
			m_pItemList[sV1]->m_sItemSpecEffectValue2 = *cp;
			cp++;
			dwp = (uint32_t *)cp;
			m_pItemList[sV1]->m_dwAttribute = *dwp;
			cp += 4;
			ZeroMemory(m_pItemList[sV1]->m_cName, sizeof(m_pItemList[sV1]->m_cName));
			memcpy(m_pItemList[sV1]->m_cName, cp, 20);
			cp += 20;
			if (m_bIsDialogEnabled[34] == true)
			{
				m_dialogBoxes[34].SetMode(3); // succes
			}
			PlaySound('E', 23, 5);
			switch (m_sPlayerType)
			{
				case 1:
				case 2:
				case 3:
					PlaySound('C', 21, 0);
					break;

				case 4:
				case 5:
				case 6:
					PlaySound('C', 22, 0);
					break;
			}
			break;

		case NOTIFY_ITEMATTRIBUTECHANGE: // 0x0BA3
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			dwTemp = m_pItemList[sV1]->m_dwAttribute;
			dwp = (uint32_t *)cp;
			m_pItemList[sV1]->m_dwAttribute = *dwp;
			cp += 4;
			dwp = (uint32_t *)cp;
			if (*dwp != 0) m_pItemList[sV1]->m_sItemSpecEffectValue1 = (short)*dwp;
			cp += 4;
			dwp = (uint32_t *)cp;
			if (*dwp != 0) m_pItemList[sV1]->m_sItemSpecEffectValue2 = (short)*dwp;
			cp += 4;
			if (dwTemp == m_pItemList[sV1]->m_dwAttribute)
			{
				if (m_bIsDialogEnabled[34] == true)
				{
					m_dialogBoxes[34].SetMode(4);// Failed
				}
				PlaySound('E', 24, 5);
			}
			else
			{
				if (m_bIsDialogEnabled[34] == true)
				{
					m_dialogBoxes[34].SetMode(3); // Success
				}
				PlaySound('E', 23, 5);
				switch (m_sPlayerType)
				{
					case 1:
					case 2:
					case 3:
						PlaySound('C', 21, 0);
						break;
					case 4:
					case 5:
					case 6:
						PlaySound('C', 22, 0);
						break;
				}
			}
			break;

		case NOTIFY_ITEMUPGRADEFAIL:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			if (m_bIsDialogEnabled[34] == false) return;
			PlaySound('E', 24, 5);
			switch (sV1)
			{
				case 1:
					m_dialogBoxes[34].SetMode(8); // Failed
					break;
				case 2:
					m_dialogBoxes[34].SetMode(9); // Failed
					break;
				case 3:
					m_dialogBoxes[34].SetMode(10); // Failed
					break;
			}
			break;

		case NOTIFY_PARTY:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV3 = *sp;
			cp += 2;
			switch (sV1)
			{
				case 1: //Create Party
					switch (sV2)
					{
						case 0: //Create party failure
							EnableDialogBox(32, 0, 0, 0);
							m_dialogBoxes[32].SetMode(9);
							break;

						case 1: //Create party success
							m_iPartyStatus = 1;
							EnableDialogBox(32, 0, 0, 0);
							m_dialogBoxes[32].SetMode(8);
							ClearPartyMembers();
							break;
					}
					break;

				case 2: //Delete party
					m_iPartyStatus = 0;
					EnableDialogBox(32, 0, 0, 0);
					m_dialogBoxes[32].SetMode(10);
					ClearPartyMembers();
					break;

				case 4: //Join party
					ZeroMemory(cTxt, sizeof(cTxt));
					memcpy(cTxt, cp, 10);
					cp += 10;

					switch (sV2)
					{
						case 0: //Join party failure
							EnableDialogBox(32, 0, 0, 0);
							m_dialogBoxes[32].SetMode(9);
							break;

						case 1: //Join party success
							if (cTxt == player_name)
							{
								m_iPartyStatus = 2;
								EnableDialogBox(32, 0, 0, 0);
								m_dialogBoxes[32].SetMode(8);
							}
							else
							{
								fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER1, cTxt);
								AddEventList(G_cTxt, 10);
							}

							m_stPartyMember.push_back(new partyMember(cTxt));
							break;
					}
					break;

				case 5: //Get list of names
					m_iTotalPartyMember = sV3;
					for (i = 0; i < sV3; i++)
					{
						ZeroMemory(cTxt, sizeof(cTxt));
						memcpy(cTxt, cp, 10);
						for (sV6 = 0, sV5 = false; sV6 < m_stPartyMember.size(); sV6++)
							if (m_stPartyMember[sV6]->cName.compare(cTxt) == 0) { sV5 = true; break; }

						if (sV5 == false)
						{
							m_stPartyMember.push_back(new partyMember(cTxt));
						}
						cp += 11;
					}
					break;

				default:
					sp = (short *)cp;
					sV4 = *sp;
					cp += 2;
					break;

				case 6: //Dismissed party member
					ZeroMemory(cTxt, sizeof(cTxt));
					memcpy(cTxt, cp, 10);
					cp += 10;

					switch (sV2)
					{
						case 0: //
							EnableDialogBox(32, 0, 0, 0);
							m_dialogBoxes[32].SetMode(7);
							break;

						case 1: //Party member successfully dismissed
							if (cTxt == player_name)
							{
								m_iPartyStatus = 0;
								EnableDialogBox(32, 0, 0, 0);
								m_dialogBoxes[32].SetMode(6);
							}
							else
							{
								fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER2, cTxt);
								AddEventList(G_cTxt, 10);
							}
							for (std::vector<partyMember *>::iterator it = m_stPartyMember.begin(); it < m_stPartyMember.end(); ++it)
							{
								if ((*it)->cName.compare(cTxt) == 0)
								{
									delete * it;
									m_stPartyMember.erase(it);
									break;
								}
							}
							break;
					}
					break;

				case 7: //Party join rejected
					EnableDialogBox(32, 0, 0, 0);
					m_dialogBoxes[32].SetMode(9);
					break;

				case 8: //You left the party
					m_iPartyStatus = 0;
					m_iTotalPartyMember = 0;
					ClearPartyMembers();
					break;
			}
			break;

		case NOTIFY_PARTY_COORDS:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;

			sp = (short *)cp;
			sV4 = *sp;
			cp += 2;

			sp = (short *)cp;
			sV5 = *sp;  //
			cp += 2;
			for (i = 0; i < m_stPartyMember.size(); i++)
			{
				if (m_stPartyMember[i]->cName.compare(cTxt) == 0)
				{
					m_stPartyMember[i]->sX = sV4;
					m_stPartyMember[i]->sY = sV5;
				}
			}
			break;

		case NOTIFY_CANNOTCONSTRUCT:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;

			CannotConstruct(sV1);
			PlaySound('E', 25, 0, 0);
			break;

		case NOTIFY_TCLOC:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			sp = (short *)cp;
			m_iTeleportLocX = *sp;
			cp += 2;

			sp = (short *)cp;
			m_iTeleportLocY = *sp;
			cp += 2;

			ZeroMemory(m_cTeleportMapName, sizeof(m_cTeleportMapName));
			memcpy(m_cTeleportMapName, cp, 10);
			cp += 10;

			sp = (short *)cp;
			m_iConstructLocX = *sp;
			cp += 2;

			sp = (short *)cp;
			m_iConstructLocY = *sp;
			cp += 2;

			ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
			memcpy(m_cConstructMapName, cp, 10);
			cp += 10;
			break;

		case NOTIFY_CONSTRUCTIONPOINT:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;

			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;

			sp = (short *)cp;
			sV3 = *sp;
			cp += 2;

			if (sV3 == 0)
			{
				if ((sV1 > m_iConstructionPoint) && (sV2 > m_iWarContribution))
				{
					fmt::format_to(G_cTxt, "%s +%d, %s +%d", CRUSADE_MESSAGE13, (sV1 - m_iConstructionPoint), CRUSADE_MESSAGE21, (sV2 - m_iWarContribution));
					SetTopMsg(G_cTxt, 5);
					PlaySound('E', 23, 0, 0);
				}

				if ((sV1 > m_iConstructionPoint) && (sV2 == m_iWarContribution))
				{
					if (m_iCrusadeDuty == 3)
					{
						fmt::format_to(G_cTxt, "%s +%d", CRUSADE_MESSAGE13, sV1 - m_iConstructionPoint);
						SetTopMsg(G_cTxt, 5);
						PlaySound('E', 23, 0, 0);
					}
				}

				if ((sV1 == m_iConstructionPoint) && (sV2 > m_iWarContribution))
				{
					fmt::format_to(G_cTxt, "%s +%d", CRUSADE_MESSAGE21, sV2 - m_iWarContribution);
					SetTopMsg(G_cTxt, 5);
					PlaySound('E', 23, 0, 0);
				}

				if (sV1 < m_iConstructionPoint)
				{
					if (m_iCrusadeDuty == 3)
					{
						fmt::format_to(G_cTxt, "%s -%d", CRUSADE_MESSAGE13, m_iConstructionPoint - sV1);
						SetTopMsg(G_cTxt, 5);
						PlaySound('E', 25, 0, 0);
					}
				}

				if (sV2 < m_iWarContribution)
				{
					fmt::format_to(G_cTxt, "%s -%d", CRUSADE_MESSAGE21, m_iWarContribution - sV2);
					SetTopMsg(G_cTxt, 5);
					PlaySound('E', 24, 0, 0);
				}
			}

			m_iConstructionPoint = sV1;
			m_iWarContribution = sV2;
			break;

		case NOTIFY_NOMORECRUSADESTRUCTURE:
			SetTopMsg(CRUSADE_MESSAGE12, 5);
			PlaySound('E', 25, 0, 0);
			break;

		case NOTIFY_GRANDMAGICRESULT:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			wp = (uint16_t *)cp;
			sV1 = *wp;
			cp += 2;

			wp = (uint16_t *)cp;
			sV2 = *wp;
			cp += 2;

			wp = (uint16_t *)cp;
			sV3 = *wp;
			cp += 2;

			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;

			wp = (uint16_t *)cp;
			sV4 = *wp;
			cp += 2;

			wp = (uint16_t *)cp;
			sV5 = *wp;  //
			cp += 2;

			if (sV5 > 0)
			{
				wp = (uint16_t *)cp;
				sV6 = *wp;
				cp += 2;
				sV5--;
			}
			else sV6 = 0;

			if (sV5 > 0)
			{
				wp = (uint16_t *)cp;
				sV7 = *wp;
				cp += 2;
				sV5--;
			}
			else sV7 = 0;

			if (sV5 > 0)
			{
				wp = (uint16_t *)cp;
				sV8 = *wp;
				cp += 2;
				sV5--;
			}
			else sV8 = 0;

			if (sV5 > 0)
			{
				wp = (uint16_t *)cp;
				sV9 = *wp;
				cp += 2;
				sV5--;
			}
			else sV9 = 0;

			GrandMagicResult(cTxt, sV1, sV2, sV3, sV4, sV6, sV7, sV8, sV9);
			break;

		case NOTIFY_METEORSTRIKECOMING:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			wp = (uint16_t *)cp;
			sV1 = *wp;
			cp += 2;
			MeteorStrikeComing(sV1);
			PlaySound('E', 25, 0, 0);
			break;

		case NOTIFY_METEORSTRIKEHIT:
			SetTopMsg(CRUSADE_MESSAGE17, 5);
			//StartMeteorStrikeEffect
			//for( i=0 ; i<36 ; i++ ) bAddNewEffect(60, m_sViewPointX +(rand() % 640), m_sViewPointY +(rand() % 480), NULL, NULL, -(rand() % 80));
			for (i = 0; i < 36; i++) bAddNewEffect(60, m_sViewPointX + (rand() % GetWidth()), m_sViewPointY + (rand() % GetHeight()), 0, 0, -(rand() % 80)); // 800x600 Resolution xRisenx
			break;

		case NOTIFY_MAPSTATUSNEXT:
			AddMapStatusInfo(pData, false);
			break;

		case NOTIFY_MAPSTATUSLAST:
			AddMapStatusInfo(pData, true);
			break;

		case NOTIFY_LOCKEDMAP:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;

			ZeroMemory(cTemp, sizeof(cTemp));
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 10);
			cp += 10;

			GetOfficialMapName(cTxt, cTemp);
			fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER3, sV1, cTemp);
			SetTopMsg(G_cTxt, 10);
			PlaySound('E', 25, 0, 0);
			break;

		case NOTIFY_CRUSADE: // Crusade msg
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			iV1 = *ip; // Crusademode
			cp += 4;
			ip = (int *)cp;
			iV2 = *ip; // crusade duty
			cp += 4;
			ip = (int *)cp;
			iV3 = *ip;
			cp += 4;
			ip = (int *)cp;
			iV4 = *ip;
			cp += 4;
			if (!m_bIsCrusadeMode)
			{
				if (iV1 != 0) // begin crusade
				{
					m_bIsCrusadeMode = true;
					m_iCrusadeDuty = iV2;

					if ((m_iCrusadeDuty != 3) && m_side == NEUTRAL)
						_RequestMapStatus("2ndmiddle", 3);

					if (m_iCrusadeDuty != 0)
						EnableDialogBox(33, 2, iV2);
					else
						EnableDialogBox(33, 1);

					if (m_side == NEUTRAL) EnableDialogBox(18, 800, 0, 0);
					else if (m_side == ARESDEN) EnableDialogBox(18, 801, 0, 0);
					else if (m_side == ELVINE) EnableDialogBox(18, 802, 0, 0);
					if (m_side == NEUTRAL) SetTopMsg(NOTIFY_MSG_CRUSADESTART_NONE, 10);
					else SetTopMsg(CRUSADE_MESSAGE9, 10);
					PlaySound('E', 25, 0, 0);
				}
				if (iV3 != 0) // Crusade finished, show XP result screen
				{
					CrusadeContributionResult(iV3);
				}
				if (iV4 == -1) // The crusade you played in was finished.
				{
					CrusadeContributionResult(0); // You connect in this crusade, but did not connect after previous one => no XP....
				}
			}
			else
			{
				if (iV1 == 0) // crusade finished show result (1st result: winner)
				{
					m_bIsCrusadeMode = false;
					m_iCrusadeDuty = 0;
					CrusadeWarResult(iV4);
					SetTopMsg(CRUSADE_MESSAGE57, 8);
				}
				else
				{
					if (iV2 == -1)
					{
						AddEventList(NOTIFY_MSG_HANDLER89, CHAT_GM);
						return;
					}
					else if (m_iCrusadeDuty != iV2)
					{
						m_iCrusadeDuty = iV2;
						EnableDialogBox(33, 2, iV2, 0);
						PlaySound('E', 25, 0, 0);
					}
				}
				if (iV4 == -1)
				{
					CrusadeContributionResult(0); // You connect in this crusade, but did not connect after previous one => no XP....
				}
			}
			break;

		case NOTIFY_SPECIALABILITYSTATUS:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV3 = *sp;
			cp += 2;
			if (sV1 == 1) // Use SA
			{
				PlaySound('E', 35, 0);
				AddEventList(NOTIFY_MSG_HANDLER4, 10); // "Use special ability!"
				switch (sV2)
				{
					case 1: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER5, sV3); break;//"You are untouchable for %d seconds!"
					case 2: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER6, sV3); break;//"
					case 3: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER7, sV3); break;//"
					case 4: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER8, sV3); break;//"
					case 5: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER9, sV3); break;//"
					case 50:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER10, sV3); break;//"
					case 51:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER11, sV3); break;//"
					case 52:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER12, sV3); break;//"
					case 55: // Spell effect
						if (sV3 > 90)
							fmt::format_to(G_cTxt, "You cast a powerful incantation, you can't use it again before %d minutes.", sV3 / 60);
						else
							fmt::format_to(G_cTxt, "You cast a powerful incantation, you can't use it again before %d seconds.", sV3);
						break;
				}
				AddEventList(G_cTxt, 10);
			}
			else if (sV1 == 2) // Finished using
			{
				if (m_iSpecialAbilityType != (int)sV2)
				{
					PlaySound('E', 34, 0);
					AddEventList(NOTIFY_MSG_HANDLER13, 10);//"Special ability has been set!"
					if (sV3 >= 60)
					{
						switch (sV2)
						{
							case 1: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER14, sV3 / 60); AddEventList(G_cTxt, 10); break;//"Ability that decreases enemy's HP by 50%: Can use after %dMin"
							case 2: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER15, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 3: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER16, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 4: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER17, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 5: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER18, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 50:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER19, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 51:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER20, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
							case 52:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER21, sV3 / 60); AddEventList(G_cTxt, 10); break;//"
						}
					}
					else
					{
						switch (sV2)
						{
							case 1: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER22, sV3); AddEventList(G_cTxt, 10); break;//"
							case 2: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER23, sV3); AddEventList(G_cTxt, 10); break;//"
							case 3: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER24, sV3); AddEventList(G_cTxt, 10); break;//"
							case 4: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER25, sV3); AddEventList(G_cTxt, 10); break;//"
							case 5: fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER26, sV3); AddEventList(G_cTxt, 10); break;//"
							case 50:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER27, sV3); AddEventList(G_cTxt, 10); break;//"
							case 51:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER28, sV3); AddEventList(G_cTxt, 10); break;//"
							case 52:fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER29, sV3); AddEventList(G_cTxt, 10); break;//""Ability that makes character untouchable: Can use after %dSec"
						}
					}
				}
				m_iSpecialAbilityType = (int)sV2;
				m_dwSpecialAbilitySettingTime = dwTime;
				m_iSpecialAbilityTimeLeftSec = (int)sV3;
			}
			else if (sV1 == 3)  // End of using time
			{
				m_bIsSpecialAbilityEnabled = false;
				m_dwSpecialAbilitySettingTime = dwTime;
				if (sV3 == 0)
				{
					m_iSpecialAbilityTimeLeftSec = 1200;
					AddEventList(NOTIFY_MSG_HANDLER30, 10);//"Special ability has run out! Will be available in 20 minutes."
				}
				else
				{
					m_iSpecialAbilityTimeLeftSec = (int)sV3;
					if (sV3 > 90)
						fmt::format_to(G_cTxt, "Special ability has run out! Will be available in %d minutes.", sV3 / 60);
					else fmt::format_to(G_cTxt, "Special ability has run out! Will be available in %d seconds.", sV3);
					AddEventList(G_cTxt, 10);
				}
			}
			else if (sV1 == 4) // Unequiped the SA item
			{
				AddEventList(NOTIFY_MSG_HANDLER31, 10);//"Special ability has been released."
				m_iSpecialAbilityType = 0;
			}
			else if (sV1 == 5) // Angel
			{
				PlaySound('E', 52, 0); // Angel
			}
			break;

		case NOTIFY_SPECIALABILITYENABLED:
			if (m_bIsSpecialAbilityEnabled == false)
			{
				PlaySound('E', 30, 5);
				AddEventList(NOTIFY_MSG_HANDLER32, 10);//"
			}
			m_bIsSpecialAbilityEnabled = true;
			break;

		case NOTIFY_ENERGYSPHEREGOALIN:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV3 = *sp;
			cp += 2;
			ZeroMemory(cTxt, sizeof(cTxt));
			memcpy(cTxt, cp, 20);

			if (sV2 == sV3)
			{
				PlaySound('E', 24, 0);
				if (cTxt == player_name)
				{
					AddEventList(NOTIFY_MSG_HANDLER33, 10);//You pushed energy sphere to enemy's energy portal! Contribution point will be decreased by 10 points."
					m_iContribution += sV1; // fixed, server must match...
					m_iContributionPrice = 0;
					if (m_iContribution < 0) m_iContribution = 0;
				}
				else
				{
					ZeroMemory(G_cTxt, sizeof(G_cTxt));
					if (m_side == ARESDEN) fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER34, cTxt);//"%s(Aresden) pushed energy sphere to enemy's portal!!..."
					else if (m_side == ELVINE) fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER34_ELV, cTxt);//"%s(Elvine) pushed energy sphere to enemy's portal!!..."
					AddEventList(G_cTxt, 10);
				}
			}
			else
			{
				PlaySound('E', 23, 0);
				if (cTxt == player_name)
				{
					switch (m_sPlayerType)
					{
						case 1:
						case 2:
						case 3:	PlaySound('C', 21, 0); break;
						case 4:
						case 5:
						case 6:	PlaySound('C', 22, 0); break;
					}
					AddEventList(NOTIFY_MSG_HANDLER35, 10);//"Congulaturations! You brought energy sphere to energy portal and earned experience and prize gold!"

					m_iContribution += 5;
					if (m_iContribution < 0) m_iContribution = 0;
				}
				else
				{
					ZeroMemory(G_cTxt, sizeof(G_cTxt));
					if (sV3 == 1)
					{
						fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER36, cTxt);//"Elvine %s : Goal in!"
						AddEventList(G_cTxt, 10);
					}
					else if (sV3 == 2)
					{
						fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER37, cTxt);//"Aresden %s : Goal in!"
						AddEventList(G_cTxt, 10);
					}
				}
			}
			break;

		case NOTIFY_ENERGYSPHERECREATED:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, cp, 20);
			cp += 20;

			ZeroMemory(G_cTxt, sizeof(G_cTxt));
			fmt::format_to(G_cTxt, NOTIFY_MSG_HANDLER38, sV1, sV2, cTempName);
			AddEventList(G_cTxt, 10);
			//AddEventList(NOTIFY_MSG_HANDLER39, 10);
			break;

		case NOTIFY_QUERY_JOINPARTY:
			ZeroMemory(m_dialogBoxes[32].cStr, sizeof(m_dialogBoxes[32].cStr));
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			strcpy(m_dialogBoxes[32].cStr, cp);

			if (m_partyAutoAccept)
			{
				bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 1, 0, 0, m_dialogBoxes[32].cStr);
			}
			else
			{
				EnableDialogBox(32, 0, 0, 0);
				m_dialogBoxes[32].SetMode(1);
			}
			break;

		case NOTIFY_DAMAGEMOVE:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			m_sDamageMove = *sp;
			cp += 2;
#ifdef RaiseCriticalLimit
			ip = (int *)cp;
			m_iDamageMoveAmount = *ip;
			cp += 2; // Maybe change back to 2 xRisenx
#else
			sp = (short *)cp;
			m_sDamageMoveAmount = *sp;
			cp += 2;
#endif
			break;

		case NOTIFY_OBSERVERMODE:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			if (*sp == 1)
			{
				AddEventList(NOTIFY_MSG_HANDLER40);//"Observer Mode On. Press 'SHIFT + ESC' to Log Out..."
				m_bIsObserverMode = true;
				m_dwObserverCamTime = unixtime();
				char cName[12];
				ZeroMemory(cName, sizeof(cName));
				memcpy(cName, player_name.c_str(), 10);
				m_pMapData->bSetOwner(m_sPlayerObjectID, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cName, 0, 0, 0, 0);
			}
			else
			{
				AddEventList(NOTIFY_MSG_HANDLER41);//"Observer Mode Off"
				m_bIsObserverMode = false;
				m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir, m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor, m_sPlayerHeadApprValue, m_sPlayerBodyApprValue, m_sPlayerArmApprValue, m_sPlayerLegApprValue, m_iPlayerStatus, player_name, OBJECTSTOP, 0, 0, 0); // Re-Coding Sprite xRisenx
			}
			break;

		case NOTIFY_BUILDITEMSUCCESS:
			DisableDialogBox(26);
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			sV1 = *sp;
			cp += 2;
			sp = (short *)cp;
			sV2 = *sp;
			cp += 2;
			if (sV1 < 10000)
			{
				EnableDialogBox(26, 6, 1, sV1, 0);
				m_dialogBoxes[26].sV1 = sV2;
			}
			else
			{
				EnableDialogBox(26, 6, 1, -1 * (sV1 - 10000), 0);
				m_dialogBoxes[26].sV1 = sV2;
			}
			AddEventList(NOTIFY_MSG_HANDLER42, 10);
			PlaySound('E', 23, 5);
			switch (m_sPlayerType)
			{
				case 1:
				case 2:
				case 3:
					PlaySound('C', 21, 0);
					break;

				case 4:
				case 5:
				case 6:
					PlaySound('C', 22, 0);
					break;
			}
			break;

		case NOTIFY_BUILDITEMFAIL:
			DisableDialogBox(26);
			EnableDialogBox(26, 6, 0, 0);
			AddEventList(NOTIFY_MSG_HANDLER43, 10);
			PlaySound('E', 24, 5);
			break;

		case NOTIFY_QUESTREWARD:
			NotifyMsg_QuestReward(pData);
			break;

		case NOTIFY_QUESTCOMPLETED:
			m_stQuest.bIsQuestCompleted = true;
			DisableDialogBox(28);
			EnableDialogBox(28, 1, 0, 0);
			switch (m_sPlayerType)
			{
				case 1:
				case 2:
				case 3:	PlaySound('C', 21, 0); break;
				case 4:
				case 5:
				case 6:	PlaySound('C', 22, 0); break;
			}
			PlaySound('E', 23, 0);
			AddEventList(NOTIFY_MSG_HANDLER44, 10);
			break;

		case NOTIFY_QUESTABORTED:
			m_stQuest.sQuestType = 0;
			DisableDialogBox(28);
			EnableDialogBox(28, 2, 0, 0);
			break;

		case NOTIFY_QUESTCONTENTS:
			NotifyMsg_QuestContents(pData);
			break;

		case NOTIFY_ITEMCOLORCHANGE:
			NotifyMsg_ItemColorChange(pData);
			break;

		case NOTIFY_ITEMSOCKETCHANGE:
			NotifyMsg_ItemSocketChange(pData);
			break;

		case NOTIFY_DROPITEMFIN_COUNTCHANGED:
			NotifyMsg_DropItemFin_CountChanged(pData);
			break;

		case NOTIFY_CANNOTGIVEITEM:
			NotifyMsg_CannotGiveItem(pData);
			break;

		case NOTIFY_GIVEITEMFIN_COUNTCHANGED:
			NotifyMsg_GiveItemFin_CountChanged(pData);
			break;

		case NOTIFY_EXCHANGEITEMCOMPLETE:
			AddEventList(NOTIFYMSG_EXCHANGEITEM_COMPLETE1, 10);
			DisableDialogBox(27);
			//Snoopy: MultiTrade
			DisableDialogBox(41);
			PlaySound('E', 23, 5);
			break;

		case NOTIFY_CANCELEXCHANGEITEM:
			PlaySound('E', 24, 5);
			AddEventList(NOTIFYMSG_CANCEL_EXCHANGEITEM1, 10);
			AddEventList(NOTIFYMSG_CANCEL_EXCHANGEITEM2, 10);
			//Snoopy: MultiTrade
			DisableDialogBox(41);
			DisableDialogBox(27);
			break;

		case NOTIFY_SETEXCHANGEITEM:
			NotifyMsg_SetExchangeItem(pData);
			break;

		case NOTIFY_OPENEXCHANGEWINDOW:
			NotifyMsg_OpenExchageWindow(pData);
			break;

		case NOTIFY_NOTFLAGSPOT:
			AddEventList(NOTIFY_MSG_HANDLER45, 10);
			break;

		case NOTIFY_ITEMPOSLIST:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			for (i = 0; i < MAXITEMS; i++)
			{
				sp = (short *)cp;
				sX = *sp;
				cp += 2;
				sp = (short *)cp;
				sY = *sp;
				cp += 2;
				if (m_pItemList[i] != 0)
				{
					if (sY < -10) sY = -10;
					if (sX < 0)   sX = 0;
					if (sX > 170) sX = 170;
					if (sY > 95)  sY = 95;

					m_pItemList[i]->m_sX = sX;
					m_pItemList[i]->m_sY = sY;
				}
			}
			break;

		case NOTIFY_ENEMYKILLS:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			m_iEnemyKillCount = *ip;
			ip++;
			m_iEnemyKillTotalCount = *ip;
			break;

		case NOTIFY_DOWNSKILLINDEXSET:
			NotifyMsg_DownSkillIndexSet(pData);
			break;

		case NOTIFY_ADMINIFO:
			NotifyMsg_AdminInfo(pData);
			break;

		case NOTIFY_NPCTALK:
			NpcTalkHandler(pData);
			break;

		case NOTIFY_PORTIONSUCCESS:
			AddEventList(NOTIFY_MSG_HANDLER46, 10);
			break;

		case NOTIFY_PORTIONFAIL:
			AddEventList(NOTIFY_MSG_HANDLER47, 10);
			break;

		case NOTIFY_LOWPORTIONSKILL:
			AddEventList(NOTIFY_MSG_HANDLER48, 10);
			break;

		case NOTIFY_NOMATCHINGPORTION:
			AddEventList(NOTIFY_MSG_HANDLER49, 10);
			break;

		case NOTIFY_SUPERATTACKLEFT:
			sp = (short *)(pData + INDEX2_MSGTYPE + 2);
			m_iSuperAttackLeft = (int)*sp;
			break;

		case NOTIFY_SAFEATTACKMODE:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			switch (*cp)
			{
				case 1:
					if (!m_bIsSafeAttackMode) AddEventList(NOTIFY_MSG_HANDLER50, 10);//"
					m_bIsSafeAttackMode = true;
					break;
				case 0:
					if (m_bIsSafeAttackMode) AddEventList(NOTIFY_MSG_HANDLER51, 10);//"
					m_bIsSafeAttackMode = false;
					break;
			}
			break;

		case NOTIFY_IPACCOUNTINFO:
			ZeroMemory(cTemp, sizeof(cTemp));
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			strcpy(cTemp, cp);
			AddEventList(cTemp);
			break;

		case NOTIFY_REWARDGOLD:
			dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
			m_iRewardGold = *dwp;
			break;

		case NOTIFY_SERVERSHUTDOWN:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			if (m_bIsDialogEnabled[25] == false)
				EnableDialogBox(25, *cp, 0, 0);
			else m_dialogBoxes[25].SetMode(*cp);
			PlaySound('E', 27, 0);
			break;

		case NOTIFY_GLOBALATTACKMODE:
			NotifyMsg_GlobalAttackMode(pData);
			break;

		case NOTIFY_WEATHERCHANGE:
			NotifyMsg_WeatherChange(pData);
			break;

		case NOTIFY_FISHCANCELED:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			wp = (uint16_t *)cp;
			switch (*wp)
			{
				case 0:
					AddEventList(NOTIFY_MSG_HANDLER52, 10);
					DisableDialogBox(24);
					break;

				case 1:
					AddEventList(NOTIFY_MSG_HANDLER53, 10);
					DisableDialogBox(24);
					break;

				case 2:
					AddEventList(NOTIFY_MSG_HANDLER54, 10);
					DisableDialogBox(24);
					break;
			}
			break;

		case NOTIFY_FISHSUCCESS:
			AddEventList(NOTIFY_MSG_HANDLER55, 10);
			PlaySound('E', 23, 5);
			PlaySound('E', 17, 5);
			switch (m_sPlayerType)
			{
				case 1:
				case 2:
				case 3:
					PlaySound('C', 21, 0);
					break;

				case 4:
				case 5:
				case 6:
					PlaySound('C', 22, 0);
					break;
			}
			break;

		case NOTIFY_FISHFAIL:
			AddEventList(NOTIFY_MSG_HANDLER56, 10);
			PlaySound('E', 24, 5);
			break;

		case NOTIFY_FISHCHANCE:
			NotifyMsg_FishChance(pData);
			break;

		case NOTIFY_EVENTFISHMODE:
			NotifyMsg_EventFishMode(pData);
			break;

		case NOTIFY_NOTICEMSG:
			NotifyMsg_NoticeMsg(pData);
			break;

		case NOTIFY_RATINGPLAYER:
			NotifyMsg_RatingPlayer(pData);
			break;

		case NOTIFY_CANNOTRATING:
			NotifyMsg_CannotRating(pData);
			break;

		case NOTIFY_ADMINUSERLEVELLOW:
			AddEventList(NOTIFY_MSG_HANDLER58, 10);
			break;

		case NOTIFY_NOGUILDMASTERLEVEL:
			AddEventList(NOTIFY_MSG_HANDLER59, 10);
			break;
		case NOTIFY_SUCCESSBANGUILDMAN:
			AddEventList(NOTIFY_MSG_HANDLER60, 10);
			break;
		case NOTIFY_CANNOTBANGUILDMAN:
			AddEventList(NOTIFY_MSG_HANDLER61, 10);
			break;

		case NOTIFY_PLAYERSHUTUP:
			NotifyMsg_PlayerShutUp(pData);
			break;

		case NOTIFY_TIMECHANGE:
			NotifyMsg_TimeChange(pData);
			break;

		case NOTIFY_TOBERECALLED:
			AddEventList(NOTIFY_MSG_HANDLER62, 10);
			break;

		case NOTIFY_HUNGER:
			NotifyMsg_Hunger(pData);
			break;

		case NOTIFY_PLAYERPROFILE:
			NotifyMsg_PlayerProfile(pData);
			break;

		case NOTIFY_WHISPERMODEON:
			NotifyMsg_WhisperMode(true, pData);
			break;

		case NOTIFY_WHISPERMODEOFF:
			NotifyMsg_WhisperMode(false, pData);
			break;

		case NOTIFY_PLAYERONGAME:
			NotifyMsg_UnitStatus(true, pData);
			break;

		case NOTIFY_PLAYERNOTONGAME:
			NotifyMsg_UnitStatus(false, pData);
			break;

		case NOTIFY_CHARISMA:
			NotifyMsg_Charisma(pData);
			break;

		case NOTIFY_ITEMSOLD:
			DisableDialogBox(23);
			break;

		case NOTIFY_ITEMREPAIRED:
			DisableDialogBox(23);
			NotifyMsg_ItemRepaired(pData);
			break;

		case NOTIFY_CANNOTREPAIRITEM:
			NotifyMsg_CannotRepairItem(pData);
			break;

		case NOTIFY_CANNOTSELLITEM:
			NotifyMsg_CannotSellItem(pData);
			break;

		case NOTIFY_REPAIRITEMPRICE:
			NotifyMsg_RepairItemPrice(pData);
			break;

		case NOTIFY_SELLITEMPRICE:
			NotifyMsg_SellItemPrice(pData);
			break;

		case NOTIFY_SHOWMAP:
			NotifyMsg_ShowMap(pData);
			break;

		case NOTIFY_SKILLUSINGEND:
			NotifyMsg_SkillUsingEnd(pData);
			break;

		case NOTIFY_TOTALUSERS:
			NotifyMsg_TotalUsers(pData);
			break;

		case NOTIFY_EVENTSTART:
			NotifyMsg_EventStart(pData);
			break;

		case NOTIFY_EVENTSTARTING:
		case NOTIFY_EVENTSTARTING2:
		case NOTIFY_EVENTSTARTING3:
			NotifyMsg_EventStarting(pData);
			break;

		case NOTIFY_CASUALTIES:
			NotifyMsg_Casualties(pData);
			break;

		case NOTIFY_SHIELDHP: // Shield xRisenx
			NotifyMsg_ShieldHP(pData);
			break;

		case NOTIFY_RELICINALTAR:
			NotifyMsg_RelicInAltar(pData);
			break;

		case NOTIFY_RELICGRABBED:
			NotifyMsg_RelicGrabbed(pData);
			break;

		case NOTIFY_EVENTWINNER:
			NotifyMsg_EventWinner(pData);
			break;

		case NOTIFY_RELICPOSITION:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			sp = (short *)cp;
			m_relicX = *sp;
			sp++;
			m_relicY = *sp;
			break;

		case NOTIFY_MAGICEFFECTOFF:
			NotifyMsg_MagicEffectOff(pData);
			break;

		case NOTIFY_MAGICEFFECTON:
			NotifyMsg_MagicEffectOn(pData);
			break;

		case NOTIFY_CANNOTITEMTOBANK:
			AddEventList(NOTIFY_MSG_HANDLER63, 10);
			break;

		case NOTIFY_SERVERCHANGE:
			NotifyMsg_ServerChange(pData);
			break;

		case NOTIFY_SKILL:
			NotifyMsg_Skill(pData);
			break;

		case NOTIFY_SETITEMCOUNT:
			NotifyMsg_SetItemCount(pData);
			break;

		case NOTIFY_ITEMDEPLETED_ERASEITEM:
			NotifyMsg_ItemDepleted_EraseItem(pData);
			break;

		case NOTIFY_DROPITEMFIN_ERASEITEM:
			NotifyMsg_DropItemFin_EraseItem(pData);
			break;

		case NOTIFY_GIVEITEMFIN_ERASEITEM:
			NotifyMsg_GiveItemFin_EraseItem(pData);
			break;

		case NOTIFY_ENEMYKILLREWARD:
			NotifyMsg_EnemyKillReward(pData);
			break;

		case NOTIFY_PKCAPTURED:
			NotifyMsg_PKcaptured(pData);
			break;

		case NOTIFY_PKPENALTY:
			NotifyMsg_PKpenalty(pData);
			break;

		case NOTIFY_ITEMTOBANK:
			NotifyMsg_ItemToBank(pData);
			break;

		case NOTIFY_ITEMTOGUILDBANK:
			NotifyMsg_ItemToGuildBank(pData);
			break;

		case NOTIFY_TRAVELERLIMITEDLEVEL:
			AddEventList(NOTIFY_MSG_HANDLER64, 10);
			break;

		case NOTIFY_LIMITEDLEVEL:
			AddEventList(NOTIFYMSG_LIMITED_LEVEL1, 10);
			break;

		case NOTIFY_ITEMLIFESPANEND:
			NotifyMsg_ItemLifeSpanEnd(pData);
			break;

		case NOTIFY_ITEMRELEASED:
			NotifyMsg_ItemReleased(pData);
			break;

		case NOTIFY_ITEMOBTAINED:
			NotifyMsg_ItemObtained(pData);
			break;

		case NOTIFY_ITEMPURCHASED:
			NotifyMsg_ItemPurchased(pData);
			break;

		case NOTIFY_QUERY_JOINGUILDREQPERMISSION:
			NotifyMsg_QueryJoinGuildPermission(pData);
			break;

		case NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
			NotifyMsg_QueryDismissGuildPermission(pData);
			break;

		case COMMONTYPE_JOINGUILDAPPROVE:
			NotifyMsg_JoinGuildApprove(pData);
			break;

		case COMMONTYPE_JOINGUILDREJECT:
			NotifyMsg_JoinGuildReject(pData);
			break;

		case COMMONTYPE_DISMISSGUILDAPPROVE:
			NotifyMsg_DismissGuildApprove(pData);
			break;

		case COMMONTYPE_DISMISSGUILDREJECT:
			NotifyMsg_DismissGuildReject(pData);
			break;

		case NOTIFY_CANNOTCARRYMOREITEM:
			AddEventList(NOTIFY_MSG_HANDLER65, 10);//"
			AddEventList(NOTIFY_MSG_HANDLER66, 10);//"
			// Bank dialog Box
			m_dialogBoxes[14].SetMode(0);
			//GuildBank
			m_dialogBoxes[56].SetMode(0);
			break;

		case NOTIFY_NOTENOUGHGOLD:
			DisableDialogBox(23);
			AddEventList(NOTIFY_MSG_HANDLER67, 10);//"Gold
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			if (*cp >= 0)
			{
				m_bIsItemDisabled[*cp] = false;
			}
			break;

		case NOTIFY_HP:
			NotifyMsg_HP(pData);
			break;
		case NOTIFY_MP:
			NotifyMsg_MP(pData);
			break;
		case NOTIFY_SP:
			NotifyMsg_SP(pData);
			break;
		case NOTIFY_KILLED:
			NotifyMsg_Killed(pData);
			break;
		case NOTIFY_EXP:
			NotifyMsg_Exp(pData);
			break;
		case NOTIFY_GUILDDISBANDED:
			NotifyMsg_GuildDisbanded(pData);
			break;
		case NOTIFY_CANNOTJOINMOREGUILDSMAN:
			NotifyMsg_CannotJoinMoreGuildsMan(pData);
			break;
		case NOTIFY_NEWGUILDSMAN:
			NotifyMsg_NewGuildsMan(pData);
			break;
		case NOTIFY_DISMISSGUILDSMAN:
			NotifyMsg_DismissGuildsMan(pData);
			break;
		case NOTIFY_MAGICSTUDYSUCCESS:
			NotifyMsg_MagicStudySuccess(pData);
			break;
		case NOTIFY_MAGICSTUDYFAIL:
			NotifyMsg_MagicStudyFail(pData);
			break;
		case NOTIFY_SKILLTRAINSUCCESS:
			NotifyMsg_SkillTrainSuccess(pData);
			break;
		case NOTIFY_SKILLTRAINFAIL:
			break;
		case NOTIFY_FORCEDISCONN:
			NotifyMsg_ForceDisconn(pData);
			break;
		case NOTIFY_FRIENDONGAME:
			NotifyMsg_FriendOnGame(pData);
			break;
		case NOTIFY_CANNOTRECALL:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, (uint8_t &)sV1);
			switch (sV1)
			{
				case 0:
					AddEventList(NOTIFY_MSG_HANDLER74, 10); //"You can't recall within 10 seconds of taking damage."
					break;
				case 1:
					AddEventList(NOTIFY_MSG_HANDLER74B, 10); //"You cannot recall in this map."
					break;
			}
			break;

		case NOTIFY_CANNOTCRUSADE:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, (uint8_t &)sV1);
			switch (sV1)
			{
				case 0:
					AddEventList(NOTIFY_MSG_HANDLER77, 10);
					break;
				case 1:
					AddEventList(NOTIFY_MSG_HANDLER78, 10);
					break;
				case 2:
					AddEventList(NOTIFY_MSG_HANDLER79, 10);
					break;
			}
			break;
		case NOTIFY_FIGHTZONERESERVE:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			ip = (int *)cp;
			switch (*ip)
			{
				case -5:
					AddEventList(NOTIFY_MSG_HANDLER68, 10);
					break;
				case -4:
					AddEventList(NOTIFY_MSG_HANDLER69, 10);
					break;
				case -3:
					AddEventList(NOTIFY_MSG_HANDLER70, 10);
					break;
				case -2:
					m_iFightzoneNumber = 0;
					AddEventList(NOTIFY_MSG_HANDLER71, 10);
					break;
				case -1:
					m_iFightzoneNumber = m_iFightzoneNumber * -1;
					AddEventList(NOTIFY_MSG_HANDLER72, 10);
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					fmt::format_to(cTxt, NOTIFY_MSG_HANDLER73, *ip);//"
					AddEventList(cTxt, 10);
					break;
			}
			break;

#ifdef TitleClient
		case NOTIFY_CHANGETITLE:
			char cCharName[11];//TODO TITLES: change this to 11 in my code...?
			ZeroMemory(cCharName, sizeof(cCharName));

			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			memcpy(cCharName, cp, 11);
			cp += 11;

			ClearPlayerTitle(cCharName);

			break;

		case NOTIFY_ADDTITLE:
			NotifyMsg_AddTitle(pData);
			break;

		case NOTIFY_TITLELEVELUP:
			NotifyMsg_TitleLevelUp(pData);
			break;

		case NOTIFY_REQTITLEANSWER:
		{
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);

			ip = (int *)cp;
			int iReturnIndex = *ip;
			cp += 4;

			ip = (int *)cp;
			int iNumPoints = *ip;
			cp += 4;

			ip = (int *)cp;
			int iRank = *ip;
			cp += 4;

			ip = (int *)cp;
			int iCrusadeJob = *ip;
			cp += 4;

			//dwp  = (DWORD *)cp;
			//DWORD dwV1 = *dwp;//iCurrentNo
			//cp += 4;

			//ip  = (int *)cp;
			//sV2 = *ip;//iReturnIndex (for m_stTitles[..])
			//cp += 4;

			//ip  = (int *)cp;
			//sV3 = *ip;//iCrusadeT
			//cp += 4;

			ZeroMemory(cTemp, sizeof(cTemp));
			memcpy(cTemp, cp, 21);
			cp += 21;

			//ZeroMemory( m_stTitles[sV2].cSubject, sizeof(m_stTitles[sV2].cSubject) );
			//strcpy(m_stTitles[sV2].cSubject, cTemp);

			//m_stTitles[sV2].iCurrentNo = dwV1;
			//m_stTitles[sV2].iCrusadeT = sV3;
			ZeroMemory(m_stTitles[iReturnIndex].cSubject, sizeof(m_stTitles[iReturnIndex].cSubject));
			strcpy(m_stTitles[iReturnIndex].cSubject, cTemp);

			m_stTitles[iReturnIndex].iCurrentNo = iNumPoints;
			m_stTitles[iReturnIndex].iCrusadeT = iCrusadeJob;
			m_stTitles[iReturnIndex].iRank = iRank;
			break;
		}
#endif

		case NOTIFY_REPAIRALLPRICE:
			NotifyMsg_RepairAllPrice(pData);
			break;

		case NOTIFY_MAILRECEIVED:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, str);

			fmt::format_to(cTxt, NOTIFY_MSG_HANDLER82, str.c_str());
			AddEventList(cTxt, 10);

			PlaySound('E', 54, 5);

			if (m_bIsDialogEnabled[DIALOG_MAILBOX])
			{
				bSendCommand(MSGID_REQ_MAILBOX);
			}
			break;

		case NOTIFY_GUILDBOARDPOST:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, str);

			if (str == player_name)
			{
				fmt::format_to(cTxt, NOTIFY_MSG_HANDLER86, str.c_str());
				m_dialogBoxes[DIALOG_GUILD].SetMode(8);
			}
			else
			{
				fmt::format_to(cTxt, NOTIFY_MSG_HANDLER85, str.c_str());
				PlaySound('E', 54, 5);
			}

			AddEventList(cTxt, 10);

			if (m_bIsDialogEnabled[DIALOG_GUILD])
			{
				bSendCommand(MSGID_REQ_GUILDBOARD);
			}
			break;

		case NOTIFY_ADMININFO:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, m_adminLevel);
			Pop(cp, m_GMFlags);
			break;

		case NOTIFY_DONATEEVENTINFO:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			Pop(cp, m_eventPot);
			break;

		case NOTIFY_PINGMAP:
			cp = (char *)(pData + INDEX2_MSGTYPE + 2);
			uint8_t type;
			uint16_t x, y;

			Pop(cp, type);
			Pop(cp, x);
			Pop(cp, y);

			AddMapPing(MapPing(x, y));
			break;
			// Gladiator Arena xRisenx
		//case NOTIFY_ARENASTATUS:
		//	cp = (char *)(pData   + INDEX2_MSGTYPE + 2);
		//	   ip  = (int *)cp;
		//	   sV1 = *ip;
		//	   cp += 4;

		//	   ip  = (int *)cp;
		//	   sV2 = *ip;
		//	   cp += 4;

		//	   ip  = (int *)cp;
		//	   sV3 = *ip;
		//	   cp += 4;

		//	   ZeroMemory(cTemp, sizeof(cTemp) );
		//	   memcpy(cTemp, cp, 21);
		//	   cp += 21;

		//	   if(sV3 > 0)
		//	   {
		//		  bool bFound = false;

		//		  for (i = 0; i < MAXARENAPLAYERS; i++)
		//		  {
		//			 if(bFound) break;
		//			 if(strcmp(m_stArenaPlayers[i].cCharName, cTemp) == 0)
		//			 {
		//				m_stArenaPlayers[i].iKills = sV1;
		//				m_stArenaPlayers[i].iDeaths = sV2;
		//				bFound = true;
		//			 }
		//		  }
		//		  if(bFound == false)
		//		  {
		//			 for (i = 0; i < MAXARENAPLAYERS; i++)
		//			 {
		//				if(strlen(m_stArenaPlayers[i].cCharName) == 0)
		//				{
		//				   m_stArenaPlayers[i].iKills = sV1;
		//				   m_stArenaPlayers[i].iDeaths = sV2;
		//				   ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName) );
		//				   strcpy(m_stArenaPlayers[i].cCharName, cTemp);
		//				   break;
		//				}
		//			 }
		//		  }
		//	   }
		//	   else // player DC/Logout/Recall, remove from list
		//	   {
		//		  for (i = 0; i < MAXARENAPLAYERS; i++)
		//		  {
		//			 if(strcmp(m_stArenaPlayers[i].cCharName, cTemp) == 0)
		//			 {
		//				m_stArenaPlayers[i].iKills = 0;
		//				m_stArenaPlayers[i].iDeaths = 0;
		//				ZeroMemory(m_stArenaPlayers[i].cCharName, sizeof(m_stArenaPlayers[i].cCharName));
		//			 }
		//		  }
		//	   }
			// Gladiator Arena xRisenx

		case NOTIFY_NPCBAR:
			NotifyMsg_NpcBar(pData);
			break;

		case NOTIFY_NPCBAR2:
			NotifyMsg_NpcBar2(pData);
			break;


			// Monster kill event xRisenx
		case NOTIFY_NPCHUNTON:
			NotifyMsg_NpcHuntingON();
			break;
		case NOTIFY_NPCHUNTOFF:
			NotifyMsg_NpcHuntingOFF();
			break;
		case NOTIFY_NPCHUNTWINNER:
			AddEventList("You Gained 50 Reputation Points", 10);
			AddEventList("You Gained 50 Contribution Points", 10);
			NotifyMsg_NpcHuntingWinner(pData);
			break;
			// Monster kill event xRisenx
	}
}

void helbreath::NotifyMsg_Heldenian(char * pData)
{
    uint16_t * wp;
    wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
    m_iHeldenianAresdenLeftTower = *wp;
    wp++;
    m_iHeldenianAresdenFlags = *wp;
    wp++;
    m_iHeldenianAresdenKill = *wp;
    wp++;
    m_iHeldenianAresdenDead = *wp;
    wp++;
    m_iHeldenianElvineLeftTower = *wp;
    wp++;
    m_iHeldenianElvineFlags = *wp;
    wp++;
    m_iHeldenianElvineKill = *wp;
    wp++;
    m_iHeldenianElvineDead = *wp;
}

void helbreath::NotifyMsg_CannotGiveItem(char * pData)
{
    char * cp, cName[21], cTxt[256];
    uint16_t * wp, wItemIndex;
    int * ip, iAmount;

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    wp = (uint16_t *)cp;
    wItemIndex = *wp;
    cp += 2;

    ip = (int *)cp;
    iAmount = *ip;
    cp += 4;

    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;

    char cStr1[64], cStr2[64], cStr3[64];
    GetItemName(m_pItemList[wItemIndex], cStr1, cStr2, cStr3);
    if (iAmount == 1) fmt::format_to(cTxt, NOTIFYMSG_CANNOT_GIVE_ITEM2, cStr1, cName);
    else fmt::format_to(cTxt, NOTIFYMSG_CANNOT_GIVE_ITEM1, iAmount, cStr1, cName);


    AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_DropItemFin_CountChanged(char * pData)
{
    char * cp, cTxt[256];
    uint16_t * wp, wItemIndex;
    int * ip, iAmount;

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    wp = (uint16_t *)cp;
    wItemIndex = *wp;
    cp += 2;

    ip = (int *)cp;
    iAmount = *ip;
    cp += 4;

    char cStr1[64], cStr2[64], cStr3[64];
    GetItemName(m_pItemList[wItemIndex]->m_cName, m_pItemList[wItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);
    fmt::format_to(cTxt, NOTIFYMSG_THROW_ITEM1, iAmount, cStr1);

    AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_CannotJoinMoreGuildsMan(char * pData)
{
	char * cp, cName[12], cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	fmt::format_to(cTxt, NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN1, cName);
	AddEventList(cTxt, 10);
	AddEventList(NOTIFYMSG_CANNOT_JOIN_MOREGUILDMAN2, 10);
}

void helbreath::NotifyMsg_DismissGuildsMan(char * pData)
{
	char * cp, cName[12], cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	if (player_name == cName)
	{
		fmt::format_to(cTxt, NOTIFYMSG_DISMISS_GUILDMAN1, cName);
		AddEventList(cTxt, 10);
	}
	ClearGuildNameList();
}

void helbreath::NotifyMsg_CannotRating(char * pData)
{
	char * cp, cTxt[120];
	uint16_t * wp, wTime;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wTime = *wp;
	cp += 2;

	if (wTime == 0) fmt::format_to(cTxt, NOTIFYMSG_CANNOT_RATING1, wTime * 3);
	else fmt::format_to(cTxt, NOTIFYMSG_CANNOT_RATING2, wTime * 3);
	AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_CannotRepairItem(char * pData)
{
	char * cp, cTxt[120], cStr1[64], cStr2[64], cStr3[64];
	uint16_t * wp, wV1, wV2;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wV1 = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
	wV2 = *wp;
	cp += 2;
	ZeroMemory(cStr1, sizeof(cStr1));
	ZeroMemory(cStr2, sizeof(cStr2));
	ZeroMemory(cStr3, sizeof(cStr3));
	GetItemName(m_pItemList[wV1], cStr1, cStr2, cStr3);

	switch (wV2)
	{
		case 1:
			fmt::format_to(cTxt, NOTIFYMSG_CANNOT_REPAIR_ITEM1, cStr1);
			AddEventList(cTxt, 10);
			break;
		case 2:
			fmt::format_to(cTxt, NOTIFYMSG_CANNOT_REPAIR_ITEM2, cStr1);
			AddEventList(cTxt, 10);
			break;
	}
	m_bIsItemDisabled[wV1] = false;
}

void helbreath::NotifyMsg_CannotSellItem(char * pData)
{
	char * cp, cTxt[120], cStr1[64], cStr2[64], cStr3[64];
	uint16_t * wp, wV1, wV2;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	wV1 = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wV2 = *wp;
	cp += 2;

	ZeroMemory(cStr1, sizeof(cStr1));
	ZeroMemory(cStr2, sizeof(cStr2));
	ZeroMemory(cStr3, sizeof(cStr3));
	GetItemName(m_pItemList[wV1], cStr1, cStr2, cStr3);

	switch (wV2)
	{
		case 1:
			fmt::format_to(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM1, cStr1);//"
			AddEventList(cTxt, 10);
			break;

		case 2:
			fmt::format_to(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM2, cStr1);//"
			AddEventList(cTxt, 10);
			break;

		case 3:
			fmt::format_to(cTxt, NOTIFYMSG_CANNOT_SELL_ITEM3, cStr1);//"
			AddEventList(cTxt, 10);
			AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM4, 10);//"
			break;

		case 4:
			AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM5, 10); // "
			AddEventList(NOTIFYMSG_CANNOT_SELL_ITEM6, 10); // "
			break;
	}
	m_bIsItemDisabled[wV1] = false;
}

void helbreath::NotifyMsg_Charisma(char * pData)
{
	uint32_t * dwp;
	int  iPrevChar;
	char cTxt[120];

	iPrevChar = m_stat[STAT_CHR];
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_stat[STAT_CHR] = (int)*dwp;

	if (m_stat[STAT_CHR] > iPrevChar)
	{
		fmt::format_to(cTxt, NOTIFYMSG_CHARISMA_UP, m_stat[STAT_CHR] - iPrevChar);//"
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}
	else
	{
		fmt::format_to(cTxt, NOTIFYMSG_CHARISMA_DOWN, iPrevChar - m_stat[STAT_CHR]);//"
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_DropItemFin_EraseItem(char * pData)
{
	char * cp;
	uint16_t * wp;
	int * ip, iAmount;
	short  sItemIndex;
	char   cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);

	ZeroMemory(cTxt, sizeof(cTxt));
	if (m_bIsItemEquipped[sItemIndex] == true)
	{
		fmt::format_to(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);
		m_sItemEquipmentStatus[m_pItemList[sItemIndex]->m_cEquipPos] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}
	if (m_iHP > 0)
	{
		fmt::format_to(cTxt, NOTIFYMSG_THROW_ITEM2, cStr1);
	}
	else
	{
		if (iAmount < 2)
			fmt::format_to(cTxt, NOTIFYMSG_DROPITEMFIN_ERASEITEM3, cStr1); // "You dropped a %s."
		else // Snoopy fix
		{
			fmt::format_to(cTxt, NOTIFYMSG_DROPITEMFIN_ERASEITEM5, cStr1); // "You dropped %s."
		}
	}
	AddEventList(cTxt, 10);
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}

void helbreath::NotifyMsg_EnemyKillReward(char * pData)
{
	uint32_t * dwp;
	short * sp, sGuildRank;
	char * cp, cName[12], cGuildName[24], cTxt[120];
	int   iExp, iEnemyKillCount, iWarContribution;

	ZeroMemory(cName, sizeof(cName));
	ZeroMemory(cGuildName, sizeof(cGuildName));

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iEnemyKillCount = *dwp;
	cp += 4;
	memcpy(cName, cp, 10);
	cp += 10;
	memcpy(cGuildName, cp, 20);
	cp += 20;
	sp = (short *)cp;
	sGuildRank = *sp;
	cp += 2;
	sp = (short *)cp;
	iWarContribution = *sp;
	cp += 2;

	if (iWarContribution > m_iWarContribution + 1)
	{
		fmt::format_to(G_cTxt, "%s +%d!", CRUSADE_MESSAGE21, iWarContribution - m_iWarContribution);
		SetTopMsg(G_cTxt, 5);
	}
	else if (iWarContribution < m_iWarContribution)
	{
	}
	m_iWarContribution = iWarContribution;

	if (sGuildRank == GUILDRANK_NONE)
	{
		fmt::format_to(cTxt, NOTIFYMSG_ENEMYKILL_REWARD1, cName);
		AddEventList(cTxt, 10);
	}
	else
	{
		fmt::format_to(cTxt, NOTIFYMSG_ENEMYKILL_REWARD2, GetGuildRankName(sGuildRank), cName, cGuildName);
		AddEventList(cTxt, 10);
	}

	/*	if( m_iExp != iExp ) // removed by snoopy because too much msg hide victim's name
		{	if (m_iExp > iExp) fmt::format_to(cTxt, EXP_DECREASED,m_iExp - iExp);
			else fmt::format_to(cTxt, EXP_INCREASED,iExp - m_iExp);
			AddEventList(cTxt, 10);
		}*/

	int32_t ekchange = iEnemyKillCount - m_iEnemyKillCount;

	if (m_iEnemyKillCount != iEnemyKillCount)
	{
		if (m_iEnemyKillCount > iEnemyKillCount)
		{
			fmt::format_to(cTxt, NOTIFYMSG_ENEMYKILL_REWARD5, -ekchange);
			AddEventList(cTxt, 10);
		}
		else
		{
			fmt::format_to(cTxt, NOTIFYMSG_ENEMYKILL_REWARD6, ekchange);
			AddEventList(cTxt, 10);
		}
	}

	if (iExp >= 0) m_iExp = iExp;
	if (iEnemyKillCount >= 0)
	{
		m_iEnemyKillCount += ekchange;
		m_iEnemyKillTotalCount += ekchange;
	}
	PlaySound('E', 23, 0);

	if (m_ekScreenshot)
		m_ekSSTime = unixtime() + 650;
}

void helbreath::NotifyMsg_EventFishMode(char * pData)
{
	short sSprite, sSpriteFrame;
	char * cp, cName[21];
	uint16_t * wp, wPrice;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	wPrice = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	sSprite = (short)*wp;
	cp += 2;

	wp = (uint16_t *)cp;
	sSpriteFrame = (short)*wp;
	cp += 2;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	EnableDialogBox(24, 0, 0, wPrice, cName);
	m_dialogBoxes[24].sV3 = sSprite;
	m_dialogBoxes[24].sV4 = sSpriteFrame;

	AddEventList(NOTIFYMSG_EVENTFISHMODE1, 10);
}

void helbreath::NotifyMsg_Exp(char * pData)
{
	uint32_t * dwp;
	int iPrevExp, * ip;
	char * cp, cTxt[120];

	iPrevExp = m_iExp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
	m_iExp = (int)*dwp;
	cp += 4;

	ip = (int *)cp;
	//	m_iRating = *ip;
	cp += 4;

	if (m_iExp > iPrevExp)
	{
		fmt::format_to(cTxt, EXP_INCREASED, m_iExp - iPrevExp);
		AddEventList(cTxt, 10);
	}
	else if (m_iExp < iPrevExp)
	{
		fmt::format_to(cTxt, EXP_DECREASED, iPrevExp - m_iExp);
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_ForceDisconn(char * pData)
{
    uint16_t * wpCount;
    wpCount = (uint16_t *)(pData + 6);
    m_bForceDisconn = true;
    //m_cLogOutCount = (char)*wpCount;
    if (m_bIsProgramActive)
    {
        if (m_cLogOutCount < 0 || m_cLogOutCount > 5)
            m_cLogOutCount = 5;
        AddEventList(NOTIFYMSG_FORCE_DISCONN1, 10);
    }
    else
    {
        socketmode(0);
        close(1000, "forcedisconnect");
        m_bEscPressed = false;
        if (m_bSoundFlag)
            m_pESound[38].stop();
        if ((m_bSoundFlag) && (m_bMusicStat == true))
        {
            m_pBGM.stop();
        }
        isItemLoaded = false;
        ChangeGameMode(GAMEMODE_ONMAINMENU);
    }
}

void helbreath::NotifyMsg_GiveItemFin_CountChanged(char * pData)
{
	char * cp, cName[21], cTxt[256];
	uint16_t * wp, wItemIndex;
	int * ip, iAmount;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[wItemIndex]->m_cName, m_pItemList[wItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);
	if (iAmount == 1) fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED1, cStr1, cName);
	fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_COUNTCHANGED2, iAmount, cStr1, cName);
	AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_GiveItemFin_EraseItem(char * pData)
{
	char * cp;
	uint16_t * wp;
	int * ip, iAmount;
	short  sItemIndex;
	char cName[21], cTxt[250];


	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex]->m_cName, m_pItemList[sItemIndex]->m_dwAttribute, cStr1, cStr2, cStr3);

	if (m_bIsItemEquipped[sItemIndex] == true)
	{
		fmt::format_to(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);

		m_sItemEquipmentStatus[m_pItemList[sItemIndex]->m_cEquipPos] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}
	if (strlen(cName) == 0) fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM2, iAmount, cStr1);
	else
	{
		if (strcmp(cName, "Howard") == 0)
			fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM3, iAmount, cStr1);
		else if (strcmp(cName, "William") == 0)
			fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM4, iAmount, cStr1);
		else if (strcmp(cName, "Kennedy") == 0)
			fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM5, iAmount, cStr1);
		else if (strcmp(cName, "Tom") == 0)
			fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM7, iAmount, cStr1);
		else if (strcmp(cName, "Princess") == 0)
			fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM3, iAmount, cStr1);

		else fmt::format_to(cTxt, NOTIFYMSG_GIVEITEMFIN_ERASEITEM8, iAmount, cStr1, cName);
	}
	AddEventList(cTxt, 10);
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}

void helbreath::NotifyMsg_GlobalAttackMode(char * pData)
{
	char * cp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	switch (*cp)
	{
		case 0:
			AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE1, 10);
			AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE2, 10);
			break;

		case 1:
			AddEventList(NOTIFYMSG_GLOBAL_ATTACK_MODE3, 10);
			break;
	}
	cp++;
}

void helbreath::NotifyMsg_HP(char * pData)
{
	uint32_t * dwp;
	int iPrevHP;
	char cTxt[120];
	int iPrevMP;

	iPrevHP = m_iHP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iHP = (int)*dwp;

	iPrevMP = m_iMP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 6);
	m_iMP = (int)*dwp;

	if (m_iHP > iPrevHP)
	{
		if ((m_iHP - iPrevHP) < 10) return;
		fmt::format_to(cTxt, NOTIFYMSG_HP_UP, m_iHP - iPrevHP);
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}
	else if (iPrevHP > m_iHP)
	{
		if (m_cLogOutCount > 0 && !m_bForceDisconn)
		{
			m_cLogOutCount = -1;
			AddEventList(NOTIFYMSG_HP2, 10);
		}
		m_dwDamagedTime = unixtime();
		if (m_iHP < 20) AddEventList(NOTIFYMSG_HP3, 10);
		if ((iPrevHP - m_iHP) < 10) return;
		fmt::format_to(cTxt, NOTIFYMSG_HP_DOWN, iPrevHP - m_iHP);
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_Hunger(char * pData)
{
	char * cp, cHLv;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cHLv = *cp;
	iHungerStatus = cHLv; // Hunger Bar xRisenx
	if ((iHungerStatus >= 100) || (iHungerStatus < 0)) // Hunger Bar xRisenx
		iHungerStatus = 100; // Hunger Bar xRisenx

	if ((cHLv <= 40) && (cHLv > 30)) AddEventList(NOTIFYMSG_HUNGER1, 10);//"
	if ((cHLv <= 25) && (cHLv > 20)) AddEventList(NOTIFYMSG_HUNGER2, 10);//"
	if ((cHLv <= 20) && (cHLv > 15)) AddEventList(NOTIFYMSG_HUNGER3, 10);//"
	if ((cHLv <= 15) && (cHLv > 10)) AddEventList(NOTIFYMSG_HUNGER4, 10);//"
	if ((cHLv <= 10) && (cHLv >= 0)) AddEventList(NOTIFYMSG_HUNGER5, 10);//"
}

void helbreath::NotifyMsg_NpcBar(char * pData)
{
	char * cp, iHP;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	iHP = *cp;
	iNpcBar_HP = iHP;
}

void helbreath::NotifyMsg_NpcBar2(char * pData)
{
	char * cp, iMP;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	iMP = *cp;
	iNpcBar_MP = iMP;
	bNpcBar = true;
	m_sNpcBar = 5;
}

void helbreath::NotifyMsg_ItemColorChange(char * pData)
{
	short * sp, sItemIndex, sItemColor;
	char * cp;
	char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	sp = (short *)cp;
	sItemColor = (short)*sp;
	cp += 2;

	if (m_pItemList[sItemIndex] != 0)
	{
		char cStr1[64], cStr2[64], cStr3[64];
		GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);
		if (sItemColor != -1)
		{
			m_pItemList[sItemIndex]->m_ItemColor = (char)sItemColor;
			fmt::format_to(cTxt, NOTIFYMSG_ITEMCOLOR_CHANGE1, cStr1);
			AddEventList(cTxt, 10);
		}
		else
		{
			fmt::format_to(cTxt, NOTIFYMSG_ITEMCOLOR_CHANGE2, cStr1);
			AddEventList(cTxt, 10);
		}
	}
}

void helbreath::NotifyMsg_ItemSocketChange(char * pData)
{
	short * sp, sItemIndex;
	char * cp;
	char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	if (!m_pItemList[sItemIndex])
		return;

	for (int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, m_pItemList[sItemIndex]->m_sockets[i]);
	}

	if (m_pItemList[sItemIndex]->m_sockets[0] == SG_VORTEXGEM)
		m_pItemList[sItemIndex]->m_ItemColor = 10;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);

	fmt::format_to(cTxt, NOTIFYMSG_ITESOCKET_CHANGE1, cStr1);
	AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_ItemDepleted_EraseItem(char * pData)
{
	char * cp;
	uint16_t * wp;
	short  sItemIndex;
	bool   bIsUseItemResult;
	char   cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;

	bIsUseItemResult = (bool)*cp;
	cp += 2;

	ZeroMemory(cTxt, sizeof(cTxt));

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);

	if (m_bIsItemEquipped[sItemIndex] == true)
	{
		fmt::format_to(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
		AddEventList(cTxt, 10);

		m_sItemEquipmentStatus[m_pItemList[sItemIndex]->m_cEquipPos] = -1;
		m_bIsItemEquipped[sItemIndex] = false;
	}

	ZeroMemory(cTxt, sizeof(cTxt));

	switch (m_pItemList[sItemIndex]->m_cItemType)
	{
		case ITEMTYPE_CONSUME:
		case ITEMTYPE_ARROW:
			fmt::format_to(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM2, cStr1);
			break;
		case ITEMTYPE_USE_DEPLETE:
			if (bIsUseItemResult)
				fmt::format_to(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM3, cStr1);
			break;
		case ITEMTYPE_EAT:
			if (bIsUseItemResult)
			{
				fmt::format_to(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM4, cStr1);
				if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
					PlaySound('C', 19, 0);
				if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
					PlaySound('C', 20, 0);
			}
			break;
		case ITEMTYPE_USE_DEPLETE_DEST:
			if (bIsUseItemResult)
				fmt::format_to(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM3, cStr1);
			break;
		default:
			if (bIsUseItemResult)
			{
				fmt::format_to(cTxt, NOTIFYMSG_ITEMDEPlETED_ERASEITEM6, cStr1);
				PlaySound('E', 10, 0);
			}
			break;
	}

	AddEventList(cTxt, 10);

	if (bIsUseItemResult == true) 	m_bItemUsingStatus = false;
	EraseItem((char)sItemIndex);
	_bCheckBuildItemStatus();
}

void helbreath::NotifyMsg_ItemLifeSpanEnd(char * pData)
{
	char * cp;
	short * sp, sEquipPos, sItemIndex;
	char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sEquipPos = *sp;
	cp += 2;
	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);
	fmt::format_to(cTxt, NOTIFYMSG_ITEMLIFE_SPANEND1, cStr1);
	AddEventList(cTxt, 10);
	m_sItemEquipmentStatus[m_pItemList[sItemIndex]->m_cEquipPos] = -1;
	m_bIsItemEquipped[sItemIndex] = false;
	m_pItemList[sItemIndex]->m_wCurLifeSpan = 0;

	PlaySound('E', 10, 0);
}

void helbreath::NotifyMsg_ItemObtained(char * pData)
{
	char * cp;
	short * sp;
	uint32_t * dwp;

	uint32_t dwCount, dwAttribute;
	char  cName[21];
	bool  bIsEquipped;
	uint16_t sSprite, sSpriteFrame, sLevelLimit;
	int8_t sSpecialEV2;
	char  cTxt[120];
	uint8_t cGenderLimit, cItemType, cEquipPos;
	uint16_t * wp, wWeight, wCurLifeSpan;
	uint32_t ItemColor;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cName, sizeof(cName));
	Pop(cp, cName, 20);
	Pop(cp, dwCount);
	Pop(cp, cItemType);
	Pop(cp, cEquipPos);
	Pop(cp, (uint8_t &)bIsEquipped);
	Pop(cp, sLevelLimit);
	Pop(cp, cGenderLimit);
	Pop(cp, wCurLifeSpan);
	Pop(cp, wWeight);
	Pop(cp, sSprite);
	Pop(cp, sSpriteFrame);
	Pop(cp, ItemColor);
	Pop(cp, (uint8_t &)sSpecialEV2);
	Pop(cp, dwAttribute);

	uint8_t sockets[MAXITEMSOCKETS];

	for (int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, sockets[i]);
	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);

	ZeroMemory(cTxt, sizeof(cTxt));
	if (dwCount == 1) fmt::format_to(cTxt, NOTIFYMSG_ITEMOBTAINED2, cStr1);
	else fmt::format_to(cTxt, NOTIFYMSG_ITEMOBTAINED1, dwCount, cStr1);

	AddEventList(cTxt, 10);

	PlaySound('E', 20, 0);

	if (cItemType == ITEMTYPE_CONSUME || cItemType == ITEMTYPE_ARROW)
	{
		for (int i = 0; i < MAXITEMS; i++)
			if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
			{
				m_pItemList[i]->m_dwCount += dwCount;
				_iCalcTotalWeight();
				m_bIsItemDisabled[i] = false;
				return;
			}
	}

	short nX = 40, nY = 30;
	for (int i = 0; i < MAXITEMS; i++)
	{
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
		{
			nX = m_pItemList[i]->m_sX;
			nY = m_pItemList[i]->m_sY;
			break;
		}
	}


	for (int i = 0; i < MAXITEMS; i++)
	{
		if (m_pItemList[i])
			continue;

		m_pItemList[i] = new class CItem;
		memcpy(m_pItemList[i]->m_cName, cName, 20);
		m_pItemList[i]->m_dwCount = dwCount;
		m_pItemList[i]->m_sX = nX;
		m_pItemList[i]->m_sY = nY;
		bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, nX, nY, 0, 0);
		m_pItemList[i]->m_cItemType = cItemType;
		m_pItemList[i]->m_cEquipPos = cEquipPos;
		m_bIsItemDisabled[i] = false;

		m_bIsItemEquipped[i] = false;
		m_pItemList[i]->m_sLevelLimit = sLevelLimit;
		m_pItemList[i]->m_cGenderLimit = cGenderLimit;
		m_pItemList[i]->m_wCurLifeSpan = wCurLifeSpan;
		m_pItemList[i]->m_wWeight = wWeight;
		m_pItemList[i]->m_sSprite = sSprite;
		m_pItemList[i]->m_sSpriteFrame = sSpriteFrame;
		m_pItemList[i]->m_ItemColor = ItemColor;
		m_pItemList[i]->m_sItemSpecEffectValue2 = sSpecialEV2;
		m_pItemList[i]->m_dwAttribute = dwAttribute;

		for (int j = 0; j < MAXITEMSOCKETS; j++)
		{
			m_pItemList[i]->m_sockets[j] = sockets[j];
		}

		_bCheckBuildItemStatus();
		_iCalcTotalWeight();

		for (int j = 0; j < MAXITEMS; j++)
		{
			if (m_cItemOrder[j] == -1)
			{
				m_cItemOrder[j] = i;
				return;
			}
		}
		return;
	}
}

void helbreath::NotifyMsg_ItemPurchased(char * pData)
{
	char * cp;
	short * sp;
	uint32_t * dwp;
	uint16_t * wp;
	int i, j;

	uint32_t dwCount;
	char  cName[21], cItemType, cEquipPos, cGenderLimit;
	bool  bIsEquipped;
	short sSprite, sSpriteFrame, sLevelLimit;
	uint16_t  wCost, wWeight, wCurLifeSpan;
	char  cTxt[120], cItemColor;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;

	cItemType = *cp;
	cp++;

	cEquipPos = *cp;
	cp++;

	bIsEquipped = (bool)*cp;
	cp++;

	sp = (short *)cp;
	sLevelLimit = *sp;
	cp += 2;

	cGenderLimit = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCurLifeSpan = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wWeight = *wp;
	cp += 2;

	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;

	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;

	cItemColor = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCost = *wp;
	ZeroMemory(cTxt, sizeof(cTxt));
	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, 0, cStr1, cStr2, cStr3);
	fmt::format_to(cTxt, NOTIFYMSG_ITEMPURCHASED, cStr1, wCost);
	AddEventList(cTxt, 10);

	if ((cItemType == ITEMTYPE_CONSUME) || (cItemType == ITEMTYPE_ARROW))
	{
		for (i = 0; i < MAXITEMS; i++)
			if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
			{
				m_pItemList[i]->m_dwCount += dwCount;
				_iCalcTotalWeight();
				return;
			}
	}

	short nX, nY;
	for (i = 0; i < MAXITEMS; i++)
	{
		if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cName, 20) == 0))
		{
			nX = m_pItemList[i]->m_sX;
			nY = m_pItemList[i]->m_sY;
			break;
		}
		else
		{
			nX = 40;
			nY = 30;
		}
	}

	for (i = 0; i < MAXITEMS; i++)
		if (m_pItemList[i] == 0)
		{
			m_pItemList[i] = new class CItem;
			memcpy(m_pItemList[i]->m_cName, cName, 20);
			m_pItemList[i]->m_dwCount = dwCount;
			//m_pItemList[i]->m_sX           = 40;
			//m_pItemList[i]->m_sY           = 30;
			m_pItemList[i]->m_sX = nX;
			m_pItemList[i]->m_sY = nY;
			bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, nX, nY, 0, 0);
			m_pItemList[i]->m_cItemType = cItemType;
			m_pItemList[i]->m_cEquipPos = cEquipPos;
			m_bIsItemDisabled[i] = false;
			m_bIsItemEquipped[i] = false;
			m_pItemList[i]->m_sLevelLimit = sLevelLimit;
			m_pItemList[i]->m_cGenderLimit = cGenderLimit;
			m_pItemList[i]->m_wCurLifeSpan = wCurLifeSpan;
			m_pItemList[i]->m_wWeight = wWeight;
			m_pItemList[i]->m_sSprite = sSprite;
			m_pItemList[i]->m_sSpriteFrame = sSpriteFrame;
			m_pItemList[i]->m_ItemColor = cItemColor;

			_iCalcTotalWeight();
			// fixed v1.11
			for (j = 0; j < MAXITEMS; j++)
				if (m_cItemOrder[j] == -1)
				{
					m_cItemOrder[j] = i;
					return;
				}

			return;
		}
}

void helbreath::NotifyMsg_ItemReleased(char * pData)
{
	char * cp;
	short * sp, sEquipPos, sItemIndex;
	char cTxt[120];

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sEquipPos = *sp;
	cp += 2;
	sp = (short *)cp;
	sItemIndex = *sp;
	cp += 2;

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[sItemIndex], cStr1, cStr2, cStr3);
	fmt::format_to(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
	AddEventList(cTxt, 10);
	m_bIsItemEquipped[sItemIndex] = false;
	m_sItemEquipmentStatus[m_pItemList[sItemIndex]->m_cEquipPos] = -1;

	if (memcmp(m_pItemList[sItemIndex]->m_cName, "AngelicPendant", 14) == 0) PlaySound('E', 53, 0);
	else PlaySound('E', 29, 0);
}

void helbreath::NotifyMsg_ItemRepaired(char * pData)
{
	char * cp, cTxt[120];
	uint32_t * dwp, dwItemID, dwLife;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	dwp = (uint32_t *)cp;
	dwItemID = *dwp;
	cp += 4;

	dwp = (uint32_t *)cp;
	dwLife = *dwp;
	cp += 4;

	m_pItemList[dwItemID]->m_wCurLifeSpan = (uint16_t)dwLife;
	m_bIsItemDisabled[dwItemID] = false;
	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(m_pItemList[dwItemID], cStr1, cStr2, cStr3);

	if (m_pItemList[dwItemID]->m_wCurLifeSpan != m_pItemList[dwItemID]->m_wMaxLifeSpan)
	{
		fmt::format_to(cTxt, NOTIFYMSG_ITEMREPAIRED2, cStr1);
	}
	else
	{
		fmt::format_to(cTxt, NOTIFYMSG_ITEMREPAIRED1, cStr1);
	}

	AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_ItemToBank(char * pData)
{
	char * cp, cIndex;
	uint32_t * dwp, dwCount, dwAttribute;
	char  cName[21], cItemType, cEquipPos, cGenderLimit, cItemColor;
	bool  bIsEquipped;
	short * sp, sSprite, sSpriteFrame, sLevelLimit, sItemEffectValue2, sItemSpecEffectValue2;
	uint16_t * wp, wWeight, wCurLifeSpan;
	char  cTxt[120];

	cp = (pData + INDEX2_MSGTYPE + 2);

	cIndex = *cp;
	cp++;

	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;

	cItemType = *cp;
	cp++;

	cEquipPos = *cp;
	cp++;

	bIsEquipped = (bool)*cp;
	cp++;

	sp = (short *)cp;
	sLevelLimit = *sp;
	cp += 2;

	cGenderLimit = *cp;
	cp++;

	wp = (uint16_t *)cp;
	wCurLifeSpan = *wp;
	cp += 2;

	wp = (uint16_t *)cp;
	wWeight = *wp;
	cp += 2;

	sp = (short *)cp;
	sSprite = *sp;
	cp += 2;

	sp = (short *)cp;
	sSpriteFrame = *sp;
	cp += 2;

	cItemColor = *cp;
	cp++;


	sp = (short *)cp;
	sItemEffectValue2 = *sp;
	cp += 2;

	dwp = (uint32_t *)cp;
	dwAttribute = *dwp;
	cp += 4;
	sItemSpecEffectValue2 = (short)*cp;
	cp++;

	uint8_t sockets[MAXITEMSOCKETS];
	for (int i = 0; i < MAXITEMSOCKETS; i++)
	{
		Pop(cp, sockets[i]);
	}

	char cStr1[64], cStr2[64], cStr3[64];
	GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);


	if (m_pBankList[cIndex] == 0)
	{
		m_pBankList[cIndex] = new class CItem;

		memcpy(m_pBankList[cIndex]->m_cName, cName, 20);
		m_pBankList[cIndex]->m_dwCount = dwCount;

		m_pBankList[cIndex]->m_cItemType = cItemType;
		m_pBankList[cIndex]->m_cEquipPos = cEquipPos;

		m_pBankList[cIndex]->m_sLevelLimit = sLevelLimit;
		m_pBankList[cIndex]->m_cGenderLimit = cGenderLimit;
		m_pBankList[cIndex]->m_wCurLifeSpan = wCurLifeSpan;
		m_pBankList[cIndex]->m_wWeight = wWeight;
		m_pBankList[cIndex]->m_sSprite = sSprite;
		m_pBankList[cIndex]->m_sSpriteFrame = sSpriteFrame;
		m_pBankList[cIndex]->m_ItemColor = cItemColor;
		m_pBankList[cIndex]->m_sItemEffectValue2 = sItemEffectValue2;
		m_pBankList[cIndex]->m_dwAttribute = dwAttribute;
		m_pBankList[cIndex]->m_sItemSpecEffectValue2 = sItemSpecEffectValue2;

		for (int i = 0; i < MAXITEMSOCKETS; i++)
		{
			m_pBankList[cIndex]->m_sockets[i] = sockets[i];
		}

		ZeroMemory(cTxt, sizeof(cTxt));
		if (dwCount == 1) fmt::format_to(cTxt, NOTIFYMSG_ITEMTOBANK3, cStr1);
		else fmt::format_to(cTxt, NOTIFYMSG_ITEMTOBANK2, dwCount, cStr1);

		if (m_bIsDialogEnabled[14] == true) m_dialogBoxes[14].sView = MAXBANKITEMS - 12;
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_Killed(char * pData)
{
	char * cp, cAttackerName[21];
	m_bCommandAvailable = false;
	m_cCommand = OBJECTSTOP;
	m_iHP = 0;
	m_cCommand = -1;
	// Restart
	m_bItemUsingStatus = false;
	ClearSkillUsingStatus();
	ZeroMemory(cAttackerName, sizeof(cAttackerName));
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	memcpy(cAttackerName, cp, 20);
	cp += 20;
	/*	if (strlen(cAttackerName) == 0) // removed in v2.20 (bug?) Many servers send the info themselves.
			AddEventList(NOTIFYMSG_KILLED1, 10);
		else
		{	fmt::format_to(G_cTxt, NOTIFYMSG_KILLED2, cAttackerName);
			AddEventList(G_cTxt, 10);
		}*/
		// Snoopy: reduced 3 lines -> 2 lines
	AddEventList(NOTIFYMSG_KILLED1, 10);
	AddEventList(NOTIFYMSG_KILLED3, 10);
	//AddEventList(NOTIFYMSG_KILLED4, 10);//"Log Out
}

void helbreath::NotifyMsg_LevelUp(char * pData)
{
	char * cp;
	int * ip;
	int i, iPrevLevel;
	char cTxt[120];

	iPrevLevel = m_iLevel;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	ip = (int *)cp;
	m_iLevel = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_STR] = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_VIT] = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_DEX] = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_INT] = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_MAG] = *ip;
	cp += 4;

	ip = (int *)cp;
	m_stat[STAT_CHR] = *ip;
	cp += 4;

	// CLEROTH - LU
	m_iLU_Point = m_iLevel * 7 - ( // Stats System xRisenx
	//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - (
		(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
		- 70)
		- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	// Stats System xRisenx
	//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;

	fmt::format_to(cTxt, NOTIFYMSG_LEVELUP1, m_iLevel);// "Level up!!! Level %d!"
	AddEventList(cTxt, 10);

	switch (m_sPlayerType)
	{
		case 1:
		case 2:
		case 3:
			PlaySound('C', 21, 0);
			break;

		case 4:
		case 5:
		case 6:
			PlaySound('C', 22, 0);
			break;
	}

	_RemoveChatMsgListByObjectID(m_sPlayerObjectID);

	for (i = 1; i < MAXCHATMSGS; i++)
		if (m_pChatMsgList[i] == 0)
		{
			ZeroMemory(cTxt, sizeof(cTxt));
			strcpy(cTxt, "Level up!");
			m_pChatMsgList[i] = new class CMsg(23, cTxt, m_dwCurTime);
			m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

			if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
			{
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = 0;
			}
			return;
		}
}

void helbreath::NotifyMsg_SettingSuccess(char * pData)
{
	char * cp;
	int * ip;
	int iPrevLevel;
	char cTxt[120];
	iPrevLevel = m_iLevel;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ip = (int *)cp;
	m_iLevel = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_STR] = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_VIT] = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_DEX] = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_INT] = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_MAG] = *ip;
	cp += 4;
	ip = (int *)cp;
	m_stat[STAT_CHR] = *ip;
	cp += 4;
	fmt::format_to(cTxt, "Your stat has been changed.");
	AddEventList(cTxt, 10);
	// LU
	m_iLU_Point = m_iLevel * 7 - ( // Stats System xRisenx
	//m_iLU_Point = m_iLevel*TOTALLEVELUPPOINT - (
		(m_stat[STAT_STR] + m_stat[STAT_VIT] + m_stat[STAT_DEX] + m_stat[STAT_INT] + m_stat[STAT_MAG] + m_stat[STAT_CHR])
		- 70)
		- 7 + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	// Stats System xRisenx
	//- TOTALLEVELUPPOINT + m_angelStat[STAT_STR] + m_angelStat[STAT_DEX] + m_angelStat[STAT_INT] + m_angelStat[STAT_MAG];
	m_luStat[STAT_STR] = m_luStat[STAT_VIT] = m_luStat[STAT_DEX] = m_luStat[STAT_INT] = m_luStat[STAT_MAG] = m_luStat[STAT_CHR] = 0;
}

void helbreath::NotifyMsg_MagicEffectOff(char * pData)
{
	char * cp;
	uint16_t * wp;
	short  sMagicType, sMagicEffect;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sMagicType = (short)*wp;
	cp += 2;
	wp = (uint16_t *)cp;
	sMagicEffect = (short)*wp;
	cp += 2;
	switch (sMagicType)
	{
		case MAGICTYPE_PROTECT:
			switch (sMagicEffect)
			{
				case 1: // "Protection from arrows has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF1, 10);
					m_bPfa = false; // Magic Icons xRisenx
					break;
				case 2:	// "Protection from magic has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF2, 10);
					m_bPfm = false; // Magic Icons xRisenx
					break;
				case 3:	// "Defense shield effect has vanished."
				case 4:	// "Defense shield effect has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF3, 10);
					m_bShield = false; // Magic Icons xRisenx
					break;
				case 5:	// "Absolute Magic Protection has been vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF14, 10);
					m_bPfm = false; // Magic Icons xRisenx
					break;
			}
			break;

		case MAGICTYPE_HOLDOBJECT:
			switch (sMagicEffect)
			{
				case 1:	// "Hold person magic effect has vanished."
					m_bParalyze = false;
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF4, 10);
					break;

				case 2:	// "Paralysis magic effect has vanished."
					m_bParalyze = false;
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF5, 10);
					break;
			}
			break;

		case MAGICTYPE_INVISIBILITY:
			switch (sMagicEffect)
			{
				case 1:	// "Invisibility magic effect has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF6, 10);
					m_bInv = false; // Magic Icons xRisenx
					break;
			}
			break;

		case MAGICTYPE_CONFUSE:
			switch (sMagicEffect)
			{
				case 1:	// "Language confuse magic effect has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF7, 10);
					break;
				case 2:	// "Confusion magic has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF8, 10);
					m_bIsConfusion = false;
					break;
				case 3:	// "Illusion magic has vanished."
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF9, 10);
					m_iIlusionOwnerH = 0;
					break;
				case 4:	// "At last, you gather your senses." // snoopy
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF15, 10);
					m_bIllusionMVT = false;
					break;
			}
			break;

		case MAGICTYPE_POISON:
			if (m_bIsPoisoned) AddEventList(NOTIFYMSG_MAGICEFFECT_OFF10, 10);
			m_bIsPoisoned = false;
			break;

		case MAGICTYPE_BERSERK:
			switch (sMagicEffect)
			{
				case 1:
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF11, 10);
					m_bZerk = false; // Magic Icons xRisenx
					break;
			}
			break;

		case MAGICTYPE_POLYMORPH:
			switch (sMagicEffect)
			{
				case 1:
					AddEventList(NOTIFYMSG_MAGICEFFECT_OFF12, 10);
					break;
			}
			break;

		case MAGICTYPE_ICE:
			//case MAGICTYPE_ICE_MB:
			//case MAGICTYPE_ICE_COTG:
			AddEventList(NOTIFYMSG_MAGICEFFECT_OFF13, 10);
			break;
	}
}

void helbreath::NotifyMsg_MagicEffectOn(char * pData)
{
	char * cp;
	uint32_t * dwp;
	uint16_t * wp;
	short  sMagicType, sMagicEffect, sOwnerH;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sMagicType = (short)*wp;
	cp += 2;
	dwp = (uint32_t *)cp;
	sMagicEffect = (short)*dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	sOwnerH = (short)*dwp;
	cp += 4;
	switch (sMagicType)
	{
		case MAGICTYPE_PROTECT:
			switch (sMagicEffect)
			{
				case 1: // "You are completely protected from arrows!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON1, 10);
					m_bPfa = true; // Magic Icons xRisenx
					m_sPfa = 60; // Magic Icons Time Limit xRisenx
					break;
				case 2: // "You are protected from magic!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON2, 10);
					m_bPfm = true; // Magic Icons xRisenx
					m_sPfm = 60; // Magic Icons Time Limit xRisenx
					break;
				case 3: // "Defense ratio increased by a magic shield!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON3, 10);
					m_bShield = true; // Magic Icons xRisenx
					m_sShield = 60; // Magic Icons Time Limit xRisenx
					break;
				case 4: // "Defense ratio increased by a magic shield!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON3, 10);
					m_bShield = true; // Magic Icons xRisenx
					m_sShield = 60; // Magic Icons Time Limit xRisenx
					break;
				case 5: // "You are completely protected from magic!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON14, 10);
					m_bPfm = true; // Magic Icons xRisenx
					m_sPfm = 60; // Magic Icons Time Limit xRisenx
					break;
			}
			break;

		case MAGICTYPE_HOLDOBJECT:
			switch (sMagicEffect)
			{
				case 1: // "You were bounded by a Hold Person spell! Unable to move!"
					m_bParalyze = true;
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON4, 10);
					break;
				case 2: // "You were bounded by a Paralysis spell! Unable to move!"
					m_bParalyze = true;
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON5, 10);
					break;
			}
			break;

		case MAGICTYPE_INVISIBILITY:
			switch (sMagicEffect)
			{
				case 1: // "You are now invisible, no one can see you!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON6, 10);
					m_bInv = true; // Magic Icons xRisenx
					m_sInv = 60; // Magic Icons Time Limit xRisenx
					break;
			}
			break;

		case MAGICTYPE_CONFUSE:
			switch (sMagicEffect)
			{
				case 1:	// Confuse Language "No one understands you because of language confusion magic!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON7, 10);
					break;

				case 2: // Confusion "Confusion magic casted, impossible to determine player allegience."
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON8, 10);
					m_bIsConfusion = true;
					break;

				case 3:	// Illusion "Illusion magic casted, impossible to tell who is who!"
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON9, 10);
					_SetIlusionEffect(sOwnerH);
					break;

				case 4:	// IllusionMouvement "You are thrown into confusion, and you are flustered yourself." // snoopy
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON15, 10);
					m_bIllusionMVT = true;
					break;
			}
			break;

		case MAGICTYPE_POISON:
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON10, 10);
			m_bIsPoisoned = true;
			break;

		case MAGICTYPE_BERSERK:
			switch (sMagicEffect)
			{
				case 1:
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
					m_bZerk = true; // Magic Icons xRisenx
					m_sZerk = 40; // Magic Icons Time Limit xRisenx
					break;

				case 2: // Greater Berserk
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
					m_bZerk = true; // Magic Icons xRisenx
					m_sZerk = 80; // Magic Icons Time Limit xRisenx

				case 3: // Ancient Berserk
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON11, 10);
					m_bZerk = true; // Magic Icons xRisenx
					m_sZerk = 300; // Magic Icons Time Limit xRisenx
					break;
			}
			break;

		case MAGICTYPE_POLYMORPH:
			switch (sMagicEffect)
			{
				case 1:
					AddEventList(NOTIFYMSG_MAGICEFFECT_ON12, 10);
					break;
			}
			break;

		case MAGICTYPE_ICE:
			//case MAGICTYPE_ICE_MB:
			//case MAGICTYPE_ICE_COTG:
			AddEventList(NOTIFYMSG_MAGICEFFECT_ON13, 10);
			break;
	}
}

void helbreath::NotifyMsg_MagicStudyFail(char * pData)
{
	char * cp, cMagicNum, cName[31], cFailCode;
	char cTxt[120];
	int * ip, iCost, iReqInt;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cFailCode = *cp;
	cp++;
	cMagicNum = *cp;
	cp++;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 30);
	cp += 30;
	ip = (int *)cp;
	iCost = *ip;
	cp += 4;
	ip = (int *)cp;
	iReqInt = *ip;
	cp += 4;
	/*	// Snoopy: remove special CLEROTH's feature
		ip = (int *)cp;
		iReqStr = *ip;
		cp += 4;
		// CLEROTH
		fmt::format_to(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL4, cName, iCost, iReqInt, iReqStr);
		AddEventList(cTxt, 10);*/

	if (iCost > 0)
	{
		fmt::format_to(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL1, cName);
		AddEventList(cTxt, 10);
	}
	else
	{
		fmt::format_to(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL2, cName);
		AddEventList(cTxt, 10);
		fmt::format_to(cTxt, NOTIFYMSG_MAGICSTUDY_FAIL3, iReqInt);
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_MagicStudySuccess(char * pData)
{
	char * cp, cMagicNum, cName[31];
	char cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cMagicNum = *cp;
	cp++;
	m_cMagicMastery[cMagicNum] = 1;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 30);
	fmt::format_to(cTxt, NOTIFYMSG_MAGICSTUDY_SUCCESS1, cName);
	AddEventList(cTxt, 10);
	PlaySound('E', 23, 0);
}

void helbreath::NotifyMsg_MP(char * pData)
{
	uint32_t * dwp;
	int iPrevMP;
	char cTxt[120];
	iPrevMP = m_iMP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iMP = (int)*dwp;
	if (abs(m_iMP - iPrevMP) < 10) return;
	if (m_iMP > iPrevMP)
	{
		fmt::format_to(cTxt, NOTIFYMSG_MP_UP, m_iMP - iPrevMP);
		AddEventList(cTxt, 10);
		PlaySound('E', 21, 0);
	}
	else
	{
		fmt::format_to(cTxt, NOTIFYMSG_MP_DOWN, iPrevMP - m_iMP);
		AddEventList(cTxt, 10);
	}
}

void helbreath::NotifyMsg_NewGuildsMan(char * pData)
{
	char * cp, cName[12], cTxt[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	fmt::format_to(cTxt, NOTIFYMSG_NEW_GUILDMAN1, cName);
	AddEventList(cTxt, 10);
	ClearGuildNameList();
}

void helbreath::NotifyMsg_PKcaptured(char * pData)
{
	char * cp;
	uint32_t * dwp;
	uint16_t * wp;
	int     iPKcount, iLevel, iExp, iRewardGold;
	char cTxt[120], cName[12];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	iPKcount = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
	iLevel = *wp;
	cp += 2;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	dwp = (uint32_t *)cp;
	iRewardGold = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	fmt::format_to(cTxt, NOTIFYMSG_PK_CAPTURED1, iLevel, cName, iPKcount);
	AddEventList(cTxt, 10);
	fmt::format_to(cTxt, EXP_INCREASED, iExp - m_iExp);
	AddEventList(cTxt, 10);
	fmt::format_to(cTxt, NOTIFYMSG_PK_CAPTURED3, iExp - m_iExp);
	AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_PKpenalty(char * pData)
{
	char * cp;
	uint32_t * dwp;
	int     iPKcount, iExp, iStr, iVit, iDex, iInt, iMag, iChr;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	dwp = (uint32_t *)cp;
	iExp = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iStr = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iVit = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iDex = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iInt = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iMag = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iChr = *dwp;
	cp += 4;
	dwp = (uint32_t *)cp;
	iPKcount = *dwp;
	cp += 4;
	fmt::format_to(G_cTxt, NOTIFYMSG_PK_PENALTY1, iPKcount);
	AddEventList(G_cTxt, 10);
	if (m_iExp > iExp)
	{
		fmt::format_to(G_cTxt, NOTIFYMSG_PK_PENALTY2, m_iExp - iExp);
		AddEventList(G_cTxt, 10);
	}
	m_iExp = iExp;
	m_stat[STAT_STR] = iStr;
	m_stat[STAT_VIT] = iVit;
	m_stat[STAT_DEX] = iDex;
	m_stat[STAT_INT] = iInt;
	m_stat[STAT_MAG] = iMag;
	m_stat[STAT_CHR] = iChr;
	m_iPKCount = iPKcount;
}

void helbreath::NotifyMsg_PlayerShutUp(char * pData)
{
	char * cp, cName[12];
	uint16_t * wp, wTime;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wTime = *wp;
	cp += 2;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	if (player_name == cName)
		fmt::format_to(G_cTxt, NOTIFYMSG_PLAYER_SHUTUP1, wTime);
	else fmt::format_to(G_cTxt, NOTIFYMSG_PLAYER_SHUTUP2, cName, wTime);

	AddEventList(G_cTxt, 10);
}

void helbreath::NotifyMsg_UnitStatus(bool bOnGame, char * pData)
{
	char cName[12], cMapName[12], * cp;
	uint16_t * wp;
	uint16_t  dx = 1, dy = 1;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	ZeroMemory(cMapName, sizeof(cMapName));
	memcpy(cMapName, cp, 10);
	cp += 10;
	wp = (uint16_t *)cp;
	dx = (uint16_t)*wp;
	cp += 2;
	wp = (uint16_t *)cp;
	dy = (uint16_t)*wp;
	cp += 2;
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	if (bOnGame == true)
	{
		if (strlen(cMapName) == 0)
			fmt::format_to(G_cTxt, NOTIFYMSG_PLAYER_STATUS1, cName);
		else fmt::format_to(G_cTxt, NOTIFYMSG_PLAYER_STATUS2, cName, cMapName, dx, dy);
	}
	else fmt::format_to(G_cTxt, NOTIFYMSG_PLAYER_STATUS3, cName);
	AddEventList(G_cTxt, 10);
}

void helbreath::NotifyMsg_QuestReward(char * pData)
{
	short * sp, sWho, sFlag;
	char * cp, cRewardName[21], cTxt[120];
	int * ip, iAmount, iIndex, iPreCon;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	sp = (short *)cp;
	sWho = *sp;
	cp += 2;
	sp = (short *)cp;
	sFlag = *sp;
	cp += 2;
	ip = (int *)cp;
	iAmount = *ip;
	cp += 4;
	ZeroMemory(cRewardName, sizeof(cRewardName));
	memcpy(cRewardName, cp, 20);
	cp += 20;
	iPreCon = m_iContribution;
	ip = (int *)cp;
	m_iContribution = *ip;
	//	m_iContributionPrice = 0;
	cp += 4;

	if (sFlag == 1)
	{
		m_stQuest.sWho = 0;
		m_stQuest.sQuestType = 0;
		m_stQuest.sContribution = 0;
		m_stQuest.sTargetType = 0;
		m_stQuest.sTargetCount = 0;
		m_stQuest.sX = 0;
		m_stQuest.sY = 0;
		m_stQuest.sRange = 0;
		m_stQuest.sCurrentCount = 0;
		m_stQuest.bIsQuestCompleted = false;
		ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
		EnableDialogBox(21, 0, sWho + 110, 0);
		iIndex = m_dialogBoxes[21].sV1;
		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
		ZeroMemory(cTxt, sizeof(cTxt));
		if (memcmp(cRewardName, "exp", 3) == 0)
		{
			if (iAmount > 0) fmt::format_to(cTxt, NOTIFYMSG_QUEST_REWARD1, iAmount);
		}
		else
		{
			fmt::format_to(cTxt, NOTIFYMSG_QUEST_REWARD2, iAmount, cRewardName);
		}
		m_pMsgTextList2[iIndex] = new class CMsg(0, cTxt, 0);
		iIndex++;
		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
		ZeroMemory(cTxt, sizeof(cTxt));
		if (iPreCon < m_iContribution)
			fmt::format_to(cTxt, NOTIFYMSG_QUEST_REWARD3, m_iContribution - iPreCon);
		else fmt::format_to(cTxt, NOTIFYMSG_QUEST_REWARD4, iPreCon - m_iContribution);

		m_pMsgTextList2[iIndex] = new class CMsg(0, "  ", 0);
		iIndex++;
	}
	else EnableDialogBox(21, 0, sWho + 120, 0);
}

void helbreath::NotifyMsg_RatingPlayer(char * pData)
{//int * ip;
	char * cp, cName[12];
	uint16_t  cValue;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cValue = *cp;
	cp++;
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);
	cp += 10;
	//	ip = (int *)cp;
	//	m_iRating = *ip;
	cp += 4;
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	if (player_name == cName)
	{
		if (cValue == 1)
		{
			strcpy(G_cTxt, NOTIFYMSG_RATING_PLAYER1);
			PlaySound('E', 23, 0);
		}
	}
	else
	{
		if (cValue == 1)
			fmt::format_to(G_cTxt, NOTIFYMSG_RATING_PLAYER2, cName);
		else fmt::format_to(G_cTxt, NOTIFYMSG_RATING_PLAYER3, cName);
	}
	AddEventList(G_cTxt, 10);
}

void helbreath::NotifyMsg_ServerChange(char * pData)
{
// 	char * cp, cWorldServerAddr[16];	//Snoopy: change names for better readability
// 	int * ip, iWorldServerPort;		//Snoopy: change names for better readability
// 
// 	ZeroMemory(m_cMapName, sizeof(m_cMapName));
// 	ZeroMemory(m_cMapMessage, sizeof(m_cMapMessage));
// 	ZeroMemory(cWorldServerAddr, sizeof(cWorldServerAddr));
// 
// 
// 	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
// 	memcpy(m_cMapName, cp, 10);
// 
// 	//	m_cMapIndex = GetOfficialMapName(m_cMapName, m_cMapMessage);
// 	cp += 10;
// 
// 	memcpy(cWorldServerAddr, cp, 15);
// 	cp += 15;
// 	ip = (int *)cp;
// 	iWorldServerPort = *ip;
// 	cp += 4;
// 	gamemode = 0;
// 	if (_socket)
// 	{
// 		_socket->stop();
// 	}
// 	if (_socket == nullptr)
// 	{
// 		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_cLogServerAddr), m_iLogServerPort);
// 		new_connection_->socket().async_connect(endpoint,
// 			boost::bind(&helbreath::handle_connect, this,
// 				boost::asio::placeholders::error));
// 	}
// 
// 	m_bIsPoisoned = false;
// 
// 	ChangeGameMode(GAMEMODE_ONCONNECTING);
// 	m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
// 	//m_wEnterGameType = ENTERGAMEMSGTYPE_NEW; //Gateway
// 	m_wEnterGameType = ENTERGAMEMSGTYPE_CHANGINGSERVER;
// 	ZeroMemory(m_cMsg, sizeof(m_cMsg));
// 	strcpy(m_cMsg, "55");
}

void helbreath::NotifyMsg_SetItemCount(char * pData)
{
	char * cp;
	uint16_t * wp;
	uint32_t * dwp;
	short  sItemIndex;
	uint32_t  dwCount;
	bool   bIsItemUseResponse;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sItemIndex = *wp;
	cp += 2;
	dwp = (uint32_t *)cp;
	dwCount = *dwp;
	cp += 4;
	bIsItemUseResponse = (bool)*cp;
	cp++;
	if (m_pItemList[sItemIndex] != 0)
	{
		m_pItemList[sItemIndex]->m_dwCount = dwCount;
		_iCalcTotalWeight();
		if (bIsItemUseResponse == true) m_bIsItemDisabled[sItemIndex] = false;
	}
}

void helbreath::NotifyMsg_ShowMap(char * pData)
{
	char * cp;
	uint16_t * wp, w1, w2;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	w1 = *wp;
	cp += 2;
	wp = (uint16_t *)cp;
	w2 = *wp;
	cp += 2;
	if (w2 == 0) AddEventList(NOTIFYMSG_SHOW_MAP1, 10);
	else EnableDialogBox(22, 0, w1, w2 - 1);
}

void helbreath::NotifyMsg_Skill(char * pData)
{
	uint16_t * wp;
	short sSkillIndex, sValue;
	char * cp;
	char cTxt[120];
	int i;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	sSkillIndex = (short)*wp;
	cp += 2;
	wp = (uint16_t *)cp;
	sValue = (short)*wp;
	cp += 2;
	_RemoveChatMsgListByObjectID(m_sPlayerObjectID);
	if (!m_pSkillCfgList[sSkillIndex]) return;
	if (m_pSkillCfgList[sSkillIndex]->m_iLevel < sValue)
	{
		fmt::format_to(cTxt, NOTIFYMSG_SKILL1, m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
		AddEventList(cTxt, 10);
		PlaySound('E', 23, 0);
		for (i = 1; i < MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == 0)
			{
				ZeroMemory(cTxt, sizeof(cTxt));
				fmt::format_to(cTxt, "%s +%d%%", m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
				m_pChatMsgList[i] = new class CMsg(20, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;
				if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
				{
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = 0;
				}
				break;
			}
	}
	else if (m_pSkillCfgList[sSkillIndex]->m_iLevel > sValue)
	{
		fmt::format_to(cTxt, NOTIFYMSG_SKILL2, m_pSkillCfgList[sSkillIndex]->m_cName, m_pSkillCfgList[sSkillIndex]->m_iLevel - sValue);
		AddEventList(cTxt, 10);
		PlaySound('E', 24, 0);
		for (i = 1; i < MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == 0)
			{
				ZeroMemory(cTxt, sizeof(cTxt));
				fmt::format_to(cTxt, "%s -%d%%", m_pSkillCfgList[sSkillIndex]->m_cName, sValue - m_pSkillCfgList[sSkillIndex]->m_iLevel);
				m_pChatMsgList[i] = new class CMsg(20, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;
				if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
				{
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = 0;
				}
				break;
			}
	}
	m_pSkillCfgList[sSkillIndex]->m_iLevel = sValue;
	m_cSkillMastery[sSkillIndex] = (unsigned char)sValue;
}

void helbreath::NotifyMsg_SkillTrainSuccess(char * pData)
{
	char * cp, cSkillNum, cSkillLevel;
	char cTemp[120];
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	cSkillNum = *cp;
	cp++;
	cSkillLevel = *cp;
	cp++;
	if (!m_pSkillCfgList[cSkillNum]) return;
	ZeroMemory(cTemp, sizeof(cTemp));
	fmt::format_to(cTemp, NOTIFYMSG_SKILL_TRAIN_SUCCESS1, m_pSkillCfgList[cSkillNum]->m_cName, cSkillLevel);
	AddEventList(cTemp, 10);
	m_pSkillCfgList[cSkillNum]->m_iLevel = cSkillLevel;
	m_cSkillMastery[cSkillNum] = (unsigned char)cSkillLevel;
	PlaySound('E', 23, 0);
}

void helbreath::NotifyMsg_SkillUsingEnd(char * pData)
{
	char * cp;
	uint16_t * wp, wResult;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	wp = (uint16_t *)cp;
	wResult = *wp;
	switch (wResult)
	{
		case 0:
			AddEventList(NOTIFYMSG_SKILL_USINGEND1, 10);
			break;
		case 1:
			AddEventList(NOTIFYMSG_SKILL_USINGEND2, 10);
			break;
	}
	m_bSkillUsingStatus = false;
}

void helbreath::NotifyMsg_SP(char * pData)
{
	uint32_t * dwp;
	int iPrevSP;
	iPrevSP = m_iSP;
	dwp = (uint32_t *)(pData + INDEX2_MSGTYPE + 2);
	m_iSP = (int)*dwp;
	if (abs(m_iSP - iPrevSP) < 10) return;
	if (m_iSP > iPrevSP)
	{
		fmt::format_to(G_cTxt, NOTIFYMSG_SP_UP, m_iSP - iPrevSP);
		AddEventList(G_cTxt, 10);
		PlaySound('E', 21, 0);
	}
	else
	{
		fmt::format_to(G_cTxt, NOTIFYMSG_SP_DOWN, iPrevSP - m_iSP);
		AddEventList(G_cTxt, 10);
	}
}

void helbreath::NotifyMsg_TotalUsers(char * pData)
{
	uint16_t * wp;
	int iTotal;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
	iTotal = (int)*wp;
	fmt::format_to(G_cTxt, NOTIFYMSG_TOTAL_USER1, iTotal);
	AddEventList(G_cTxt, 10);
}

void helbreath::NotifyMsg_RelicInAltar(char * pData)
{
	char * cp;
	Side altarSide;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	altarSide = (Side)*cp;
	fmt::format_to(G_cTxt, NOTIFYMSG_RELICINALTAR, sideName[altarSide]);
	SetTopMsg(G_cTxt, 10);

	m_relicOwnedSide = altarSide;
	m_relicOwnedTime = unixtime() + 1 * 1000;
}

void helbreath::NotifyMsg_RelicGrabbed(char * pData)
{
	char * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	char playerName[12];
	ZeroMemory(playerName, sizeof(playerName));
	memcpy(playerName, cp, 10);

	fmt::format_to(G_cTxt, NOTIFYMSG_RELICGRABBED, playerName);
	AddEventList(G_cTxt, 10);

	m_relicOwnedTime = 0;
}

void helbreath::NotifyMsg_EventWinner(char * pData)
{
	char * cp = (char *)(pData + INDEX2_MSGTYPE + 2);

	EventType eType = ET_NONE;
	Pop(cp, (uint8_t &)eType);

	Side winnerSide = NEUTRAL;
	Pop(cp, (uint8_t &)winnerSide);

	fmt::format_to(G_cTxt, NOTIFYMSG_EVENTWINNER, sideName[winnerSide], eventName[eType]);
	SetTopMsg(G_cTxt, 10);

	m_bIsAstoriaMode = false;

	if (winnerSide != m_side)
		return;

	switch (eType)
	{
		case ET_CAPTURE:
			AddEventList(NOTIFYMSG_EVENTWINNERCTR, 10);
			break;

		case ET_DESTROY_SHIELD:
			AddEventList(NOTIFYMSG_EVENTWINNERDTS, 10);
			if (memcmp(m_cMapName, "astoria", 7) == 0)
			{
				AddEventList(NOTIFYMSG_EVENTWINNERDTS2, 10);
				m_iContribution += 30;
			}
			else
			{
				AddEventList(NOTIFYMSG_EVENTWINNERDTS2B, 10);
			}
			break;

	}
}

void helbreath::NotifyMsg_EventStarting(char * pData)
{
	char * cp;
	EventType eType;

	uint16_t * wp = (uint16_t *)(pData + INDEX2_MSGTYPE);
	uint32_t msgID = *wp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	eType = (EventType)*cp;

	std::stringstream msg;
	msg.str().c_str();
	msg.flush();
	msg.str().c_str();

	switch (eType)
	{
		case ET_CAPTURE:
			msg << "A " << eventName[eType] << " event will be starting in ";
			break;
		case ET_DESTROY_SHIELD:
			msg << "The " << eventName[eType] << " will be starting in ";
			break;
		case ET_APOCALYPSE:
			msg << "The portal to " << eventName[eType] << " will be open in ";
			break;
		case ET_CRUSADE:
			msg << "The " << eventName[eType] << " will be starting in ";
			break;
		case ET_MONSTEREVENT: // Monster Kill Event xRisenx
			msg << "The " << eventName[eType] << " will be starting in ";
			break;
		default:
			msg << "An unknown event will be starting in ";
			break;
	}

	switch (msgID)
	{
		case NOTIFY_EVENTSTARTING:
			msg << "2 hours.";
			break;
		case NOTIFY_EVENTSTARTING2:
			msg << "30 minutes.";
			break;
		case NOTIFY_EVENTSTARTING3:
			msg << "10 minutes.";
			break;
	}

	SetTopMsg(msg.str().c_str(), 10);
}

void helbreath::NotifyMsg_EventStart(char * pData)
{
	char * cp;
	EventType eType;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	eType = (EventType)*cp;

	for (int i = 0; i < MAXSIDES; i++)
	{
		m_astoriaStats[i].deaths = 0;
		m_astoriaStats[i].kills = 0;
		m_astoriaShieldsHP[i] = 0;
	}
	m_bIsAstoriaMode = true;
	m_relicOwnedTime = 0;

	fmt::format_to(G_cTxt, NOTIFYMSG_EVENTSTART_CTR, eventName[eType]);

	strcat(G_cTxt, NOTIFYMSG_EVENTSTART);
	SetTopMsg(G_cTxt, 10);
}

void helbreath::NotifyMsg_Casualties(char * pData)
{
	uint16_t * wp;
	wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);

	m_astoriaStats[ARESDEN].deaths = *wp;
	wp++;
	m_astoriaStats[ELVINE].deaths = *wp;
	wp++;
	m_astoriaStats[ISTRIA].deaths = *wp;
	wp++;
	m_astoriaStats[ARESDEN].kills = *wp;
	wp++;
	m_astoriaStats[ELVINE].kills = *wp;
	wp++;
	m_astoriaStats[ISTRIA].kills = *wp;

	m_bIsAstoriaMode = true;
}

void helbreath::NotifyMsg_ShieldHP(char * pData) // Shield xRisenx
{
	pData += 6;

	Side side = NEUTRAL;
	Pop(pData, (uint8_t &)side);

	uint32_t hp; // 16->32
	Pop(pData, (uint32_t &)hp); // 16->32

	m_astoriaShieldsHP[side] = hp;

	m_bIsAstoriaMode = true;
}

void helbreath::NotifyMsg_WhisperMode(bool bActive, char * pData)
{
	char * cp;
	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(m_cWhisperName, sizeof(m_cWhisperName));
	memcpy(m_cWhisperName, cp, 10);
	if (bActive == true)
	{
		fmt::format_to(G_cTxt, NOTIFYMSG_WHISPERMODE1, m_cWhisperName);
		if (m_pWhisperMsg[MAXWHISPERMSG - 1] != 0)
		{
			delete m_pWhisperMsg[MAXWHISPERMSG - 1];
			m_pWhisperMsg[MAXWHISPERMSG - 1] = 0;
		}
		for (int i = MAXWHISPERMSG - 2; i >= 0; i--)
		{
			m_pWhisperMsg[i + 1] = m_pWhisperMsg[i];
			m_pWhisperMsg[i] = 0;
		}
		m_pWhisperMsg[0] = new class CMsg(0, m_cWhisperName, 0);
		m_cWhisperIndex = 0;
	}
	else fmt::format_to(G_cTxt, NOTIFYMSG_WHISPERMODE2, m_cWhisperName);

	AddEventList(G_cTxt, 10);
}

void helbreath::NotifyMsg_FriendOnGame(char * pData)
{
	char cName[12], * cp;

	cp = (char *)(pData + INDEX2_MSGTYPE + 2);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 10);

	for (int i = 0; i < m_iTotalFriends; i++)
		if (strcmp(cName, friendsList[i].friendName) == 0)
		{
			friendsList[i].online = true;
			friendsList[i].updated = true;
		}
}

void helbreath::NotifyMsg_ItemToGuildBank(char * pData)
{
    char * cp, cIndex;
    uint32_t * dwp, dwCount, dwAttribute;
    char  cName[21], cItemType, cEquipPos, cGenderLimit, cItemColor;
    bool  bIsEquipped;
    short * sp, sSprite, sSpriteFrame, sLevelLimit, sItemEffectValue2, sItemSpecEffectValue2;
    uint16_t * wp, wWeight, wCurLifeSpan;
    char  cTxt[120];

    cp = (pData + INDEX2_MSGTYPE + 2);

    CItem * item = new CItem;

    Pop(cp, item->m_serverPtr);
    Pop(cp, m_guildBankVer);

    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;

    dwp = (uint32_t *)cp;
    dwCount = *dwp;
    cp += 4;

    cItemType = *cp;
    cp++;

    cEquipPos = *cp;
    cp++;

    bIsEquipped = (bool)*cp;
    cp++;

    sp = (short *)cp;
    sLevelLimit = *sp;
    cp += 2;

    cGenderLimit = *cp;
    cp++;

    wp = (uint16_t *)cp;
    wCurLifeSpan = *wp;
    cp += 2;

    wp = (uint16_t *)cp;
    wWeight = *wp;
    cp += 2;

    sp = (short *)cp;
    sSprite = *sp;
    cp += 2;

    sp = (short *)cp;
    sSpriteFrame = *sp;
    cp += 2;

    cItemColor = *cp;
    cp++;


    sp = (short *)cp;
    sItemEffectValue2 = *sp;
    cp += 2;

    dwp = (uint32_t *)cp;
    dwAttribute = *dwp;
    cp += 4;
    sItemSpecEffectValue2 = (short)*cp;
    cp++;

    for (int i = 0; i < MAXITEMSOCKETS; i++)
    {
        Pop(cp, item->m_sockets[i]);
    }

    char cStr1[64], cStr2[64], cStr3[64];
    GetItemName(cName, dwAttribute, cStr1, cStr2, cStr3);



    memcpy(item->m_cName, cName, 20);
    item->m_dwCount = dwCount;

    item->m_cItemType = cItemType;
    item->m_cEquipPos = cEquipPos;

    item->m_sLevelLimit = sLevelLimit;
    item->m_cGenderLimit = cGenderLimit;
    item->m_wCurLifeSpan = wCurLifeSpan;
    item->m_wWeight = wWeight;
    item->m_sSprite = sSprite;
    item->m_sSpriteFrame = sSpriteFrame;
    item->m_ItemColor = cItemColor;
    item->m_sItemEffectValue2 = sItemEffectValue2;
    item->m_dwAttribute = dwAttribute;
    item->m_sItemSpecEffectValue2 = sItemSpecEffectValue2;

    ItemMapIter dstIt = m_guildBankMap.insert(std::pair<CItem *, uint32_t>(item, item->m_serverPtr));
    ItemMapIter it = m_guildBankMap.begin();

    uint32_t pos = 0;
    while (it != dstIt)
    {
        pos++;
        ++it;
    }

    if (*(*dstIt).first < (*it).first)
        m_dialogBoxes[56].sView++;

    int32_t itPos = 0;
    while (it != dstIt)
    {
        if (*(*dstIt).first < (*it).first)
        {
            itPos--;
            --it;
        }
        else
        {
            itPos++;
            ++it;
        }
    }

    ZeroMemory(cTxt, sizeof(cTxt));
    if (dwCount == 1) fmt::format_to(cTxt, NOTIFYMSG_ITEMTOBANK3, cStr1);
    else fmt::format_to(cTxt, NOTIFYMSG_ITEMTOBANK2, dwCount, cStr1);

    if (m_bIsDialogEnabled[56])
    {
        if (m_guildBankMap.size() > m_dialogBoxes[56].sV1)
        {
            if (pos < m_dialogBoxes[56].sView)
            {

                while (pos < m_dialogBoxes[56].sView)
                {
                    m_dialogBoxes[56].sView--;
                    --m_guildBankIt;
                }
            }
            else
            {
                while (pos > m_dialogBoxes[56].sView + m_dialogBoxes[56].sV1 - 1)
                {
                    m_dialogBoxes[56].sView++;
                    ++m_guildBankIt;
                }
            }
        }
        else
            m_guildBankIt = m_guildBankMap.begin();
    }

    AddEventList(cTxt, 10);
}

void helbreath::NotifyMsg_RepairAllPrice(char * pData)
{
    uint32_t price;
    char * cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    Pop(cp, price);
    if (price == 0)
    {
        AddEventList(NOTIFYMSG_NOTHING_TO_REPAIR, 10); //"You don't have any items that require repair."
    }
    else
    {
        EnableDialogBox(20, 11, 0, price);
    }
}

void helbreath::NotifyMsg_Soccer(char * pData)
{
    uint16_t * wp;
    wp = (uint16_t *)(pData + INDEX2_MSGTYPE + 2);
    m_SoccerAresdenGoals = *wp;
    wp++;
    m_SoccerElvineGoals = *wp;
    wp++;
}

void helbreath::NotifyMsg_AddTitle(char * pData)
{

    char * cp, cSubject[21];
    int   i, j;
    int * ip;
    uint32_t * dwp;
    uint32_t   iCurrentNo;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);

    ZeroMemory(cSubject, sizeof(cSubject));
    memcpy(cSubject, cp, 21);
    cp += 21;

    dwp = (uint32_t *)cp;
    iCurrentNo = *dwp;
    cp += 4;

    ip = (int *)cp;
    int iNextRankPoints = *ip;
    cp += 4;

    ip = (int *)cp;
    int iRank = *ip;
    cp += 4;

    ip = (int *)cp;
    int iServerTitleIndex = *ip;
    cp += 4;

    char cActiveTitle = *cp;//this is 0 if this isnt our active title, 1 if it is our active title
    cp++;

    // check for existing title
    for (i = 0; i < MAXMENUITEMS; i++)
    {
        if (m_pTitles[i] != 0)
        {
            if (memcmp(m_pTitles[i]->m_cSubject, cSubject, 21) == 0)
            {
                m_pTitles[i]->m_iCurrentNo = iCurrentNo;
                m_pTitles[i]->m_iNextRankPoints = iNextRankPoints;
                m_pTitles[i]->m_iRank = iRank;
                m_pTitles[i]->m_iServerTitleIndex = iServerTitleIndex;
                if (cActiveTitle == 1)
                    m_iTitleIndex = i;
                return;//BESK added
            }
        }
    }

    int k;
    for (k = 0; k < MAXMENUITEMS; k++)
    {
        if (m_pTitles[k] == 0)
        {
            m_pTitles[k] = new class Title;
            break;
        }
    }

    memcpy(m_pTitles[k]->m_cSubject, cSubject, sizeof(cSubject));
    m_pTitles[k]->m_iCurrentNo = iCurrentNo;
    m_pTitles[k]->m_iNextRankPoints = iNextRankPoints;
    m_pTitles[k]->m_iRank = iRank;
    m_pTitles[k]->m_iServerTitleIndex = iServerTitleIndex;
    if (cActiveTitle == 1)
        m_iTitleIndex = k;
}

void helbreath::NotifyMsg_TitleLevelUp(char * pData)//BESK TITLES
{

    char * cp, cSubject[21], cMsg[120], cTxt[120];
    int   i, j;
    int * ip;
    uint32_t * dwp;
    uint32_t   iCurrentNo;
    cp = (char *)(pData + INDEX2_MSGTYPE + 2);

    ZeroMemory(cSubject, sizeof(cSubject));
    memcpy(cSubject, cp, 21);
    cp += 21;

    dwp = (uint32_t *)cp;
    iCurrentNo = *dwp;
    cp += 4;

    ip = (int *)cp;
    int iNextRankPoints = *ip;
    cp += 4;

    ip = (int *)cp;
    int iRank = *ip;
    cp += 4;

    ip = (int *)cp;
    int iServerTitleIndex = *ip;
    cp += 4;

    // check for existing title, and update it
    //bool bUsingLeveledUpTitle = false;//is the newly leveled up title the one we are wearing?
    for (i = 0; i < MAXMENUITEMS; i++)
    {
        if (m_pTitles[i] != 0)
        {
            if (memcmp(m_pTitles[i]->m_cSubject, cSubject, 21) == 0)
            {// FUCK IDK? D:
                m_pTitles[i]->m_iCurrentNo = iCurrentNo;
                m_pTitles[i]->m_iNextRankPoints = iNextRankPoints;
                m_pTitles[i]->m_iRank = iRank;
                m_pTitles[i]->m_iServerTitleIndex = iServerTitleIndex;
                //if (i == m_iTitleIndex) bUsingLeveledUpTitle = true;
                break;
            }
        }
    }

    ZeroMemory(cMsg, sizeof(cMsg));
    ZeroMemory(cTxt, sizeof(cTxt));
    ZeroMemory(cTitleLevelUpSubject, sizeof(cTitleLevelUpSubject));

    GetTitleName(cSubject, iRank, cTxt);

    //if(memcmp(cSubject, "Events", 6) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by winning %i events!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Donation", 8) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by donating %i pounds!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Gladiator", 9) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Constructor", 11) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by completing %i constructions!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Treasure", 8) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by finded %i rare items!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Suicide", 7) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by committing suicide %i times!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Crafting", 8) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by crafting %i items!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Hero", 4) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by killing %i players from the enemy town!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "Drunkard", 8) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by being drunk for %i minutes!", iCurrentNo);
    //}
    //else if (memcmp(cSubject, "SweetTooth", 10) == 0)
    //{
    //	fmt::format_to(cMsg, "You have earned a new title by eating %i sweets!", iCurrentNo);
    //}
    if (false) {}
    else if (memcmp(cSubject, "Alchemy", 7) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by creating %i potions!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Mining", 6) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by mining %i ores!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Blacksmithing", 13) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by smithing %i items!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Farming", 7) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by farming %i crops!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Fishing", 7) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by catching %i fish!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Gemcutting", 10) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by creating %i pieces of jewelery!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Gladiator", 9) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
    }
    else if (memcmp(cSubject, "Hero", 4) == 0)
    {
        fmt::format_to(cMsg, "You have earned a new title by earning 25 consecutive kills %i times in the arena!", iCurrentNo);
    }
    else
    {
        fmt::format_to(cMsg, "You have earned a new title by killing %i %s!", iCurrentNo, cSubject);
    }
    AddEventList(cMsg, 10);

    switch (m_sPlayerType)
    {
        case 1:
        case 2:
        case 3:
        case 7:
            PlaySound('C', 21, 0);
            break;

        case 4:
        case 5:
        case 6:
        case 8:
            PlaySound('C', 22, 0);
            break;
    }

    dwTitleLevelUpTime = m_dwCurTime;

    fmt::format_to(cTitleLevelUpSubject, cTxt);

    //for (i = 0; i < DEF_MAXMENUITEMS; i++) {
    //	if (m_pTitles[i] != NULL) {
    //		delete m_pTitles[i];
    //		m_pTitles[i] = NULL;
    //	}
    //}

    //m_iTitleIndex = -1;//BESK TODO: in my code, dont do this! why reset the player's active title on any title rankup???
    //if (bUsingLeveledUpTitle)//send SETTITLE to the server to force us and everyone else to refresh our title, since we ranked up
    //	bSendCommand(MSGID_SETTITLE, NULL, NULL, m_pTitles[m_iTitleIndex]->m_iServerTitleIndex, NULL, NULL, NULL);

    bSendCommand(MSGID_REQUEST_TITLES, 0, 0, 0, 0, 0, 0);
    m_cCommandCount--;
    m_dwReqTitlesTime = m_dwCurTime;
}

void helbreath::NotifyMsg_NpcHuntingON()
{
    SetTopMsg("Npc Hunting Game Activated!", 10);
}

void helbreath::NotifyMsg_NpcHuntingOFF()
{
    SetTopMsg("Npc Hunting Game Deacticated!", 10);
}

void helbreath::NotifyMsg_NpcHuntingWinner(char * pData)
{
    char * cp;
    int * ip;
    uint32_t * dwp;

    cp = (char *)(pData + INDEX2_MSGTYPE + 2);
    ip = (int *)cp;
    m_iContribution = *ip;
    cp += 4;
    ip = (int *)cp;
    m_iRating = *ip;
    cp += 4;
}
