//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Main engine class source
//
//****************************************************************************************

#ifndef VIPER2D_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define VIPER2D_CPP_//define one time and let everyone share, so VS doesn't give annoying errors


//includes
#include "Viper2D.h"
#include "..\Sprite\2dengine.h"





namespace Viper2D//viper engine namespace
{

	// ----------------------------------------------------------------
//  Name:           
//	Struct:			MyData
//  Description:    thread data with the engine object and the entity list pointer
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	typedef struct MyData//thread object struct
	{
		Engine* v8;//engine object
		std::list<cBaseEntity*>::iterator point;//entity list iterator pointer
		
	}MyData;



//typedef struct MyData {
//    Audio* val1;
//	char *filename;
//	int volume;
//	bool loop;
// } MYDATA, *PMYDATA;

	// ----------------------------------------------------------------
//  Name:           Engine
//	Class:			Engine
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	Engine::Engine()//constructor
	{
		srand((unsigned int)time(NULL));//random generator
		p_maximizeProcessor = false;//full utilization
		p_frameCount_core = 0;//render frame var
		p_frameRate_core = 0;//render frame var
		p_frameCount_real = 0;//render frame var
		p_frameRate_real = 0;//render frame var
		p_ambientColor = D3DCOLOR_RGBA(0,0,0, 0); //abient color
		p_windowHandle = 0;//window handle count
		p_pauseMode = false;//pause flag
		p_versionMajor = VERSION_MAJOR;//set version major
		p_versionMinor = VERSION_MINOR;//set version minor
		p_revision = REVISION;//set revision
		
		//set default values
		this->setAppTitle("Viper 2D Engine");//set the Title bar caption
		this->setScreenWidth(640);//set the screen width(windowed mode only)
		this->setScreenHeight(480);//set the screen height (windowed mode only)
		this->setColorDepth(32);//set the color depth
		this->setFullscreen(false);//set to full screen
		
		//window handle must be set later on for DirectX!
		this->setWindowHandle(0);//set window handle
		//input=NULL;
		//keyboard=NULL;
		//mouse=NULL;
		//gamepad=NULL;
		//xbox=NULL;
		//audio=NULL;

		for(int x=0;x<4;x++)//clear mouse array
		{
			oldmouse[x]=false;
		}

		for(int x=0;x<255;x++)//clear keyboard array
		{
			oldkeys[x]=false;
		}

		mouseContinuous=true;//set the mouse continuous flag(true:continuous input and false:non-continous input)
		keyContinuous=true;//set the keyboard continuous flag(true:continuous input and false:non-continous input)
		gamepadContinuous=true;//set the gamepad continuous flag(true:continuous input and false:non-continous input)
		xboxContinuous=true;//set the xbox controller continuous flag(true:continuous input and false:non-continous input)

		object.clear();//clear entity list

	
	}
	
	// ----------------------------------------------------------------
//  Name:           ~Engine
//	Class:			Engine
//  Description:    class deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	Engine::~Engine()//deconstructor
	{
		Free();

	   

	}


// ----------------------------------------------------------------
//  Name:           Free
//	Class:			Engine
//  Description:    free up all the objects,devices,and mutexes
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
	void Engine::Free()
	{
		 //if (this->p_device) this->p_device->Release();//release video card structure
	   //if (this->p_d3d) this->p_d3d->Release();//release directx structure
	   //if (this->p_backbuffer) this->p_backbuffer->Release();
	   //if (this->p_sprite_handler) this->p_sprite_handler->Release();
		ReleaseCOM(this->p_device);//release video card structure
		ReleaseCOM(this->p_d3d);//release directx structure
		ReleaseCOM(this->p_backbuffer);//release video card back buffer
		ReleaseCOM(this->p_sprite_handler);//release 2D sprite handler

		keyboard.Free();//free keyboard structure
		mouse.Free();//free mouse structure
		input.Shutdown();//input shutdown

		delete mutex;//delete mutex object for threading


	   
		//if(keyboard != NULL)
	   //{
		//	delete keyboard;
		//	keyboard=NULL;
		//}
		//if(mouse != NULL)
		//{
		//	delete mouse;
		//	mouse=NULL;
		//}

	   //if(gamepad != NULL)
	  //{
		//   delete gamepad;
		//	gamepad=NULL;
	   //}

	   //if(xbox != NULL)
	   //{
		 //  delete xbox;
		   //xbox=NULL;
	   //}

		//if(input != NULL)
	   //{
	//	   delete input;
	//	   input=NULL;
	  // }



	  // if(audio != NULL)
	  // {
	//	   delete audio;
	//	   audio=NULL;
	  // }


	}


	// ----------------------------------------------------------------
//  Name:           getVersionText
//	Class:			Engine
//  Description:    output the name of engine, version number, and place it into string object s.
//  Arguments:      None.
//  Return Value:   std::string
// ----------------------------------------------------------------
	
