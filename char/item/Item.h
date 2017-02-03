// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include "common.h"
#include "items.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum ItemStat{
	ITEMSTAT_NONE,
	ITEMSTAT_CRITICAL,
	ITEMSTAT_POISONING,
	ITEMSTAT_RIGHTEOUS,
	ITEMSTAT_UNUSED,		// UNUSED
	ITEMSTAT_AGILE,
	ITEMSTAT_LIGHT,
	ITEMSTAT_SHARP,
	ITEMSTAT_STRONG,
	ITEMSTAT_ANCIENT,
	ITEMSTAT_CASTPROB,
	ITEMSTAT_MANACONV,
	ITEMSTAT_CRITICAL2,
	ITEMSTAT_FLAWLESSSHARP, // 13
	ITEMSTAT_FLAWLESSANCIENT // 14
};

enum ItemStat2{
	ITEMSTAT2_NONE,
	ITEMSTAT2_PSNRES,
	ITEMSTAT2_HITPROB,
	ITEMSTAT2_DEF,
	ITEMSTAT2_HPREC,
	ITEMSTAT2_SPREC,
	ITEMSTAT2_MPREC,
	ITEMSTAT2_MR,
	ITEMSTAT2_PA,
	ITEMSTAT2_MA,
	ITEMSTAT2_CAD,
	ITEMSTAT2_EXP,
	ITEMSTAT2_GOLD
};

#define MAXITEMEQUIPPOS		15
#define EQUIPPOS_NONE		0
#define EQUIPPOS_HEAD		1
#define EQUIPPOS_BODY		2
#define EQUIPPOS_ARMS		3
#define EQUIPPOS_PANTS		4
#define EQUIPPOS_BOOTS		5
#define EQUIPPOS_NECK		6
#define EQUIPPOS_LHAND		7
#define EQUIPPOS_RHAND		8
#define EQUIPPOS_TWOHAND	9
#define EQUIPPOS_RFINGER	10
#define EQUIPPOS_LFINGER	11
#define EQUIPPOS_BACK		12
#define EQUIPPOS_FULLBODY	13

enum ItemTypes
{
	ITEMTYPE_NOTUSED = -1,
	ITEMTYPE_NONE,
	ITEMTYPE_EQUIP,
	ITEMTYPE_APPLY,
	ITEMTYPE_USE_DEPLETE,
	ITEMTYPE_INSTALL,
	ITEMTYPE_CONSUME,
	ITEMTYPE_ARROW,
	ITEMTYPE_EAT,
	ITEMTYPE_USE_SKILL,
	ITEMTYPE_USE_PERM,
	ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX,
	ITEMTYPE_USE_DEPLETE_DEST,
	ITEMTYPE_MATERIAL,
	ITEMTYPE_RELIC
};

enum SocketGems : uint8_t{
	SG_NONE = (uint8_t)ITEM_NONE,
	SG_REJUGEM7 = 1,
	SG_REJUGEM14,
	SG_REJUGEM21,
	SG_BLOODGEM7,
	SG_BLOODGEM14,
	SG_BLOODGEM21,
	SG_MINDGEM7,
	SG_MINDGEM14,
	SG_MINDGEM21,
	SG_ARMORGEM7,
	SG_ARMORGEM14,
	SG_ARMORGEM21,
	SG_ENCHANTEDGEM2,
	SG_ENCHANTEDGEM4,
	SG_ENCHANTEDGEM6,
	SG_TACTICALGEM3,
	SG_TACTICALGEM5,
	SG_TACTICALGEM7,
	SG_VORTEXGEM,
	SG_MAX
};

static const char * SocketGems[SG_MAX] = {
	"none",
	"RejuGem7",
	"RejuGem14",
	"RejuGem21",
	"BloodGem7",
	"BloodGem14",
	"BloodGem21",
	"MindGem7",
	"MindGem14",
	"MindGem21",
	"ArmorGem7",
	"ArmorGem14",
	"ArmorGem21",
	"EnchantedGem2",
	"EnchantedGem4",
	"EnchantedGem6",
	"TacticalGem3",
	"TacticalGem5",
	"TacticalGem7",
	"VortexGem"
};

class CItem  
{
public:
	CItem();
	virtual ~CItem();

	bool operator<(CItem * item) const {
		return (strcmp(m_cName, item->m_cName) < 0) ? true : false;
	}

	bool IsManued()	const { return (m_dwAttribute & 1); }
	uint32_t GetMaxSockets() const;
	uint32_t GetNUsedSockets() const;
	uint32_t GetManuCompletion()	const { return m_sItemSpecEffectValue2 + 100; }
	bool IsVortexed()	const { return m_sockets[0] == SG_VORTEXGEM; }
	void GetGemAttr(char * txt, uint8_t gem) const;
	void GetGemAttr(char * txt) const;

	char  m_cName[21];
	char  m_cItemType;
	char  m_cEquipPos;
	uint32_t m_ItemColor;
	char  m_cSpeed;
	char  m_cGenderLimit;
//	short m_sItemEffectType;
	short m_sLevelLimit;
	short m_sSprite;
	short m_sSpriteFrame;
	short m_sX, m_sY;
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3, m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6; 
	uint16_t  m_wCurLifeSpan;
	uint16_t  m_wMaxLifeSpan;
	uint16_t  m_wWeight;
	uint32_t m_wPrice;
	uint32_t m_dwCount;
	uint32_t m_dwAttribute;
	uint8_t m_sockets[MAXITEMSOCKETS];
	ItemUID ItemUniqueID;
	uint32_t m_serverPtr;

	const struct ItemMapComp
	{
		bool operator()(CItem * x, CItem * y) const
		{
			return (strcmp(x->m_cName, y->m_cName) < 0) ? true : false;
		}
	};
};



typedef std::multimap<CItem*, uint32_t, CItem::ItemMapComp> ItemMap;
typedef std::multimap<CItem*, uint32_t, CItem::ItemMapComp>::iterator ItemMapIter;

#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
