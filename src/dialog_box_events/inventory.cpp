
#include "../helbreath.h"

#include "../lan_eng.h"

bool helbreath::DlgBoxPress_Inventory()
{
    int i;
    char  cItemID;
    short sX, sY, x1, x2, y1, y2;

#ifdef _DEBUG
    AddEventList("Press Inventory", 10);
#endif

    if (m_bIsDialogEnabled[2] == false) return false;
    if (m_bIsDialogEnabled[17] == true) return false;
    if (m_bIsDialogEnabled[4] == true) return false;

    short msY = m_stMCursor.sY;
    short msX = m_stMCursor.sX;

    sX = m_dialogBoxes[2].m_X;
    sY = m_dialogBoxes[2].m_Y;

    for (i = 0; i < MAXITEMS; i++)
        if (m_cItemOrder[MAXITEMS - 1 - i] != -1)
        {
            cItemID = m_cItemOrder[MAXITEMS - 1 - i];

            if (m_pItemList[cItemID] != 0)
            {
                m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX,
                    sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
                x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;
                if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                {

                    if (m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_bCheckCollison(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame, msX, msY) == true)
                    {
                        _SetItemOrder(0, cItemID);
                        if ((m_bIsGetPointingMode == true) && (m_iPointCommandType < 100) && (m_iPointCommandType >= 0)
                            && (m_pItemList[m_iPointCommandType] != 0)
                            && (m_pItemList[m_iPointCommandType]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
                            && (m_iPointCommandType != cItemID))
                        {
                            PointCommandHandler(0, 0, cItemID);
                            //m_bCommandAvailable  = FALSE;
                            m_bIsGetPointingMode = false;
                        }
                        else
                        {
                            m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                            m_stMCursor.sSelectedObjectID = cItemID;
                            m_stMCursor.sDistX = msX - x1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                            m_stMCursor.sDistY = msY - y1 + (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        }
                        return true;
                    }
                }
            }
        }
    return false;
}


void helbreath::_SetItemOrder(char cWhere, char cItemID)
{
    int i;

    switch (cWhere)
    {
        case 0:
            for (i = 0; i < MAXITEMS; i++)
                if (m_cItemOrder[i] == cItemID)
                    m_cItemOrder[i] = -1;

            for (i = 1; i < MAXITEMS; i++)
                if ((m_cItemOrder[i - 1] == -1) && (m_cItemOrder[i] != -1))
                {
                    m_cItemOrder[i - 1] = m_cItemOrder[i];
                    m_cItemOrder[i] = -1;
                }

            for (i = 0; i < MAXITEMS; i++)
                if (m_cItemOrder[i] == -1)
                {
                    m_cItemOrder[i] = cItemID;
                    return;
                }
            break;
    }
}

bool helbreath::_bCheckDraggingItemRelease(char dlgID)
{
    //char cDlgID = MouseOverDialog();

    if (dlgID)
    {
        EnableDialogBox(dlgID, 0, 0, 0);
        m_dialogBoxes[dlgID].OnDropItem();
        return true;
    }

    bItemDrop_ExternalScreen((char)m_stMCursor.sSelectedObjectID, m_stMCursor.sX, m_stMCursor.sY);
    return false;
}

void helbreath::bItemDrop_ExternalScreen(char cItemID, short msX, short msY)
{
    std::string cName;
    short sType, tX, tY;
    int iStatus, dlgType = 0;

    if (bCheckItemOperationEnabled(cItemID) == false) return;

    //if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 8) && (abs(m_sPlayerY - m_sMCY) <= 8))
    if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 11) && (abs(m_sPlayerY - m_sMCY) <= 9))//besk resolution
    {
        m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sType, &iStatus, &m_wCommObjectID);
        if (player_name == cName)
        {
            if (((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW))
                && (m_pItemList[cItemID]->m_dwCount > 1))
            {
                m_dialogBoxes[17].m_X = msX - 140;
                m_dialogBoxes[17].m_Y = msY - 70;
                if (m_dialogBoxes[17].m_Y < 0) m_dialogBoxes[17].m_Y = 0;
                m_dialogBoxes[17].sV1 = m_sMCX;
                m_dialogBoxes[17].sV2 = m_sMCY;
                m_dialogBoxes[17].sV3 = sType;
                m_dialogBoxes[17].sV4 = m_wCommObjectID;
                memset(m_dialogBoxes[17].cStr, 0, sizeof(m_dialogBoxes[17].cStr));
                if (sType < 10)
                    memcpy(m_dialogBoxes[17].cStr, cName.c_str(), 10);
                else
                {
                    GetNpcName(sType, m_dialogBoxes[17].cStr);
                }
                EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {
                switch (sType)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if (!dlgType) dlgType = 1;

                    case 20: // Howard
                        if (!dlgType) dlgType = 3;

                    case 15: // ShopKeeper-W
                    case 24: // Tom
                        if (!dlgType) dlgType = 2;

                    case NPC_PRINCESS: // Princess
                        if (!dlgType) dlgType = 10;

                        EnableDialogBox(20, dlgType, cItemID, sType);
                        //m_dialogBoxes[20].//CentreOverPoint(msX, msY);
                        m_dialogBoxes[20].sV3 = 1;
                        m_dialogBoxes[20].sV4 = m_wCommObjectID;
                        m_dialogBoxes[20].sV5 = m_sMCX;
                        m_dialogBoxes[20].sV6 = m_sMCY;

                        memset(m_dialogBoxes[20].cStr, 0, sizeof(m_dialogBoxes[20].cStr));
                        GetNpcName(sType, m_dialogBoxes[20].cStr);
                        break;

                    default:
                        bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName);
                        break;
                }
                //m_bIsItemDisabled[cItemID] = TRUE;
            }
            m_bIsItemDisabled[cItemID] = true;
        }
    }
    else
    {
        if (((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW))
            && (m_pItemList[cItemID]->m_dwCount > 1))
        {
            m_dialogBoxes[17].m_X = msX - 140;
            m_dialogBoxes[17].m_Y = msY - 70;
            if (m_dialogBoxes[17].m_Y < 0)		m_dialogBoxes[17].m_Y = 0;
            m_dialogBoxes[17].sV1 = 0;
            m_dialogBoxes[17].sV2 = 0;
            m_dialogBoxes[17].sV3 = 0;
            m_dialogBoxes[17].sV4 = 0;
            memset(m_dialogBoxes[17].cStr, 0, sizeof(m_dialogBoxes[17].cStr));
            EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
        }
        else
        {
            if (_ItemDropHistory(m_pItemList[cItemID]->m_cName))
            {
                m_dialogBoxes[4].m_X = msX - 140;
                m_dialogBoxes[4].m_Y = msY - 70;
                if (m_dialogBoxes[4].m_Y < 0)	m_dialogBoxes[4].m_Y = 0;
                m_dialogBoxes[4].sV1 = 0;
                m_dialogBoxes[4].sV2 = 0;
                m_dialogBoxes[4].sV3 = 1;
                m_dialogBoxes[4].sV4 = 0;
                m_dialogBoxes[4].sV5 = cItemID;
                memset(m_dialogBoxes[4].cStr, 0, sizeof(m_dialogBoxes[4].cStr));
                EnableDialogBox(4, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_ITEMDROP, 0, cItemID, 1, 0, m_pItemList[cItemID]->m_cName);
            }
        }
        m_bIsItemDisabled[cItemID] = true;
    }
}


