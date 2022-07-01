//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "helbreath.h"
#include "lan_eng.h"

// todo fix this shitfest
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

void helbreath::DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY)
{
	//int ix, iy, indexX, indexY, dX, dY, iDvalue;
	int ix, iy, indexX, indexY, dX, dY, iDvalue, iItemDropPosX[21 + 10][15 + 10], iItemDropPosY[21 + 10][15 + 10], iTempX = 0, iTempY = 0; // Show Item On Ground xRisenx
	//char cItemColor;
	int i;
	char cItemColor, cItemDropList[21 + 10][15 + 10][21], cStr1[64], cStr2[64], cStr3[64]; // Show Item On Ground xRisenx
	bool bIsPlayerDrawed = false;
	bool bContact = false;
	bool bRet = false;
	uint32_t dwItemAttribute, dwItemDropAttr[21 + 10][15 + 10]; // Show Item On Ground xRisenx
	short sItemSprite, sItemSpriteFrame, sObjSpr, sObjSprFrame, sDynamicObject, sDynamicObjectFrame;
	static uint32_t dwMCAnimTime = G_dwGlobalTime;
	static short sMCAnimFrame = 1;
	// Xmas
	static int ix1[100];
	static int iy2[100];
	static int iXmasTreeBulbDelay = 76;
	int idelay = 75;
	uint32_t groundPivotPoint = (m_bigItems) ? SPRID_ITEMPACK_PIVOTPOINT : SPRID_ITEMGROUND_PIVOTPOINT;

	if (sDivY < 0 || sDivX < 0) return;
	m_sMCX = 0;
	m_sMCY = 0;
	memset(m_cMCName, 0, sizeof(m_cMCName));

	//dwTime = G_dwGlobalTime;
	uint64_t dwTime = m_dwCurTime;
	m_stMCursor.sCursorFrame = 0;
	// Show Item On Ground xRisenx
	/*for(iy = 0; iy < 15+10; iy++)
	{	for(ix = 0; ix < 21+10; ix++)
		{	iItemDropPosX[ix][iy] = -1;
			iItemDropPosY[ix][iy] = -1;
			dwItemDropAttr[ix][iy] = NULL;
			ZeroMemory(cItemDropList[ix][iy], sizeof(cItemDropList[ix][iy]));
	}	}*/

	indexY = sDivY + sPivotY - 7;//BESK TODO: keep this?
	//for (iy = -sModY-224; iy <= 427+352; iy += 32)
	for (iy = -sModY - 224; iy <= GetHeight() + 100; iy += 32) // 800x600 xRisenx
	{
		indexX = sDivX + sPivotX - 4;
		//for (ix = -sModX-128 ; ix <= 640 + 128; ix += 32)
		for (ix = -sModX - 128; ix <= GetWidth() + 128; ix += 32) // 800x600 Resolution xRisenx
		{
			sDynamicObject = 0;
			bRet = false;
			//if ((ix >= -sModX) && (ix <= 640+16) && (iy >= -sModY) && (iy <= 427+32+16))
			if ((ix >= -sModX) && (ix <= GetWidth() + 16) && (iy >= -sModY) && (iy <= GetHeight() + 100)) // 800x600 Resolution xRisenx
			{
				_tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_sHeadApprValue = _tmp_sBodyApprValue = _tmp_sArmApprValue = _tmp_sLegApprValue = _tmp_iStatus = 0;
				_tmp_cDir = _tmp_cFrame = 0;
				_tmp_iEffectType = _tmp_iEffectFrame = _tmp_iChatIndex = 0;
				memset(_tmp_cName, 0, sizeof(_tmp_cName));
				if ((indexX < m_pMapData->m_sPivotX) || (indexX > m_pMapData->m_sPivotX + MAPDATASIZEX) ||
					(indexY < m_pMapData->m_sPivotY) || (indexY > m_pMapData->m_sPivotY + MAPDATASIZEY))
				{
					sItemSprite = 0;
					sItemSpriteFrame = 0;
					bRet = false;
					cItemColor = 0;
					//dwItemAttribute = NULL; // 1234
				}
				else
				{
					_tmp_dX = dX = indexX - m_pMapData->m_sPivotX;
					_tmp_dY = dY = indexY - m_pMapData->m_sPivotY; // 296x17 = CRASH POINT. BAD HEADAPPRVALUE
					_tmp_wObjectID = m_pMapData->m_pData[dX][dY].m_wDeadObjectID;
					_tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sDeadOwnerType;
					if (_tmp_sOwnerType == NPC_AS)
						_tmp_sOwnerType = NPC_ESG;

					_tmp_cDir = m_pMapData->m_pData[dX][dY].m_cDeadDir;
					_tmp_sAppr1 = m_pMapData->m_pData[dX][dY].m_sDeadAppr1;
					_tmp_sAppr2 = m_pMapData->m_pData[dX][dY].m_sDeadAppr2;
					_tmp_sAppr3 = m_pMapData->m_pData[dX][dY].m_sDeadAppr3;
					_tmp_sAppr4 = m_pMapData->m_pData[dX][dY].m_sDeadAppr4;
					_tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iDeadApprColor;

					// Variable gets naughty value
					_tmp_sHeadApprValue = m_pMapData->m_pData[dX][dY].m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
					// Bad value is patched to ensure client operation.
					//if(_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0; // this is the ACTUAL max? so that math works? No i can add upto 65k with short :D, we just need a safe value for sprite ID
					// there aren't 65k sprites NO lemme find out :D
					// u so pro even when i do noob stuff to u xD 18 is max
					_tmp_sBodyApprValue = m_pMapData->m_pData[dX][dY].m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
					_tmp_sArmApprValue = m_pMapData->m_pData[dX][dY].m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
					_tmp_sLegApprValue = m_pMapData->m_pData[dX][dY].m_sDeadLegApprValue; // Re-Coding Sprite xRisenx // hmm? no trace beyond dY... :S
					_tmp_cFrame = m_pMapData->m_pData[dX][dY].m_cDeadOwnerFrame;
					_tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iDeadChatMsg;
					_tmp_iStatus = m_pMapData->m_pData[dX][dY].m_iDeadStatus;
					strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cDeadOwnerName.c_str());
					sItemSprite = m_pMapData->m_pData[dX][dY].m_sItemSprite;
					sItemSpriteFrame = m_pMapData->m_pData[dX][dY].m_sItemSpriteFrame;
					//dwItemAttribute = m_pMapData->m_pData[dX][dY].m_dwItemAttribute; // 1234
					cItemColor = m_pMapData->m_pData[dX][dY].m_cItemColor;
					sDynamicObject = m_pMapData->m_pData[dX][dY].m_sDynamicObjectType;
					sDynamicObjectFrame = (short)m_pMapData->m_pData[dX][dY].m_cDynamicObjectFrame;
					cDynamicObjectData1 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData1;
					cDynamicObjectData2 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData2;
					cDynamicObjectData3 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData3;
					cDynamicObjectData4 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData4;
					bRet = true;
				}

				if ((bRet == true) && (sItemSprite != 0) && !m_altPressed)
				{
					if (cItemColor == 0)
						m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
					else
					{
						switch (sItemSprite)
						{
							case 1: // Swds
							case 2: // Bows
							case 3: // Shields
							case 15: // Axes hammers
								m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
									, sItemSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
								break;
							default:
								m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
									, sItemSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
								break;
						}
					}

					if ((ix - 13 < msX) && (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
					{
						if ((dwTime - dwMCAnimTime) > 200)
						{
							dwMCAnimTime = dwTime;
							if (sMCAnimFrame == 1)
								sMCAnimFrame = 2;
							else sMCAnimFrame = 1;
						}
						m_stMCursor.sCursorFrame = sMCAnimFrame;
					}
				}

				if ((bRet == true) && (_tmp_wObjectID != 0))
				{
					bContact = DrawObject_OnDead(indexX, indexY, ix, iy, false, dwTime, msX, msY);
					if (!m_bShiftPressed)
						bContact = false;
				}

				//if ((bRet == TRUE) && (sItemSprite != NULL)/* && m_altPressed*/)
				//{	if (cItemColor == 0) 
				//		m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
				//	else
				//	{	switch (sItemSprite) {
				//		case 1: // Swds
				//		case 2: // Bows
				//		case 3: // Shields
				//		case 15: // Axes hammers
				//		case 17: // Wands
				//			m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
				//				, sItemSpriteFrame, m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], dwTime);
				//			break;
				//		default:
				//			m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
				//				, sItemSpriteFrame, m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], dwTime);
				//			break;
				//	}	}

				//	if (m_bShiftPressed && ((sItemSprite == 6 && sItemSpriteFrame > 8 && !(sItemSpriteFrame >= 17 && sItemSpriteFrame <= 54) && !(sItemSpriteFrame >= 93 && sItemSpriteFrame <= 107)) || sItemSprite != 6/* || memcmp(m_pItemConfigList[sIDnum]->m_cName, "Power", 5) == 0 || memcmp(m_pItemConfigList[sIDnum]->m_cName, "Dilution", 8) == 0*/))
				//	{	iItemDropPosX[iTempX-1][iTempY-1] = ix;
				//		iItemDropPosY[iTempX-1][iTempY-1] = iy;
				//		dwItemDropAttr[iTempX-1][iTempY-1] = dwItemAttribute;
				//		strcpy(cItemDropList[iTempX-1][iTempY-1],  m_pBuildItemListStats[i]->m_cName);
				//	}
				//	
				//	if ((ix - 13 < msX)	&& (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
				//	{	if ((dwTime - dwMCAnimTime)	> 200)
				//		{	dwMCAnimTime = dwTime;
				//			if (sMCAnimFrame == 1)
				//				 sMCAnimFrame = 2;
				//			else sMCAnimFrame = 1;
				//		}
				//		m_stMCursor.sCursorFrame  = sMCAnimFrame;

				//		if (m_bCtrlPressed && !m_bShiftPressed)
				//		{	iItemDropPosX[iTempX-1][iTempY-1] = msX;
				//			iItemDropPosY[iTempX-1][iTempY-1] = msY+30;
				//			dwItemDropAttr[iTempX-1][iTempY-1] = dwItemAttribute;
				//			strcpy(cItemDropList[iTempX-1][iTempY-1], m_pBuildItemListStats[i]->m_cName);
				//		}
				//}	}

				if ((bRet == true) && (sItemSprite != 0) && m_altPressed)
				{
					if (cItemColor == 0)
						m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteFast(ix, iy, sItemSpriteFrame, dwTime);
					else
					{
						switch (sItemSprite)
						{
							case 1: // Swds
							case 2: // Bows
							case 3: // Shields
							case 15: // Axes hammers
								m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
									, sItemSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
								break;
							default:
								m_pSprite[groundPivotPoint + sItemSprite]->PutSpriteRGB(ix, iy
									, sItemSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
								break;
						}
					}

					if ((ix - 13 < msX) && (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
					{
						if ((dwTime - dwMCAnimTime) > 200)
						{
							dwMCAnimTime = dwTime;
							if (sMCAnimFrame == 1)
								sMCAnimFrame = 2;
							else sMCAnimFrame = 1;
						}
						m_stMCursor.sCursorFrame = sMCAnimFrame;
					}
				}

				//if ((bContact == TRUE) && (msY <= 431))
				if ((bContact == true) && (msY <= GetHeight() - 49)) // Resolution Limit, Decides how long Down(y) you can see npcs ( Fixed xRisenx )
				{
					m_sMCX = indexX;
					m_sMCY = indexY;
					sFocusX = ix;
					sFocusY = iy;
					sFocusOwnerType = _tmp_sOwnerType;
					cFocusAction = OBJECTDEAD;
					wFocusObjectID = _tmp_wObjectID;
					cFocusFrame = _tmp_cFrame;
					cFocusDir = _tmp_cDir;
					sFocusAppr1 = _tmp_sAppr1;
					sFocusAppr2 = _tmp_sAppr2;
					sFocusAppr3 = _tmp_sAppr3;
					sFocusAppr4 = _tmp_sAppr4;
					iFocusApprColor = _tmp_iApprColor;
					sFocusHeadApprValue = _tmp_sHeadApprValue; // Re-Coding Sprite xRisenx
					sFocusBodyApprValue = _tmp_sBodyApprValue; // Re-Coding Sprite xRisenx
					sFocusArmApprValue = _tmp_sArmApprValue; // Re-Coding Sprite xRisenx
					sFocusLegApprValue = _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
					iFocuiStatus = _tmp_iStatus;
					memset(cFocusName, 0, sizeof(cFocusName));
					strcpy(cFocusName, _tmp_cName);
					memset(m_cMCName, 0, sizeof(m_cMCName));
					strcpy(m_cMCName, _tmp_cName);
					sFocus_dX = _tmp_dX;
					sFocus_dY = _tmp_dY;
					bContact = false;
				}

				_tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_sHeadApprValue = _tmp_sBodyApprValue = _tmp_sArmApprValue = _tmp_sLegApprValue = _tmp_iStatus = 0; // Re-Coding Sprite xRisenx
				_tmp_cFrame = _tmp_cDir = 0;
				_tmp_iEffectType = _tmp_iEffectFrame = _tmp_iApprColor = _tmp_iChatIndex = 0;
				memset(_tmp_cName, 0, sizeof(_tmp_cName));

				if ((indexX < m_pMapData->m_sPivotX) || (indexX > m_pMapData->m_sPivotX + MAPDATASIZEX) ||
					(indexY < m_pMapData->m_sPivotY) || (indexY > m_pMapData->m_sPivotY + MAPDATASIZEY))
				{
					sItemSprite = 0;
					bRet = false;
				}
				else
				{
					_tmp_dX = dX = indexX - m_pMapData->m_sPivotX;
					_tmp_dY = dY = indexY - m_pMapData->m_sPivotY;
					_tmp_wObjectID = m_pMapData->m_pData[dX][dY].m_wObjectID;
					_tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sOwnerType;
					if (_tmp_sOwnerType == NPC_AS)
						_tmp_sOwnerType = NPC_ESG;

					_tmp_cAction = m_pMapData->m_pData[dX][dY].m_cOwnerAction;
					_tmp_iStatus = m_pMapData->m_pData[dX][dY].m_iStatus;
					_tmp_cDir = m_pMapData->m_pData[dX][dY].m_cDir;
					_tmp_sAppr1 = m_pMapData->m_pData[dX][dY].m_sAppr1;
					_tmp_sAppr2 = m_pMapData->m_pData[dX][dY].m_sAppr2;
					_tmp_sAppr3 = m_pMapData->m_pData[dX][dY].m_sAppr3;
					_tmp_sAppr4 = m_pMapData->m_pData[dX][dY].m_sAppr4;
					_tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iApprColor;
					_tmp_sHeadApprValue = m_pMapData->m_pData[dX][dY].m_sHeadApprValue; // Re-Coding Sprite xRisenx
					_tmp_sBodyApprValue = m_pMapData->m_pData[dX][dY].m_sBodyApprValue; // Re-Coding Sprite xRisenx
					_tmp_sArmApprValue = m_pMapData->m_pData[dX][dY].m_sArmApprValue; // Re-Coding Sprite xRisenx
					_tmp_sLegApprValue = m_pMapData->m_pData[dX][dY].m_sLegApprValue; // Re-Coding Sprite xRisenx
					_tmp_cFrame = m_pMapData->m_pData[dX][dY].m_cOwnerFrame;
					_tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iChatMsg;
					_tmp_iEffectType = m_pMapData->m_pData[dX][dY].m_iEffectType;
					_tmp_iEffectFrame = m_pMapData->m_pData[dX][dY].m_iEffectFrame;

					strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cOwnerName.c_str());
					bRet = true;

					if (m_iIlusionOwnerH != 0)
					{
						if ((strcmp(_tmp_cName, player_name.c_str()) != 0) && (_tmp_sOwnerType < 10))
						{
							_tmp_sOwnerType = m_cIlusionOwnerType;
							if (_tmp_iStatus & STATUS_INVISIBILITY)
							{
								_tmp_iStatus = m_iStatus_IE;
								_tmp_iStatus |= STATUS_INVISIBILITY;
							}
							else
							{
								_tmp_iStatus = m_iStatus_IE;
							}
							_tmp_sAppr1 = m_sAppr1_IE;
							_tmp_sAppr2 = m_cIlusionOwnerType ? m_sAppr2_IE : 0xF000;
							_tmp_sAppr3 = m_sAppr3_IE;
							_tmp_sAppr4 = m_sAppr4_IE;
							_tmp_iApprColor = m_iApprColor_IE;
							_tmp_sHeadApprValue = m_sHeadApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sBodyApprValue = m_sBodyApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sArmApprValue = m_sArmApprValue_IE; // Re-Coding Sprite xRisenx
							_tmp_sLegApprValue = m_sLegApprValue_IE; // Re-Coding Sprite xRisenx
						}
					}
				}

				if ((bRet == true) && (strlen(_tmp_cName) > 0))
				{
					_tmp_dx = 0;
					_tmp_dy = 0;
					switch (_tmp_cAction)
					{
						case OBJECTSTOP:
							bContact = DrawObject_OnStop(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTMOVE:
							bContact = DrawObject_OnMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTDAMAGEMOVE:
							bContact = DrawObject_OnDamageMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTRUN:
							bContact = DrawObject_OnRun(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTATTACK:
							bContact = DrawObject_OnAttack(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTATTACKMOVE:
							bContact = DrawObject_OnAttackMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTMAGIC:
							bContact = DrawObject_OnMagic(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTGETITEM:
							bContact = DrawObject_OnGetItem(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTDAMAGE:
							bContact = DrawObject_OnDamage(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							break;

						case OBJECTDYING:
							bContact = DrawObject_OnDying(indexX, indexY, ix, iy, false, dwTime, msX, msY);
							if (!m_bShiftPressed)
								bContact = false;
							break;
					}

					//if(bContact && msY <= 431)
					if (bContact && msY <= GetHeight() - 49/*551*/) // Resolution Limit, Decides how long Down(y) you can see npcs ( Fixed xRisenx )
					{
						m_sMCX = indexX;
						m_sMCY = indexY;
						sFocusX = ix;
						sFocusY = iy;
						wFocusObjectID = _tmp_wObjectID;
						sFocusOwnerType = _tmp_sOwnerType;
						cFocusAction = _tmp_cAction;
						cFocusFrame = _tmp_cFrame;
						cFocusDir = _tmp_cDir;
						sFocusAppr1 = _tmp_sAppr1;
						sFocusAppr2 = _tmp_sAppr2;
						sFocusAppr3 = _tmp_sAppr3;
						sFocusAppr4 = _tmp_sAppr4;
						iFocusApprColor = _tmp_iApprColor;
						sFocusHeadApprValue = _tmp_sHeadApprValue; // Re-Coding Sprite xRisenx
						sFocusBodyApprValue = _tmp_sBodyApprValue; // Re-Coding Sprite xRisenx
						sFocusArmApprValue = _tmp_sArmApprValue; // Re-Coding Sprite xRisenx
						sFocusLegApprValue = _tmp_sLegApprValue; // Re-Coding Sprite xRisenx
						iFocuiStatus = _tmp_iStatus;
						memset(cFocusName, 0, sizeof(cFocusName));
						strcpy(cFocusName, _tmp_cName);
						memset(m_cMCName, 0, sizeof(m_cMCName));
						strcpy(m_cMCName, _tmp_cName);
						sFocus_dX = _tmp_dX;
						sFocus_dY = _tmp_dY;
						bContact = false;
					}

					if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0)
					{
						if (m_bIsObserverMode == false)
						{
							//m_sViewDstX = (indexX*32) - 288 - 32;
							//m_sViewDstY = (indexY*32) - 224;
							m_sViewDstX = (indexX * 32) - 288 - 32 - 32 - 32; // 800x600 Resolution xRisenx Center Char
							m_sViewDstY = (indexY * 32) - 224 - 32 - 32; // 800x600 Resolution xRisenx Center Char
						}
						SetRect(&m_rcPlayerRect, m_rcBodyRect.left, m_rcBodyRect.top, m_rcBodyRect.right, m_rcBodyRect.bottom);
						bIsPlayerDrawed = true;
					}
				}
			}

			// CLEROTH
			sObjSpr = m_pMapData->m_tile[indexX][indexY].m_sObjectSprite;
			sObjSprFrame = m_pMapData->m_tile[indexX][indexY].m_sObjectSpriteFrame;

			if (sObjSpr != 0)
			{
				if ((sObjSpr < 100) || (sObjSpr >= 200))
				{
					switch (sObjSpr)
					{
						case 200:
						case 223:
							m_pTileSpr[sObjSpr]->PutShadowSprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
							break;

						case 224:
							switch (sObjSprFrame)
							{
								case 24:
								case 34:
								case 35:
								case 36:
								case 37:
								case 38:
									break;
								default:
									m_pTileSpr[sObjSpr]->PutShadowSprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
									break;
							}
					}
					if (m_cDetailLevel == 0) // Special Grass & Flowers
					{
						if ((sObjSpr != 6) && (sObjSpr != 9))
							m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					}
					else
					{
						m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					}

					switch (sObjSpr)
					{
						case 200:
							if (sObjSprFrame == 32)
							{
								if (G_cSpriteAlphaDegree == 2)
								{
									int iDvalue1 = -1 * (rand() % 5);
									m_pEffectSpr[0]->PutTransSpriteRGB(ix - 2, iy - 75, 1, iDvalue1, iDvalue1, iDvalue1, dwTime);
								}
							}
							break;
						case 223:
							if (sObjSprFrame == 4)
							{
								if (G_cSpriteAlphaDegree == 2) //nuit
								{
									int iDvalue1 = -1 * (rand() % 5);
									int iDvalue2 = -1 * (rand() % 5);
									int iDvalue3 = -1 * (rand() % 5);
									m_pEffectSpr[0]->PutTransSpriteRGB(ix - 2, iy - 150, 1, iDvalue1, iDvalue1, iDvalue1, dwTime);
									m_pEffectSpr[0]->PutTransSpriteRGB(ix + 16, iy - 96, 1, iDvalue2, iDvalue2, iDvalue2, dwTime);
									m_pEffectSpr[0]->PutTransSpriteRGB(ix - 19, iy - 126, 1, iDvalue3, iDvalue3, iDvalue3, dwTime);
								}
							}
							break;

						case 370: // nuit
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 16 + 30, m_sViewPointY + iy - 16 - 334, 0, 0, 0, 0);
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 16 + 17, m_sViewPointY + iy - 16 - 300, 0, 0, 0, 0);
							break;

						case 374: // nuit
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 7, m_sViewPointY + iy - 122, 0, 0, 0, 0);
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 6) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 14, m_sViewPointY + iy - 321, 0, 0, 0, 0);
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 7) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix + 7, m_sViewPointY + iy - 356, 0, 0, 0, 0);
							break;

						case 376: // nuit
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 12) && (G_cSpriteAlphaDegree == 2))
							{
								bAddNewEffect(65, m_sViewPointX + ix - 16, m_sViewPointY + iy - 346, 0, 0, 0, 0);
								bAddNewEffect(65, m_sViewPointX + ix + 11, m_sViewPointY + iy - 308, 0, 0, 0, 0);
							}
							break;

						case 378: // nuit
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix, m_sViewPointY + iy - 91, 0, 0, 0, 0);
							break;

						case 382: // nuit
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2))
							{
								bAddNewEffect(65, m_sViewPointX + ix + 73, m_sViewPointY + iy - 264, 0, 0, 0, 0);
								bAddNewEffect(65, m_sViewPointX + ix + 23, m_sViewPointY + iy - 228, 0, 0, 0, 0);
							}
							break;

						case 429:
							if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2)) bAddNewEffect(65, m_sViewPointX + ix - 15, m_sViewPointY + iy - 224, 0, 0, 0, 0);
							break;
					}
				}
				else // sprites 100..199: Trees and tree shadows
				{
					m_pTileSpr[sObjSpr]->_GetSpriteRect(ix - 16, iy - 16, sObjSprFrame);
					//if (m_cDetailLevel==0)
					//{	
					if (sObjSpr < 100 + 11) m_pTileSpr[100 + 4]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					else if (sObjSpr < 100 + 23) m_pTileSpr[100 + 9]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					else if (sObjSpr < 100 + 32) m_pTileSpr[100 + 23]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					else m_pTileSpr[100 + 32]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//}else
					//{	
					//	//Druncncity crash fixed
					//	if ((bIsPlayerDrawed == TRUE) && (m_pTileSpr[sObjSpr] != 0) && (m_pTileSpr[sObjSpr]->m_rcBound.top <= m_rcPlayerRect.top) && (m_pTileSpr[sObjSpr]->m_rcBound.bottom >= m_rcPlayerRect.bottom) &&
					//		(m_cDetailLevel >= 2) && (m_pTileSpr[sObjSpr]->m_rcBound.left <= m_rcPlayerRect.left) && (m_pTileSpr[sObjSpr]->m_rcBound.right >= m_rcPlayerRect.right))
					//	{	
					//		m_pTileSpr[sObjSpr + 50]->PutFadeSprite(ix , iy , sObjSprFrame, dwTime);
					//		m_pTileSpr[sObjSpr]->PutTransSprite2(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//	}else
					//	{	
					//		m_pTileSpr[sObjSpr + 50]->PutSpriteFast(ix , iy , sObjSprFrame, dwTime);
					//		m_pTileSpr[sObjSpr]->PutSpriteFast(ix - 16, iy - 16, sObjSprFrame, dwTime);
					//	}

					//	if(m_bIsXmas && G_cSpriteAlphaDegree == 2) // nuit
					//	{	
					//		if( iXmasTreeBulbDelay < 0 || iXmasTreeBulbDelay > idelay + 1) iXmasTreeBulbDelay = 0;
					//		if( iXmasTreeBulbDelay > idelay )
					//		{	
					//			for (int i = 0; i < 100; i++) 
					//			{
					//				ix1[i] = 1*(rand() % 400)-200;
					//				iy2[i] = -1*(rand() % 300);
					//			}
					//			iXmasTreeBulbDelay = 0;
					//		}	
					//		else 
					//			iXmasTreeBulbDelay++;

					//		for (int j = 0; j < 100; j++)
					//		{	
					//			if( m_pTileSpr[sObjSpr]->_bCheckCollison(ix-16, iy-16, sObjSprFrame, ix + ix1[j], iy + iy2[j]) )
					//			{	
					//				m_pEffectSpr[66+(j%6)]->PutTransSprite(ix + ix1[j], iy + iy2[j], (iXmasTreeBulbDelay>>2), dwTime);
					//			}	
					//		}	
					//	}
					//}	
				}
			}

			// Dynamic Object
			if ((bRet == true) && (sDynamicObject != 0))
			{
				switch (sDynamicObject)
				{
					case DYNAMICOBJECT_PCLOUD_BEGIN:	// 10
						if (sDynamicObjectFrame >= 0)
							m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame, dwTime);
						break;

					case DYNAMICOBJECT_PCLOUD_LOOP:		// 11
						m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame + 8, dwTime);
						break;

					case DYNAMICOBJECT_PCLOUD_END:		// 12
						m_pEffectSpr[23]->PutTransSprite50_NoColorKey(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame + 16, dwTime);
						break;

					case DYNAMICOBJECT_ICESTORM:		// 8
						iDvalue = (rand() % 5) * (-1);
						m_pEffectSpr[0]->PutTransSpriteRGB(ix, iy, 1, iDvalue, iDvalue, iDvalue, dwTime);
						m_pEffectSpr[13]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame, dwTime);
						break;

					case DYNAMICOBJECT_FIRE:			// 1
					case DYNAMICOBJECT_FIRE3:			// 14
						switch (rand() % 3)
						{
							case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(ix, iy, 1, dwTime); break;
							case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(ix, iy, 1, dwTime); break;
							case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(ix, iy, 1, dwTime); break;
						}
						m_pEffectSpr[9]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame / 3, dwTime);
						break;

					case DYNAMICOBJECT_FIRE2:			// 13
						switch (rand() % 3)
						{
							case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(ix, iy, 1, dwTime); break;
							case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(ix, iy, 1, dwTime); break;
							case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(ix, iy, 1, dwTime); break;
						}
						break;

					case DYNAMICOBJECT_FISH:			// 2
					{
						char cTmpDOdir, cTmpDOframe;
						cTmpDOdir = m_Misc.cCalcDirection(cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData1 + cDynamicObjectData3, cDynamicObjectData2 + cDynamicObjectData4);
						cTmpDOframe = ((cTmpDOdir - 1) * 4) + (rand() % 4);
						m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 0]->PutTransSprite2(ix + cDynamicObjectData1, iy + cDynamicObjectData2, cTmpDOframe, dwTime);
					}
					break;

					case DYNAMICOBJECT_MINERAL1:		// 4
						if (m_cDetailLevel != 0) m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->PutShadowSprite(ix, iy, 0, dwTime);
						m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->PutSpriteFast(ix, iy, 0, dwTime);
						if ((m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top != -1)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top < msY)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.bottom > msY)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.left < msX)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.right > msX))
						{
							m_sMCX = indexX;
							m_sMCY = indexY;
							iFocuiStatus = 0;
							memset(cFocusName, 0, sizeof(cFocusName));
							memset(m_cMCName, 0, sizeof(m_cMCName));
						}
						break;

					case DYNAMICOBJECT_MINERAL2:		// 5
						if (m_cDetailLevel != 0) m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->PutShadowSprite(ix, iy, 1, dwTime);
						m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->PutSpriteFast(ix, iy, 1, dwTime);
						if ((m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top != -1)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top < msY)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.bottom > msY)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.left < msX)
							&& (m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.right > msX))
						{
							m_sMCX = indexX;
							m_sMCY = indexY;
							iFocuiStatus = 0;
							memset(cFocusName, 0, sizeof(cFocusName));
							memset(m_cMCName, 0, sizeof(m_cMCName));
						}
						break;

					case DYNAMICOBJECT_SPIKE:			// 9
						m_pEffectSpr[17]->PutTransSprite70_NoColorKey(ix, iy, sDynamicObjectFrame, dwTime);
						break;

					case DYNAMICOBJECT_ARESDENFLAG:  // 6
					case DYNAMICOBJECT_ELVINEFLAG: // 7
						m_pSprite[SPRID_ITEMDYNAMIC_PIVOTPOINT + 2]->PutSpriteFast(ix, iy, sDynamicObjectFrame, dwTime);
						break;
				}
			}
			indexX++;
		}
		indexY++;
	}

	if ((dwTime - m_dwEnvEffectTime) > 400) m_dwEnvEffectTime = dwTime;

	if (m_sMCX != 0)	// CLEROTH - STATUS
	{
		if (_iGetFOE(iFocuiStatus) < 0) m_stMCursor.sCursorFrame = 3;
		else m_stMCursor.sCursorFrame = 6;

		_tmp_wObjectID = wFocusObjectID;
		_tmp_sOwnerType = sFocusOwnerType;

		if (_tmp_sOwnerType == NPC_AS)
			_tmp_sOwnerType = NPC_ESG;

		_tmp_cAction = cFocusAction;
		_tmp_cFrame = cFocusFrame;
		_tmp_cDir = cFocusDir;
		_tmp_sAppr1 = sFocusAppr1;
		_tmp_sAppr2 = sFocusAppr2;
		_tmp_sAppr3 = sFocusAppr3;
		_tmp_sAppr4 = sFocusAppr4;
		_tmp_iApprColor = iFocusApprColor;
		//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
		_tmp_sHeadApprValue = sFocusHeadApprValue; // Re-Coding Sprite xRisenx // Maybe something like this to patch the crash
		_tmp_sBodyApprValue = sFocusBodyApprValue; // Re-Coding Sprite xRisenx
		_tmp_sArmApprValue = sFocusArmApprValue; // Re-Coding Sprite xRisenx
		_tmp_sLegApprValue = sFocusLegApprValue; // Re-Coding Sprite xRisenx
		_tmp_iStatus = iFocuiStatus;
		strcpy(_tmp_cName, cFocusName);
		_tmp_dX = sFocus_dX;
		_tmp_dY = sFocus_dY;

		if ((_tmp_cAction != OBJECTDEAD) && (_tmp_cFrame < 0)) return;
		switch (_tmp_cAction)
		{
			case OBJECTSTOP:
				DrawObject_OnStop(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;
			case OBJECTMOVE:
				switch (_tmp_sOwnerType)
				{
					case 1:
					case 2:
					case 3: // Human M
					case 4:
					case 5:
					case 6: // Human F

					case 28: // Troll.
					case 29: // Ogre
					case 30: // Liche
					case 31: // DD
					case 32: // Uni
					case 33: // WW
					case 43: // LWB
					case 44: // GHK
					case 45: // GHKABS
					case 46: // TK
					case 47: // BG
					case 48: // SK
					case 49: // HC
					case 50: // TW
					case 51: // CP
					case 52: // GG
					case 53: // BB
					case 54: // DE
					case 55: // Rabbit
					case 56: // Cat
					case 57: // Frog
					case 58: // MG
					case 59: // Ettin
					case 60: // Plant
					case 61: // Rudolph
					case 62: // DireBoar
					case 63: // Frost
					case 65: // Ice-Golem
					case 66: // Wyvern
					case 70: // Dragon..........Ajouts par Snoopy
					case 71: // Centaur
					case 72: // ClawTurtle
					case 73: // FireWyvern
					case 74: // GiantCrayfish
					case 75: // Gi Lizard
					case 76: // Gi Tree
					case 77: // Master Orc
					case 78: // Minaus
					case 79: // Nizie
					case 80: // Tentocle
					case 81: // Abaddon
					case 82: // Sorceress
					case 83: // ATK
					case 84: // MasterElf
					case 85: // DSK
					case 86: // HBT
					case 87: // CT
					case 88: // Barbarian
					case 89: // AGC
					case 91: // Gate
					//case 103: // Orc Lord
					case 104: // Dark Unicorn
					case 105: // Black Demon
					case 106: // Earth Wyvern
					case 107: // Wind Wyvern
					case 108: // Dark Wyvern
					case 109: // Grass Wyvern
					case 110: // Sky Wyvern
					case 111: // Sea Wyvern
					case 112: // Metal Wyvern
					case 113: // Void Wyvern
					case 114: // Eternal Dragon
					//case 115: // Mother Dragon
					//case 116: // Hell Dragon
					//case 117: // Alligator
					//case 118: // Puma
					//case 119: // Zombie King
					//case 120: // Slime King
					//case 121: // Slime Priness
					//case 122: // Enraged Troll
					case 123: // Enraged Cyclops
					case 124: // Enraged Stalker
					case 125: // Enraged Gagoyle
					case 126: // Enraged Hellclaw
					case 127: // Enraged Tigerworm

				/*	case 15: // ShopKeeper // Ils ont 8 sprites Stop
					case 19: // Gandalf
					case 20: // Howard
					case 24: // Tom
					case 25: // William
					case 26: // Kenedy
					case 90: // Gail*/
						break;

					default: // 10..27
						_tmp_cFrame = _tmp_cFrame * 2; //
						break;
				}

				DrawObject_OnMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTDAMAGEMOVE:
				DrawObject_OnDamageMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTRUN:
				DrawObject_OnRun(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTATTACK:
				DrawObject_OnAttack(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTATTACKMOVE:
				DrawObject_OnAttackMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTMAGIC:
				DrawObject_OnMagic(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTDAMAGE:
				DrawObject_OnDamage(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTDYING: //10
				DrawObject_OnDying(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;

			case OBJECTDEAD: //101
				DrawObject_OnDead(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
				break;
		}
	}
	// Show Gr
	//if (!bObjSprRange) bSetSprRangeObj(-1, true);

	/*for(iy = 0; iy < 15+10; iy++)
	{	for(ix = 0; ix < 21+10; ix++)
		{	if (iItemDropPosX[ix][iy] != -1 && iItemDropPosY[ix][iy] != -1) {
				iTempX = iTempY = 0;
				GetItemName(cItemDropList[ix][iy], dwItemDropAttr[ix][iy], cStr1, cStr2, cStr3);
				if (strlen(cStr2) > 0) {
					iTempX = strlen(cStr2);
					iTempY++;
				}
				if (strlen(cStr3) > 0) {
					if (strlen(cStr3) > iTempX) iTempX = strlen(cStr3);
					iTempY++;
				}
				//DIRECTX m_DDraw.DrawShadowBox(iItemDropPosX[ix][iy]+2, iItemDropPosY[ix][iy]+2, iItemDropPosX[ix][iy]+4+iTempX*11/5, iItemDropPosY[ix][iy]+4+iTempY*15);
				iTempY = 0;
				PutString(iItemDropPosX[ix][iy]+4, iItemDropPosY[ix][iy]+4, cStr1,Color(255,250,0,0));
				if (strlen(cStr2) > 0) {
					PutString(iItemDropPosX[ix][iy]+6, iItemDropPosY[ix][iy]+4+15, cStr2,Color(255,0,250,0));
					iTempY = 1;
				}
				if (strlen(cStr3) > 0) PutString(iItemDropPosX[ix][iy]+6, iItemDropPosY[ix][iy]+4+15+15*iTempY, cStr3,Color(255,0,250,0));
			}
	}	}*/

	if (m_bIsGetPointingMode == true)
	{
		if ((m_iPointCommandType >= 100) && (m_iPointCommandType < 200)) // spell
		{
			if (m_bCommandAvailable == true)
			{
				if (m_sMCX != 0)
				{
					if (_iGetFOE(iFocuiStatus) < 0)
						m_stMCursor.sCursorFrame = 5;   // Red enemi for spell
					else m_stMCursor.sCursorFrame = 4;  // Blue friend for spell
				}
				else m_stMCursor.sCursorFrame = 4;     // Blue friend for spell
			}
			else m_stMCursor.sCursorFrame = 8;
		}
		else if ((m_iPointCommandType >= 0) && (m_iPointCommandType < 50)) // item
		{
			m_stMCursor.sCursorFrame = 10;				// hand to grap item
		}
	}
}


bool helbreath::DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}
	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
				if (iWeapon == 0) iAdd = 6;
				if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
				if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/

				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/

				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 5;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
				iWeaponIndex = -1;
				iShieldIndex = -1;
			}
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
				if (iWeapon == 0) iAdd = 6;
				if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
				if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/

				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0) // So its tmp that gets a wrong value somewhere
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd; // 4112 // Should be 10 thats why iHelmIndex is wierd.. cause you take 4112 *15 = iHelmIndex
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 5;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/

				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
				iWeaponIndex = -1;
				iShieldIndex = -1;
			}
			break;

		default:
			if (_tmp_sAppr2 != 0)
			{
				iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
				_tmp_cFrame = _tmp_sAppr2 - 1;
			}
			else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
			else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
			else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
			else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
			else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
			else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
			else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
			else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
			else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
			else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
			else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
			else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iBodyArmorIndex = -1;
			iArmArmorIndex = -1;
			iBootsIndex = -1;
			iPantsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode ||
		(m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0))
	{
		DrawObjectFOE(sX, sY, _tmp_cFrame);
	}

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // FireWyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
				{
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
				}
				else
				{
					m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
				if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			}

			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			}
			else if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (((_tmp_sAppr2 & 0x000F) == 8) && (iShieldGlare == 1))
			{
				m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
			}
			else
				if (iShieldIndex != -1)
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
					switch (iShieldGlare)
					{
						case 0: break;
							//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
						case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}
		}
		else
		{
			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			}
			else if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iShieldIndex != -1)
			{
				if (iShieldColor == 0)
					m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
				else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
				if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);


		if ((_tmp_iStatus & 0x20) != 0) // Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel((_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 8, dwTime);
		CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}

	// Snoopy: Abaddon effects
	if (_tmp_sOwnerType == 81)
	{
		int randFrame = _tmp_cFrame % 12;
		m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
		m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
		m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
		m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
		m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
		m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
		switch (_tmp_cDir)
		{
			case 1:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 2:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 3:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 4:
				m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 5:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 6:
				m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 7:
				m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 8:
				m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
				break;
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex, dx, dy, dsx, dsy;
	int cFrameMoveDots;
	bool bInv = false, bDashDraw = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}

	switch (_tmp_cFrame)
	{
		case 4:  _tmp_cFrame = 4; break;
		case 5:  _tmp_cFrame = 4; break;
		case 6:  _tmp_cFrame = 4; break;
		case 7:  _tmp_cFrame = 4; break;
		case 8:  _tmp_cFrame = 4; break;
		case 9:  _tmp_cFrame = 4; break;
		case 10: _tmp_cFrame = 5; break;
		case 11: _tmp_cFrame = 6; break;
		case 12: _tmp_cFrame = 7; break;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
				if (iWeapon == 0) iAdd = 6;
				if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
				if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 5;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
				iWeaponIndex = -1;
				iShieldIndex = -1;
			}
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
				if (iWeapon == 0) iAdd = 6;
				if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
				if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 5;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 5;*/
				}
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 5;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 5;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 5;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 5;*/
				iWeaponIndex = -1;
				iShieldIndex = -1;
			}
			break;
		default:
			iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iBodyArmorIndex = -1;
			iArmArmorIndex = -1;
			iBootsIndex = -1;
			iPantsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	dx = 0;
	dy = 0;
	if ((_tmp_cFrame >= 1) && (_tmp_cFrame <= 3))
	{
		switch (_tmp_cFrame)
		{
			case 1: cFrameMoveDots = 26; break;
			case 2: cFrameMoveDots = 16; break;
			case 3: cFrameMoveDots = 0;  break;
		}
		switch (_tmp_cDir)
		{
			case 1: dy = cFrameMoveDots; break;
			case 2: dy = cFrameMoveDots; dx = -cFrameMoveDots; break;
			case 3: dx = -cFrameMoveDots; break;
			case 4: dx = -cFrameMoveDots; dy = -cFrameMoveDots; break;
			case 5: dy = -cFrameMoveDots; break;
			case 6: dy = -cFrameMoveDots; dx = cFrameMoveDots; break;
			case 7: dx = cFrameMoveDots; break;
			case 8: dx = cFrameMoveDots; dy = cFrameMoveDots; break;
		}
		switch (_tmp_cFrame)
		{
			case 1: dy++;    break;
			case 2: dy += 2; break;
			case 3: dy++;    break;
		}
		switch (_tmp_cFrame)
		{
			case 2: bDashDraw = true; cFrameMoveDots = 26; break;
			case 3: bDashDraw = true; cFrameMoveDots = 16; break;
		}
		dsx = 0;
		dsy = 0;
		switch (_tmp_cDir)
		{
			case 1: dsy = cFrameMoveDots; break;
			case 2: dsy = cFrameMoveDots; dsx = -cFrameMoveDots; break;
			case 3: dsx = -cFrameMoveDots; break;
			case 4: dsx = -cFrameMoveDots; dsy = -cFrameMoveDots; break;
			case 5: dsy = -cFrameMoveDots; break;
			case 6: dsy = -cFrameMoveDots; dsx = cFrameMoveDots; break;
			case 7: dsx = cFrameMoveDots; break;
			case 8: dsx = cFrameMoveDots; dsy = cFrameMoveDots; break;
		}
	}
	else if (_tmp_cFrame > 3)
	{
		dx = 0;
		dy = 0;
	}
	else
	{
		switch (_tmp_cDir)
		{
			case 1: dy = 32; break;
			case 2: dy = 32; dx = -32; break;
			case 3: dx = -32; break;
			case 4: dx = -32; dy = -32; break;
			case 5: dy = -32; break;
			case 6: dy = -32; dx = 32; break;
			case 7: dx = 32; break;
			case 8: dx = 32; dy = 32; break;
		}
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode ||
		(m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0))
	{
		DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);
	}

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
				if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			}

			if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iShieldIndex != -1)
			{
				if (iShieldColor == 0)
					m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

				else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}
		}
		else
		{
			if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}


			if (iShieldIndex != -1)
			{
				if (iShieldColor == 0)
					m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
				if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 6, -2, 6, dwTime);

		// Berserk
		if ((_tmp_iStatus & 0x20) != 0)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(8 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 8, dwTime);
		CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

		if (bDashDraw == true)
		{
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			if (iWeaponIndex != -1) m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
			if (iShieldIndex != -1) m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dsx, sY + dsy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
		}
	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
	}

	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;

	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

	return false;
}

