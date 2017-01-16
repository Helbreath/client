//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "BuildItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuildItem::CBuildItem()
{
    int i;

    memset(m_cName, 0, sizeof(m_cName));

    m_bBuildEnabled = false;
    m_iSkillLimit = 0;
    m_iMaxSkill = 0;

    m_iSprH = 0;
    m_iSprFrame = 0;

    memset(m_cElementName1, 0, sizeof(m_cElementName1));
    memset(m_cElementName2, 0, sizeof(m_cElementName2));
    memset(m_cElementName3, 0, sizeof(m_cElementName3));
    memset(m_cElementName4, 0, sizeof(m_cElementName4));
    memset(m_cElementName5, 0, sizeof(m_cElementName5));
    memset(m_cElementName6, 0, sizeof(m_cElementName6));

    for (i = 0; i < 7; i++)
    {
        m_iElementCount[i] = 0;
        m_bElementFlag[i] = 0;
    }
}

CBuildItem::~CBuildItem()
{
}