	 std::string Engine::getVersionText()
    {
        std::ostringstream s;//create std string
		//set text to string
        s << "Viper2D Engine v" << p_versionMajor << "." << p_versionMinor << "." << p_revision;
        return s.str();//return string
    }

		// ----------------------------------------------------------------
//  Name:           Message
//	Class:			Engine
//  Description:    prints message box to user
//  Arguments:      std::string message, std::string title
//  Return Value:   void
// ----------------------------------------------------------------
	
	void Engine::message(std::string message, std::string title)//prints message box to user
	{
		MessageBox(0, message.c_str(), title.c_str(), 0);//create and show message box
	}

// ----------------------------------------------------------------
//  Name:           fatalerror
//	Class:			Engine
//  Description:    prints error on messagebox and shutsdown engine
//  Arguments:      std::string message, std::string title
//  Return Value:   void
// ----------------------------------------------------------------

	
	void Engine::fatalerror(std::string message, std::string title)
	{
		this->message(message,title);//prints error on messagebox
		Shutdown();//shutdown engine
	}

	// ----------------------------------------------------------------
//  Name:           Init
//	Class:			Engine
//  Description:    initializes the rendering pipeline of the engine
//  Arguments:      int width, int height, int colordepth, bool fullscreen
//  Return Value:   int
// ----------------------------------------------------------------
	
	int Engine::Init(int width, int height, int colordepth, bool fullscreen)
	{
	   
		this->p_d3d = Direct3DCreate9(D3D_SDK_VERSION);//init direct3d
		if (this->p_d3d == NULL) {//if direct3d doesn't exist
			//return error!!!;
			fatalerror("FATAL ERROR!:: Directx 9 could not initialize", "Viper 2D Engine");
		}
	
		//get system desktop color depth
		D3DDISPLAYMODE dm;//display mode structure
		this->p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);//get the current display mode from the video card
	
		//set configuration options for Direct3D
		D3DPRESENT_PARAMETERS d3dpp;//create video card attributes
		ZeroMemory(&d3dpp, sizeof(d3dpp));//zero out the structure
		d3dpp.Windowed = (!fullscreen);//set to windowed mode
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;//allows the video card to choose the best way to
												//implement the swap chain algorithm for the backbuffer
		d3dpp.EnableAutoDepthStencil = TRUE;//allows direct3d to manage the depth buffers or Z-buffer on the video card
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//16 bit Z-Buffer
		//d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_TWO;//immediate swap frames from backbuffer to the frontbuffer
		d3dpp.BackBufferFormat = dm.Format;//set the backbuffer to the same mode as the screen
		d3dpp.BackBufferCount = 1;//number of backbuffers
		d3dpp.BackBufferWidth = width;//set the width of the backbuffer/s
		d3dpp.BackBufferHeight = height;//set the height of the backbuffer/s
		d3dpp.hDeviceWindow = p_windowHandle;//set the front buffer to the main window frame
	
		
		this->p_d3d->CreateDevice(//create video card object
			D3DADAPTER_DEFAULT,//primary video adapter
			D3DDEVTYPE_HAL,//Hardware rasterization
			this->p_windowHandle,//attach to main window shell
			D3DCREATE_HARDWARE_VERTEXPROCESSING,//hardware vertex processing
			&d3dpp,//apply the video card attributes to object
			&this->p_device);//returned pointer from the device
	
		if (this->p_device == NULL) fatalerror("FATAL ERROR!:: Video Card could not initialize", "Viper 2D Engine");
		//make sure video card object was created
	
		
		this->ClearScene(D3DCOLOR_XRGB(0,0,0));//TBA
	
		
		this->p_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->p_backbuffer);//Retrieves a back buffer from the device's swap chain
		
	    //use ambient lighting and z-buffering
		this->p_device->SetRenderState(D3DRS_ZENABLE, TRUE);//enable Z-buffering
		this->p_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//make sure to fill solids
		this->SetAmbient(this->p_ambientColor);//TBA
		
		HRESULT result = D3DXCreateSprite(this->p_device, &this->p_sprite_handler);//initialize 2D renderer
		if (result != D3D_OK)
			//return 0;
		fatalerror("FATAL ERROR!:: 2D Renderer could not initialize", "Viper 2D Engine");

	//audio= new Audio;
	//	audio->Init(this->getWindowHandle());
		
		//call game initialization extern function
		if (!game_init(this->getWindowHandle()))
			//return 0;
			fatalerror("FATAL ERROR!:: game cannot initialize", "Viper 2D Engine");
	
		
		SetDefaultMaterial();//set a default material//TBA
		//input= new cInput(this->getWindowHandle(), this->getWindowInstance());
		//keyboard=new cInputDevice();
		//mouse=new cInputDevice();
		//gamepad=new cInputDevice();
		//xbox=new cInputDevice(this->getWindowHandle(), this->getWindowInstance());
		//input->Init();
		//bool test;
		//test=keyboard->Create(input,KEYBOARD,1);
		//if(test == true)
		//	MessageBox(NULL,"CREATE","TEST",MB_OK);
		//else
		//	MessageBox(NULL,"NO","TEST",MB_OK);
		//mouse->Create(input,MOUSE,1);
		//gamepad->Create(input,GAMEPAD,true);
		//xbox->Create(xbox->getInput(),XBOX,true);
		 input.Init(this->getWindowHandle(), this->getWindowInstance());
		keyboard.Create(&input, KEYBOARD);
		mouse.Create(&input,MOUSE);
		mutex= new cMutex();
		thread = new cThread();
		MyData *input = new MyData();
		input->v8=g_engine;
		
		
		//Thread_t *type;
		//Thread_create(type,thread_function_bury_entities,&data);
		thread->Thread_Create(thread_function_bury_entities,input);
		
		
		
		

		return 1;
	}

	// ----------------------------------------------------------------
