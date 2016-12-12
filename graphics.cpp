// Programming 2D Games
// Copyright (c) 2011,2012 by:
// Charles Kelly
// graphics.cpp v1.1


#include "graphics.h"

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
	pLine = NULL;
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;    // width & height are replaced in initialize()
	height = GAME_HEIGHT;
	CameraFactory::Create(); //Cameraはグラフィックス全体で必要なためここでクリエイト
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
	CameraFactory::Instance().ReleaseAll();
	CameraFactory::Destroy();
	delete pLine;
	releaseAll();
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//=============================================================================
// 初期化
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	//initialize Direct3D
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp();        // init D3D presentation parameters
	if (fullscreen)      // if full-screen mode
	{
		if (isAdapterCompatible())   // is the adapter compatible
			// set the refresh rate with a compatible one
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
			"The graphics device does not support the specified resolution and/or format."));
	}

	// determine if graphics card supports hardware texturing and lighting and vertex shaders
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

	//create Direct3D device
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

	// Configure for alpha blend of primitives
	device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3DVIEWPORT9 viewData = { 0, 0, width, height, 0.0f, 1.0f };
	device3d->SetViewport(&viewData);

}

//=============================================================================
//  D3D presentation parameters　の初期化
//=============================================================================
void Graphics::initD3Dpp()
{
	try{
		ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
		// fill in the parameters we need

		d3dpp = { 0, 0, D3DFMT_UNKNOWN, 0, D3DMULTISAMPLE_NONE, 0,
			D3DSWAPEFFECT_DISCARD, NULL, TRUE, TRUE, D3DFMT_D24S8, 0, D3DPRESENT_RATE_DEFAULT, D3DPRESENT_INTERVAL_IMMEDIATE };
#ifdef false


		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		if (fullscreen)                                  // if fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24 bit color
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // use desktop setting
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFORMAT::D3DFMT_D16;
		d3dpp.Flags = 0;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));
	}
}
//=============================================================================
//Xファイルのロード
//=============================================================================
HRESULT Graphics::loadXMesh(const char *file, COLOR_ARGB transcolor, MeshData &mesh, const char *texdir){
	result = E_FAIL;
	try{
		if (file == NULL)
		{
			mesh.Mesh = NULL;
			return D3DERR_INVALIDCALL;
		}
		result = D3DXLoadMeshFromX(
			file,
			D3DXMESH_MANAGED,
			device3d,
			NULL,
			&mesh.pMaterials,
			NULL,
			&mesh.NumMaterial,
			&mesh.Mesh
		);

		if (result != E_FAIL && texdir!=""){
		/*	if (!(mesh.Mesh->GetFVF() & D3DFVF_NORMAL)) {

				ID3DXMesh* pTempMesh = NULL;

				mesh.Mesh->CloneMeshFVF(mesh.Mesh->GetOptions(),
					mesh.Mesh->GetFVF() | D3DFVF_NORMAL, device3d, &pTempMesh);

				D3DXComputeNormals(pTempMesh, NULL);
				mesh.Mesh->Release();
				mesh.Mesh = pTempMesh;
			}*/
			mesh.Texture = (LP_TEXTURE*)malloc(sizeof(LP_TEXTURE) * mesh.NumMaterial);

			//mesh.Texture = new LP_TEXTURE[mesh.NumMaterial];
			for (int i = 0; i < (int)mesh.NumMaterial; i++){
				D3DXMATERIAL *mat = ((D3DXMATERIAL*)(mesh.pMaterials->GetBufferPointer()) + i);
				if (mat[i].pTextureFilename != NULL){
					std::string filename = mat[i].pTextureFilename;
					filename.erase(0,2); //取得文字列の先頭に.//が含まれているためとりあえずこれの消去。
					const char* c = filename.c_str();
					char texturefile[1024];
					ZeroMemory(texturefile, sizeof(texturefile));
					sprintf_s(texturefile,1024,"%s\\%s", texdir, c);


					if (S_OK != D3DXCreateTextureFromFile(device3d, (LPCSTR)texturefile, &mesh.Texture[i])) {
						mesh.Texture[i] = nullptr;
					}


				}

			}

		}


	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadXMesh"));
	}
	return result;

}
//=============================================================================
// テクスチャのロード
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// The struct for reading file info
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// Get width and height from file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// Create the new texture by loading from file
		result = D3DXCreateTextureFromFileEx(
			device3d,           //3D device
			filename,           //image filename
			info.Width,         //texture width
			info.Height,        //texture height
			1,                  //mip-map levels (1 for no chain)
			0,                  //usage
			D3DFMT_UNKNOWN,     //surface format (default)
			D3DPOOL_DEFAULT,    //memory class for the texture
			D3DX_DEFAULT,       //image filter
			D3DX_DEFAULT,       //mip filter
			transcolor,         //color key for transparency
			&info,              //bitmap file info (from loaded file)
			NULL,               //color palette
			&texture);         //destination texture

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}

