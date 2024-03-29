//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include "Tile.h"
#include "global_def.h"
#include "ActionID.h"
#include "tile_sprite.h"
#include "npcType.h"
#include "DynamicObjectID.h"

#define MAPDATASIZEX 550 //40
#define MAPDATASIZEY 550 //35

class CMapData
{
public:
    CMapData(class helbreath * pGame);
    virtual ~CMapData();
    void Init();
    void OpenMapDataFile(char * cFn);
    void GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, int * pStatus, int * pColor, std::string & pName);
    void ClearDeadChatMsg(short sX, short sY);
    void ClearChatMsg(short sX, short sY);
    void ShiftMapData(char cDir);
    void _bDecodeMapInfo(char * pHeader);
    bool __fastcall bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex);
    bool __fastcall bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, int iStatus, std::string & pName);
    bool __fastcall bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, char * pFrame, std::string & pName, short * pItemSprite, short * pItemSpriteFrame, int * pCharIndex);

#ifdef SHOWALLDAMAGE // Remove Critical xRisenx
    bool __fastcall bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, UnitStatus iStatus, std::string & pName, short sAction, int sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
    bool __fastcall bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, std::string & pName, char * pAction, char * pFrame, int * pChatIndex, int * pV1, short * pV2);
#else
    bool __fastcall bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, short sHeadApprValue, short sBodyApprValue, short sArmApprValue, short sLegApprValue, UnitStatus iStatus, std::string & pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
    bool __fastcall bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, short * pHeadApprValue, short * pBodyApprValue, short * pArmApprValue, short * pLegApprValue, int * pStatus, std::string & pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2);
#endif
    bool __fastcall bGetOwner(short sX, short sY, std::string & pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID, short * dynObjectType = 0);
    bool bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent);
    bool bIsTeleportLoc(short sX, short sY);
    bool bGetIsLocateable(short sX, short sY);
    bool bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, uint32_t cItemColor, /*DWORD m_dwItemAttribute,*/ bool bDropEffect = true); // 1234 Added dwItemAttribute
    int iObjectFrameCounter(std::string & cPlayerName, short sViewPointX, short sViewPointY);

    int getChatMsgIndex(uint16_t wObjectID) const;

    CTile m_pData[550][550];
    CTile m_pTmpData[MAPDATASIZEX][MAPDATASIZEY];
    tile_sprite m_tile[550][550]; //[752][752];
    helbreath * m_pGame;

    struct
    {
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
