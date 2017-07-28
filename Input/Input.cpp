//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Input Class Code 
//
//****************************************************************************************

#include "Input.h"


namespace Viper2D
{
// ----------------------------------------------------------------
//  Name:			cInput()
//  Class:			cInput
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cInput::cInput()
{
  
  m_pDI = NULL;
}

// ----------------------------------------------------------------
//  Name:			~cInput
//  Class:			cInput
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cInput::~cInput()
{
  // Force a shutdown
  Shutdown();
}

// ----------------------------------------------------------------
//  Name:			GethWnd()
//  Class:			cInput
//  Description:    return the parent window handle
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

HWND cInput::GethWnd()
{
  
  return m_hWnd;
}

// ----------------------------------------------------------------
//  Name:			GetDirectInputCOM
//  Class:			cInput
//  Description:    return a pointer to IDirectInput8 object
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

IDirectInput8 *cInput::GetDirectInputCOM()
{
  
  return m_pDI;
}

// ----------------------------------------------------------------
//  Name:			Init
//  Class:			cInput
//  Description:    create DirectInput object
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

bool cInput::Init(HWND hWnd, HINSTANCE hInst)
{
  // Free a prior Init
  Shutdown();

  // Record parent Window handle
  m_hWnd = hWnd;

  // Create a DirectInput interface
  if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
    return FALSE;

  // Return a success
  return TRUE;
}


// ----------------------------------------------------------------
//  Name:			Shutdown
//  Class:			cInput
//  Description:    shutdown the DirectInput object
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInput::Shutdown()
{
  // Release the COM objects
  ReleaseCOM(m_pDI);

  // Clear parent Window handle
  m_hWnd = NULL;

  // Return a success
  return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			cInput
//  Description:    release directx object
//  Arguments:      IDirectInputDevice8
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void cInput::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}

// ----------------------------------------------------------------
//  Name:			cInputDevice
//  Class:			cInputDevice
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cInputDevice::cInputDevice()
{
  // Clear parent cInput object pointer
  m_Input = NULL;

  // Setup device to none
  m_Type = NONE;

  // Set windowed usage to TRUE
  m_Windowed = TRUE;

  // Clear the DirectInput interface pointer
  m_pDIDevice = NULL;

  // Point the mouse and joystick structures to the state buffer
  m_MouseState    = (DIMOUSESTATE*)&m_State;
  m_JoystickState = (DIJOYSTATE*)&m_State;

  // Clear the device variables
  Clear();
}

// ----------------------------------------------------------------
//  Name:			~cInputDevice()
//  Class:			cInputDevice
//  Description:    deconstructor:free device
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cInputDevice::~cInputDevice()
{
  // Free a prior install
  Free();
}

// ----------------------------------------------------------------
//  Name:			DeviceCOM
//  Class:			cInputDevice
//  Description:    return the pointer to the IDirectDevice8 object
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
IDirectInputDevice8 *cInputDevice::DeviceCOM()
{
  
  return m_pDIDevice;
}

// ----------------------------------------------------------------
//  Name:			Create
//  Class:			cInputDevice
//  Description:    create direct input device
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::Create(cInput *Input, short Type, bool Windowed)
{
  DIDATAFORMAT *DataFormat;
  DIPROPRANGE   DIprg;
  DIPROPDWORD   DIpdw;

  // Free a prior device
  Free();

  // Check for a valid parent cInput class
  if((m_Input = Input) == NULL)
    return FALSE;

  // Create the device and remember device data format
  switch(Type) {
    case KEYBOARD:
           if(FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL))) 
             return FALSE;
           DataFormat = (DIDATAFORMAT*)&c_dfDIKeyboard;
           break;

