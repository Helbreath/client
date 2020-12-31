// Tile.cpp: implementation of the CTile class.
//
//////////////////////////////////////////////////////////////////////

#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTile::CTile()
{
    m_sOwnerType = 0;
    m_cOwnerName = "";
    m_sDeadOwnerType = 0;
    m_cDeadOwnerName = "";
    m_cDeadOwnerFrame = -1;

    m_sItemSprite = 0;
    m_sItemSpriteFrame = 0;

    m_sDynamicObjectType = 0;
    m_cDynamicObjectFrame = 0;

    m_iChatMsg = 0;
    m_iDeadChatMsg = 0;

    m_wObjectID = 0;

    m_iEffectType = 0;
    m_iEffectFrame = 0;
    m_iEffectTotalFrame = 0;
    m_dwEffectTime = 0;
}

CTile::~CTile()
{
}

void CTile::Clear()
{
    m_wObjectID = 0;
    m_wDeadObjectID = 0;

    m_sOwnerType = 0;
    m_cOwnerName = "";

    m_sDeadOwnerType = 0;
    m_cDeadOwnerName = "";

    m_cDeadOwnerFrame = -1;
    m_dwDeadOwnerTime = 0;

    m_cOwnerAction = 0;
    m_cDir = 0;
    m_cOwnerFrame = 0;

    m_sItemSprite = 0;
    m_sItemSpriteFrame = 0;
    m_cItemColor = 0;

    m_sDynamicObjectType = 0;
    m_cDynamicObjectFrame = 0;

    m_iChatMsg = 0;
    m_iDeadChatMsg = 0;

    m_iStatus = 0;
    m_iDeadStatus = 0;

    m_sV1 = 0;
    m_sV2 = 0;
    m_sV3 = 0;

    m_sAppr1 = 0;
    m_sAppr2 = 0;
    m_sAppr3 = 0;
    m_sAppr4 = 0;
    m_iApprColor = 0;

    m_sHeadApprValue = 0;
    m_sBodyApprValue = 0;
    m_sArmApprValue = 0;
    m_sLegApprValue = 0;

    m_iEffectType = 0;
    m_iEffectFrame = 0;
    m_iEffectTotalFrame = 0;
    m_dwEffectTime = 0;

    m_dwOwnerTime = 0;
}