//  Name:           SetDefaultMaterial
//	Class:			Engine
//  Description:    applys diffuse material on an object for light reflection
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
	
	void Engine::SetDefaultMaterial()
	{
	   D3DMATERIAL9 mat;//Specifies material properties
		memset(&mat, 0, sizeof(mat));//sets a the character "0" to the memeory of the structure mat
		mat.Diffuse.r = 0.0f;//Value specifying the red diffuse color of the material
		mat.Diffuse.g = 0.0f;//Value specifying the green diffuse color of the material
		mat.Diffuse.b = 0.0f;//Value specifying the blue diffuse color of the material
		mat.Diffuse.a = 0.0f;//Value specifying the alpha diffuse color of the material
		p_device->SetMaterial(&mat);//apply material attributes to video card object
	
	
	
	
	}



	// ----------------------------------------------------------------
//  Name:           ClearScene
//	Class:			Engine
//  Description:    clears the front and back buffers on the video card
//  Arguments:      D3DCOLOR structure.
//  Return Value:   void
// ----------------------------------------------------------------
	
	
	void Engine::ClearScene(D3DCOLOR color)
	{	//clear the entire viewport rectangle including clearing the render target and the depth buffer. also setting the depth and stencil buffer
		//to 1.0 and zero respectfully 
		this->p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
	}
	

		// ----------------------------------------------------------------
//  Name:           SetAmbient
//	Class:			Engine
//  Description:    sets environment light
//  Arguments:      D3DCOLOR structure.
//  Return Value:   void
// ----------------------------------------------------------------
	void Engine::SetAmbient(D3DCOLOR colorvalue)//D3DCOLOR_COLORVALUE(r,g,b,a)
	{
		this->p_ambientColor = colorvalue;//Direct3D color type
		this->p_device->SetRenderState(D3DRS_AMBIENT, this->p_ambientColor);//set the Ambient light color to whatever was given
	}


		// ----------------------------------------------------------------
//  Name:           RenderStart
//	Class:			Engine
//  Description:    start the rendering pipeline
//  Arguments:      NONE
//  Return Value:   INT.
// ----------------------------------------------------------------
	
	
	int Engine::RenderStart()
	{
		if (!this->p_device) fatalerror("FATAL ERROR!:: Lost Video Card access ", "Viper 2D Engine");
										
		if (this->p_device->BeginScene() != D3D_OK) fatalerror("FATAL ERROR!:: Rendering Pipeline could not initialize", "Viper 2D Engine");
		
		return 1;//return success
	}


			// ----------------------------------------------------------------
//  Name:           RenderStop
//	Class:			Engine
//  Description:    Stop redering pipeline
//  Arguments:      NONE
//  Return Value:   INT.
// ----------------------------------------------------------------
	

		int Engine::RenderStop()
	{
	   if (!this->p_device) return 0;
	   if (this->p_device->EndScene() != D3D_OK) return 0;
	   if (p_device->Present(NULL, NULL, NULL, NULL) != D3D_OK) return 0;
	   return 1;
	}


	
	
	

			// ----------------------------------------------------------------
//  Name:           Shutdown
//	Class:			Engine
//  Description:    shutdown engine
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------

	void Engine::Shutdown()
	{
		gameover = true;//we may need to add on to this fuction especially for those needed shutdowns when something is not working
		Free();
		exit(1);//for now I'm going to exit the program here. Since all the declaration are under classes, all releases are done automatically,
				//so no worries.
	}

			// ----------------------------------------------------------------
