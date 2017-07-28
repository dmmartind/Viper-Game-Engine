//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Texture Class Code
//
//****************************************************************************************

#include "Texture.h"

namespace Viper2D//main engine namespace
{
// ----------------------------------------------------------------
//  Name:           Texture
//	Class:			Texture
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	Texture::Texture()
	{
		this->texture = NULL;//set the texture to null
		counter=0;//set the texture to 0
	}


// ----------------------------------------------------------------
//  Name:           Texture
//	Class:			Texture
//  Description:    class deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	Texture::~Texture()
	{
		Free();//free everything

	}


// ----------------------------------------------------------------
//  Name:           Free
//	Class:			Texture
//  Description:    free object
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
	void Texture::Free()
	{
		ReleaseCOM(this->texture);//free object
		ZeroMemory(&info,sizeof(info));//zero out the structure
	}


// ----------------------------------------------------------------
//  Name:           getWidth
//	Class:			Texture
//  Description:    return width of texture
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------

	int Texture::getWidth()
	{
		return info.Width;//get the width
	}
// ----------------------------------------------------------------
//  Name:           getHeight
//	Class:			Texture
//  Description:    return height of texture
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------

	int Texture::getHeight()
	{
		return info.Height;//return height
	}

// ----------------------------------------------------------------
//  Name:           Load
//	Class:			Texture
//  Description:    load textures
//  Arguments:      filename, color
//  Return Value:   bool
// ----------------------------------------------------------------
	bool Texture::Load(char* filename, D3DCOLOR color)
	{
		HRESULT result; //windows result variable
		//get the width and height from bitmap file
		result=D3DXGetImageInfoFromFile(filename, &info);
		if(result != D3D_OK)
		{//if the result killed 
			texture = NULL;//set texture pointer to null
			return false;//return 1;
		}
		
		D3DXCreateTextureFromFileEx(g_engine->getDevice(),//Pointer to an IDirect3DDevice9 interface
								filename,//Pointer to a filename string 
								info.Width,//width in pixels
								info.Height,//height in pixels
								1,//level of resolution with 1 being the highest
								D3DUSAGE_RENDERTARGET, //specifies that the resource is placed
													// in the preferred memory for device access 
								 					//surface is to be used as a render target
								D3DFMT_UNKNOWN,//windowed mode back buffer 
								D3DPOOL_DEFAULT,//resource is placed in the preferred memory for device access
								D3DX_DEFAULT, //A default value for the filter
								D3DX_DEFAULT,//a default value for the mip filter
								color, //color structure
								&info,//image attributes
								NULL,//pallet
								&texture);//pointer to the structure

		if(result != D3D_OK)//if not successful
		{
			texture = NULL;//set the texture to null
			return 0;//return failed
		}

		return 1;//return success

}



// ----------------------------------------------------------------
//  Name:           ReleaseCOM////may go away????????????
//	Class:			Texture
//  Description:    release pointers for textures
//  Arguments:      T& x
//  Return Value:   void
// ----------------------------------------------------------------

	template<class T>
	void Texture::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}




// ----------------------------------------------------------------
//  Name:           Release
//	Class:			Texture
//  Description:    release pointers for textures
//  Arguments:      T& x
//  Return Value:   void
// ----------------------------------------------------------------

	
	void Texture::Release()
	{

		if(texture)
		{
			texture->Release();
			ZeroMemory(&texture,sizeof(texture));
			texture = NULL;
		}
}






	// ----------------------------------------------------------------
//  Name:           GetTexture
//	Class:			Texture
//  Description:    return texture
//  Arguments:      None.
//  Return Value:   LPDIRECT3DTEXTURE9
// ----------------------------------------------------------------
	LPDIRECT3DTEXTURE9 Texture::GetTexture()
	{
		return texture;

	}
	


};