//=============================================================================
// Load the texture into system memory (system memory is lockable)
// Provides direct access to pixel data. Use the TextureManager class to load
// textures for display.
// Pre: filename is name of texture file.
//      transcolor transparent color
// Post: width and height = size of texture
//       texture points to texture
// Returns HRESULT and fills TextureData structure.
//=============================================================================
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// The struct for reading bitmap file info
	D3DXIMAGE_INFO info;
	result = E_FAIL;        // Standard Windows return value

	try{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// Get width and height from bitmap file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// Create the new texture by loading a bitmap image file
		result = D3DXCreateTextureFromFileEx(
			device3d,           //3D device
			filename,           //bitmap filename
			info.Width,         //bitmap image width
			info.Height,        //bitmap image height
			1,                  //mip-map levels (1 for no chain)
			0,                  //usage
			D3DFMT_UNKNOWN,     //surface format (default)
			D3DPOOL_SYSTEMMEM,  //systemmem is lockable
			D3DX_DEFAULT,       //image filter
			D3DX_DEFAULT,       //mip filter
			transcolor,         //color key for transparency
			&info,              //bitmap file info (from loaded file)
			NULL,               //color palette
			&texture);         //destination texture

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}



//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // default to fail, replace on success
	// Display backbuffer to screen
	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		d3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dpp.BackBufferFormat,
			i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}