void helbreath::bItemDrop_TopPanel()
{
    if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;
    if (m_cCommand < 0) return;

    char onButton = m_dialogBoxes[30].OnButton();
    if (onButton == 4)
    {
        m_droppedOnIconbar = true;
        bItemDrop_Inventory();
    }
    else if (onButton == 3)
    {
        bItemDrop_Character();
    }
}

void helbreath::bItemDrop_IconPanel()
{
    if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;
    if (m_cCommand < 0) return;

    char onButton = m_dialogBoxes[30].OnButton();
    if (onButton == 4)
    {
        m_droppedOnIconbar = true;
        bItemDrop_Inventory();
    }
    else if (onButton == 3)
    {
        bItemDrop_Character();
    }
}


void helbreath::DlgBoxClick_TopPanel() // New Top Bar xRisenx
{
    char onButton = m_dialogBoxes[62].OnButton();
    switch (onButton)
    {
        case 1: // Combat Mode Toggle
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
            break;
    }
    if (onButton)
        PlaySound('E', 14, 5);
}

void helbreath::DlgBoxClick_IconPanel()
{
    // CLEROTH - LU
/*	if ((msX > 322)	&& (msX < 355) && (434 < msY) && (475 > msY))
    {	if (m_iLU_Point <= 0) return;
        if (m_bIsDialogEnabled[12] == TRUE)
             DisableDialogBox(12);
        else EnableDialogBox(12);
        PlaySound('E', 14, 5);
    }*/
    char onButton = m_dialogBoxes[30].OnButton();
    switch (onButton)
    {
        case 1: // Crusade
            if (m_bIsCrusadeMode == false) return;
            switch (m_iCrusadeDuty)
            {
                case 1: // Fighter
                    EnableDialogBox(38);
                    break;

                case 2: // Constructor
                    EnableDialogBox(37);
                    break;

                case 3: // Commander
                    EnableDialogBox(36);
                    break;
            }
            break;

        case 2: // Combat Mode Toggle
            bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
            break;

        case 3: // Character
            if (m_bIsDialogEnabled[1] == true)
                DisableDialogBox(1);
            else EnableDialogBox(1);
            break;

        case 4: // Inventory
            if (m_bIsDialogEnabled[2] == true)
                DisableDialogBox(2);
            else EnableDialogBox(2);
            break;

        case 5: // Magic
            if (m_bIsDialogEnabled[3] == true)
                DisableDialogBox(3);
            else EnableDialogBox(3);
            break;

        case 6: // Skill
            //if (m_bIsDialogEnabled[15] == TRUE)
            //	 DisableDialogBox(15);
            //else EnableDialogBox(15);
            //break;
            if (m_bIsDialogEnabled[63] == true)
                DisableDialogBox(63);
            else EnableDialogBox(63);
            break;
        case 9: // Title
            if (m_bIsDialogEnabled[70] == true)
                DisableDialogBox(70);
            else EnableDialogBox(70);
            break;

        case 7: // Chat
            if (m_bIsDialogEnabled[10] == true)
                DisableDialogBox(10);
            else EnableDialogBox(10);
            break;

        case 10: // News
            if (m_bIsDialogEnabled[35] == true)
                DisableDialogBox(35);
            else EnableDialogBox(35);
            break;

        case 8: // System Menu
            if (m_bIsDialogEnabled[19] == true)
                DisableDialogBox(19);
            else EnableDialogBox(19);
            break;
    }
    if (onButton)
        PlaySound('E', 14, 5);
}


