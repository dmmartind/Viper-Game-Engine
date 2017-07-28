//========= Copyright © 2009, David Martin, All rights reserved. ============
//
// Purpose: Console Class Code
//
//=============================================================================

#include "Console.h"

namespace Viper2D
{

// ----------------------------------------------------------------
//  Name:           CGameConsole
//	Class:			CGameConsole
//  Description:    Constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
CGameConsole::CGameConsole()
{
	isVisible = false;//set visible to false
    currentLine = 0;//set current line to 0
    //clear();
}

// ----------------------------------------------------------------
//  Name:           ~CGameConsole
//	Class:			CGameConsole
//  Description:    Destructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

CGameConsole::~CGameConsole()
{
	 delete font;//delete font object
     delete panel;//delete panel object
}

// ----------------------------------------------------------------
//  Name:           Initialize
//	Class:			CGameConsole
//  Description:    sets up the console for use
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------

bool CGameConsole::Initialize()
{
	panel = new _2DEngine();//create panel as a 2d object
	if(!panel->loadImage("panel.tga"))//load image panel.tga
		return false;//if failed,return
	double scale = g_engine->getScreenWidth() / 640.0f;//calculate scale to upper half of the screen
	panel->setScale(scale);//set scale
	panel->setColor(D3DCOLOR_ARGB(50,255,255,255));//set color to transparent white
	

	font = new cFontManager();//create font as a font object
	if(!font->loadImage("system12.tga"))//load font picture
		return false;//return false, if failed
	font->setColumns(16);//set the number of columns in the picture 
	font->setFontSize(14,16);//set the size of font in the picture

	if(!font->loadFontData("system12.dat"))//load font data for extracting the font
		return false;//return false, if failed

	return true;//return true otherwise
}



// ----------------------------------------------------------------
//  Name:           Activate
//	Class:			CGameConsole
//  Description:    activates the console, makes it visible and brings it to the foreground
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
bool CGameConsole::Activate()
{
	int x=5, y = 0;//row and column panel location for text
	if(!isVisible)//if console is not visible
		return false;//return fail
	panel->draw();//draw panel
	for(unsigned int n = 0; n < textlines.size(); n++)//interate through the text rows
	{
		font->writeText(x,y*14,textlines[n],D3DCOLOR_XRGB(255,255,255));//write text
		y += 1;//increment y
		if(y > 26)//if y > 26
		{
			if(x > 10)//if x > 10
				break;//break out of loop
			x=g_engine->getScreenWidth()/2 + 5;//reset x
			y=0;//reset y
		}


	}

	return true;//return success

}
// ----------------------------------------------------------------
//  Name:           Show
//	Class:			CGameConsole
//  Description:    shows the console
//  Arguments:      None.
//  Return Value:   VOID
// ----------------------------------------------------------------
void CGameConsole::Show()
{
	isVisible = true;//set the visible flag is true
}

// ----------------------------------------------------------------
//  Name:           Hide
//	Class:			CGameConsole
//  Description:    hides the console
//  Arguments:      None.
//  Return Value:   VOID
// ----------------------------------------------------------------
void CGameConsole::Hide()
{
	isVisible = false;//set the visible flag to false
}

// ----------------------------------------------------------------
//  Name:           Clear
//	Class:			CGameConsole
//  Description:    clears the console
//  Arguments:      None.
//  Return Value:   VOID
// ----------------------------------------------------------------
void CGameConsole::Clear()
{
	//for(int x=0;x<(signed)textlines.size();x++)
	//{
	//	if(!textlines[x].empty())
	//		textlines[x].erase(x);
	//}
	textlines.clear();//clear the panel

}

// ----------------------------------------------------------------
//  Name:           Printf
//	Class:			CGameConsole
//  Description:    prints a message to the console
//  Arguments:      std::string text, int line
//  Return Value:   VOID
// ----------------------------------------------------------------
void CGameConsole::Printf(std::string text, int line)
{
	if (line > -1) //if line is > -1
		currentLine = line;//set current line to line
	textlines.push_back(text);//push textline
	if (currentLine++ > 52)//increment current line and if it's > 52
		currentLine = 0;//set it to 0

}

// ----------------------------------------------------------------
//  Name:           DPrintf
//	Class:			CGameConsole
//  Description:    function for debugging purpose.
//					***future feature***
//  Arguments:      const char *format, ...
//  Return Value:   VOID
// ----------------------------------------------------------------
void CGameConsole::DPrintf(const char *format, ...)
{
//					***future feature***

}


// ----------------------------------------------------------------
//  Name:           IsConsoleVisible
//	Class:			CGameConsole
//  Description:    returns true if the console is currently in focus
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
bool CGameConsole::IsConsoleVisible()
{
	return isVisible;//returns 
}





};