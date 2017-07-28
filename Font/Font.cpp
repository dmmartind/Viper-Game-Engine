//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Font Class Code
//
//****************************************************************************************
#include "Font.h"


namespace Viper2D
{
// ----------------------------------------------------------------
//  Name:			cFontManager()
//  Class:			cFontManager
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	cFontManager::cFontManager() : _2DEngine()
{
	memset(&widths,0,sizeof(widths));
}


// ----------------------------------------------------------------
//  Name:			~cFontManager()
//  Class:			cFontManager
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cFontManager::~cFontManager()
{
	
}

// ----------------------------------------------------------------
//  Name:			writeText
//  Class:			cFontManager
//  Description:    write text to the screen
//  Arguments:      int x,int y,std::string text,int color
//  Return Value:   VOID
// ----------------------------------------------------------------
void cFontManager::writeText(int x,int y,std::string text,int color)
{
	float fx=(float)x;
	float fy=(float)y;

	this->setColor(color);

	for(unsigned int x=0; x<text.length();x++)
	{
		int frame = (int)text[x];
		this->setCurrentFrame(frame);
		this->setx(fx);
		this->sety(fy);
		this->draw();


		if(widths[frame] == 0)
			widths[frame] = this->getWidth();
		fx+=(float)(widths[frame] * this->getScale()); 
	}

	

}


// ----------------------------------------------------------------
//  Name:			loadFontData
//  Class:			cFontManager
//  Description:    load font data from the font data file
//  Arguments:      char *filename
//  Return Value:   BOOL
// ----------------------------------------------------------------
bool cFontManager::loadFontData(char *filename)
{
	
	FILE *fp=new FILE();
	char *buffer=(char*)malloc(sizeof(char*)*512);
	fopen_s(&fp,filename,"rb");
		
	if(!fp)
		return false;
	fread(buffer,sizeof(char*),512,fp);
	if(ferror(fp) != 0)
	{
		fclose(fp);
		return false;
	}

	for (int y=0; y<256; y++)
	{
		widths[y]=(int)buffer[y*2];
	}

	return true;
}


// ----------------------------------------------------------------
//  Name:			getFontWidth
//  Class:			cFontManager
//  Description:    get font width
//  Arguments:      NONE
//  Return Value:   INT
// ----------------------------------------------------------------
int cFontManager::getFontWidth()
{
	return this->getWidth();
}
// ----------------------------------------------------------------
//  Name:			setFontWidth
//  Class:			cFontManager
//  Description:    set font width
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cFontManager::setFontWidth(int input)
{
	
	this->setWidth(input);


}
// ----------------------------------------------------------------
//  Name:			getFontHeight
//  Class:			cFontManager
//  Description:    get font height
//  Arguments:      NONE
//  Return Value:   INT
// ----------------------------------------------------------------

int cFontManager::getFontHeight()
{
	return this->getHeight();
}

// ----------------------------------------------------------------
//  Name:			setFontSize
//  Class:			cFontManager
//  Description:    set font size
//  Arguments:      int width,int height
//  Return Value:   void
// ----------------------------------------------------------------
void cFontManager::setFontSize(int width,int height)
{
	setWidth(width);
	setHeight(height);
}






};