void helbreath::DrawDialogBox_TopPanel()
{
    uint64_t dwTime = m_dwCurTime;

    char onButton = m_dialogBoxes[62].OnButton();
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[62].m_X, m_dialogBoxes[62].m_Y, 43, dwTime); // Icon Pannel Background

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 4, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506 + 127, 433 + 120, 47, dwTime); // Fixed xRisenx
       //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 5, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
        else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506 + 127, 433 + 120, 46, dwTime); // Fixed xRisenx
    }
    PutAlignedString(103 + 25, 245 + 25, 456 + 120, G_cTxt, Color(255, 250, 250, 220));
}
void helbreath::DrawDialogBox_IconPanel()
{
    uint64_t dwTime = m_dwCurTime;

    char onButton = m_dialogBoxes[30].OnButton();
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[30].m_X, m_dialogBoxes[30].m_Y, 14, dwTime); // Icon Pannel Background
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(m_dialogBoxes[62].m_X, m_dialogBoxes[62].m_Y, 43, dwTime); // Icon Pannel Background

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 4, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(506+127, 433+120, 4, dwTime); // Fixed xRisenx
            m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(12, GetHeight() - 43, 47, dwTime); // Fixed xRisenx
       //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(368+127, 440+120, 5, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
        else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(12, GetHeight() - 43, 46, dwTime); // Fixed xRisenx
    }

    fmt::format_to(G_cTxt, "Contri Pts: %d - Majs Pts: %d", m_iContribution, m_iGizonItemUpgradeLeft);
    PutAlignedString(188, 342, 555, G_cTxt, Color(255, 0, 20, 0));
    //fmt::format_to(G_cTxt, "Def Ratio: %d - Hit Ratio: %d",m_iAC ,m_iTHAC0); // Armor Def dont show, Hitrate Only shows weaps. Fix later xRisenx
    fmt::format_to(G_cTxt, "Def Ratio: %d - Hit Ratio: %d", m_iDefenseRatio, m_iTHAC0); // Hitrate Only shows weaps. Fix later xRisenx
    PutAlignedString(188, 342, 565, G_cTxt, Color(255, 0, 20, 0));

    if ((m_bIsCrusadeMode) && (m_iCrusadeDuty != 0))
    {
        if (m_side == ARESDEN)
        {
            if (onButton == 1)
                //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 1, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 1, dwTime); // Fixed xRisenx
           //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 2, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 2, dwTime); // Fixed xRisenx
        }
        else if (m_side == ELVINE)
        {
            if (onButton == 1)
                //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 0, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
                m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 0, dwTime); // Fixed xRisenx
           //else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 15, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
            else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(129, 554, 15, dwTime); // Fixed xRisenx
        }
    }

    //// CLEROTH - LU
    //if ( m_iLU_Point > 0 ) {
    //	if (onButton == 1)
    //			 m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 17, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
    //		else m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(322+127, 434+120, 18, dwTime); // 800x600 Resolution xRisenx Right? +127 +120
    //}

    short textX = m_stMCursor.sX - 10;
    short textY = m_stMCursor.sY - 20;
    if (onButton == 3)
    { // Character    
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(573, 555, 49, dwTime);
        fmt::format_to(G_cTxt, "Character Menu");
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }
    else if (onButton == 4)
    { // Inventory
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(610, 555, 50, dwTime);
        fmt::format_to(G_cTxt, "Inventory");
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }
    else if (onButton == 5)
    { // Magic
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(647, 555, 51, dwTime);
        fmt::format_to(G_cTxt, "Spell Book");
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }
    else if (onButton == 6)
    { // Skill
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(684, 555, 52, dwTime);
        fmt::format_to(G_cTxt, "Title List"); // Titles xRisenx
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }
    else if (onButton == 7)
    { // History
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(721, 555, 53, dwTime);
        fmt::format_to(G_cTxt, "Chat History");
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }
    else if (onButton == 8)
    { // System Menu
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(758, 555, 54, dwTime);
        fmt::format_to(G_cTxt, "Options");
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }

    // Experience Status Bar xRisenx
    if (onButton == 12)
    {
        fmt::format_to(G_cTxt, MSG_EXP "%d / %d", m_iExp, iGetLevelExp(m_iLevel + 1));
        PutString(textX, textY, G_cTxt, Color(255, 250, 250, 220));
    }

    // Magic Icons xRisenx
    if (m_bPfm)
    {
        fmt::format_to(G_cTxt, "%d", m_sPfm);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 22, dwTime);
    }
    if (m_bPfm)
    {
        fmt::format_to(G_cTxt, "Pfm");
        PutString2(547, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bZerk)
    {
        fmt::format_to(G_cTxt, "%d", m_sZerk);
        PutString2(522, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(513, 560, 21, dwTime);
    }
    if (m_bZerk)
    {
        fmt::format_to(G_cTxt, "Zerk");
        PutString2(517, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bInv)
    {
        fmt::format_to(G_cTxt, "%d", m_sInv);
        PutString2(493, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(484, 560, 20, dwTime);
    }
    if (m_bInv)
    {
        fmt::format_to(G_cTxt, "Inv");
        PutString2(490, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bShield)
    {
        fmt::format_to(G_cTxt, "%d", m_sShield);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 24, dwTime);
    }
    if (m_bShield)
    {
        fmt::format_to(G_cTxt, "Def");
        PutString2(548, 548, G_cTxt, 255, 255, 255);
    }
    if (m_bPfa)
    {
        fmt::format_to(G_cTxt, "%d", m_sPfa);
        PutString2(551, 585, G_cTxt, 255, 255, 255);
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(542, 560, 23, dwTime);
    }
    if (m_bPfa)
    {
        fmt::format_to(G_cTxt, "Pfa");
        PutString2(548, 548, G_cTxt, 255, 255, 255);
    }
    if (bNpcBar)
    {
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(20, 35, 26, dwTime);		// ShadowEvil edited for mouse coordinates
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFast(20, 42, 27, dwTime);		// ShadowEvil edited for mouse coordinates
    }
    fmt::format_to(G_cTxt, "%s(%d,%d)", m_cMapMessage, m_sPlayerX, m_sPlayerY);
    PutAlignedString(188, 342, 576, G_cTxt, Color(255, 250, 250, 220)); // Fixed xRisenx

}

void helbreath::DrawDialogBox_GaugePanel()
{
    uint32_t iMaxPoint, iBarWidth, iTemp;
    // HP bar
    //iMaxPoint = m_stat[STAT_VIT]*3 + m_iLevel*2 + m_stat[STAT_STR]/2;
    iMaxPoint = m_stat[STAT_VIT] * 8 + m_stat[STAT_STR] * 2 + m_stat[STAT_INT] * 2 + m_iLevel * 8; // HP System xRisenx
    if (m_iHP > iMaxPoint) m_iHP = iMaxPoint;
    /*iBarWidth = 101 - (m_iHP*101)/iMaxPoint;
    if( iBarWidth < 0 ) iBarWidth = 0;
    if( iBarWidth > 101 ) iBarWidth = 101;*/
    iBarWidth = 66 - (m_iHP * 66) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 66) iBarWidth = 66;
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 437,  12, iBarWidth, m_dwCurTime);
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 437+120,  12, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(4, 434+120,  12, iBarWidth, m_dwCurTime); // Fixed xRisenx
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 556, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx

    fmt::format_to(G_cTxt, "(%d)", (short)m_iHP);
    if (m_bIsPoisoned)
    {
        //PutString_SprNum(85, 441+120, G_cTxt, m_wR[5]*11, m_wG[5]*11, m_wB[5]*11); // 800x600 Resolution xRisenx Right?
        PutString_SprNum(77, 557, G_cTxt, m_wR[5] * 11, m_wG[5] * 11, m_wB[5] * 11); // Fixed xRisenx
        //PutString_SprFont3(35, 439+120, "Poisoned", m_wR[5]*8, m_wG[5]*8, m_wB[5]*8, TRUE, 2); // 800x600 Resolution xRisenx Right?
        PutString_SprFont3(77, 557, "", m_wR[5] * 8, m_wG[5] * 8, m_wB[5] * 8, true, 2); // Fixed xRisenx
    }
    //else PutString_SprNum(80, 441+120, G_cTxt, 200, 100, 100); // 800x600 Resolution xRisenx Right?
    //else PutString_SprNum(58, 435+120, G_cTxt, 200, 100, 100); // Fixed xRisenx
    else PutString_SprNum(77, 557, G_cTxt, 200, 100, 100); // Fixed xRisenx

    //MP bar
    iMaxPoint = m_stat[STAT_MAG] * 3 + m_iLevel * 2 + m_stat[STAT_INT] * 2; // MP System xRisenx
    if (m_iMP > iMaxPoint) m_iMP = iMaxPoint;
    iBarWidth = 66 - (m_iMP * 66) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 66) iBarWidth = 66;
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 459,  12, iBarWidth, m_dwCurTime);
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(23, 459+120,  12, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 571, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx
    fmt::format_to(G_cTxt, "%d", m_iMP);
    //PutString_SprNum(80, 463+120, G_cTxt, 100, 100, 200);  // 800x600 Resolution xRisenx Right?
    PutString_SprNum(77, 572, G_cTxt, 100, 100, 200);  // Fixed xRisenx

    // SP bar
    iMaxPoint = m_iLevel * 17 + m_stat[STAT_STR]; // SP System xRisenx
    if (m_iSP > iMaxPoint) m_iSP = iMaxPoint;
    //iBarWidth = 167 - (m_iSP*167)/iMaxPoint;
    iBarWidth = 66 - (m_iSP * 66) / iMaxPoint; // Fixed xRisenx
    if (iBarWidth < 0) iBarWidth = 0;
    //if( iBarWidth > 167 ) iBarWidth = 167;
    if (iBarWidth > 66) iBarWidth = 66; // Fixed xRisenx
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147, 434, 13, iBarWidth, m_dwCurTime);
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147, 434+120, 13, iBarWidth, m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(53, 586, 44, iBarWidth, m_dwCurTime); // Fixed xRisenx
    iTemp = m_iSP;
    fmt::format_to(G_cTxt, "%d", iTemp, iMaxPoint);
    //PutString_SprNum(225, 436+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?
    PutString_SprNum(77, 587, G_cTxt, 100, 200, 100); // Fixed xRisenx

    // Experience Gauge
    iMaxPoint = m_levelExpTable[m_iLevel + 1] - m_levelExpTable[m_iLevel];
    iTemp = m_iExp - m_levelExpTable[m_iLevel];
    //iBarWidth = 167 - (iTemp*167)/iMaxPoint;
   //iBarWidth = 800 - (iTemp*800)/iMaxPoint; // Fixed xRisenx
    iBarWidth = (iTemp * 800) / iMaxPoint; // Fixed xRisenx
    if (iBarWidth < 0) iBarWidth = 0;
    //if( iBarWidth > 167 ) iBarWidth = 167;
    if (iBarWidth > 800) iBarWidth = 800; // Fixed xRisenx
    //m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(147 + iBarWidth, 434/*+13*/+120, 13, (167-iBarWidth), m_dwCurTime); // 800x600 Resolution xRisenx Right?
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(0, 427 + 120, 17, iBarWidth, m_dwCurTime); // Fixed xRisenx
    iTemp = iTemp - iMaxPoint;
    //fmt::format_to(G_cTxt, "%d", iTemp);
    //PutString_SprNum(216, 449+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?
    //PutString_SprNum(216, 449+120, G_cTxt, 100, 100, 200); // 800x600 Resolution xRisenx Right?

   // Hunger Bar xRisenx
    iMaxPoint = 100;
    iTemp = iHungerStatus;
    iBarWidth = 42 - (iHungerStatus * 42) / 100;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 42) iBarWidth = 42;
    m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(123, 556, 25, iBarWidth, m_dwCurTime);
    iTemp = iHungerStatus;
    if (bNpcBar)
    {
        iMaxPoint = 100;
        iBarWidth = 66 - (iNpcBar_HP * 66) / iMaxPoint;
        if (iBarWidth < 0) iBarWidth = 0;
        if (iBarWidth > 66) iBarWidth = 66;
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(20, 35, 28, iBarWidth, m_dwCurTime);		// ShadowEvil Edited for Mouse cordinates
        iTemp = iNpcBar_HP;

        iMaxPoint = 100;
        iBarWidth = 66 - (iNpcBar_MP * 66) / iMaxPoint;
        if (iBarWidth < 0) iBarWidth = 0;
        if (iBarWidth > 66) iBarWidth = 66;
        m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL]->PutSpriteFastWidth(20, 42, 28, iBarWidth, m_dwCurTime);		// ShadowEvil Edited for Mouse cordinates
        iTemp = iNpcBar_MP;
    }
}

