
#include "../helbreath.h"


void helbreath::DlgBoxClick_GuildOp()
{
    short sX, sY;
    char cName[12], cName20[24];
    char onButton = m_dialogBoxes[8].OnButton();

    ZeroMemory(cName, sizeof(cName));
    ZeroMemory(cName20, sizeof(cName20));

    switch (m_stGuildOpList[0].cOpMode)
    {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (onButton == 1)
            {
                _ShiftGuildOperationList();
                if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
            }
            return;
    }

    if (onButton == 2)
    {
        // Approve
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;
        }
        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }

    if (onButton == 1)
    {
        // Reject
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, 0, cName20);
                break;
        }

        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }
}