//=============================================================================
//メッシュの描画
//=============================================================================
void Graphics::drawMesh(const MeshData &meshData){
	D3DXMATRIX World;
	D3DXMATRIX Scale;
	D3DXMATRIX Rot_x,Rot_y,Rot_z,Rot_Axis;
	D3DXMATRIX RotMat;
	D3DXMATRIX Offset;
	VECTOR3 sca = meshData.scale;
	VECTOR3 Pos = meshData.position;
	VECTOR3 ang = meshData.Angle;
	D3DXMatrixScaling(&Scale, sca.x, sca.y, sca.z);
	D3DXMatrixRotationX(&Rot_x, D3DXToRadian(meshData.Angle.x));
	D3DXMatrixRotationY(&Rot_y, D3DXToRadian(meshData.Angle.y));
	D3DXMatrixRotationZ(&Rot_z, D3DXToRadian(meshData.Angle.z));
	D3DXMatrixMultiply(&RotMat, &RotMat, &Rot_x);
	D3DXMatrixMultiply(&RotMat, &RotMat, &Rot_y);
	D3DXMatrixMultiply(&RotMat, &RotMat, &Rot_z);
	D3DXQUATERNION que;
	transformRotMatToQuaternion(que,
		RotMat.m[0][0], RotMat.m[0][1], RotMat.m[0][2],
		RotMat.m[1][0], RotMat.m[1][1], RotMat.m[1][2],
		RotMat.m[2][0], RotMat.m[2][1], RotMat.m[2][2]
		);
	D3DXMatrixRotationAxis(&Rot_Axis, &meshData.ar.Axis, meshData.ar.Angle);
	D3DXMatrixTranslation(&Offset, Pos.x, Pos.y, Pos.z);

	D3DXMatrixIdentity(&World);
	D3DXMatrixMultiply(&World, &World, &Scale);
	D3DXMatrixMultiply(&World, &World, &Rot_x);
	D3DXMatrixMultiply(&World, &World, &Rot_y);
	D3DXMatrixMultiply(&World, &World, &Rot_z);
	D3DXMatrixMultiply(&World, &World, &Rot_Axis);
//	D3DXMatrixMultiply(&World, &World, &Offset);
	World.m[3][0] = Pos.x;
	World.m[3][1] = Pos.y;
	World.m[3][2] = Pos.z;
	World.m[3][3] = 1;
	device3d->SetTransform(D3DTS_WORLD, &World);
	//CameraSet();
	if (meshData.isWire){
		device3d->SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	for (unsigned int i = 0; i < meshData.NumMaterial; i++){
		D3DXMATERIAL *mat = ((D3DXMATERIAL*)(meshData.pMaterials->GetBufferPointer())+i);
		mat->MatD3D.Ambient = meshData.MAmbient;
		device3d->SetMaterial(&mat->MatD3D);
		if (mat[i].pTextureFilename != NULL){
			device3d->SetTexture(0, meshData.Texture[i]);
		}
		meshData.Mesh->DrawSubset(i);

	}
	if (meshData.isWire){
		device3d->SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE,D3DFILL_SOLID);
	}

}
//=============================================================================
// Draw the sprite described in SpriteData structure
// Color is optional, it is applied like a filter, WHITE is default (no change)
// Pre : sprite->Begin() is called
// Post: sprite->End() is called
// spriteData.rect defines the portion of spriteData.texture to draw
//   spriteData.rect.right must be right edge + 1
//   spriteData.rect.bottom must be bottom edge + 1
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)      // if no texture
		return;


	float zScale = 1;//MainCamera->getZoomScale();
	float zoomScale = spriteData.scale*zScale;
	D3DXVECTOR2 camPos = VECTOR2(0, 0);//MainCamera->getcameraPos();
	int camWidth = 256;//MainCamera->getCameraWidth();
	int camHeight = 256;// MainCamera->getCameraHeight();
	// Find center of sprite
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)((spriteData.width) / 2 * zoomScale),
		(float)((spriteData.height) / 2 * zoomScale));
	// Screen position of the sprite
	D3DXVECTOR2 translate = D3DXVECTOR2(((float)spriteData.x)*zScale + camPos.x*zoomScale,
		(float)spriteData.y*zScale + camPos.y*zoomScale);
	// Scaling X,Y
	D3DXVECTOR2 scaling(spriteData.scale*zScale, spriteData.scale*zScale);
	if (spriteData.flipHorizontal)  // if flip horizontal
	{
		scaling.x *= -1;            // negative X scale to flip
		// Get center of flipped image.
		spriteCenter.x -= (float)(spriteData.width*zoomScale);
		// Flip occurs around left edge, translate right to put
		// Flipped image in same location as original.
		translate.x += (float)(spriteData.width*zoomScale);
	}
	if (spriteData.flipVertical)    // if flip vertical
	{
		scaling.y *= -1;            // negative Y scale to flip
		// Get center of flipped image
		spriteCenter.y -= (float)(spriteData.height*zoomScale);
		// Flip occurs around top edge, translate down to put
		// Flipped image in same location as original.
		translate.y += (float)(spriteData.height*zoomScale);
	}
	// Create a matrix to rotate, scale and position our sprite
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,                // the matrix
		NULL,                   // keep origin at top left when scaling
		0.0f,                   // no scaling rotation
		&scaling,               // scale amount
		&spriteCenter,          // rotation center
		(float)(spriteData.angle),  // rotation angle
		&translate);            // X,Y location

	// Tell the sprite about the matrix "Hello Neo"
	sprite->SetTransform(&matrix);

	// Draw the sprite
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;    // default to fail, replace on success
	if (device3d == NULL)
		return  result;
	result = device3d->TestCooperativeLevel();
	return result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::reset()
{
	result = E_FAIL;    // default to fail, replace on success
	initD3Dpp();                        // init D3D presentation parameters
	sprite->OnLostDevice();
	result = device3d->Reset(&d3dpp);   // attempt to reset graphics device

	// Configure for alpha blend of primitives
	device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	sprite->OnResetDevice();
	return result;
}

//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try{
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)      // if already in fullscreen mode
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false) // if already in window mode
				return;
			fullscreen = false; break;
		default:        // default to toggle window/fullscreen
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)  // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else            // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);   // get size of client area of window
			MoveWindow(hwnd,
				0,                                           // Left
				0,                                           // Top
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
				TRUE);                                       // Repaint the window
		}

	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}
}