void helbreath::DrawDialogBox_Slates()
{
    int iAdjX, iAdjY;
    short sX, sY;
    uint64_t dwTime = m_dwCurTime;

    iAdjX = 5;
    iAdjY = 8;

    switch (m_dialogBoxes[40].GetMode())
    {
        // Slates Dialog - Diuuude
        case 1:
            sX = m_dialogBoxes[40].m_X;
            sY = m_dialogBoxes[40].m_Y;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX, sY, 4);

            if (m_dialogBoxes[40].sV1 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 12, 5);
            }
            if (m_dialogBoxes[40].sV2 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 87, 6);
            }
            if (m_dialogBoxes[40].sV3 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 85, sY + 32, 7);
            }
            if (m_dialogBoxes[40].sV4 != -1)
            {
                DrawNewDialogBox(SPRID_INTERFACE_ND_INVENTORY, sX + 70, sY + 97, 8);
            }

            if ((m_dialogBoxes[40].sV1 != -1) && (m_dialogBoxes[40].sV2 != -1) && (m_dialogBoxes[40].sV3 != -1) && (m_dialogBoxes[40].sV4 != -1))
            {
                if (m_dialogBoxes[40].OnButton() == 1)
                    PutString_SprFont(sX + 120, sY + 150, "Casting", 6, 6, 20);
                else PutString_SprFont(sX + 120, sY + 150, "Casting", 0, 0, 7);
            }

            break;

            // Slates Dialog - Diuuude
        case 2:
            PlaySound('E', 16, 0);
            if (m_dialogBoxes[40].cStr[0] != 0)
            {
                sX = m_dialogBoxes[40].m_X + iAdjX + (m_dialogBoxes[40].cStr[0] - (rand() % (m_dialogBoxes[40].cStr[0] * 2)));
                sY = m_dialogBoxes[40].m_Y + iAdjY + (m_dialogBoxes[40].cStr[0] - (rand() % (m_dialogBoxes[40].cStr[0] * 2)));
            }
            else
            {
                sX = m_dialogBoxes[40].m_X;
                sY = m_dialogBoxes[40].m_Y;
            }
            m_pSprite[SPRID_INTERFACE_ND_INVENTORY]->PutSpriteFast(sX, sY, 4, dwTime);
            m_pSprite[SPRID_INTERFACE_ND_INVENTORY]->PutSpriteFast(sX + 22, sY + 14, 3, dwTime);
            //PutString_SprFont(sX + iAdjX + 170, sY + iAdjY + 170, "KURURURURURURURURU!!!", 20,6,6);
            PutAlignedString(199, 438, 201, "KURURURURURURURURU!!!", Color(255, 220, 140, 160));
            PutAlignedString(200, 439, 200, "KURURURURURURURURU!!!", Color(255, 90, 220, 200));

            if ((dwTime - m_dialogBoxes[40].dwT1) > 1000)
            {
                m_dialogBoxes[40].dwT1 = dwTime;
                m_dialogBoxes[40].cStr[0]++;
            }
            if (m_dialogBoxes[40].cStr[0] >= 5)
            {
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_CREATESLATE, 0, m_dialogBoxes[40].sV1, m_dialogBoxes[40].sV2, m_dialogBoxes[40].sV3, 0, m_dialogBoxes[40].sV4);
                DisableDialogBox(40);
            }
            break;
    }
}


