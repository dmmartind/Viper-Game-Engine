//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: An application framework 

//****************************************************************************************

#ifndef _WINMAIN_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _WINMAIN_CPP_//define one time and let everyone share, so VS doesn't give annoying errors

//macro to read the key states
//#define KEY_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000)?1:0)


#include "winmain.h"//winmain header


Viper2D::Engine *g_engine;//global engine object



// ----------------------------------------------------------------
//  Name:           cApplication
//	Class:          cApplication
//  Description:    Constructor to inititialize the object
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

cApplication::cApplication()//constructor
{
	m_hInst = GetModuleHandle(NULL);
	ZeroMemory(&msg, sizeof(MSG));//zero out the message structure
	srand((unsigned int)time(NULL));//random generator 
	
	g_engine = new Viper2D::Engine();//new engine object
	
	
	if (!game_preload()) {//run game preload function on 2nd layer
		Error(true,"Error in game preload!" );//error if failed
	}
	
	
	//get window caption from the engine object
	sprintf_s(title,sizeof(g_engine->getAppTitle())+1, "%s", g_engine->getAppTitle().c_str());

	//set window dimensions
	windowRect.left = (long)0;
	windowRect.right = (long)g_engine->getScreenWidth();
	windowRect.top = (long)0;
	windowRect.bottom = (long)g_engine->getScreenHeight();


   //create the window class structure
	m_wcex.cbSize = sizeof(WNDCLASSEX); 

	//fill the struct with info
	m_wcex.style		 = CS_HREDRAW | CS_VREDRAW;//redraw the width and height changes
	m_wcex.lpfnWndProc   = (WNDPROC)WinProc;//window procedure pointer function
	m_wcex.cbClsExtra	 = 0;//null
	m_wcex.cbWndExtra	 = 0;//null
	m_wcex.hInstance	 = m_hInst;//instance of the class
	m_wcex.hIcon		 = NULL;//default application icon
	m_wcex.hCursor	   = LoadCursor(NULL, IDC_ARROW);//cursor
	m_wcex.hbrBackground = NULL;//default screen background
	m_wcex.lpszMenuName  = NULL;//no menu
	m_wcex.lpszClassName = title;//use the title name as the window class name
	m_wcex.hIconSm	   = NULL;//default application icon
}


// ----------------------------------------------------------------
//  Name:           ~cApplication
//	Class:          cApplication
//  Description:    Deconstructor to clean up
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cApplication::~cApplication()
{
	
	if(g_engine != NULL)//if engine object exist
		delete g_engine;//delete the object
}


// ----------------------------------------------------------------
//  Name:           GethWnd
//	Class:          cApplication
//  Description:    return the handle of the current window
//  Arguments:      None.
//  Return Value:   HWND.
// ----------------------------------------------------------------
HWND cApplication::GethWnd()
{
  return m_hWnd;//
}

// ----------------------------------------------------------------
//  Name:           GethInst
//	Class:          cApplication
//  Description:    return the current instanace of a handle being used
//  Arguments:      None.
//  Return Value:   HWND.
// ----------------------------------------------------------------

HINSTANCE cApplication::GethInst()
{
  return m_hInst;
}


// ----------------------------------------------------------------
//  Name:           Run
//	Class:          cApplication
//  Description:    Game Loop 
//  Arguments:      None.
//  Return Value:   BOOL.
// ----------------------------------------------------------------


BOOL cApplication::Run()
{
  
	RegisterClassEx(&m_wcex);//register the window class with the window manager

	//set up the screen in windowed or fullscreen mode?

	if (g_engine->getFullscreen()) //if fullscreen
	{
	   DEVMODE dm;//video card/printer data structure to set the attributes
	   memset(&dm, 0, sizeof(dm));//set the structure to zero
	   dm.dmSize = sizeof(dm);//set the size of the structure to the default size
	   dm.dmPelsWidth = g_engine->getScreenWidth();//set the screen width
	   dm.dmPelsHeight = g_engine->getScreenHeight();//set the screen height
	   dm.dmBitsPerPel = g_engine->getColorDepth();//set the color depth
	   dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//flags for the surface buffer setting the 
	   //bit resolution, the height, and the width

	   if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
		   //change the screen settings
		  Error(true, "Display mode failed");//display error message without shutting down the 
						//engine
		  
	   }

		dwStyle = WS_POPUP;//pop-up window
		dwExStyle = WS_EX_APPWINDOW;//the title is set in the taskbar
		ShowCursor(FALSE);//disable the cursor
	}
	else {//if it is not fullscreen
		dwStyle = WS_OVERLAPPEDWINDOW;//general formed window with a title, min,max,close buttons, 
		//and a system menu
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;//Specifies that a window has a border with a raised edge.
														//and taskbar title
	}

	//adjust window to true requested size
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//create the program window
	m_hWnd = CreateWindowEx( 0,
	   title,								 //window class
	   title,								 //title bar
	   dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	  
	   0, 0,											 //x,y coordinate
	   windowRect.right - windowRect.left,			   //width of the window
	   windowRect.bottom - windowRect.top,			   //height of the window
	   0,											 //parent window
	   0,											 //menu
	   m_hInst,									  //application instance
	   0);											//window parameters

	//was there an error creating the window?
	if (!m_hWnd) 	{
		Error(true,"Error creating program window!");
		
	}

	//display the window
	ShowMouse(0);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	//initialize the engine
	g_engine->setWindowHandle(m_hWnd);//set the window handle to the engine
	g_engine->setWindowInstance();
	if (!g_engine->Init(g_engine->getScreenWidth(), g_engine->getScreenHeight(), g_engine->getColorDepth(), g_engine->getFullscreen())) 	{
		//start the engine with the screen width,height, color depth, and fullscreen flag
		Error(true, "Error initializing the engine");
		}

	// main message loop
	gameover = false;//set the gameover state to false
	while (!gameover)//while the flag is false
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//pop the message queue
		{
			TranslateMessage(&msg);//extract the arguments
			DispatchMessage(&msg);//send the new message to the window manager 
									//to call the messege procedure
		}
		g_engine->Update();//frame render
	}

   if (g_engine->getFullscreen()) {//if the screen is fullscreen
	  ShowCursor(TRUE);//show the cursor
   }

	g_engine->Close();//kill the engine at close
	delete g_engine;//delete the engine object

   return 1;//return 1 for success
}