//  Name:           Update
//	Class:			Engine
//  Description:    rendering update and frame rate control
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------

	
	void Engine::Update()
	{
		
		static Timer timedUpdate;//create timer object
	
		//calculate core framerate
		p_frameCount_core++;//increment render frame
		if (p_coreTimer.stopwatch(999)) {//stop clock at 999ms
			p_frameRate_core = p_frameCount_core;//if already past,framerate = framecount
			p_frameCount_core = 0;//if still going, set count to 0
		}


		getMutex()->lock();
		//fast update with no timing
		game_update();//2nd layer defined function
		getMutex()->unlock();

		getMutex()->lock();
		 //update entities
        if (!p_pauseMode)
			UpdateEntities();
		getMutex()->unlock();


		//perform global collision testing at 20 Hz
        //if (!p_pauseMode && collTimer.stopwatch(0)) 
        //{
			getMutex()->lock();
            collisionTest();
			getMutex()->unlock();
        //}
	
		//update with 60fps timing
		if (!timedUpdate.stopwatch(14)) {//stop clock at 14ms//if stoped and clock reset
			if (!this->getMaximizeProcessor()) //if the need is to slow the processor down
			{
				Sleep(1);//sleep
			}
		}
		else {//if clock was still going
			//calculate real framerate
			p_frameCount_real++; //increament frame counter
			if (p_realTimer.stopwatch(999)) {//set the clock to stop at 999 ms
				p_frameRate_real = p_frameCount_real;//if the clock stoped and reset then set the count to the rate
				p_frameCount_real = 0;//if not reste the count
			}


			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//keyboard->Read();
		//updateGamePad();
		updateKeyboard();
		updateMouse();
		//updateGamePad();
		//updateXBOX();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
			//begin rendering
			this->RenderStart();//start the rendering pipeline


			//3D renderer
			game_render3d();


			//render 3D entities 
			getMutex()->lock();
            if (!p_pauseMode)
				Draw3DEntities();
			getMutex()->unlock();

			Render2D_Start();


			
			getMutex()->lock();
			//render 2D entities 
            if (!p_pauseMode)
				Draw2DEntities();
			getMutex()->unlock();

			getMutex()->lock();
			game_render2d();
			getMutex()->unlock();

			Render2D_Stop();
			
			
			//done rendering
			this->RenderStop();//stop the rendering pipeline
		}
			//remove dead entities from the list    
        //BuryEntities();
	}


			// ----------------------------------------------------------------
//  Name:           Close
//	Class:			Engine
//  Description:    scripted engine shutdown
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------


	void Engine::Close()
	{
		try {
			game_end();//2nd layer function
			//this->getMutex()->~cMutex();
			Shutdown();//shutdown engine

		}
		catch (...) { fatalerror("FATAL ERROR!:: Engine ended unexpectedly", "Viper 2D Engine");}
	}

				// ----------------------------------------------------------------
//  Name:           SetIdentity
//	Class:			Engine
//  Description:    set indentity matrix
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------


	void Engine::SetIdentity()
	{
		D3DXMATRIX matrixWorld;
		D3DXMatrixIdentity(&matrixWorld);
		g_engine->getDevice()->SetTransform(D3DTS_WORLD, &matrixWorld);
	}

				// ----------------------------------------------------------------
//  Name:           isPaused
//	Class:			Engine
//  Description:    return if game is in paused state
//  Arguments:      NONE
//  Return Value:   bool
// ----------------------------------------------------------------

	bool Engine::isPaused()
	{ 
		return this->p_pauseMode; 
	}//

				// ----------------------------------------------------------------
//  Name:           setPaused
//	Class:			Engine
//  Description:    set the pause state
//  Arguments:		bool value
//  Return Value:   void
// ----------------------------------------------------------------
	
	void Engine::setPaused(bool value) 
	{
		this->p_pauseMode = value; 
	}//

				// ----------------------------------------------------------------
//  Name:           getDevice
//	Class:			Engine
//  Description:    get the directx 9 video card
//  Arguments:      NONE
//  Return Value:   LPDIRECT3DDEVICE9
// ----------------------------------------------------------------

	LPDIRECT3DDEVICE9 Engine::getDevice() 
		{
			return this->p_device; 
		}

					// ----------------------------------------------------------------
//  Name:           getBackBuffer
//	Class:			Engine
//  Description:    get the back buffer
//  Arguments:      NONE
//  Return Value:   LPDIRECT3DSURFACE9
// ----------------------------------------------------------------
		
	LPDIRECT3DSURFACE9 Engine::getBackBuffer()
		{
			return this->p_backbuffer; 
		}

					// ----------------------------------------------------------------
//  Name:           getSpriteHandler
//	Class:			Engine
//  Description:    get renderer
//  Arguments:      NONE
//  Return Value:   LPD3DXSPRITE
// ----------------------------------------------------------------
		
	LPD3DXSPRITE Engine::getSpriteHandler() 
		{
			return this->p_sprite_handler; 
		}//

					// ----------------------------------------------------------------
//  Name:           setWindowHandle
//	Class:			Engine
//  Description:   set the window handle
//  Arguments:      HWND hwnd
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setWindowHandle(HWND hwnd) 
		{ 
			this->p_windowHandle = hwnd; 
		}

					// ----------------------------------------------------------------
