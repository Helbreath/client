// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "GlobalDef.h"

class CMsg;

class CTile
{
public:
    void Clear();

    CTile();
    virtual ~CTile();
    uint64_t m_dwOwnerTime;
    uint64_t m_dwEffectTime;
    uint64_t m_dwDeadOwnerTime;
    uint64_t m_dwDynamicObjectTime;

    int m_iChatMsg;
    uint32_t m_cItemColor;
    int m_iEffectType;
    int m_iDeadApprColor;
    int m_iEffectFrame, m_iEffectTotalFrame;
    int m_iApprColor;
    int m_iDeadChatMsg;

    uint32_t m_wDeadObjectID;
    uint32_t m_wObjectID;

    short m_sOwnerType;
    short m_sAppr1;
    short m_sAppr2;
    short m_sAppr3;
    short m_sAppr4;
    int m_iStatus;

    short m_sHeadApprValue;
    short m_sBodyApprValue;
    short m_sArmApprValue;
    short m_sLegApprValue;

    short m_sDeadOwnerType;
    short m_sDeadAppr1;
    short m_sDeadAppr2;
    short m_sDeadAppr3;
    short m_sDeadAppr4;
    int m_iDeadStatus;

    short m_sDeadHeadApprValue;
    short m_sDeadBodyApprValue;
    short m_sDeadArmApprValue;
    short m_sDeadLegApprValue;

    short m_sV1;
    short m_sV2;
    short m_sV3;
    short m_sDynamicObjectType;

    short m_sItemSprite;
    short m_sItemSpriteFrame;

    char m_cDeadOwnerFrame;
    char m_cOwnerAction;
    char m_cOwnerFrame;
    char m_cDir;
    char m_cDeadDir;

    char m_cDynamicObjectFrame;
    char m_cDynamicObjectData1, m_cDynamicObjectData2, m_cDynamicObjectData3, m_cDynamicObjectData4;
    std::string m_cOwnerName;
    std::string m_cDeadOwnerName;
};