void helbreath::DrawDialogBox_ItemDrop()
{
    short sX, sY;
    char cTxt[120], cStr1[64], cStr2[64], cStr3[64];

    char onButton = m_dialogBoxes[4].OnButton();
    sX = m_dialogBoxes[4].m_X;
    sY = m_dialogBoxes[4].m_Y;

    DrawNewDialogBox(SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    GetItemName(m_pItemList[m_dialogBoxes[4].sView]->m_cName, m_pItemList[m_dialogBoxes[4].sView]->m_dwAttribute, cStr1, cStr2, cStr3);

    if (strlen(m_dialogBoxes[4].cStr) == 0)  fmt::format_to(cTxt, "%s", cStr1);

    if (m_bIsSpecial)
    {
        PutString3(sX + 35, sY + 20, cTxt, Color(255, 0, 255, 0));
    }
    else
    {
        PutString3(sX + 35, sY + 20, cTxt, Color(255, 4, 0, 50));
    }
    PutString3(sX + 35, sY + 36, DRAW_DIALOGBOX_ITEM_DROP1, Color(255, 4, 0, 50));

    if (m_bItemDrop)
    {
        if (onButton == 3)
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(255, 255, 255, 255));
        }
        else
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(255, 4, 0, 50));
        }
    }
    else
    {
        if (onButton == 3)
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(255, 255, 255, 255));
        }
        else
        {
            PutString3(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(255, 4, 0, 50));
        }
    }

    if (onButton == 1)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

    if (onButton == 2)
        DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
    else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
}