//  Name:           getWindowHandle
//	Class:			Engine
//  Description:    get the window handle
//  Arguments:      NONE
//  Return Value:   HWND
// ----------------------------------------------------------------
		
	HWND Engine::getWindowHandle() 
		{
			return this->p_windowHandle; 
		}

					// ----------------------------------------------------------------
//  Name:           getAppTitle
//	Class:			Engine
//  Description:    get the title of the application
//  Arguments:      NONE
//  Return Value:   std::string
// ----------------------------------------------------------------
		
	std::string Engine::getAppTitle() 
		{
			return this->p_apptitle; 
		}

					// ----------------------------------------------------------------
//  Name:           setAppTitle
//	Class:			Engine
//  Description:    set the game title
//  Arguments:      std::string value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setAppTitle(std::string value) 
		{
			this->p_apptitle = value; 
		}

					// ----------------------------------------------------------------
//  Name:           getVersionMajor
//	Class:			Engine
//  Description:    get the version major
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getVersionMajor() 
		{
			return this->p_versionMajor; 
		}

					// ----------------------------------------------------------------
//  Name:           getVersionMinor
//	Class:			Engine
//  Description:    get the version minor
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getVersionMinor() 
		{
			return this->p_versionMinor; 
		}//

					// ----------------------------------------------------------------
//  Name:           getRevision
//	Class:			Engine
//  Description:    get the revision
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getRevision() 
		{
			return this->p_revision; 
		}//

					// ----------------------------------------------------------------
//  Name:           getFrameRate_core
//	Class:			Engine
//  Description:    get the core frame rate
//  Arguments:      NONE
//  Return Value:   long
// ----------------------------------------------------------------
		
	long Engine::getFrameRate_core() 
		{
			return this->p_frameRate_core; 
		}

					// ----------------------------------------------------------------
//  Name:           getFrameRate_real
//	Class:			Engine
//  Description:	get the real frame rate
//  Arguments:      NONE
//  Return Value:   long
// ----------------------------------------------------------------
		
	long Engine::getFrameRate_real() 
		{
			return this->p_frameRate_real; 
		};//

					// ----------------------------------------------------------------
//  Name:           getScreenWidth
//	Class:			Engine
//  Description:    get the screen width
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getScreenWidth() 
		{
			return this->p_screenwidth;
		}

					// ----------------------------------------------------------------
//  Name:           setScreenWidth
//	Class:			Engine
//  Description:    set the screen width
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setScreenWidth(int value) 
		{
			this->p_screenwidth = value; 
		}

					// ----------------------------------------------------------------
//  Name:           getScreenHeight
//	Class:			Engine
//  Description:    get the screen height
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getScreenHeight() 
		{
			return this->p_screenheight;
		}

					// ----------------------------------------------------------------
//  Name:           setScreenHeight
//	Class:			Engine
//  Description:    set the screen height
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setScreenHeight(int value)
		{
			this->p_screenheight = value;
		}

					// ----------------------------------------------------------------
//  Name:           getColorDepth
//	Class:			Engine
//  Description:    get the color depth
//  Arguments:      NONE
//  Return Value:   int
// ----------------------------------------------------------------
		
	int Engine::getColorDepth() 
		{
			return this->p_colordepth;
		}

					// ----------------------------------------------------------------
//  Name:           setColorDepth
//	Class:			Engine
//  Description:    set the color depth
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setColorDepth(int value) 
		{
			this->p_colordepth = value;
		}//set the color depth

					// ----------------------------------------------------------------
//  Name:           getFullscreen
//	Class:			Engine
//  Description:    get the fullscreen
//  Arguments:      NONE
//  Return Value:   bool
// ----------------------------------------------------------------
		
	bool Engine::getFullscreen() 
		{
			return this->p_fullscreen;
		}//get the fullscreen

					// ----------------------------------------------------------------
//  Name:           setFullscreen
//	Class:			Engine
//  Description:    set the full screen
//  Arguments:      bool value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setFullscreen(bool value) 
		{
			this->p_fullscreen = value;
		}

					// ----------------------------------------------------------------
//  Name:           getMaximizeProcessor
//	Class:			Engine
//  Description:    speed up processor
//  Arguments:      NONE
//  Return Value:   bool
// ----------------------------------------------------------------
		
	bool Engine::getMaximizeProcessor() 
		{
			return this->p_maximizeProcessor;
		}

					// ----------------------------------------------------------------
//  Name:           setMaximizeProcessor
//	Class:			Engine
//  Description:    set the maximize processor speed
//  Arguments:      bool value
//  Return Value:   void
// ----------------------------------------------------------------
		
	void Engine::setMaximizeProcessor(bool value) 
		{
			this->p_maximizeProcessor = value;
		}
	
