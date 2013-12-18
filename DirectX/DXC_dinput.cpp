// DXC_dinput.cpp: implementation of the DXC_dinput class.
//
//////////////////////////////////////////////////////////////////////

#include "DXC_dinput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

short ms::X; 
short ms::Y; 
short ms::Z; 
char ms::LB;
char ms::RB; 
char ms::MB;

DXC_dinput::DXC_dinput()
{
	m_pDI    = NULL;
	m_pMouse = NULL;
	m_sX     = 0;
	m_sY     = 0;
	m_sZ     = 0;
}

DXC_dinput::~DXC_dinput()
{
	if (m_pMouse != NULL) {
		m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = NULL;
	}
	if (m_pDI != NULL) {
		m_pDI->Release();
        m_pDI = NULL;
	}
}

BOOL DXC_dinput::bInit(HWND hWnd, HINSTANCE hInst)
{
 HRESULT hr;
 DIMOUSESTATE dims;	
 POINT Point;

	GetCursorPos(&Point);
	m_sX     = (short)(Point.x);
	m_sY     = (short)(Point.y); 

	hr = DirectInputCreate( hInst, DIRECTINPUT_VERSION, &m_pDI, NULL );
    if (hr != DI_OK) return FALSE;
	hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DI_OK) return FALSE;

//	m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		//return TRUE;
	}

	return TRUE;
}


void DXC_dinput::SetAcquire(BOOL bFlag)
{
 DIMOUSESTATE dims;

	if (m_pMouse == NULL) return;
	if (bFlag == TRUE) {
		m_pMouse->Acquire();
		m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	}
	else m_pMouse->Unacquire();
}

void DXC_dinput::UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB, char * pMB)
{
	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		return;
	}
	m_sX += (short)dims.lX;
	m_sY += (short)dims.lY;
	if( (short)dims.lZ != 0 )m_sZ = (short)dims.lZ;
	if (m_sX < 0) m_sX = 0;
	if (m_sY < 0) m_sY = 0;
	//if (m_sX > 639) m_sX = 639;
	if (m_sX > 799) m_sX = 799; // 800x600 Resolution xRisenx
	//if (m_sY > 479) m_sY = 479;
	if (m_sY > 599) m_sY = 599; // 800x600 Resolution xRisenx
	*pX = m_sX;
	*pY = m_sY;
	*pZ = m_sZ;
	*pLB = (char)dims.rgbButtons[0];
	*pRB = (char)dims.rgbButtons[1];
	*pMB = (char)dims.rgbButtons[2];
}

void DXC_dinput::UpdateMouseState()
{
	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		return;
	}
	m_sX += (short)dims.lX;
	m_sY += (short)dims.lY;
	if( (short)dims.lZ != 0 )m_sZ = (short)dims.lZ;

	if (m_sX < 0) m_sX = 0;
	else if (m_sX > 799) m_sX = 799; // 800x600 Resolution xRisenx
	else if (m_sX > 799) m_sX = 799; // 800x600 Resolution xRisenx

	if (m_sY < 0) m_sY = 0;
	else if (m_sY > 599) m_sY = 599; // 800x600 Resolution xRisenx
	else if (m_sY > 599) m_sY = 599; // 800x600 Resolution xRisenx

	ms::X = m_sX;
	ms::Y = m_sY;
	ms::Z = m_sZ;
	ms::LB = (char)dims.rgbButtons[0];
	ms::RB = (char)dims.rgbButtons[1];
	ms::MB = (char)dims.rgbButtons[2];
}