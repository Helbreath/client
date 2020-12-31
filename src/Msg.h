// Msg.h: interface for the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string.h>
#include "common.h"

class CMsg
{
public:
    static bool OrderPred(const CMsg * a, const CMsg * b)
    {
        return a->m_msgid < b->m_msgid;
    };
    static bool RemovePred(const CMsg * msg);
    static void SetPred(ChatType t) { predremove = t; }
    static ChatType predremove;
    static uint32_t nmsgs;

    CMsg(char cType, std::string pMsg, uint64_t dwTime);
    CMsg();
    virtual ~CMsg();

    char   m_cType;
    std::string m_pMsg;
    short  m_sX, m_sY;
    uint64_t  m_dwTime;
    uint64_t m_msgid;
    bool used;

    int    m_iObjectID;

};