void helbreath::DlgBoxDoubleClick_Inventory()
{
    int i;
    char  cItemID, cTxt[120];
    short sX, sY, x1, x2, y1, y2;
    char cStr1[64], cStr2[64], cStr3[64];

    short msX = m_stMCursor.sX;
    short msY = m_stMCursor.sY;
    //if (m_iHP <= 0) return;
    if (m_bItemUsingStatus == true)
    {
        AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY1, 10);
        return;
    }
    sX = m_dialogBoxes[2].m_X;
    sY = m_dialogBoxes[2].m_Y;
    for (i = 0; i < MAXITEMS; i++)
    {
        if (m_cItemOrder[MAXITEMS - 1 - i] == -1) continue;
        cItemID = m_cItemOrder[MAXITEMS - 1 - i];
        if (m_pItemList[cItemID] == 0) continue;

        m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
        // Order
        x1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
        y1 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
        x2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
        y2 = (short)m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

        if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
        {	// Order
            _SetItemOrder(0, cItemID);
            GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3);

            if (m_bIsDialogEnabled[11] && (m_bIsDialogEnabled[23] == false) && (m_bIsDialogEnabled[23] == false) && (m_dialogBoxes[39].sV3 == 24))
            {
                if (m_pItemList[cItemID]->m_cEquipPos != EQUIPPOS_NONE)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_REPAIRITEM, 0, cItemID, m_dialogBoxes[39].sV3, 0, m_pItemList[cItemID]->m_cName, m_dialogBoxes[39].sV4);
                    return;
                }
            }

            if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_PERM)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_ARROW)
                || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT))
            {
                if (bCheckItemOperationEnabled(cItemID) == false) return;
                if ((unixtime() - m_dwDamagedTime) < 20 * 1000)
                {
                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 9))
                    {
                        fmt::format_to(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);//"Item %s: Scrolls cannot be used until 10 seconds after taking damage."
                        AddEventList(G_cTxt, 10);
                        return;
                    }
                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 89))
                    {
                        fmt::format_to(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);//"Item %s: Scrolls cannot be used until 10 seconds after taking damage."
                        AddEventList(G_cTxt, 10);
                        return;
                    }
                }
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_REQ_USEITEM, 0, cItemID, 0, 0, 0);

                if ((m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE)
                    || (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EAT))
                {
                    m_bIsItemDisabled[cItemID] = true;
                    m_bItemUsingStatus = true;
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_SKILL)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }
                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);//"You are already using another skill."
                    return;
                }
                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    fmt::format_to(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY7, cStr1);//"Item %s: Select a position which you want to use."
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }
                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY13, 10);//"You are already using another skill."
                    return;
                }
                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    fmt::format_to(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY8, cStr1);//"Item %s: Select an item which you want to use."
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);//"Your hands should be free to use this item."
                    return;
                }

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);//"You are already using another skill."
                    return;
                }

                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);//"You can't use this item because it is exhausted."
                }
                else
                {
                    switch (m_pItemList[cItemID]->m_sSpriteFrame)
                    {
                        case 55: // Alchemy pot
                            //if (m_cSkillMastery[SKILL_ALCHEMY] == 0)
                            //{	AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY9, 10);//"You should learn alchemy skill to use this item."
                            //}else
                            //{	
                            EnableDialogBox(26, 1, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY10, 10);//"Using alchemy skill..."
                        //}
                            break;
                        case 113: // Smith's Anvil
                            //if (m_cSkillMastery[SKILL_MANUFACTURING] == 0)
                            //{	AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY11, 10);//"You should learn manufacturing skill to use this item.."
                            //}/else
                            //{	
                            EnableDialogBox(26, 3, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);//"Using a manufacturing skill..."
                        //}
                            break;

                            // Crafting
                        case 0:
                            //if (m_pItemList[i]->m_sSprite == 22)
                        {
                            EnableDialogBox(26, 7, 0, 0, 0);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY17, 10);	//  "Initiating item Crafting..."
                        }
                        break;

                        case 151:
                        case 152:
                        case 153:
                        case 154:
                            EnableDialogBox(40, 1, 0, 0, 0);
                            break;
                    }
                }
            }

            if (m_bIsDialogEnabled[26] == true)
            {
                if (m_dialogBoxes[26].GetMode() == 1 ||
                    m_dialogBoxes[26].GetMode() == 4 ||
                    m_dialogBoxes[26].GetMode() == 7)
                {
                    bItemDrop_SkillDialog();
                    bItemDrop_ExternalScreen(cItemID, m_dialogBoxes[26].m_X + 50, m_dialogBoxes[26].m_Y + 50);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == ITEMTYPE_EQUIP)
            {
                m_stMCursor.cSelectedObjectType = SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = (short)cItemID;
                bItemDrop_Character();
                m_stMCursor.cSelectedObjectType = 0;
                m_stMCursor.sSelectedObjectID = 0;
            }
            return;
        }
    }
}

