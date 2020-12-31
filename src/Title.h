// Title.h: interface for the Title class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class Title
{
public:
    Title();
    virtual ~Title();

    char m_cSubject[21];

    uint32_t m_iCurrentNo;

    int m_iNextRankPoints;
    int m_iRank;
    int m_iServerTitleIndex;
};
