//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <functional>
#include <vector>
#include "common.h"

#define MAXBUTTONS 40

class helbreath;

enum Alignment {
    POS_LEFT,
    POS_CENTER,
    POS_RIGHT
};

enum dialogIDs
{
    DIALOG_CHARACTER = 1,
    DIALOG_INVENTORY = 2,
    DIALOG_MAGIC = 3,
    DIALOG_ITEMDROP = 4,
    DIALOG_15AGEMSG = 5,
    DIALOG_WARNINGMSG = 6,
    DIALOG_GUILDMENU = 7,
    DIALOG_GUILDOPERATION = 8,
    DIALOG_GUIDEMAP = 9,
    DIALOG_CHAT = 10,
    DIALOG_SHOP = 11,
    DIALOG_LEVELUPSETTING = 12,
    DIALOG_CITYHALLMENU = 13,
    DIALOG_BANK = 14,
    DIALOG_SKILL = 15,
    DIALOG_MAGICSHOP = 16,
    DIALOG_QUERYDROPITEMAMOUNT = 17,
    DIALOG_TEXT = 18,
    DIALOG_SYSMENU = 19,
    DIALOG_NPCACTIONQUERY = 20,
    DIALOG_NPCTALK = 21,
    DIALOG_MAP = 22,
    DIALOG_SELLORREPAIRITEM = 23,
    DIALOG_FISHING = 24,
    DIALOG_SHUTDOWNMSG = 25,
    DIALOG_SKILLDLG = 26,
    DIALOG_EXCHANGE = 27,
    DIALOG_QUEST = 28,
    DIALOG_GAUGEPANEL = 29,
    DIALOG_ICONPANEL = 30,
    DIALOG_SELLLIST = 31,
    DIALOG_PARTY = 32,
    DIALOG_CRUSADEJOB = 33,
    DIALOG_ITEMUPGRADE = 34,
    DIALOG_HELP = 35,
    DIALOG_COMMANDER = 36,
    DIALOG_CONSTRUCTOR = 37,
    DIALOG_SOLDIER = 38,

    DIALOG_SLATES = 40,
    DIALOG_CONFIRMEXCHANGE = 41,
    DIALOG_CHANGESTATSMAJESTIC = 42,
    DIALOG_FRIENDLIST = 43,
    DIALOG_GUILDQUERY = 44,
    DIALOG_GUILDSUMMONS = 49,
    DIALOG_RESURECT = 50,
    DIALOG_CMDHALLMENU = 51,
    DIALOG_DKMENUWEAPONS = 53,

    DIALOG_GUILDBANK = 56,
    DIALOG_GUILD = 57,
    DIALOG_GUILDCONTRIBUTE = 58,
    DIALOG_EXTENDEDSYSMENU = 59,
    DIALOG_MUTELIST = 60,
    DIALOG_QUESTS = 61,
    DIALOG_TOPPANEL = 62,
    //#ifdef TitleClient
    DIALOG_TITLE = 63,
    //#endif
    DIALOG_MAILBOX,
    DIALOG_YESNO
};

struct MailItemSend {
    uint8_t index;
    uint32_t count;
};

class dialog_box
{
    using HandlerFunct = std::function<void(void)>;
    HandlerFunct _clickHandler;
    HandlerFunct _drawHandler;
    HandlerFunct _dblClickHandler;
    HandlerFunct _dropItemHandler;

    char _buttonCount;
    char _mode;
    char _dialogID;
    char _scrollBar;
    bool _isTopDialog;

    short _background;
    int _backgroundFrame;
    int _titleTextFrame;
    bool _trans;

    struct CButton
    {
        short _X, _Y, _X2, _Y2;
        char _ID;
        void SetupButton(short X, short X2, short Y, short Y2, char ID)
        {
            _X = X;
            _X2 = X2;
            _Y = Y;
            _Y2 = Y2;
            _ID = ID; //ID must be above 0
        }
    }
    _buttons[MAXBUTTONS];

public:
    dialog_box();
    ~dialog_box();

    void SetupDialog(char dialogID, short X, short Y, short sizeX, short sizeY, short background = -1, int backFrame = -1, int titleTxtFrame = -1, bool trans = false, helbreath * game = nullptr);
    void SetupHandlers(HandlerFunct draw, HandlerFunct click, HandlerFunct dblClick, HandlerFunct dropItem);

    char GetMode() const;
    void SetMode(char mode);
    void ClearButtons();
    void AddButtonList(short X, short X2, short Y, short Y2, char count, char startingID, short spacing = 0);
    void AddButton(short X, short X2, short Y, short Y2, char ID);

    //Scroll bar related
    void AddScrollBar(short X, short X2, short Y, short Y2, char ID/*, bool vertical = true*/);
    bool IsOnScrollBar() const;
    Point HandleScroll(int visibleAmt, int listSize);
    bool IsScrollable() const { return _scrollBar; }

    bool IsTop() const { return _isTopDialog; }
    void SetTop(bool isTop) { _isTopDialog = isTop; }

    void MoveOnScreen(short X = -1, short Y = -1);
    void CenterOverPoint(short X, short Y);

    char OnButton() const;
    bool OnDialogBox() const;

    bool HasBackground(int & sprite, int & frame) const;
    bool GetTrans() const;
    bool HasTitleText(int & frame) const;
    const RECT GetButton(int buttonID) const;
    const int ButtonX(int buttonID) const;
    const int ButtonY(int buttonID) const;

    //Handlers
    void Draw()				const { if (_drawHandler) _drawHandler(); }
    void OnClick()			const { if (_clickHandler) _clickHandler(); }
    void OnDblClick()		const { if (_dblClickHandler) _dblClickHandler(); }
    void OnDropItem()		const { if (_dropItemHandler) _dropItemHandler(); }
    bool HasDblClick()	const { return _dblClickHandler ? true : false; }

    //Debug functions
    void GetAllButtons(std::vector<RECT> * rects) const;
    void ValidateButtons() const;

    std::vector<MailItemSend> vvec;
    int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sV8;
    uint32_t dwV1, dwV2, dwT1;
    bool  bFlag;
    short x, y;
    short width, height;
    short sView;
    char  cStr[32], cStr2[32], cStr3[32], cStr4[32];
    char txt[MAX_MAIL_MSG_LENGTH];
    bool  bIsScrollSelected;

private:
    helbreath * game_ = nullptr;
};