// ----------------------------------------------------------------
//  Name:           Render2D_Start
//	Class:			Engine
//  Description:    start rendering in 2D space
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------
	int Engine::Render2D_Start()
	{
		if(p_sprite_handler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
			return 0;
		else
			return 1;

	}

// ----------------------------------------------------------------
//  Name:           Render2D_Stop
//	Class:			Engine
//  Description:    stop rendering the 2D space
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------
	int Engine::Render2D_Stop()
	{
		p_sprite_handler->End();
		return 1;
	}

// ----------------------------------------------------------------
//  Name:           setWindowInstance
//	Class:			Engine
//  Description:    set the app instance
//  Arguments:      None.
//  Return Value:   void.
// ----------------------------------------------------------------

	void Engine::setWindowInstance()//set the window instance
	{
	

		p_windowInstance=GetWindowInstance(p_windowHandle);


	}

// ----------------------------------------------------------------
//  Name:           getWindowInstance
//	Class:			Engine
//  Description:    get the instance of the current app
//  Arguments:      None.
//  Return Value:   HINSTANCE
// ----------------------------------------------------------------
	HINSTANCE Engine::getWindowInstance()//get the window instance
	{

		return p_windowInstance;




	}


// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			Engine
//  Description:    release objects
//  Arguments:      T& x
//  Return Value:   void
// ----------------------------------------------------------------
	
template<class T>
	void Engine::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}

// ----------------------------------------------------------------
//  Name:           updateMouse
//	Class:			Engine
//  Description:    get the mouse input
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
	void Engine::updateMouse()
    {	
		
		mouse.Acquire(true);
		mouse.Read();
		static int oldPosX = 0;
        static int oldPosY = 0;
		int deltax = mouse.GetXDelta();
		int deltay = mouse.GetYDelta();
    
		if(getMouseContinuous() == false)
		{
			//check mouse buttons 1-3
			for (int n=0; n<4; n++)
			{
				if (mouse.GetButtonState(n))
				{
					if((oldmouse[n] == false))
					{	
						game_mouseButton(n);
						oldmouse[n]= true;
					}
				}

				if(oldmouse[n])
				{
					game_mouseButtonR(n);
					oldmouse[n]= mouse.GetButtonState(n);
				}
			
			}
		}
		else
		{	
			for (int n=0; n<4; n++)
			{
				if (mouse.GetButtonState(n))
				{
						game_mouseButton(n);
						oldmouse[n]= true;
					
				}

				if(oldmouse[n])
				{
					game_mouseButtonR(n);
					oldmouse[n]= mouse.GetButtonState(n);
				}
			
			}


		}

		//
    
        //check mouse position
		if (mouse.GetXPos() != oldPosX || mouse.GetYPos() != oldPosY) {
			game_mouseMove(mouse.GetXPos(), mouse.GetYPos() );
			oldPosX = mouse.GetXPos();
			oldPosY = mouse.GetYPos();
        }
    
        //check mouse motion
        if (deltax != 0 || deltay != 0 ) {
            game_mouseMotion(deltax,deltay);
        }
    
        //check mouse wheel
		int wheel = mouse.GetZPos();
		if (wheel != 0 && (mouse.GetZDelta() > 0))
			game_mouseWheel(1);
		if (wheel != 0 && (mouse.GetZDelta() < 0))
			game_mouseWheel(-1);

           
    
    }


// ----------------------------------------------------------------
//  Name:           updateKeyboard
//	Class:			Engine
//  Description:    get the keyboard input
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------  
    void Engine::updateKeyboard()
    {
		keyboard.Acquire(TRUE);   // Read keyboard
		keyboard.Read();
		//keyboard->Read();
		if(getKeyContinuous() == false)
		{
			for (int n=0; n<256; n++) 
			{//MessageBox(NULL,(LPCSTR)keyboard->GetKeyState(n),"NULL",MB_OK);
				//check for key press
				if (keyboard._GetKeyState(n) & 0x80)
				{
					if((oldkeys[n] == false))
					{	
						game_keyPress(n);
						oldkeys[n] = keyboard._GetKeyState(n);
					}
				}
				else if (oldkeys[n] & 0x80)
				{
						game_keyRelease(n);
						oldkeys[n]= keyboard._GetKeyState(n);
				}
				
			}


			
		
		}	
		else
		{

			for (int n=0; n<256; n++) 
			{//MessageBox(NULL,(LPCSTR)keyboard->GetKeyState(n),"NULL",MB_OK);
				//check for key press
				if (keyboard._GetKeyState(n) & 0x80)
				{
					game_keyPress(n);
					oldkeys[n] = keyboard._GetKeyState(n);
				} 
				//check for release
				else if (oldkeys[n] & 0x80)
				{
					
					game_keyRelease(n);
					oldkeys[n]= keyboard._GetKeyState(n);
				}
			}





		}
	
	
	
	
	
	}

