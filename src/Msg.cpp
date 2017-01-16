//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Msg.h"
ChatType CMsg::predremove;
uint32_t CMsg::nmsgs = 0;

CMsg::CMsg()
{
    used = false;
    m_dwTime = 0;
    m_iObjectID = 0;
    m_msgid = 0;
    m_cType = 0;
}

CMsg::CMsg(char cType, std::string pMsg, uint64_t dwTime)
{
    used = true;
    m_cType = cType;

    m_pMsg = pMsg;
    m_dwTime = dwTime;
    m_iObjectID = -1;

    m_msgid = nmsgs++;
}

CMsg::~CMsg()
{
}

bool CMsg::RemovePred(const CMsg * msg)
{
    return msg->m_dwTime == predremove;
}
