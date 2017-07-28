//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Mesh Class Code 

//****************************************************************************************

#ifndef _MESH_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _MESH_CPP_//define one time and let everyone share, so VS doesn't give annoying errors


#include "Mesh.h"



namespace Viper2D
{
	// ----------------------------------------------------------------
//  Name:           Mesh
//	Class:			Mesh
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	Mesh::Mesh(void):cBaseEntity(RENDER3D)//mesh contrsuctor
	{
		mesh=0;//set mesh to 0
		materials=0;//set materials to 0
		d3dxMaterials=0;//set directx materials to 0
		matbuffer=0;//set the material buffer to 0
		material_count=0;//set material count to 0
		textures=0;//set textures to 0
		position= D3DXVECTOR3(0.0f,0.0f,0.0f);//set the mesh position to 0
		velocity= D3DXVECTOR3(0.0f,0.0f,0.0f);//set the mesh velocity to 0
		rotation= D3DXVECTOR3(0.0f,0.0f,0.0f);//set the mesh rotation to 0
		scale= D3DXVECTOR3(1.0f,1.0f,1.0f);// set the mesh scale to 1


	}

	// ----------------------------------------------------------------
//  Name:           ~Mesh
//	Class:			Mesh
//  Description:    class destructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	Mesh::~Mesh(void)//mesh destructor
	{
		Free();

	}



	void Mesh::Free()
	{

		if( materials != NULL)//if there any materials in the buffer
			delete[] materials;//delete all the materials
			//ReleaseCOM(materials);
			materials = NULL;


		//remove textures from memory
		if(textures != NULL)//if there are any textures in memeory
		{
			for(DWORD i =0; i< material_count; i++)//for each count
			{
				if(textures[i] != NULL)//set the array pointer to null 
					textures[i]->Release();//release pointer
			}
			delete[] textures;//delete texture pointer
			textures = NULL;
		}
		if(mesh != NULL)//if mesh is still in memory
			ReleaseCOM(mesh);

		if(d3dxMaterials != NULL)//if mesh is still in memory
			d3dxMaterials = NULL;//just a pointer to the material buffer, so just NULL it

		if(matbuffer != NULL)//if mesh is still in memory
			ReleaseCOM(matbuffer);//**********do another
		
			
		
		
		

	}

	// ----------------------------------------------------------------
//  Name:           GetFaceCount
//	Class:			Mesh
//  Description:    get the polygon face count
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	int Mesh::GetFaceCount()
	{
		return this->mesh->GetNumFaces();
	}

	// ----------------------------------------------------------------
//  Name:           GetVertexCount
//	Class:			Mesh
//  Description:    get the number of vertices
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	int Mesh::GetVertexCount()
	{
		return this->mesh->GetNumVertices();
	}

