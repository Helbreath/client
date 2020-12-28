// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "common.h"
#include "GlobalDef.h" //xRisenx needed for #define SHOWALLDAMAGE

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
	
	int   m_iChatMsg;
	//int   m_cItemColor; 
	//int   m_iChatMsg;
	uint32_t m_cItemColor; 
	int   m_iEffectType;
	int   m_iDeadApprColor; 
	int   m_iEffectFrame, m_iEffectTotalFrame;
	int   m_iApprColor; 
	int   m_iDeadChatMsg;

	uint32_t  m_wDeadObjectID;
	uint32_t  m_wObjectID;

	short m_sOwnerType;							// +B2C
	short m_sAppr1;								// +B2E
	short m_sAppr2;								// +B30
	short m_sAppr3;								// +B32
	short m_sAppr4;								// +B34
	int m_iStatus;								// +B38

	short m_sHeadApprValue; // Re-Coding Sprite xRisenx
	short m_sBodyApprValue; // Re-Coding Sprite xRisenx
	short m_sArmApprValue; // Re-Coding Sprite xRisenx
	short m_sLegApprValue; // Re-Coding Sprite xRisenx

	short m_sDeadOwnerType;						// +B3C
	short m_sDeadAppr1;
	short m_sDeadAppr2;
	short m_sDeadAppr3;
	short m_sDeadAppr4;
	int m_iDeadStatus;

	short m_sDeadHeadApprValue; // Re-Coding Sprite xRisenx
	short m_sDeadBodyApprValue; // Re-Coding Sprite xRisenx
	short m_sDeadArmApprValue; // Re-Coding Sprite xRisenx
	short m_sDeadLegApprValue; // Re-Coding Sprite xRisenx

//#ifdef RemoveCriticalHG
	//int m_sV1;		//BESK: this value is the amount of damage done to something in this tile
//#else
	short m_sV1;	//BESK: this value is the amount of damage done to something in this tile
//#endif
	short m_sV2;					
	short m_sV3;								// +B50
	short m_sDynamicObjectType;

	short m_sItemSprite;
	short m_sItemSpriteFrame;
	
	//DWORD m_dwItemAttribute; // 1234

	char  m_cDeadOwnerFrame;
	char  m_cOwnerAction;						// +B59
	char  m_cOwnerFrame;						// +B5A
	char  m_cDir;
	char  m_cDeadDir;
	
	char  m_cDynamicObjectFrame;
	char  m_cDynamicObjectData1, m_cDynamicObjectData2, m_cDynamicObjectData3, m_cDynamicObjectData4;
	std::string m_cOwnerName;
	std::string m_cDeadOwnerName;
};

#endif // !defined(AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