bool helbreath::DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
	bool bInv = false;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}

	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0)
			bInv = true;
		else
		{
			if (_tmp_iChatIndex != 0)
			{
				if (m_pChatMsgList[_tmp_iChatIndex] != 0)
				{
					m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
					m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
				}
				else
				{
					m_pMapData->ClearChatMsg(indexX, indexY);
				}
			}
			return false;
		}
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 8;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 8;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 8;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 8;*/

			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 8;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 8;*/
			break;
		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 8;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 8;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 8;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 8;*/

			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 8;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 8;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 8;*/
			break;
	}

	/*
	switch (_tmp_cFrame) {
	case 15:
		_tmp_cFrame = 14;
		break;
	case 16:
		_tmp_cFrame = 14;
		break;
	case 17:
		_tmp_cFrame = 15;
		break;
	case 18:
		_tmp_cFrame = 15;
		break;
	case 19:
		_tmp_cFrame = 15;
		break;
	case 20:
		_tmp_cFrame = 15;
		break;
	case 21:
		_tmp_cFrame = 15;
		break;
	case 22:
		_tmp_cFrame = 15;
		break;
	case 23:
		_tmp_cFrame = 14;
		break;
	}
	*/
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);
	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 127: // Enraged Tigerworm
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

		if (bInv == true)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
		else
		{
			if ((_tmp_iStatus & STATUS_FROZEN) != 0)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
			else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
		}
		SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

		if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, iR, iG, iB, dwTime);
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}

		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}

		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}

		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}

		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}

		if (iMantleIndex != -1)
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

		if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(32 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 16, dwTime);
		CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
	bool bInv = false;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}

	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 9;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 9;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 9;*/

			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 9;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 9;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 9; 		*/
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 9;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 9;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 9;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 9;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 9;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 9;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 9;*/
			break;
		default:
			iUndiesIndex = -1;
			iHairIndex = -1;
			iArmArmorIndex = -1;
			iBodyArmorIndex = -1;
			iPantsIndex = -1;
			iBootsIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 127: // Enraged Tigerworm
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

		if (bInv == true)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX, sY, _tmp_cFrame, dwTime);
		else
		{
			if ((_tmp_iStatus & STATUS_FROZEN) != 0)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
			else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
		}
		SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

		if (iUndiesIndex != -1)
		{
			if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
		}

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, iR, iG, iB, dwTime);
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}
		}

		if (iPantsIndex != -1)
		{
			if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}
		}

		if (iArmArmorIndex != -1)
		{
			if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}
		}

		if (iBodyArmorIndex != -1)
		{
			if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}
		}

		if (iHelmIndex != -1)
		{
			if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}
		}

		if (iMantleIndex != -1)
		{
			if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
			else
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

		if ((_tmp_iStatus & 0x20) != 0) // Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(40 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 4, dwTime);
		CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB;
	int iAdd, iDrawMode, iMantleIndex;
	char cFrame;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 81 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}
	cFrame = _tmp_cFrame;
	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if (cFrame < 4)
			{
				if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
				else iAdd = 0;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
				iDrawMode = 0;
			}
			else
			{
				cFrame -= 4;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 10;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
				iDrawMode = 1;
			}
			break;
		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if (cFrame < 4)
			{
				if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
				else iAdd = 0;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
				iDrawMode = 0;
			}
			else
			{
				cFrame -= 4;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 10;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 10;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
				iDrawMode = 1;
			}
			break;

		default:
			if (cFrame < 4)
			{
				if (_tmp_sAppr2 != 0)
				{
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					cFrame = _tmp_sAppr2 - 1;
				}
				else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 67) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 68) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 69) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
				else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
				else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
				else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
				else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
				else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
				else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
				else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
				else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			}
			else
			{
				cFrame -= 4;
				if (_tmp_sAppr2 != 0)
				{
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					cFrame = _tmp_sAppr2 - 1;
				}
				else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 67) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 68) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 69) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
				else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
				else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
				else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
				else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
				else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
				else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
				else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
				else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
				else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
			}
			iUndiesIndex = -1;
			iHairIndex = -1;
			iArmArmorIndex = -1;
			iBodyArmorIndex = -1;
			iPantsIndex = -1;
			iBootsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			iDrawMode = 0;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, cFrame);

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 127: // Enraged Tigerworm
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
		if (iDrawMode == 1) // Etrange, 1 semble impossible avec des mobs !
		{
			if (_cDrawingOrder[_tmp_cDir] == 1)
			{
				if (iWeaponIndex != -1)
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
					DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
					switch (iWeaponGlare)
					{
						case 0: break;
						case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

				if (_tmp_sOwnerType == 35)
					m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

				if (_tmp_sOwnerType == 81) // Abaddon
				{
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				}
				else if (bInv == true)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				else
				{
					if ((_tmp_iStatus & STATUS_FROZEN) != 0)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
				}
				SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

				if ((iHairIndex != -1) && (iHelmIndex == -1))
				{
					_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
					m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 1))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iPantsIndex != -1)
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}

				if (iArmArmorIndex != -1)
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 0))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iBodyArmorIndex != -1)
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}

				if (iHelmIndex != -1)
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iShieldIndex != -1)
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
					switch (iShieldGlare)
					{
						case 0: break;
							//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
						case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}
			else
			{
				if (_tmp_sOwnerType == 35)
					m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

				if (_tmp_sOwnerType == 81) // Abaddon
				{
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				}
				else if (bInv == true)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				else
				{
					if ((_tmp_iStatus & STATUS_FROZEN) != 0)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
				}
				SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

				if ((iHairIndex != -1) && (iHelmIndex == -1))
				{
					_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
					m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 1))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iPantsIndex != -1)
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}

				if (iArmArmorIndex != -1)
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 0))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iBodyArmorIndex != -1)
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}

				if (iHelmIndex != -1)
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iShieldIndex != -1)
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
					switch (iShieldGlare)
					{
						case 0: break;
							//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
						case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iWeaponIndex != -1)
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
					DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
					switch (iWeaponGlare)
					{
						case 0: break;
						case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}
			}

			if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

			if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
			DrawAngel(16 + (_tmp_cDir - 1), sX + 20, sY - 20, cFrame % 4, dwTime);
			CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

		}
		else // DrawMode != 1
		{
			if (_cDrawingOrder[_tmp_cDir] == 1)
			{
				if (iWeaponIndex != -1)
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
					DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
					switch (iWeaponGlare)
					{
						case 0: break;
						case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}
				if (_tmp_sOwnerType == 35)
					m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

				if (_tmp_sOwnerType == 81) // Abaddon
				{
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				}
				else if (bInv == true)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				else
				{
					if ((_tmp_iStatus & STATUS_FROZEN) != 0)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
				}
				SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

				if ((iHairIndex != -1) && (iHelmIndex == -1))
				{
					_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
					m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 1))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iPantsIndex != -1)
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}

				if (iArmArmorIndex != -1)
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 0))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iBodyArmorIndex != -1)
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}

				if (iHelmIndex != -1)
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iShieldIndex != -1)
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
					switch (iShieldGlare)
					{
						case 0: break;
							//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
						case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}
			else
			{
				if (_tmp_sOwnerType == 35)
					m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

				if (_tmp_sOwnerType == 81) // Abaddon
				{
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				}
				else if (bInv == true)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
				else
				{
					if ((_tmp_iStatus & STATUS_FROZEN) != 0)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
				}

				SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

				if ((iHairIndex != -1) && (iHelmIndex == -1))
				{
					_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
					m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 1))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iPantsIndex != -1)
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}

				if (iArmArmorIndex != -1)
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}

				if ((iBootsIndex != -1) && (iSkirtDraw == 0))
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}

				if (iBodyArmorIndex != -1)
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}

				if (iHelmIndex != -1)
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iShieldIndex != -1)
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
					switch (iShieldGlare)
					{
						case 0: break;
							//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
						case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}

				if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}

				if (iWeaponIndex != -1)
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
					DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
					switch (iWeaponGlare)
					{
						case 0: break;
						case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
						case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
						case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
					}
				}
			}

			if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

			if ((_tmp_iStatus & 0x20) != 0)	// Berserk
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
			DrawAngel(16 + (_tmp_cDir - 1), sX + 20, sY - 20, cFrame % 4, dwTime);
			CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);
		}
	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	// Snoopy: Abaddon effects
	if (_tmp_sOwnerType == 81)
	{
		int randFrame = _tmp_cFrame % 12;
		m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
		m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
		m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
		m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
		m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
		m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
		switch (_tmp_cDir)
		{
			case 1:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 2:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 3:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 4:
				m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 5:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 6:
				m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 7:
				m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 8:
				m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
				break;
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

	return false;
}

bool helbreath::DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;
	char cFrame;

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	cFrame = _tmp_cFrame;

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if (cFrame < 6)
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Dax
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 0;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 0;*/
				}
				// Dax
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 0;*/

				// Dax
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 0;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

				// Dax
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
			}
			else
			{
				cFrame -= 6;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Dax
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 11;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
				}
				// Dax
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

				// Dax
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

				// Dax
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
			}
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if (cFrame < 6)
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Dax
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 0;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 0;*/
				}
				// Dax
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 0;*/

				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 0;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

				// Dax
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
			}
			else
			{
				cFrame -= 6;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Dax
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 11;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
				}
				// Dax
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

				// Dax
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

				// Dax
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 11;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
			}
			break;

		default:
			if (cFrame < 4)
			{
				if (_tmp_sAppr2 != 0)
				{
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					cFrame = _tmp_sAppr2 - 1;
				}
				else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
				iUndiesIndex = -1;
				iHairIndex = -1;
				iArmArmorIndex = -1;
				iBodyArmorIndex = -1;
				iPantsIndex = -1;
				iBootsIndex = -1;
				iMantleIndex = -1;
				iHelmIndex = -1;
				switch (_tmp_sOwnerType)
				{
					case 36: // AGT
					case 37: // CGT
					case 38: // MS
					case 39: // DT
					case 40: // ESG
					case 41: // GMG
					case 42: // ManaStone
						if (_tmp_sAppr2 == 0) cFrame = 0;
						break;
					case 51: cFrame = 0; break;
				}
			}
			else
			{
				switch (_tmp_sOwnerType)
				{
					case 51: cFrame = 0; break;
					default: cFrame -= 4; break;
				}
				if (_tmp_sAppr2 != 0)
				{
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					cFrame = _tmp_sAppr2 - 1;
				}
				else if (_tmp_sOwnerType == 66) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 81) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
				else if (_tmp_sOwnerType == 91) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
				else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Earth Wyvern
				else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Wind Wyvern
				else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Dark Wyvern
				else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Grass Wyvern
				else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Sky Wyvern
				else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Sea Wyvern
				else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Metal Wyvern
				else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8); // Void Wyvern
				else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
				iUndiesIndex = -1;
				iHairIndex = -1;
				iArmArmorIndex = -1;
				iBodyArmorIndex = -1;
				iPantsIndex = -1;
				iBootsIndex = -1;
				iMantleIndex = -1;
				iHelmIndex = -1;
			}
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, cFrame);

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 127: // Enraged Tigerworm
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
				}
				break;
		}
		if (_tmp_sOwnerType == 81)
		{
			m_pEffectSpr[152]->PutTransSprite70(sX - 80, sY - 15, _tmp_iEffectFrame % 27, dwTime); // Explosion Abaddon
			m_pEffectSpr[152]->PutTransSprite70(sX, sY - 15, _tmp_iEffectFrame % 27, dwTime);
			m_pEffectSpr[152]->PutTransSprite70(sX - 40, sY, _tmp_iEffectFrame % 27, dwTime);
			m_pEffectSpr[163]->PutTransSprite70(sX - 90, sY - 80, _tmp_iEffectFrame % 12, dwTime); // Ames qui s'envolent
			m_pEffectSpr[160]->PutTransSprite70(sX - 60, sY - 50, _tmp_iEffectFrame % 12, dwTime);
			m_pEffectSpr[161]->PutTransSprite70(sX - 30, sY - 20, _tmp_iEffectFrame % 12, dwTime);
			m_pEffectSpr[162]->PutTransSprite70(sX, sY - 100, _tmp_iEffectFrame % 12, dwTime);
			m_pEffectSpr[163]->PutTransSprite70(sX + 30, sY - 30, _tmp_iEffectFrame % 12, dwTime);
			m_pEffectSpr[162]->PutTransSprite70(sX + 60, sY - 90, _tmp_iEffectFrame % 12, dwTime);
			m_pEffectSpr[163]->PutTransSprite70(sX + 90, sY - 50, _tmp_iEffectFrame % 12, dwTime);
			switch (_tmp_cDir)
			{
				case 1: m_pEffectSpr[140]->PutTransSprite70(sX, sY, cFrame, dwTime); break; // Abbadon dying
				case 2: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame, dwTime); break; // fixed sprit IDs
				case 3: m_pEffectSpr[142]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 4: m_pEffectSpr[143]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 5: m_pEffectSpr[144]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 6: m_pEffectSpr[145]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 7: m_pEffectSpr[146]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 8: m_pEffectSpr[147]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
			}
		}
		else if (_tmp_sOwnerType == 66) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
		else if (_tmp_sOwnerType == 73)
		{	//m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
			m_pSprite[33]->PutTransSprite(sX, sY, cFrame, dwTime);
			switch (_tmp_cDir)
			{
				case 1: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break; // Abbadon qui meurt
				case 2: m_pEffectSpr[142]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
				case 3: m_pEffectSpr[143]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
				case 4: m_pEffectSpr[144]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
				case 5: m_pEffectSpr[145]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
				case 6: m_pEffectSpr[146]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
				case 7: m_pEffectSpr[147]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break;
					//case 8: m_pEffectSpr[148]->PutTransSprite70(sX, sY, cFrame, dwTime); break;
				case 8: m_pEffectSpr[141]->PutTransSprite70(sX, sY, cFrame + 8, dwTime); break; //due to buggy Sprite nb
			}
		}
		else
		{
			if ((_tmp_iStatus & STATUS_FROZEN) != 0)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
			else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
		}

		SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

		if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}

		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}

		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}

		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}

		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}

		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}

		if (iMantleIndex != -1)
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 6, -2, 6, dwTime);

		if ((_tmp_iStatus & 0x20) != 0) // Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, cFrame, 0, -5, -5, dwTime);
		DrawAngel(24 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame, dwTime);
		CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}

	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iFrame, iMantleIndex, iHelmIndex;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 66) return false;

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			iFrame = 7;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// Dax
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 11;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
			}
			// Dax
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

			// Dax
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

			// Dax
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			iFrame = 7;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 11;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 11;*/
			}
			// Dax
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 11;*/

			// Dax
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 11;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

			// Dax
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 11;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 11;*/
			break;
		default:
			switch (_tmp_sOwnerType)
			{
				case 122: // Enraged Troll
				case 28: // Troll
				case 29: // Ogre
				case 30: // Liche
				case 31: // DD		// les 2 dernieres sont pas bonnes pour un mort !
				case 63: // Frost	// les 2 dernieres sont pas bonnes pour un mort !
					iFrame = 5;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					break;

				case 32: // Uni
				case 33: // WW
				case 43: // LWB
				case 44: // GHK
				case 45: // GHKABS
				case 46: // TK
				case 47: // BG
				case 48: // SK
				case 49: // HC
				case 50: // TW
				case 53: // BB
				case 54: // DE
				case 55: // Rabbit
				case 56: // Cat
				case 57: // Frog
				case 58: // MG
				case 59: // Ettin
				case 60: // Plant
				case 61: // Rudolph
				case 62: // Direboar
				case 64: // Crops  ----------- Crop ici! etonant, pourtant !
				case 65: // IceGolem
				case 70: // Dragon..........Ajouts par Snoopy
				case 71: // Centaur
				case 72: // ClawTurtle
				case 74: // GiantCrayfish
				case 75: // Gi Lizard
				case 76: // Gi Tree
				case 77: // Master Orc
				case 78: // Minaus
				case 79: // Nizie
				case 80: // Tentocle
				case 82: // Sorceress
				case 83: // ATK
				case 84: // MasterElf
				case 85: // DSK
				case 88: // Barbarian
				case 126: // Enraged Hellclaw
				case 127: // Enraged Tigerworm
					iFrame = 7;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					break;

				case 86: // HBT
				case 87: // CT
				case 89: // AGC
					iFrame = 7;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
					break;

				case 66: // Wyvern
					iFrame = 15;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
					break;

				case 73: // FireWyvern
					iFrame = 7;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
					bTrans = true; // Prevents showing hugly corpse
					break;

				case 81: // Abaddon
					iFrame = 0;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
					bTrans = true; // Prevents showing hugly corpse
					break;

				case 51: // CP
					iFrame = 0;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					break;

				case 52: // GG
					iFrame = 11;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					break;

				case 91: // Gate
					iFrame = 5;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
					break;

				default: // 40*4 (10...27)
					iFrame = 3;
					iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
					break;
			}
			iUndiesIndex = -1;
			iHairIndex = -1;
			iArmArmorIndex = -1;
			iBodyArmorIndex = -1;
			iPantsIndex = -1;
			iBootsIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
	if (bTrans == false)
	{
		if (_tmp_cFrame == -1)
		{
			_tmp_cFrame = 7;
			if ((_tmp_iStatus & STATUS_FROZEN) != 0)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, iFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
			else
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, iFrame, dwTime);

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if (iMantleIndex != -1)
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}
			if ((_tmp_iStatus & 0x0F00) >> 8 == 9)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, 6, -2, 6, dwTime);
		}
		else if ((_tmp_iStatus & 0x20) != 0)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame + 5, -2 * _tmp_cFrame - 5, -2 * _tmp_cFrame - 5, dwTime);
		else
		{
			if ((_tmp_iStatus & 0x0F00) >> 8 == 9)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame + 6, -2 * _tmp_cFrame - 2, -2 * _tmp_cFrame + 6, dwTime);
			else
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, iFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, dwTime);
		}
	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
	}

	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearDeadChatMsg(indexX, indexY);
		}
	}
	// Snoopy: Abaddon effects
	if (_tmp_sOwnerType == 81)
	{
		Abaddon_corpse(sX, sY); // By Snoopy....
	}
	else if (_tmp_sOwnerType == 73)
	{	//m_pEffectSpr[35]->PutTransSprite70(sX+120, sY+120, rand(), dwTime);
		m_pEffectSpr[35]->PutTransSprite70(sX + 20, sY - 15, rand() % 10, dwTime);
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int dx, dy;
	int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66 || _tmp_sOwnerType == 73)	bInv = true; //Energy-Ball, Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iAdd = 3;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 2;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 2;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else  iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
			}
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iAdd = 3;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 2;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
				}
				// xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 2;*/
				// xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
				// xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
			}
			break;

		default:
			if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iBodyArmorIndex = -1;
			iArmArmorIndex = -1;
			iBootsIndex = -1;
			iPantsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	dx = 0;
	dy = 0;
	switch (_tmp_cDir)
	{
		case 1: dy = 28 - (_tmp_cFrame << 2); break;
		case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
		case 3: dx = (_tmp_cFrame << 2) - 28; break;
		case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
		case 5: dy = (_tmp_cFrame << 2) - 28; break;
		case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
		case 7: dx = 28 - (_tmp_cFrame << 2); break;
		case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
	}
	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:

		case 28: // Troll.
		case 29: // Orge.
		case 30: // Liche
		case 31: // DD
		case 32: // Uni
		case 33: // ww

		case 43: // LWB
		case 44: // GHK
		case 45: // GHKABS
		case 46: // TK
		case 47: // BG
		case 48: // SK
		case 49: // HC
		case 50: // TW

		case 52: // GG
		case 53: // BB
		case 54: // DE
		case 55: // Rabbit
		case 56: // Cat
		case 57: // Frog
		case 58: // MG
		case 59: // Ettin
		case 60: // Plant
		case 61: // Rudolph
		case 62: // DireBoar
		case 63: // Frost

		case 65: // Ice-Golem
		case 66: // Wyvern

		case 70: // Dragon..........Ajouts par Snoopy
		case 71: // Centaur
		case 72: // ClawTurtle
		case 73: // FireWyvern
		case 74: // GiantCrayfish
		case 75: // Gi Lizard
		case 76: // Gi Tree
		case 77: // Master Orc
		case 78: // Minaus
		case 79: // Nizie
		case 80: // Tentocle
		case 81: // Abaddon
		case 82: // Sorceress
		case 83: // ATK
		case 84: // MasterElf
		case 85: // DSK
		case 86: // HBT
		case 87: // CT
		case 88: // Barbarian
		case 89: // AGC
		case 90: // Gail
		case 106: // Earth Wyvern
		case 107: // Wind Wyvern
		case 108: // Dark Wyvern
		case 109: // Grass Wyvern
		case 110: // Sky Wyvern
		case 111: // Sea Wyvern
		case 112: // Metal Wyvern
		case 113: // Void Wyvern
		case 114: // Eternal Dragon
		case 122: // Enraged Troll
		//case 123: // Enraged Cyclops
		case 124: // Enraged Stalker
		case 125: // Enraged Gagoyle
		case 126: // Enraged Hellclaw
		case 127: // Enraged Tigerworm
			break;

		default:
			_tmp_cFrame = _tmp_cFrame / 2;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (_tmp_sOwnerType == 65) // IceGolem
	{	/*m_pEffectSpr[77]->PutTransSprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);*/
		switch (rand() % 3)
		{
			case 0:	m_pEffectSpr[76]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
			case 1:	m_pEffectSpr[77]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
			case 2:	m_pEffectSpr[78]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime); break;
		}
	}
	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			else if (bInv == true)
				//m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}
		}
		else
		{
			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			else if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 6, -2, 6, dwTime);

		// Berserk
		if ((_tmp_iStatus & 0x20) != 0)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(40 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 4, dwTime);
		CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
	}

	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;
	// Snoopy: Abaddon effects
	if (_tmp_sOwnerType == 81)
	{
		int randFrame = _tmp_iEffectFrame % 12;
		m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
		m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
		m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
		m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
		m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
		m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
		switch (_tmp_cDir)
		{
			case 1:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 108, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 50, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 2:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 70, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 3:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 105, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 90, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 4:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx - 35, sY + dy + 100, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 80, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 5:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 65, sY + dy - 5, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 6:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx + 45, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 31, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 7:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx + 40, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 30, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 8:
				m_pEffectSpr[153]->PutTransSprite70(sX + dx + 20, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX + dx - 20, sY + dy + 16, _tmp_iEffectFrame % 15, dwTime);
				break;
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int cFrame, cDir;
	int dx, dy;
	int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iShieldIndex, iMantleIndex;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 67 || _tmp_sOwnerType == 68 || _tmp_sOwnerType == 69 || _tmp_sOwnerType == 81) return false;
	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}
	cDir = _tmp_cDir;
	switch (_tmp_cDir)
	{
		case 1: _tmp_cDir = 5; break;
		case 2: _tmp_cDir = 6; break;
		case 3: _tmp_cDir = 7; break;
		case 4: _tmp_cDir = 8; break;
		case 5: _tmp_cDir = 1; break;
		case 6: _tmp_cDir = 2; break;
		case 7: _tmp_cDir = 3; break;
		case 8: _tmp_cDir = 4; break;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 10;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
			break;
		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 10;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 10;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 10;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 10;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 10;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 10;*/
			break;
		default:
			if (_tmp_sOwnerType == 66)      iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			else if (_tmp_sOwnerType == 73) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			else if (_tmp_sOwnerType == 86) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
			else if (_tmp_sOwnerType == 87) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);// Ne devrait pas arriver!
			else if (_tmp_sOwnerType == 89) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);// Ne devrait pas arriver!
			else if (_tmp_sOwnerType == 106) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Earth Wyvern
			else if (_tmp_sOwnerType == 107) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Wind Wyvern
			else if (_tmp_sOwnerType == 108) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Dark Wyvern
			else if (_tmp_sOwnerType == 109) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Grass Wyvern
			else if (_tmp_sOwnerType == 110) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sky Wyvern
			else if (_tmp_sOwnerType == 111) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Sea Wyvern
			else if (_tmp_sOwnerType == 112) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Metal Wyvern
			else if (_tmp_sOwnerType == 113) iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8); // Void Wyvern
			else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iArmArmorIndex = -1;
			iBodyArmorIndex = -1;
			iPantsIndex = -1;
			iBootsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	dx = 0;
	dy = 0;
	switch (_tmp_cDir)
	{
		case 1: dy = 28 - (_tmp_cFrame << 2); break;
		case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
		case 3: dx = (_tmp_cFrame << 2) - 28; break;
		case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
		case 5: dy = (_tmp_cFrame << 2) - 28; break;
		case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
		case 7: dx = 28 - (_tmp_cFrame << 2); break;
		case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
	}
	cFrame = _tmp_cFrame;
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;
	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, cFrame);
	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX + dy, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, cFrame, dwTime);
				}
				break;
		}

		CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
				else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

			if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iShieldIndex != -1)
			{
				if (iShieldColor == 0)
					m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}
		}
		else
		{
			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

			if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iPantsIndex != -1)
			{
				if (iPantsColor == 0)
					m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
			}

			if (iArmArmorIndex != -1)
			{
				if (iArmColor == 0)
					m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (iBootsColor == 0)
					m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
			}

			if (iBodyArmorIndex != -1)
			{
				if (iArmorColor == 0)
					m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
			}

			if (iHelmIndex != -1)
			{
				if (iHelmColor == 0)
					m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iShieldIndex != -1)
			{
				if (iShieldColor == 0)
					m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (iMantleColor == 0)
					m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
				else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
			}

			if (iWeaponIndex != -1)
			{
				if (iWeaponColor == 0)
					m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
				else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dx, cFrame, 6, -2, 6, dwTime);

		if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, cFrame, 0, -5, -5, dwTime);
		DrawAngel(16 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, cFrame % 4, dwTime);
		CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
	}
	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