//=============================================================================
// 三角形ファンを使って、アルファ透過性を持つ四角形を表示
//実行前：createVertexBufferを使ってvertexBufferを作成し、
//		四角形を時計回りの順序で定義する4つの頂点を格納しておく
//		g3ddev->BeginSceneを呼び出す
//実行後：四角形が描画される	
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{
	HRESULT result = E_FAIL; //標準のWindows戻り値
	if (vertexBuffer == NULL)
		return false;


	//アルファブレンドを有効化
	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device3d->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	device3d->SetFVF(D3DFVF_VERTEX);
	result = device3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	//アルファブレンドを無効化
	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	if (FAILED(result))
		return false;
	return true;
}

//=============================================================================
// 円の描画用
//=============================================================================
bool Graphics::drawCircle(D3DXVECTOR2 *vec)
{
	HRESULT result = E_FAIL; //標準のWindows戻り値
	if (vec == NULL)
		return false;
	result = D3DXCreateLine(device3d, &pLine);

	pLine->Begin();
	pLine->Draw(vec, 60, D3DCOLOR_ARGB(0, 255, 0, 0));
	pLine->End();
	if (FAILED(result))
		return false;
	return true;
}


//=============================================================================
//頂点バッファを作成
//実行前：verts[]に頂点データが格納されている
//			size = verts[]のサイズ
//実行後：成功した場合、&vertexBufferがバッファを指す
//=============================================================================
HRESULT Graphics::createVertexBuffer(
	CUSTOMVERTEX verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
	//標準のWindowsの戻り値
	HRESULT result = E_FAIL;
	//頂点バッファを作成
	result = device3d->CreateVertexBuffer(
		size,	//頂点バッファのサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,	//頂点バッファの使用方法今回は書き込みだけを行う
		D3DFVF_VERTEX,	//頂点のフォーマットを記述します。
		D3DPOOL_DEFAULT, //バッファをどのメモリプールに配置するか。今回はデフォルト
		&vertexBuffer, //IDirext3DVertexBuffer9インターフェースへのポインタのアドレス
		NULL);	//予約済み
	if (FAILED(result))
		return result;
	void *ptr;
	result = vertexBuffer->Lock(0, size, (void**)&ptr, 0); //データ転送前にバッファをロック
	if (FAILED(result))
		return result;
	memcpy(ptr, verts, size); //頂点データをバッファにコピー
	vertexBuffer->Unlock();
	return result;
}

