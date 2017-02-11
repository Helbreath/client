// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "../../shared/common.h"
#include "../../shared/items.h"
#include "../../language/lan_eng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	memset(m_cName, 0, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
	m_dwAttribute = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;
	m_serverPtr = 0;
	
	for(int i = 0; i < MAXITEMSOCKETS; i++)
		m_sockets[i] = SG_NONE;
}

CItem::~CItem()
{
	
}

uint32_t CItem::GetMaxSockets() const
{
	if(m_sockets[0] == SG_VORTEXGEM)
	{
		return 2;
	}
	else if(!IsManued())
	{
		return 0;
	}

	switch(m_cEquipPos)
	{
	case EQUIPPOS_BODY:
		if(GetManuCompletion() < 50)
			return 1;
		else if(GetManuCompletion() < 100)
			return 2;
		else if(GetManuCompletion() >= 100)
			return 3;
		break;

	case EQUIPPOS_ARMS:
	case EQUIPPOS_PANTS:
		if(GetManuCompletion() >= 50 && GetManuCompletion() < 100)
			return 1;
		else if(GetManuCompletion() >= 100)
			return 2;
		break;

	case EQUIPPOS_HEAD:
		if(GetManuCompletion() >= 50)
			return 1;
		break;
	}

	return 0;
}

uint32_t CItem::GetNUsedSockets() const
{
	uint32_t n = 0;
	for(int i = 0; i < GetMaxSockets(); i++)
	{
		if(m_sockets[i] != SG_NONE && m_sockets[i] != SG_VORTEXGEM)
		{
			n++;
		}
	}

	return n;
}

void CItem::GetGemAttr(char * txt, uint8_t gem) const
{
	switch(gem)
	{
	case SG_REJUGEM7:
		sprintf(txt, GET_ITEM_NAME29, 7);
		break;
	case SG_REJUGEM14:
		sprintf(txt, GET_ITEM_NAME29, 14);
		break;
	case SG_REJUGEM21:
		sprintf(txt, GET_ITEM_NAME29, 21);
		break;
	case SG_BLOODGEM7:
		sprintf(txt, GET_ITEM_NAME27, 7);
		break;
	case SG_BLOODGEM14:
		sprintf(txt, GET_ITEM_NAME27, 14);
		break;
	case SG_BLOODGEM21:
		sprintf(txt, GET_ITEM_NAME27, 21);
		break;
	case SG_MINDGEM7:
		sprintf(txt, GET_ITEM_NAME30, 7);
		break;
	case SG_MINDGEM14:
		sprintf(txt, GET_ITEM_NAME30, 14);
		break;
	case SG_MINDGEM21:
		sprintf(txt, GET_ITEM_NAME30, 21);
		break;
	case SG_ARMORGEM7:
		sprintf(txt, GET_ITEM_NAME26, 7);
		break;
	case SG_ARMORGEM14:
		sprintf(txt, GET_ITEM_NAME26, 14);
		break;
	case SG_ARMORGEM21:
		sprintf(txt, GET_ITEM_NAME26, 21);
		break;
	case SG_ENCHANTEDGEM2:
		sprintf(txt, GET_ITEM_NAME32, 2);
		break;
	case SG_ENCHANTEDGEM4:
		sprintf(txt, GET_ITEM_NAME32, 4);
		break;
	case SG_ENCHANTEDGEM6:
		sprintf(txt, GET_ITEM_NAME32, 6);
		break;
	case SG_TACTICALGEM3:
		sprintf(txt, GET_ITEM_NAME31, 3);
		break;
	case SG_TACTICALGEM5:
		sprintf(txt, GET_ITEM_NAME31, 5);
		break;
	case SG_TACTICALGEM7:
		sprintf(txt, GET_ITEM_NAME31, 7);
		break;
	}
}

void CItem::GetGemAttr(char * txt) const
{
	if(!strcmp(m_cName, "RejuGem7"))
	{
		sprintf(txt, GET_ITEM_NAME29, 7);
	}else if(!strcmp(m_cName, "RejuGem14"))
	{
		sprintf(txt, GET_ITEM_NAME29, 14);
	}else if(!strcmp(m_cName, "RejuGem21"))
	{
		sprintf(txt, GET_ITEM_NAME29, 21);

	}else if(!strcmp(m_cName, "BloodGem7"))
	{
		sprintf(txt, GET_ITEM_NAME27, 7);
	}else if(!strcmp(m_cName, "BloodGem14"))
	{
		sprintf(txt, GET_ITEM_NAME27, 14);
	}else if(!strcmp(m_cName, "BloodGem21"))
	{
		sprintf(txt, GET_ITEM_NAME27, 21);

	}else if(!strcmp(m_cName, "MindGem7"))
	{
		sprintf(txt, GET_ITEM_NAME30, 7);
	}else if(!strcmp(m_cName, "MindGem14"))
	{
		sprintf(txt, GET_ITEM_NAME30, 14);
	}else if(!strcmp(m_cName, "MindGem21"))
	{
		sprintf(txt, GET_ITEM_NAME30, 21);

	}else if(!strcmp(m_cName, "ArmorGem7"))
	{
		sprintf(txt, GET_ITEM_NAME26, 7);
	}else if(!strcmp(m_cName, "ArmorGem14"))
	{
		sprintf(txt, GET_ITEM_NAME26, 14);
	}else if(!strcmp(m_cName, "ArmorGem21"))
	{
		sprintf(txt, GET_ITEM_NAME26, 21);

	}else if(!strcmp(m_cName, "EnchantedGem2"))
	{
		sprintf(txt, GET_ITEM_NAME32, 2);
	}else if(!strcmp(m_cName, "EnchantedGem4"))
	{
		sprintf(txt, GET_ITEM_NAME32, 4);
	}else if(!strcmp(m_cName, "EnchantedGem6"))
	{
		sprintf(txt, GET_ITEM_NAME32, 6);

	}else if(!strcmp(m_cName, "TacticalGem3"))
	{
		sprintf(txt, GET_ITEM_NAME31, 3);
	}else if(!strcmp(m_cName, "TacticalGem5"))
	{
		sprintf(txt, GET_ITEM_NAME31, 5);
	}else if(!strcmp(m_cName, "TacticalGem7"))
	{
		sprintf(txt, GET_ITEM_NAME31, 7);
	}
}