    case MOUSE:
           if(FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL))) 
             return FALSE;
           DataFormat = (DIDATAFORMAT*)&c_dfDIMouse;
           break;

    case JOYSTICK:
           if(FAILED(m_Input->GetDirectInputCOM()->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticks, this, DIEDFL_ATTACHEDONLY)))
             return FALSE;
           if(m_pDIDevice == NULL)
             return FALSE;
           DataFormat = (DIDATAFORMAT*)&c_dfDIJoystick;
           break;

    default: return FALSE;
  }

  // Set the windowed usage
  m_Windowed = Windowed;

  // Set the data format of keyboard
  if(FAILED(m_pDIDevice->SetDataFormat(DataFormat)))
    return FALSE;

  // Set the cooperative level - Foreground & Nonexclusive
  if(FAILED(m_pDIDevice->SetCooperativeLevel(m_Input->GethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
    return FALSE;

  // Set the special properties if it's a joystick
  if(Type == JOYSTICK) {
    // Set the special properties of the joystick - range
    DIprg.diph.dwSize       = sizeof(DIPROPRANGE);
    DIprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    DIprg.diph.dwHow        = DIPH_BYOFFSET;
    DIprg.lMin              = -1024;
    DIprg.lMax              = +1024;

    // Set X range
    DIprg.diph.dwObj = DIJOFS_X;
    if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
      return FALSE;

    // Set Y rangine
    DIprg.diph.dwObj = DIJOFS_Y;
    if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
      return FALSE;

    // Set the special properties of the joystick - deadzone 12%
    DIpdw.diph.dwSize       = sizeof(DIPROPDWORD);
    DIpdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    DIpdw.diph.dwHow        = DIPH_BYOFFSET;
    DIpdw.dwData            = 128;
    
    // Set X deadzone
    DIpdw.diph.dwObj = DIJOFS_X;
    if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
      return FALSE;

    // Set Y deadzone
    DIpdw.diph.dwObj = DIJOFS_Y;
    if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
      return FALSE;
  }

  // Acquire the device for use
  if(FAILED(m_pDIDevice->Acquire()))
    return FALSE;

  // Set the device type
  m_Type = Type;

  // Clear the device information
  Clear();

  // Return a success
  return FALSE;
}

// ----------------------------------------------------------------
//  Name:			Free
//  Class:			cInputDevice
//  Description:    delete device
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::Free()
{
  // Unacquire and release the object
  if(m_pDIDevice != NULL) {
    m_pDIDevice->Unacquire();
    ReleaseCOM(m_pDIDevice);
  }

  // Set to no device installed
  m_Type = NONE;

  // Clear the data  
  Clear();

  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			Clear
//  Class:			cInputDevice
//  Description:    clear memory
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::Clear()
{
  short i;

  ZeroMemory(&m_State, 256);
  
  for(i=0;i<256;i++)
    m_Locks[i]  = FALSE;

  m_XPos = 0;
  m_YPos = 0;
  m_ZPos = 0;

  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			Read
//  Class:			cInputDevice
//  Description:    read structure and filter for each device
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::Read()
{
  HRESULT hr;
  long BufferSizes[3] = { 256, sizeof(DIMOUSESTATE), sizeof(DIJOYSTATE) };
  short i;
  

  // Make sure to have a valid IDirectInputDevice8 object
  if(m_pDIDevice == NULL)
    return FALSE;

  // Make sure device type if in range
  if(m_Type < 1 || m_Type > 3)
    return FALSE;

  // Loop polling and reading until succeeded or unknown error
  // Also take care of lost-focus problems
  while(1) {
    // Poll
    m_pDIDevice->Poll();

    // Read in state
    if(SUCCEEDED(hr = m_pDIDevice->GetDeviceState(BufferSizes[m_Type-1], (LPVOID)&m_State)))
      break;

    // Return on an unknown error  
    
		if(hr == DI_OK)
		{
			MessageBox(NULL,"DI_OK","TEST",MB_OK);
			return FALSE;
		}
		if(hr == S_FALSE)
		{
			MessageBox(NULL,"S_FALSE","TEST",MB_OK);
			return FALSE;
		}
		if(hr == DIERR_INVALIDPARAM)
		{
			MessageBox(NULL,"DIERR_INVALIDPARAM","TEST",MB_OK);
			return FALSE;
		}
		if(hr == DIERR_NOTINITIALIZED)
		{
			MessageBox(NULL,"DIERR_NOTINITIALIZED","TEST",MB_OK);
			return FALSE;
		}
		if(hr == DIERR_OTHERAPPHASPRIO)
		{
			MessageBox(NULL,"DIERR_OTHERAPPHASPRIO","TEST",MB_OK);
			return FALSE;
		}		
		
		
		
		
  
    // Reacquire and try again
    if(FAILED(m_pDIDevice->Acquire()))
      return FALSE;
  }
	
  // Since only the mouse coordinates are relative, you'll
  // have to deal with them now
  if(m_Type == MOUSE) {
    // If windowed usage, ask windows for coordinates
    if(m_Windowed == TRUE) {
      POINT pt;
      GetCursorPos(&pt);
      ScreenToClient(m_Input->GethWnd(), &pt);
      m_XPos = pt.x;
      m_YPos = pt.y;
	  
    } else {
      m_XPos += m_MouseState->lX;
      m_YPos += m_MouseState->lY;
	  m_ZPos += m_MouseState->lZ;
    }
  }

  // Released keys and button need to be unlocked
  switch(m_Type) {
    case KEYBOARD:
		//MessageBox(NULL,"key","TEST",MB_OK);
		for(i=0;i<256;i++) {
        if(!(m_State[i] & 0x80))
          m_Locks[i] = FALSE;
      }
      break;
    case MOUSE:
      for(i=0;i<4;i++) {
        if(!(m_MouseState->rgbButtons[i]))
          m_Locks[i] = FALSE;
       }
       break;
    case JOYSTICK:
      for(i=0;i<32;i++) {
        if(!(m_JoystickState->rgbButtons[i]))
          m_Locks[i] = FALSE;
       }
       break;
  }

  // return a success
  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			Acquire
//  Class:			cInputDevice
//  Description:    Aquire access to the device
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::Acquire(BOOL Active)
{
  if(m_pDIDevice == NULL)
    return FALSE;

  if(Active == TRUE)
    m_pDIDevice->Acquire();
  else
    m_pDIDevice->Unacquire();

  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			GetLock
//  Class:			cInputDevice
//  Description:    get lock for key
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::GetLock(char Num)
{
  return m_Locks[Num];
}

// ----------------------------------------------------------------
//  Name:			SetLock
//  Class:			cInputDevice
//  Description:    set lock on key
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::SetLock(char Num, bool State)
{
  m_Locks[Num] = State;
  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			GetXPos
//  Class:			cInputDevice
//  Description:    get x position on the joystick and mouse
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetXPos()
{
  // Update coordinates if a joystick
  if(m_Type == JOYSTICK)
    m_XPos = m_JoystickState->lX;

  return m_XPos;
}

// ----------------------------------------------------------------
//  Name:			SetXPos
//  Class:			cInputDevice
//  Description:    set x position of the mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::SetXPos(long XPos)
{
  m_XPos = XPos;
  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			GetYPos
//  Class:			cInputDevice
//  Description:    get y position for mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetYPos()
{
  // Update coordinates if a joystick
  if(m_Type == JOYSTICK)
    m_YPos = m_JoystickState->lY;

  return m_YPos;
}

// ----------------------------------------------------------------
//  Name:			SetYPos
//  Class:			cInputDevice
//  Description:    set y position for mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::SetYPos(long YPos)
{
  m_YPos = YPos;
  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			GetXDelta
//  Class:			cInputDevice
//  Description:    get change in x for the mouse and the joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetXDelta()
{
  switch(m_Type) {
    case MOUSE:
           return m_MouseState->lX;

    case JOYSTICK:
           return m_JoystickState->lX - m_XPos;

    default: return 0;
  }
}

// ----------------------------------------------------------------
//  Name:			GetYDelta
//  Class:			cInputDevice
//  Description:    get change in y for the mouse and the joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetYDelta()
{
  switch(m_Type) {
    case MOUSE:
           return m_MouseState->lY;

    case JOYSTICK:
           return m_JoystickState->lY - m_YPos;

    default: return 0;
  }
}

// ----------------------------------------------------------------
//  Name:			GetKeyState
//  Class:			cInputDevice
//  Description:    get the state of the key checking the state of the key and key lock
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::GetKeyState(char Num)
{
  // Check if key/button is pressed
  if(m_State[Num] & 0x80 && m_Locks[Num] == FALSE)
    return TRUE;
  return FALSE;
}

// ----------------------------------------------------------------
//  Name:			SetKeyState
//  Class:			cInputDevice
//  Description:    set key state
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::SetKeyState(char Num, BOOL State)
{
  m_State[Num] = State;
  return TRUE;
}

// ----------------------------------------------------------------
//  Name:			GetPureKeyState
//  Class:			cInputDevice
//  Description:    get key state without lock
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::GetPureKeyState(char Num)
{
  return ((m_State[Num] & 0x80) ? TRUE : FALSE);
}

// ----------------------------------------------------------------
//  Name:			GetKeypress
//  Class:			cInputDevice
//  Description:    get what key is pressed
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
short cInputDevice::GetKeypress(long TimeOut)
{
  static HKL KeyboardLayout = GetKeyboardLayout(0);
  unsigned char WinKeyStates[256], DIKeyStates[256];
  unsigned short i, ScanCode, VirtualKey, Keys, Num;
  unsigned long EndTime;
  
  // Make sure it's a keyboard and its initialized
  if((m_Type != KEYBOARD) || (m_pDIDevice == NULL))
    return 0;

  // Calculate end time for TimeOut
  EndTime = GetTickCount() + TimeOut;

  // Loop until timeout or key pressed
  while(1) {
    // Get Windows keyboard state
    GetKeyboardState(WinKeyStates);

    // Get DirectInput keyboard state
    m_pDIDevice->GetDeviceState(256, DIKeyStates);

    // Scan through looking for key presses
    for(i=0;i<256;i++) {
      // If one found, try to convert it
      if(DIKeyStates[i] & 0x80) {
        // Get virtual key code
        if((VirtualKey = MapVirtualKeyEx((ScanCode = i), 1, KeyboardLayout))) {
          // Get ASCII code of key and return it
          Num = ToAsciiEx(VirtualKey, ScanCode, WinKeyStates, &Keys, 0, KeyboardLayout);
          if(Num)
            return Keys;
        }
      }
    }

    // Check for TimeOut
    if(TimeOut) {
      if(GetTickCount() > EndTime)
        return 0;
    }
  }

  return 0;
}

// ----------------------------------------------------------------
//  Name:			GetNumKeyPresses
//  Class:			cInputDevice
//  Description:    get the number of presses of a particular key with locks
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetNumKeyPresses()
{
  long i, Num = 0;

  for(i=0;i<256;i++) {
    if(m_State[i] & 0x80 && m_Locks[i] == FALSE)
      Num++;
  }

  return Num;
}

// ----------------------------------------------------------------
//  Name:			GetNumPureKeyPresses
//  Class:			cInputDevice
//  Description:    get number of presses of a key without locks
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetNumPureKeyPresses()
{
  long i, Num = 0;

  for(i=0;i<256;i++) {
    if(m_State[i] & 0x80)
      Num++;
  }

  return Num;
}

// ----------------------------------------------------------------
//  Name:			GetButtonState
//  Class:			cInputDevice
//  Description:    get button state from mouse,keyboard, and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::GetButtonState(char Num)
{
  char State = 0;

  if(m_Type == MOUSE)
    State = m_MouseState->rgbButtons[Num];

  if(m_Type == JOYSTICK)
    State = m_JoystickState->rgbButtons[Num];

  // Check if key/button is pressed
  if(State & 0x80 && m_Locks[Num] == FALSE)
    return TRUE;
  return FALSE;
}

// ----------------------------------------------------------------
//  Name:			SetButtonState
//  Class:			cInputDevice
//  Description:    set button state on a mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
bool cInputDevice::SetButtonState(char Num, BOOL State)
{
  if(m_Type == MOUSE) {
    m_MouseState->rgbButtons[Num] = State;
    return TRUE;
  }

  if(m_Type == JOYSTICK) {
    m_JoystickState->rgbButtons[Num] = State;
    return TRUE;
  }

  return FALSE;
}

// ----------------------------------------------------------------
//  Name:			GetPureButtonState
//  Class:			cInputDevice
//  Description:    get button state of a mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
BYTE cInputDevice::GetPureButtonState(char Num)
{
  if(m_Type == MOUSE)
    return m_MouseState->rgbButtons[Num];

  if(m_Type == JOYSTICK)
    return m_JoystickState->rgbButtons[Num];

  return FALSE;
}

// ----------------------------------------------------------------
//  Name:			GetNumButtonPresses
//  Class:			cInputDevice
//  Description:    get number of button presses from mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetNumButtonPresses()
{
  long i, Num = 0;
  
  if(m_Type == MOUSE) {
    for(i=0;i<4;i++) {
      if(m_MouseState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
        Num++;
    }
  } else
  if(m_Type == JOYSTICK) {
    for(i=0;i<32;i++) {
      if(m_JoystickState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
        Num++;
    }
  }

  return Num;
}

// ----------------------------------------------------------------
//  Name:			GetNumPureButtonPresses
//  Class:			cInputDevice
//  Description:    get number of button presses for mouse and joystick
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetNumPureButtonPresses()
{
  long i, Num = 0;
  
  if(m_Type == MOUSE) {
    for(i=0;i<4;i++) {
      if(m_MouseState->rgbButtons[i] & 0x80)
        Num++;
    }
  } else
  if(m_Type == JOYSTICK) {
    for(i=0;i<32;i++) {
      if(m_JoystickState->rgbButtons[i] & 0x80)
        Num++;
    }
  }

  return Num;
}

// ----------------------------------------------------------------
//  Name:			EnumJoysticks
//  Class:			cInputDevice
//  Description:    enumerate through to find all joystick devices
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
BOOL FAR PASCAL cInputDevice::EnumJoysticks(LPCDIDEVICEINSTANCE pdInst, LPVOID pvRef)
{
  cInputDevice *Input;

  // Stop enumeration if no parent cInputDevice pointer
  if((Input = (cInputDevice*)pvRef) == NULL)
    return DIENUM_STOP;

  // Try to create a joystick interface
  if(FAILED(Input->m_Input->GetDirectInputCOM()->CreateDevice(pdInst->guidInstance, &Input->m_pDIDevice, NULL)))
    return DIENUM_CONTINUE;

  // All done - stop enumeration
  return DIENUM_STOP;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			cInputDevice
//  Description:    release directx object
//  Arguments:      IDirectInputDevice8
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void cInputDevice::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}


// ----------------------------------------------------------------
//  Name:			_GetKeyState
//  Class:			cInputDevice
//  Description:    get key state of a specific key
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
char cInputDevice::_GetKeyState(int key)
{ 
	return m_State[key]; 
}

// ----------------------------------------------------------------
//  Name:			GetZPos
//  Class:			cInputDevice
//  Description:    get z position for a mouse
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
long cInputDevice::GetZPos()
{

	return m_MouseState->lZ;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetZDelta
//	Class:			cInputDevice
//  Description:    get change in x for mouse
//  Arguments:      NONE
//  Return Value:   long
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
long cInputDevice::GetZDelta()
{
 return m_MouseState->lZ - m_ZPos;

}



};