//==============================================================================================
// GetSpriteScreenPosition(const SpriteData &spriteData)
//spriteDataとMainCameraを元にspriteDataの画面上の位置を返す
//この場合返してるのは中心位置に実座標の値を足した値
//==============================================================================================
D3DXVECTOR2 Graphics::GetSpriteScreenPosition(const SpriteData &spriteData){
	D3DXVECTOR2 screenPosition = D3DXVECTOR2(0, 0);
	D3DXVECTOR2 cPos = VECTOR2(0, 0);//MainCamera->getCenter();
	float X = cPos.x + spriteData.x / spriteData.scale + GAME_WIDTH / 2.0f;
	float Y = cPos.y + spriteData.y / spriteData.scale + GAME_HEIGHT / 2.0f;
	screenPosition = D3DXVECTOR2(X, Y);
	return screenPosition;
}
//メッシュ描画前にカメラの情報をパイプラインに
void Graphics::CameraSet(){

	D3DXMATRIX View;
	D3DXMATRIX Pers;
	MainCamera = CameraFactory::Instance().GetMainCamera(); //Mainカメラは処理ごとに取得?
	if (MainCamera == nullptr)
		return;
	if (MainCamera->initialized){
		D3DXMatrixLookAtLH(
			&View,
			&MainCamera->position,
			&MainCamera->LookAt,
			&MainCamera->Up
			);
		if (!MainCamera->orthographics){
			D3DXMatrixPerspectiveFovLH(
				&Pers,
				MainCamera->fovY,
				MainCamera->aspect,
				MainCamera->nearClip,
				MainCamera->farClip
				);
		}
		device3d->SetTransform(D3DTS_VIEW, &View);
		device3d->SetTransform(D3DTS_PROJECTION, &Pers);
	}
}
//メッシュ描画前にライトの情報をパイプラインに
void Graphics::LightSet(){
	
	if (&LightFactory::Instance() == nullptr)
		return;
	LightFactory::LightLink* light = LightFactory::Instance().GetFirstLink();
	int mSize = LightFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		_D3DLIGHT9 lg = light->pointer->getlight();
		device3d->SetLight(mSize, &lg);
		device3d->LightEnable(mSize,true);
		if (light->next != nullptr)
			light = light->next;
		else
			i = mSize + 1;

	}
	device3d->SetRenderState(D3DRS_LIGHTING, TRUE);
	device3d->SetRenderState(D3DRS_AMBIENT, 0x00808080);
}
//一旦Angleで回転させてから軸回転を入れ込むためにQuaternionにする
bool Graphics::transformRotMatToQuaternion(
	D3DXQUATERNION &qs,
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33
	){
	float elem[4];
	elem[0] = m11 - m22 - m33 + 1.0f;
	elem[1] = -m11 + m22 - m33 + 1.0f;
	elem[2] = -m11 - m22+ m33 + 1.0f;
	elem[3] = m11 + m22 + m33 + 1.0f;

	unsigned biggestIndex = 0; //最大の大きさの値を求める
	for (int i = 1; i < 4; i++){
		if (elem[i]>elem[biggestIndex])
			biggestIndex = i;

	}

	if (elem[biggestIndex] < 0.0f)//これは引数に問題があるときにしか起きない
		return false;

	float* q[4] = { &(qs.x), &(qs.y), &(qs.z), &(qs.w) };
	float v = sqrtf(elem[biggestIndex])*0.5f; //二乗の1/2
	*q[biggestIndex] = v;
	float mult = 0.25f/v;

	switch (biggestIndex){
	case 0: //xのとき
		*q[1] = (m12 + m21)*mult;
		*q[2] = (m31 + m13)*mult;
		*q[3] = (m23 - m32)*mult;
		break;
	case 1: //yのとき
		*q[0] = (m12 + m21)*mult;
		*q[2] = (m23 + m32)*mult;
		*q[3] = (m31 - m13)*mult;
		break;
	case 2: //zのとき
		*q[0] = (m31 + m13)*mult;
		*q[1] = (m23 + m32)*mult;
		*q[3] = (m12 - m21)*mult;
		break;
	case 3: //wのとき
		*q[0] = (m23 - m32)*mult;
		*q[1] = (m31 - m13)*mult;
		*q[2] = (m12 - m21)*mult;
		break;

	}
	return true;
}

void Graphics::getScreenPosition(D3DXVECTOR3 pos, D3DXVECTOR3 &screenpos){

	D3DXMATRIX View;
	D3DXMATRIX Pers;
	MainCamera = CameraFactory::Instance().GetMainCamera(); //Mainカメラは処理ごとに取得?
	if (MainCamera == nullptr)
		return;
	if (MainCamera->initialized){
		D3DXMatrixLookAtLH(
			&View,
			&MainCamera->position,
			&MainCamera->LookAt,
			&MainCamera->Up
			);
		if (!MainCamera->orthographics){
			D3DXMatrixPerspectiveFovLH(
				&Pers,
				MainCamera->fovY,
				MainCamera->aspect,
				MainCamera->nearClip,
				MainCamera->farClip
				);
		}
	}
	float width = GAME_WIDTH;
	float height = GAME_HEIGHT;

	D3DXMATRIX POS = {	pos.x,0,0,0,
						pos.y, 0, 0, 0,
						pos.z, 0, 0, 0,
						1.0, 0, 0, 0
	};
	/*D3DXMATRIX Viewport = { width,	 0,				0,	0,
							0,			-height,	0, 0,
							0,			 0,				1, 0,
							width / 2,	 height / 2,	0, 1 };*/
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMATRIX scpos;
	D3DVIEWPORT9 viewData = { 0, 0, width, height, 0.0f, 1.0f };

	D3DXVec3Project(&screenpos, &pos,&viewData, &Pers, &View,&mat);

/*	D3DXMatrixMultiply(&scpos, &POS, &View);
	D3DXMatrixMultiply(&scpos, &scpos, &Pers);
	D3DXMatrixMultiply(&scpos, &scpos, &Viewport);
	screenpos = { scpos._11 + width / 2, scpos._21 , scpos._31 +height/2 };
	*/

}