bool helbreath::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	short dx, dy;
	int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
	bool bInv = false;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
	iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
	iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
	iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
	iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
	iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
	iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
	iHelmColor = (_tmp_iApprColor & 0x0000000F);

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iAdd = 3;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Re-Coding Sprite xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else
					{	iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;
					}*/
				}
				else iBodyArmorIndex = -1;
				// Re-Coding Sprite xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + iAdd;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				// Re-Coding Sprite xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + iAdd;

				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Re-Coding Sprite xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 2;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
				}
				else iBodyArmorIndex = -1;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 2;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
				// Re-Coding Sprite xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 2;
				// Re-Coding Sprite xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 2;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
				// Re-Coding Sprite xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
			}
			break;
		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iAdd = 3;
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Re-Coding Sprite xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + iAdd;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + iAdd;*/
				}
				else  iBodyArmorIndex = -1;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + iAdd;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + iAdd;*/
				// Re-Coding Sprite xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + iAdd;
				// Re-Coding Sprite xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + iAdd;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + iAdd;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + iAdd;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
				if ((_tmp_sAppr4 & 0x80) == 0)
				{
					// Re-Coding Sprite xRisenx
					if (_tmp_sBodyApprValue == 0)
						iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 2;
					/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
						 iBodyArmorIndex = -1;
					else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 2;*/
				}
				else iBodyArmorIndex = -1;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 2;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 2;*/
				// Re-Coding Sprite xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 2;
				// Re-Coding Sprite xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 2;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
				// Re-Coding Sprite xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 2;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 2;*/
			}
			break;
		default:
			iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iBodyArmorIndex = -1;
			iArmArmorIndex = -1;
			iBootsIndex = -1;
			iPantsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iHelmIndex = -1;
			break;
	}
	dx = 0;
	dy = 0;
	if (_cDrawingOrder[_tmp_cDir] == 1)
	{
		if (iWeaponIndex != -1)
		{
			if (iWeaponColor == 0)
				m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
		}
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				}
				break;
		}
		if (bInv == true)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
		else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);


		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iUndiesIndex != -1)
		{
			if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
		}

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}
		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}
		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if (iShieldIndex != -1)
		{
			if (iShieldColor == 0)
				m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
	}
	else
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 127: // Enraged Tigerworm
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				}
				break;
		}

		if (bInv == true)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
		else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}
		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}
		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if (iShieldIndex != -1)
		{
			if (iShieldColor == 0)
				m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iWeaponIndex != -1)
		{
			if (iWeaponColor == 0)
				m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
		}
	}

	if (_tmp_iChatIndex != 0)
	{
		if (m_pChatMsgList[_tmp_iChatIndex] != 0)
		{
			DrawChatMsgBox(sX + dx, sY + dy, _tmp_iChatIndex, false);
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}

void helbreath::DrawStatusText(int sX, int sY)
{
	if (m_altPressed)
	{
		fmt::format_to(G_cTxt, "Appr1: 0x%.4X ", (uint16_t)_tmp_sAppr1);
		PutString2(sX + 70, sY + (14 * 1), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "Appr2: 0x%.4X ", (uint16_t)_tmp_sAppr2);
		PutString2(sX + 70, sY + (14 * 2), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "Appr3: 0x%.4X ", (uint16_t)_tmp_sAppr3);
		PutString2(sX + 70, sY + (14 * 3), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "Appr4: 0x%.4X ", (uint16_t)_tmp_sAppr4);
		PutString2(sX + 70, sY + (14 * 4), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "HeadAppr: 0x%.4X ", (uint16_t)_tmp_sHeadApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX + 70, sY + (14 * 5), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "BodyAppr: 0x%.4X ", (uint16_t)_tmp_sBodyApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX + 70, sY + (14 * 6), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "ArmAppr: 0x%.4X ", (uint16_t)_tmp_sArmApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX + 70, sY + (14 * 7), G_cTxt, 30, 255, 30);
		fmt::format_to(G_cTxt, "LegAppr: 0x%.4X ", (uint16_t)_tmp_sLegApprValue); // Re-Coding Sprite xRisenx
		PutString2(sX + 70, sY + (14 * 8), G_cTxt, 30, 255, 30);
	}
}

bool helbreath::DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int iBodyIndex, iUndiesIndex, iHairIndex, iBodyArmorIndex, iArmArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iShieldIndex, iMantleIndex;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66 || _tmp_sOwnerType == 81) bInv = true; //Energy-Ball, Wyvern
	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}

	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}

	// CLEROTH - Single-direction monsters
	switch (_tmp_sOwnerType)
	{
		case 110: // Air Elemental
			_tmp_cDir = 1; // North
			break;
		case 91: // Snoopy: Gate
			if (_tmp_cDir <= 3) _tmp_cDir = 3;
			else  _tmp_cDir = 5;
			break;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			_tmp_cFrame = _tmp_cFrame / 2;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 1;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;
				//if ((_tmp_sAppr4 & 0x80) == 0)
				//{	
					// Re-Coding Sprite xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 1;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 1;*/
				//}
				// Re-Coding Sprite xRisenx
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 1;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 1;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 1;*/
				// Re-Coding Sprite xRisenx
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 1;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 1;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;
				// Re-Coding Sprite xRisenx
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 1;
				//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 1;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
				iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15;
				iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;
				//if ((_tmp_sAppr4 & 0x80) == 0)
				//{	
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15;*/
				//}
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15;*/

				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15;
				/*if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15;*/
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 0;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 0;
				//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
			}
			break;

		case 4:
		case 5:
		case 6:
			_tmp_cFrame = _tmp_cFrame / 2;
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			if ((_tmp_sAppr2 & 0xF000) != 0)
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 1;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;
				//if ((_tmp_sAppr4 & 0x80) == 0)
				//{	
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 1;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 1;*/
				//}
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 1;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 1;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 1;*/
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 1;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 1;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 1;
				//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 1;*/
			}
			else
			{
				iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
				iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15;
				iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;
				//if ((_tmp_sAppr4 & 0x80) == 0)
				//{	
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15; // 11800 + (6 * 15) = 11891
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15;*/
				//}
				if (_tmp_sArmApprValue == 0)
					iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15;
				/*if ((_tmp_sAppr3 & 0x000F) == 0)
					 iArmArmorIndex = -1;
				else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15;
				if ((_tmp_sAppr3 & 0x0F00) == 0)
					 iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15;*/
				if (_tmp_sLegApprValue == 0)
					iPantsIndex = -1;
				else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15;
				if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
					iBootsIndex = -1;
				else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;
				if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
					iWeaponIndex = -1;
				else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
				if ((_tmp_sAppr2 & 0x000F) == 0)
					iShieldIndex = -1;
				else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 0;
				if ((_tmp_sAppr4 & 0x0F00) == 0)
					iMantleIndex = -1;
				else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;
				if (_tmp_sHeadApprValue == 0)
					iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 0; // 12800 + (4096 * 15) = 74241 [CRASH POINT]
				//if (_tmp_sHeadApprValue < 0 && _tmp_sHeadApprValue > 25) _tmp_sHeadApprValue = 0;
				/*if ((_tmp_sAppr3 & 0x00F0) == 0)
					 iHelmIndex = -1;
				else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 0;*/
			}
			break;
		case 20: // Howard
		case 24: // Tom
			_tmp_sAppr2 = 0;
		default:
			if (_tmp_sAppr2 != 0)
			{
				iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
				_tmp_cFrame = (_tmp_sAppr2 & 0x00FF) - 1;
			}
			/*	else if (_tmp_sOwnerType == 66) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
				else if (_tmp_sOwnerType == 73) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
				else if (_tmp_sOwnerType == 81) iBodyIndex =  SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);*/
			else iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
			iUndiesIndex = -1;
			iHairIndex = -1;
			iBodyArmorIndex = -1;
			iArmArmorIndex = -1;
			iBootsIndex = -1;
			iPantsIndex = -1;
			iWeaponIndex = -1;
			iShieldIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX, sY, _tmp_cFrame);
	switch (_tmp_sOwnerType)
	{ // hum? la lumiere en dessous ?
		case 15: // ShopKeeper
		case 19: // Gandalf
		case 20: // Howard
		case 24: // Tom
		case 25: // William
		case 26: // Kenedy
		case 51: // CP
		case 86: // HBT
		case 90: // Gail
		case NPC_PRINCESS:
			m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);
			break;
	}
	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}
	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);

			}
			else if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					//else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir-1) * 8 + _tmp_cFrame, m_wR[iPantsColor] -m_wR[0], m_wG[iPantsColor] -m_wG[0], m_wB[iPantsColor] -m_wB[0], dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 255, 0, 0, dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}
		}
		else
		{
			if (_tmp_sOwnerType == 35)
				m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);
			if (_tmp_sOwnerType == 81) // Abaddon
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			}
			else if (bInv == true)
			{
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
			}
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					//			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir-1) * 8 + _tmp_cFrame, m_wR[iPantsColor] -m_wR[0], m_wG[iPantsColor] -m_wG[0], m_wB[iPantsColor] -m_wB[0], dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 255, 0, 0, dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						// 11896 = iBodyArmorIndex value for last sprite crash
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}
		}
		if (_tmp_sOwnerType == 64) // crop
		{
			switch (_tmp_cFrame)
			{
				case 0: // color effect for crop
					m_pEffectSpr[84]->PutTransSprite(sX + 52, sY + 54, (dwTime % 3000) / 120, dwTime);
					break;
				case 1: // color effect for crop
					m_pEffectSpr[83]->PutTransSprite(sX + 53, sY + 59, (dwTime % 3000) / 120, dwTime);
					break;
				case 2: // color effect for crop
					m_pEffectSpr[82]->PutTransSprite(sX + 53, sY + 65, (dwTime % 3000) / 120, dwTime);
					break;
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 6, -2, 6, dwTime);

		// Berserk
		if ((_tmp_iStatus & 0x20) != 0)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(40 + (_tmp_cDir - 1), sX + 20, sY - 20, _tmp_cFrame % 4, dwTime);
		CheckActiveAura2(sX, sY, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
		else
		{
			DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
		}
#ifdef _DEBUG
		DrawStatusText(sX, sY);
#endif
	}

	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	// Snoopy: Abaddon effects
	if (_tmp_sOwnerType == 81)
	{
		int randFrame = _tmp_cFrame % 12;
		m_pEffectSpr[154]->PutTransSprite70(sX - 50, sY - 50, randFrame, dwTime);
		m_pEffectSpr[155]->PutTransSprite70(sX - 20, sY - 80, randFrame, dwTime);
		m_pEffectSpr[156]->PutTransSprite70(sX + 70, sY - 50, randFrame, dwTime);
		m_pEffectSpr[157]->PutTransSprite70(sX - 30, sY, randFrame, dwTime);
		m_pEffectSpr[158]->PutTransSprite70(sX - 60, sY + 90, randFrame, dwTime);
		m_pEffectSpr[159]->PutTransSprite70(sX + 65, sY + 85, randFrame, dwTime);
		switch (_tmp_cDir)
		{
			case 1:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 2:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 3:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 4:
				m_pEffectSpr[153]->PutTransSprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 5:
				m_pEffectSpr[153]->PutTransSprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 6:
				m_pEffectSpr[153]->PutTransSprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 7:
				m_pEffectSpr[153]->PutTransSprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
				break;
			case 8:
				m_pEffectSpr[153]->PutTransSprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
				m_pEffectSpr[164]->PutTransSprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
				break;
		}
	}
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}


