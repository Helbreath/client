// MapData.h: interface for the CMapData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPDATA_H__DA7A0760_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_MAPDATA_H__DA7A0760_758B_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "Tile.h"
#include "../GlobalDef.h"
#include "../char/ActionID.h"
#include "../engine/TileSpr.h"
#include "../shared/npcType.h"
#include "../char/DynamicObjectID.h"


#define MAPDATASIZEX	550//40
#define MAPDATASIZEY	550//35

class CMapData  
{
public:
	CMapData(class CGame * pGame);
	virtual ~CMapData();
	void Init();
	void OpenMapDataFile(char * cFn);
	void GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, int * pStatus, int * pColor, char * pName);
	void ClearDeadChatMsg(short sX, short sY);
	void ClearChatMsg(short sX, short sY);
	void ShiftMapData(char cDir);
	void _bDecodeMapInfo(char * pHeader);
	bool __fastcall bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex);
	bool __fastcall bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, int iStatus, char * pName);
	bool __fastcall bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pCharIndex);
	
#ifdef SHOWALLDAMAGE // Remove Critical xRisenx
	bool __fastcall bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, UnitStatus iStatus, char * pName, short sAction, int sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
	bool __fastcall bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, int * pV1, short * pV2);
#else	
	bool __fastcall bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, UnitStatus iStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
	bool __fastcall bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2);
#endif
	bool __fastcall bGetOwner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID, short * dynObjectType = 0);
	bool bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent);
	bool bIsTeleportLoc(short sX, short sY);
	bool bGetIsLocateable(short sX, short sY);
	bool bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, uint32_t cItemColor, /*DWORD m_dwItemAttribute,*/ bool bDropEffect = true); // 1234 Added dwItemAttribute
	int  iObjectFrameCounter(char * cPlayerName, short sViewPointX, short sViewPointY);

	int getChatMsgIndex(uint16_t wObjectID) const;

	class CTile m_pData[550][550];
	class CTile m_pTmpData[MAPDATASIZEX][MAPDATASIZEY];
	class CTileSpr m_tile[550][550];//[752][752];
	class CGame * m_pGame;

	struct {
		short m_sMaxFrame;
		short m_sFrameTime;
	} m_stFrame[TOTALCHARACTERS][TOTALACTION];
	uint64_t m_dwFrameTime;
    uint64_t m_dwDOframeTime;
    uint64_t m_dwFrameCheckTime;
	int m_iObjectIDcacheLocX[30000];
	int m_iObjectIDcacheLocY[30000];
    uint64_t m_dwFrameAdjustTime;
	short m_sMapSizeX, m_sMapSizeY;
	short m_sRectX, m_sRectY;
	short m_sPivotX, m_sPivotY;
};

#endif // !defined(AFX_MAPDATA_H__DA7A0760_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
