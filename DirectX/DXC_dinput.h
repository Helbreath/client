// DXC_dinput.h: interface for the DXC_dinput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXC_DINPUT_H__639F0280_78D8_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_DXC_DINPUT_H__639F0280_78D8_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#define INITGUID
#include <windows.h>
#include "dinput.h"

struct ms 
{
	static short X, Y, Z; 
	static char LB, RB, MB;
};

class DXC_dinput  
{
public: 

	DXC_dinput();
	virtual ~DXC_dinput();
	void UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB, char * pMB);
	void UpdateMouseState();
	void SetAcquire(BOOL bFlag);
	BOOL bInit(HWND hWnd, HINSTANCE hInst);

	DIMOUSESTATE dims;
	IDirectInput *           m_pDI;
	IDirectInputDevice *     m_pMouse;
	short m_sX, m_sY, m_sZ;

};

#endif // !defined(AFX_DXC_DINPUT_H__639F0280_78D8_11D2_A8E6_00001C7030A6__INCLUDED_)