bool   helbreath::DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, uint64_t dwTime, int msX, int msY)
{
	int dx, dy;
	int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB, iMantleIndex;
	bool bInv = false;
	int iWeaponGlare, iShieldGlare;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 73 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

	if (m_cDetailLevel == 0)
	{
		iWeaponColor = 0;
		iShieldColor = 0;
		iArmorColor = 0;
		iMantleColor = 0;
		iArmColor = 0;
		iPantsColor = 0;
		iBootsColor = 0;
		iHelmColor = 0;
	}
	else
	{
		iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
		iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
		iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
		iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
		iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
		iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
		iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
		iHelmColor = (_tmp_iApprColor & 0x0000000F);
	}
	iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
	iShieldGlare = (_tmp_sAppr4 & 0x0003);
	if ((_tmp_iStatus & 0x10) != 0)
	{
		if (memcmp(player_name.c_str(), _tmp_cName, 10) == 0) bInv = true;
		else if (bCheckItemEquiped("NecklaceOfBeholder") == true) bInv = true; // Beholder Necklace Fix xRisenx
		else if (_iGetFOE(_tmp_iStatus) == 1) bInv = true;
		else return false;
	}

	switch (_tmp_sOwnerType)
	{
		case 1:
		case 2:
		case 3:
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 4;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + _tmp_sBodyApprValue * 15 + 4;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 4;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + _tmp_sArmApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F)*15 + 4;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + _tmp_sLegApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 4;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else
			{
				iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
			}
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 6;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + _tmp_sHeadApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 4;*/
			break;

		case 4:
		case 5:
		case 6:
			if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 4;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				// xRisenx
				if (_tmp_sBodyApprValue == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + _tmp_sBodyApprValue * 15 + 4;
				/*if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					 iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12)*15 + 4;*/
			}
			// xRisenx
			if (_tmp_sArmApprValue == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + _tmp_sArmApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x000F) == 0)
				 iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F)*15 + 4;*/
			// xRisenx
			if (_tmp_sLegApprValue == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + _tmp_sLegApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x0F00) == 0)
				 iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8)*15 + 4;*/
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else
			{
				iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
			}
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 6;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;
			// xRisenx
			if (_tmp_sHeadApprValue == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + _tmp_sHeadApprValue * 15 + 4;
			/*if ((_tmp_sAppr3 & 0x00F0) == 0)
				 iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4)*15 + 4;*/
			break;

		default:
			iUndiesIndex = -1;
			iHairIndex = -1;
			iArmArmorIndex = -1;
			iBodyArmorIndex = -1;
			iPantsIndex = -1;
			iBootsIndex = -1;
			iMantleIndex = -1;
			iHelmIndex = -1;
			break;
	}
	dx = 0;
	dy = 0;
	switch (_tmp_cDir)
	{
		case 1: dy = 28 - (_tmp_cFrame << 2); break;
		case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
		case 3: dx = (_tmp_cFrame << 2) - 28; break;
		case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
		case 5: dy = (_tmp_cFrame << 2) - 28; break;
		case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
		case 7: dx = 28 - (_tmp_cFrame << 2); break;
		case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
	}
	if (m_pSprite[iBodyIndex + (_tmp_cDir - 1)] == 0) return false;

	if (m_bIsCrusadeMode || (m_bIsAstoriaMode && memcmp(m_cCurLocation, "astoria", 7) == 0)) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

	if (_tmp_iEffectType != 0)
	{
		switch (_tmp_iEffectType)
		{
			case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
			case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
		}
	}

	if (bTrans == false)
	{
		switch (_tmp_sOwnerType)
		{ // Pas d'ombre pour ces mobs
			case 10: // Slime
			case 35: // Energy Sphere
			case 50: // TW
			case 51: // CP
			case 60: // Plant
			case 65: // IceGolem
			case 66: // Wyvern
			case 73: // Fire Wyvern
			case 81: // Abaddon
			case 91: // Gate
			case 106: // Earth Wyvern
			case 107: // Wind Wyvern
			case 108: // Dark Wyvern
			case 109: // Grass Wyvern
			case 110: // Sky Wyvern
			case 111: // Sea Wyvern
			case 112: // Metal Wyvern
			case 113: // Void Wyvern
			case 114: // Eternal Dragon
			case 127: // Enraged Tigerworm
				break;
			default:
				if (m_cDetailLevel != 0)
				{
					if (sX < 50)
						m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				}
				break;
		}
		CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
		if (_cDrawingOrder[_tmp_cDir] == 1)
		{
			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if (bInv == true)
				//m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime); // GM effect
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}
		}
		else
		{
			if (bInv == true)
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else
			{
				if ((_tmp_iStatus & STATUS_FROZEN) != 0)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}

			SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
				m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iUndiesIndex != -1)
			{
				if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			}

			if ((iHairIndex != -1) && (iHelmIndex == -1))
			{
				_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
				m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 1))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iPantsIndex != -1)
			{
				if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iPantsColor == 0)
						m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
				}
			}

			if (iArmArmorIndex != -1)
			{
				if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmColor == 0)
						m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
				}
			}

			if ((iBootsIndex != -1) && (iSkirtDraw == 0))
			{
				if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iBootsColor == 0)
						m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
				}
			}

			if (iBodyArmorIndex != -1)
			{
				if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iArmorColor == 0)
						m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
				}
			}

			if (iHelmIndex != -1)
			{
				if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iHelmColor == 0)
						m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iHelmIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iShieldIndex != -1)
			{
				if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iShieldColor == 0)
						m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
				}
				switch (iShieldGlare)
				{
					case 0: break;
						//case 1: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
					case 2: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iShieldIndex]->PutTransSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}

			if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
			{
				if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
				else
				{
					if (iMantleColor == 0)
						m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
					else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
				}
			}

			if (iWeaponIndex != -1)
			{
				if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else
				{
					if (iWeaponColor == 0)
						m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
					else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
				}
				DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);
				switch (iWeaponGlare)
				{
					case 0: break;
					case 1: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
					case 2: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
					case 3: m_pSprite[iWeaponIndex]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
				}
			}
		}

		if ((_tmp_iStatus & 0x0F00) >> 8 == 9) // Highly trained
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dx, _tmp_cFrame, 6, -2, 6, dwTime);

		if ((_tmp_iStatus & 0x20) != 0) 	// Berserk
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
		DrawAngel(40 + (_tmp_cDir - 1), sX + dx + 20, sY + dy - 20, _tmp_cFrame % 4, dwTime);
		CheckActiveAura2(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

	}
	else if (strlen(_tmp_cName) > 0)
	{
		if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
		else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
	}

	if (_tmp_iChatIndex != 0)
	{
		if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
		{
			m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
			m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1)
		&& (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY)
		&& (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY)
		&& (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX)
		&& (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;
	return false;
}