	// ----------------------------------------------------------------
//  Name:           Load
//	Class:			Mesh
//  Description:    loads mesh from file, creates array for materials and textures and save them to memeory
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	bool Mesh::Load(char* filename)//mesh loader
	{
		HRESULT result;//create result var for capturing result
		//load mesh from the specified file
		result=D3DXLoadMeshFromX(filename,D3DXMESH_SYSTEMMEM,g_engine->getDevice(),NULL,&matbuffer,NULL,&material_count,&mesh);
		//load mesh with the filename,vertex buffers and index buffer class,directx device,no adjacent buffer,
		//material buffer,no effect buffer, number of materials in the meterial buffer, mesh buffer
		if(result != D3D_OK)//if the load was unsuccessful
		{
			g_engine->fatalerror("FATAL ERROR!:: Mesh not found", "Viper 2D Engine");
			return false;//return false
		}

		//extract material properties and texture names from material buffers
		d3dxMaterials=(LPD3DXMATERIAL)matbuffer->GetBufferPointer();//get the pointer to the material buffer
		materials= new D3DMATERIAL9[material_count];//create new pointer with the meterial count for the
		//length of the array
		textures= new LPDIRECT3DTEXTURE9[material_count];//create new texture buffer with the material count 
		//as the length of the array

		//create te materials and textures
		for(DWORD i=0; i< material_count; i++)//for each material count
		{
			//grab the material
			materials[i]= d3dxMaterials[i].MatD3D;//structure that describes the material properties. 

			//set ambient color for material
			materials[i].Ambient= materials[i].Diffuse;//set the ambient color the same as the
			//diffused or reflected light
			materials[i].Emissive= materials[i].Diffuse;//emmision of light from the object
			materials[i].Power=0.5f;//increases or decreases the sharpness of specular emmisions
			materials[i].Specular=materials[i].Diffuse;//shiness of an object

			textures[i]=NULL;//set current texture to null
			//make sure the texture filename is not null and the string length is > 0
			if(d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
			
			{//load texture file specified in .x file
				result=D3DXCreateTextureFromFile(g_engine->getDevice(),d3dxMaterials[i].pTextureFilename, &textures[i]);
				//directx device, texture filename, texture buffer
				if(result != D3D_OK)//if not successful
				{
					return false;
				}
			}
		}

		

		return true;//return true is successful
	}


	// ----------------------------------------------------------------
//  Name:           CreateSphere
//	Class:			Mesh
//  Description:    Uses a left-handed coordinate system to create a mesh containing a sphere
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::CreateSphere(float radius, int slices, int stacks)
	{
		
		D3DXCreateSphere(g_engine->getDevice(), radius,slices,stacks,&mesh,NULL);
		//directx, Radius of the sphere, Number of slices about the main axis,Number of stacks along the main axis
		//mesh buffer,no adjacent buffer

	}


	// ----------------------------------------------------------------
//  Name:           CreateCube
//	Class:			Mesh
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::CreateCube(float width, float height, float depth)
	{
		//Uses a left-handed coordinate system to create a mesh containing an axis-aligned box	
		D3DXCreateBox(g_engine->getDevice(), width, height, depth,&mesh,NULL);
		//get directx,width of the box, height of the box, depth of the box, mesh buffer,adjacent buffer nulled

	}


	// ----------------------------------------------------------------
//  Name:           Draw
//	Class:			Mesh
//  Description:    draw the subset
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::Draw()
	{
		if (material_count == 0)//if the material count is 0
			mesh->DrawSubset(0);//Draws a subset of a mesh;Specifies which subset of the mesh to draw
		else
		{
			//draw each mesh subset
		for(DWORD i=0; i < material_count; i++)//looping through the material
		{
			//set the material and texture for this subset
			g_engine->getDevice()->SetMaterial(&materials[i]);//set material

			if(textures[i])//if textures are part of this set
			{
				if(textures[i]->GetType() == D3DRTYPE_TEXTURE)//as long as it is a texture
				{
					D3DSURFACE_DESC desc;//Describes a surface
					textures[i]->GetLevelDesc(0,&desc);//Retrieves a description of one face of the specified cube
					//texture level
					if(desc.Width > 0)//Width of the surface > 0
					{
						//set texture
						g_engine->getDevice()->SetTexture(0,textures[i]);
					}
				}
			}
		//draw the mesh subset
		mesh->DrawSubset(i);
		}
	}
}

	// ----------------------------------------------------------------
//  Name:           Transform
//	Class:			Mesh
//  Description:    creates the transform matrix
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::Transform()
	{
		//set rotation matrix
		float x= D3DXToRadian(rotation.x);//convert x to radians
		float y= D3DXToRadian(rotation.y);//convert y to radians
		float z= D3DXToRadian(rotation.z);//convert z to radians
		D3DXMatrixRotationYawPitchRoll(&matRotate, x, y, z);//create rotation matrix

		//set scaling matrix
		D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);//create scaling matrix
		D3DXMatrixTranslation(&matTranslate, position.x, position.y,position.z);//create translation matrix

		//transform the mesh
		matWorld= matRotate * matScale * matTranslate;//create the world matrix
		g_engine->getDevice()->SetTransform(D3DTS_WORLD, &matWorld);//create the transform matrix
	}


	// ----------------------------------------------------------------
//  Name:           Rotate
//	Class:			Mesh
//  Description:    rotate a mesh
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::Rotate(D3DXVECTOR3 rot)
	{
		Rotate(rot.x,rot.y,rot.z);
	}


	// ----------------------------------------------------------------
//  Name:           Rotate
//	Class:			Mesh
//  Description:    rotate a mesh
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::Rotate(float x, float y, float z)
	{
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}


	// ----------------------------------------------------------------
//  Name:           Update
//	Class:			Mesh
//  Description:    move the position of the mesh
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::move()
	{
		position.x += velocity.x;
		position.y += velocity.y;
		position.z += velocity.z;
	}


	// ----------------------------------------------------------------
//  Name:           LimitBoundary
//	Class:			Mesh
//  Description:    basic collision detection for the mesh in reference to the screen
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::LimitBoundary(float left, float right, float top, float bottom, float back, float front)
	{
		if(position.x < left || position.x > right)
		{
			velocity.x *= -1;
		}

		if(position.y < bottom || position.y > top)
		{
			velocity.y *= -1;
		}

		if(position.z < front || position.z > back)
		{
			velocity.z *= -1;
		}
	}


// ----------------------------------------------------------------
//  Name:           SetPosition
//	Class:			Mesh
//  Description:    //set position
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	void Mesh::SetPosition(D3DXVECTOR3 pos)
		{
			position=pos;
		}

		// ----------------------------------------------------------------
//  Name:           SetPosition
//	Class:			Mesh
//  Description:    //set position
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetPosition(float x, float y, float z)
		{
			position=D3DXVECTOR3(x,y,z);
		}

		// ----------------------------------------------------------------
//  Name:           GetPosition
//	Class:			Mesh
//  Description:   //get position
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	D3DXVECTOR3 Mesh::GetPosition()
		{
			return position;
		}

		// ----------------------------------------------------------------
//  Name:			SetVelocity
//	Class:			Mesh
//  Description:    //set velocity
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetVelocity(D3DXVECTOR3 vel)
		{
			velocity = vel;
		}