// ----------------------------------------------------------------
//  Name:           Error
//	Class:          cApplication
//  Description:    Error Function 
//  Arguments:      BOOL Fatal, char *Text, ....
//  Return Value:   BOOL.
// ----------------------------------------------------------------

BOOL cApplication::Error(BOOL Fatal, char *Text, ...)//Errors???? NO WAY!!!!!
{
  char CaptionText[12];//caption of the messagebox
  char ErrorText[2048];//text of the messagebox
  va_list valist;//create a new argument list

  // Build the message box caption based on fatal flag
  if(Fatal == FALSE)
    strcpy_s(CaptionText,sizeof("Error"), "Error");
  else 
    strcpy_s(CaptionText, sizeof("Fatal Error"),"Fatal Error");

  // Build variable text buffer
  va_start(valist, Text);//start buffer macro and add Text string to it
  vsprintf_s(ErrorText, Text, valist);//copy text string into the ErrorText buffer
  va_end(valist);//end macro

  // Display the message box
  MessageBox(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);

  // Post a quit message if error was fatal
  if(Fatal == TRUE)
    PostQuitMessage(0);

  return TRUE;//return success
}

// ----------------------------------------------------------------
//  Name:           Move
//	Class:          cApplication
//  Description:    repaint window to new loaction
//  Arguments:      long XPos, long YPos
//  Return Value:   BOOL.
// ----------------------------------------------------------------

BOOL cApplication::Move(long XPos, long YPos)//
{
  RECT ClientRect;//create rectangle structure

  GetClientRect(m_hWnd, &ClientRect);//set the structure to the handle of the current window
  MoveWindow(m_hWnd, XPos, YPos, ClientRect.right, ClientRect.bottom, TRUE);//repaint window in new position

  return TRUE;//return success
}

// ----------------------------------------------------------------
//  Name:           Resize
//	Class:          cApplication
//  Description:    repaint window to resize to new cordinates
//  Arguments:      long XPos, long YPos
//  Return Value:   BOOL.
// ----------------------------------------------------------------

BOOL cApplication::Resize(long Width, long Height)//
{
  RECT WndRect, ClientRect;//create rectangle structure 
  long WndWidth, WndHeight;//declare var

  GetWindowRect(m_hWnd, &WndRect);//set the rectangle structs to the current window handle
  GetClientRect(m_hWnd, &ClientRect);//set the rectangle strut to the current window handle

  WndWidth  = (WndRect.right  - (ClientRect.right  - Width))  - WndRect.left;//calculate the new cordinates for the width of the window 
  WndHeight = (WndRect.bottom - (ClientRect.bottom - Height)) - WndRect.top;//recalculate the new cordinates of the height of the new window

  MoveWindow(m_hWnd, WndRect.left, WndRect.top, WndWidth, WndHeight, TRUE);//repaint new window

  return TRUE;//return success
}


// ----------------------------------------------------------------
//  Name:           ShowMouse
//	Class:          cApplication
//  Description:    tell mouse to stay or leave
//  Arguments:      BOOL Show
//  Return Value:   BOOL.
// ----------------------------------------------------------------

BOOL cApplication::ShowMouse(BOOL Show)//tell mouse to stay or leave
{
  ShowCursor(Show);//show mouse/ make mouse disappear
  return TRUE;//return sucess
}

// ----------------------------------------------------------------
//  Name:           WinProc
//  Description:    window message procedure
//  Arguments:      HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
//  Return Value:   long FAR PASCAL.
// ----------------------------------------------------------------


LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )//check message structure message
	{
		 case WM_QUIT://if quit message
		 case WM_CLOSE://if close message
		 case WM_DESTROY://if destroy message
			 gameover = true;//set gameover state flag to true
			 break;//break out of switch
   }
	return DefWindowProc( hWnd, msg, wParam, lParam );//if message is not handled, go to default


}


// ----------------------------------------------------------------
//  Name:           WinMain
//  Description:    entry-point
//  Arguments:      HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
//  Return Value:   long FAR PASCAL.
// ----------------------------------------------------------------

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,          \
                   LPSTR szCmdLine, int nCmdShow)
{
  cApplication App;//create application object
  return App.Run();//run application
}

// ----------------------------------------------------------------
//  Name:           SethWnd
//	Class:          cApplication
//  Description:    sets the application handle to be used elsewhere
//  Arguments:      HWND 
//  Return Value:   VOID
// ----------------------------------------------------------------
void cApplication::SethWnd(HWND input)
{

	this->m_hWnd=input;//set the window handle





}

//void cApplication::SethInst(HINSTANCE input)
//{

//	this->m_hInst=input;


//}

#endif