/*
	bool Engine::updateGamePad()
	{
		gamepad->Read();
	
		for(int x=0;x< 32; x++)
		{
			if(gamepad->GetButtonState(x))
				game_GamePadPress(x);
		}

		if(gamepad->getPOVState() >=0)
			game_POVPress(gamepad->getPOVState());

		if((gamepad->GetXPos() > 0) && (gamepad->GetYPos() > 0))
			gamepadLeftThumbMove(gamepad->GetXPos(),gamepad->GetYPos());

		if((gamepad->getRotationX() > 0) && (gamepad->getRotationY()))
			gamepadRightThumbMove(gamepad->getRotationX(),gamepad->getRotationY());
	

		return true;
	}

	bool Engine::updateXBOX()
	{

		return false;



	}

*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           UpdateEntities
//	Class:			Engine
//  Description:    move or animate each entity that is alive and not the background
//					also checks to see if the entity is dead and if it is, sets the flag to false
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
 void Engine::UpdateEntities()
 {//begin
	 std::list<cBaseEntity*>::iterator iter;
	 cBaseEntity *entity;
	 iter= object.begin();
	 while(iter != object.end())
	 {//while
		 entity = *iter;
		 if(entity->IsAlive())
		 {//alive
			 if(entity->getObjectType() != OBJECT_BACKGROUND)
			 {
				 entity->move();
				 entity->animate();
			 }

			 game_entityUpdate(entity);//update VM*********
			 if(entity->getLifeExpect() > 0)
			 {//lifeexp
				 if(entity->lifetimeExpired())
				 {//lifetime
					 entity->setAlive(false);
				 }//lifetime

			 }//lifeexp
		 }//alive
		 ++iter;
	 }//while
 
 }//begin


 
// ----------------------------------------------------------------
//  Name:           Draw3DEntities
//	Class:			Engine
//  Description:    look through the list to find 3D objects and if their visible and alive, then draw them
//  Arguments:      None.
//  Return Value:   void 
// ----------------------------------------------------------------
 void Engine::Draw3DEntities()
 {

	 std::list<cBaseEntity*>::iterator iter;
	cBaseEntity *entity;
	iter = object.begin();
	while(iter != object.end())
	{
		entity= *iter;
		if(entity->getRenderType() == RENDER3D)
		{
			if(entity->IsAlive() && entity->IsVisible())
			{
				entity->draw();
				game_entityRender( entity );//VM update
			}
		}
		++iter;

	}
 }


// ----------------------------------------------------------------
//  Name:           Draw2DEntities
//	Class:			Engine
//  Description:    look through the list and find 2D objects that are visible and alive to draw
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
 void Engine::Draw2DEntities()
 {

	 cBaseEntity* entity;
	 std::list<cBaseEntity*>::iterator iter;
	 iter = object.begin();
	 while(iter != object.end())
	 {
		 entity = *iter;
		 if(entity->getRenderType() == RENDER2D)
		 {
			 if(entity->IsAlive() && entity->IsVisible())
			 {
				 entity->draw();
				 game_entityRender( entity );//VM update;
			 }
		 }
	 ++iter;
	 }
 }
	 


 
// ----------------------------------------------------------------
//  Name:           BuryEntities
//	Class:			Engine
//  Description:    garbage collector(non-threaded)
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------	 
void Engine::BuryEntities()
{
	std::list<cBaseEntity*>::iterator iter;
	iter = object.begin();
	while(iter != object.end())
	{
		if((*iter)->IsAlive() == false)
		{
			delete (*iter);
			iter = object.erase(iter);
		}
		else iter++;
	}
}

// ----------------------------------------------------------------
//  Name:           thread_function_bury_entities
//	Class:			
//  Description:    garbage collector
//  Arguments:      void* data
//  Return Value:   static void* __cdecl
// ----------------------------------------------------------------
 static void* __cdecl thread_function_bury_entities(void* data)
{

	static Timer timer;
		std::list<cBaseEntity*>::iterator iter;
		MyData *object= (MyData*)data;
		std::list<cBaseEntity*> tlist = object->v8->getObject();
		cMutex* tmutex = object->v8->getMutex();
		Engine* engine = object->v8;


	    while(!gameover)
	    {
			if (timer.stopwatch(2000))
			{
				tmutex->lock();
				if(tlist.empty() == false)
				{
					//iterate through entity list
					iter = tlist.begin();
				
					while (iter != tlist.end())
					{
						if (*iter)
						{
							if ( (*iter)->IsAlive() == false ) 
							{
								delete (*iter);
								iter = tlist.erase( iter );
							}
							else
							{
								iter++;
								if (gameover)
									break;
							}
						}	
					}
				}
				if (gameover) 
					break;

				tmutex->unlock();
			} //if
		} //gameover

	    _endthread();
	    return NULL;


 }




//cBaseEntity* Engine::findEntity(std::string name)
//{
//	std::list<cBaseEntity*>::iterator i;
//	i = object.begin();
//	while(i != object.end())
//	{
//		if((*i)->IsAlive() == true && (*i)->GetModelName() == name )
//			return *i;
//		else
//			++i;
//	}
//	return NULL;
//}


// ----------------------------------------------------------------
//  Name:           findEntity
//	Class:			Engine
//  Description:    find a particular node
//  Arguments:      ObjectType objectType
//  Return Value:   std::list<cBaseEntity*>::iterator
// ----------------------------------------------------------------
std::list<cBaseEntity*>::iterator Engine::findEntity(ObjectType objectType)
{
	std::list<cBaseEntity*>::iterator i;
	i = object.begin();
	while(i != object.end())
	{
		if(((*i)->IsAlive() == true) && ((*i)->getObjectType() == objectType))
				return i;
			else
				++i;
	}
	(*i) = NULL;
	return i;
}

// ----------------------------------------------------------------
//  Name:           getEntityCount
//	Class:			Engine
//  Description:    get the node count in the list
//  Arguments:      int objectType
//  Return Value:   int
// ----------------------------------------------------------------
int Engine::getEntityCount(int objectType)
{
	 int total = 0;
            std::list<cBaseEntity*>::iterator i = object.begin();
            while (i != object.end())
            {
				if ( (*i)->IsAlive() == true && (*i)->getObjectType() == objectType ) 
				{
                    total++;
					++i;
				}
				else
                    ++i;
            }
            return total;
}

// ----------------------------------------------------------------
//  Name:           addEntity
//	Class:			Engine
//  Description:    add a node to the list
//  Arguments:      cBaseEntity *entity
//  Return Value:   void
// ----------------------------------------------------------------
void Engine::addEntity(cBaseEntity *entity)
{

	static int id = 0;
	entity->SetModelIndex(id);
	object.push_back(entity);
	id++;
}

// ----------------------------------------------------------------
//  Name:           deleteEntity
//	Class:			Engine
//  Description:    delete a node from the list
//  Arguments:      ObjectType input
//  Return Value:   void
// ----------------------------------------------------------------
void Engine::deleteEntity(ObjectType input)
{
	std::list<cBaseEntity*>::iterator iter;
	//cBaseEntity *iter;
	iter=g_engine->findEntity(input);
	g_engine->object.erase(iter);
}

//Audio *Engine::getAudio()
//{
//	return this->audio;



//}

//void Engine::_loadSoundTrap(char* filename,int volume,bool loop)
//{
//	HANDLE m_hThread;
//	DWORD dwThreadId;
//	PMYDATA pa;
//	pa = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
  //              sizeof(MYDATA));


//	pa->filename=filename;
//	pa->loop=loop;
//	pa->volume=volume;
//	pa->val1=this->getAudio();
//
	

//	m_hThread = CreateThread(NULL, 0, loadSound, pa, 0, &dwThreadId);
  
//}

//DWORD WINAPI Engine::loadSound( LPVOID lpParam ) 
//{ 
//   PMYDATA temp;
//   temp=(PMYDATA)lpParam;
//   temp->val1->Play(temp->filename,temp->volume,temp->loop);
	
   

//    return 1; 
//} 

// ----------------------------------------------------------------
//  Name:           getMouseContinuous
//	Class:			Engine
//  Description:    get whether the continuous flag is true/false for the mouse
//  Arguments:      None.
//  Return Value:   bool
// ----------------------------------------------------------------
bool Engine::getMouseContinuous()
{
	return mouseContinuous;
}

// ----------------------------------------------------------------
//  Name:           getKeyContinuous
//	Class:			Engine
//  Description:    get whether the continuous flag is true/false for the keyboard
//  Arguments:      None.
//  Return Value:   bool
// ----------------------------------------------------------------
bool Engine::getKeyContinuous()
{
	return keyContinuous;
}



// ----------------------------------------------------------------
//  Name:           setMouseContinuous
//	Class:			Engine
//  Description:    sets each button to have continuous/non continuouse input
//  Arguments:      bool input
//  Return Value:   void 
// ----------------------------------------------------------------
void Engine::setMouseContinuous(bool input)
{
	mouseContinuous = input;
}

// ----------------------------------------------------------------
//  Name:           setKeyContinuous
//	Class:			Engine
//  Description:    sets each key to have continuous/non continuouse input
//  Arguments:      bool input
//  Return Value:   void
// ----------------------------------------------------------------
void Engine::setKeyContinuous(bool input)
{
	keyContinuous = input;
}

// ----------------------------------------------------------------
//  Name:           getObject
//	Class:			Engine
//  Description:    return entity list
//  Arguments:      None.
//  Return Value:   std::list<cBaseEntity*>
// ----------------------------------------------------------------
std::list<cBaseEntity*> Engine::getObject()
{


	return object;

}

// ----------------------------------------------------------------
//  Name:           getMutex
//	Class:			Engine
//  Description:    return mutex
//  Arguments:      None.
//  Return Value:   cMutex*
// ----------------------------------------------------------------
cMutex *Engine::getMutex()
{
	return mutex;
}

}; //namespace
#endif