		// ----------------------------------------------------------------
//  Name:           SetVelocity
//	Class:			Mesh
//  Description:    //set velocity
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetVelocity(float x,float y, float z)
		{
			velocity=D3DXVECTOR3(x,y,z);
		}

		// ----------------------------------------------------------------
//  Name:           GetVelocity
//	Class:			Mesh
//  Description:   //get velocity
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	D3DXVECTOR3 Mesh::GetVelocity()
		{
			return velocity;
		}

		// ----------------------------------------------------------------
//  Name:           SetRotation
//	Class:			Mesh
//  Description:    //set rotation
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetRotation(D3DXVECTOR3 rot)
		{
			rotation=rot;
		}

		// ----------------------------------------------------------------
//  Name:           SetRotation
//	Class:			Mesh
//  Description:   //set rotation
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
		
	void Mesh::SetRotation(float x, float y, float z)
		{
			rotation= D3DXVECTOR3(x,y,z);
		}

		// ----------------------------------------------------------------
//  Name:           SetRotationX
//	Class:			Mesh
//  Description:    //set the rotation x
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetRotationX(float x)
		{
			rotation.x=(float)x;
		}

		// ----------------------------------------------------------------
//  Name:           GetRotationX
//	Class:			Mesh
//  Description:    //get rotation x
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	double Mesh::GetRotationX()
		{
			return rotation.x;

		}

		// ----------------------------------------------------------------
//  Name:           SetRotationY
//	Class:			Mesh
//  Description:    //set the rotation y
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetRotationY(float y)
		{
			rotation.y=y;

		}

		// ----------------------------------------------------------------
//  Name:           GetRotationY
//	Class:			Mesh
//  Description:    //get rotation y
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	double Mesh::GetRotationY()
		{

			return rotation.y;


		}

		// ----------------------------------------------------------------
//  Name:           SetRotationZ
//	Class:			Mesh
//  Description:    //set rotation z
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetRotationZ(float z)
		{

			rotation.z=z;

		}

		// ----------------------------------------------------------------
//  Name:           GetRotationZ
//	Class:			Mesh
//  Description:    //get rotation z
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
		
	float Mesh::GetRotationZ()
		{
			return rotation.z;
		}

		// ----------------------------------------------------------------
//  Name:           SetScale
//	Class:			Mesh
//  Description:    //set scale
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------


	void Mesh::SetScale(D3DXVECTOR3 value)
		{
			scale= value;
		}

		// ----------------------------------------------------------------
//  Name:           SetScale
//	Class:			Mesh
//  Description:    //set scale
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
		
	void Mesh::SetScale(float x, float y, float z)
		{
			scale= D3DXVECTOR3(x, y,z);
		}

		// ----------------------------------------------------------------
//  Name:           GetScale
//	Class:			Mesh
//  Description:    //get scale
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	D3DXVECTOR3 Mesh::GetScale()
		{
			return scale;
		}

		// ----------------------------------------------------------------
//  Name:           SetScaleX
//	Class:			Mesh
//  Description:   //set scale x
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------


	void Mesh::SetScaleX(float x)
		{
			scale.x=x;
		}

		// ----------------------------------------------------------------
//  Name:           GetScaleX
//	Class:			Mesh
//  Description:    //get scale x
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	float Mesh::GetScaleX()//get scale x
		{
			return scale.x;
		}

		// ----------------------------------------------------------------
//  Name:           SetScaleY
//	Class:			Mesh
//  Description:    //set scale y
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetScaleY(float y)
		{
			scale.y=y;

		}

		// ----------------------------------------------------------------
//  Name:           GetScaleY
//	Class:			Mesh
//  Description:    //get scale y
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	float Mesh::GetScaleY()
		{
			return scale.y;
		}
		

		// ----------------------------------------------------------------
//  Name:           SetScaleZ
//	Class:			Mesh
//  Description:    //set scale z
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::SetScaleZ(float z)
		{
			scale.z=z;
		}

		// ----------------------------------------------------------------
//  Name:           GetScaleZ
//	Class:			Mesh
//  Description:    //get scale z
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	float Mesh::GetScaleZ(float z)
		{
			return scale.z;

		}
	
	template<class T> void Mesh::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
	}


		// ----------------------------------------------------------------
//  Name:           draw
//	Class:			Mesh
//  Description:    (virtual function)draw the subset
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	void Mesh::draw()
	{
		Transform();

		if(material_count == 0)
			mesh->DrawSubset(0);
		else
		{
			for(DWORD i=0;i<material_count; i++)
			{
				g_engine->getDevice()->SetMaterial(&materials[i]);
	

				if(textures[i])
				{
					if(textures[i]->GetType() == D3DRTYPE_TEXTURE)
					{
						D3DSURFACE_DESC desc;
						textures[i]->GetLevelDesc(0,&desc);
						if(desc.Width > 0)
						{
							g_engine->getDevice()->SetTexture(0,textures[i]);
						}
					}
				}
	mesh->DrawSubset(i);
			}

		}

	}

};



#endif
