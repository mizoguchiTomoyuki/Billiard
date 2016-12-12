#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"
#include "lightFactory.h"
#include "cameraFactory.h"
#define LP_TEXTURE  LPDIRECT3DTEXTURE9
#define LP_SPRITE   LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#define VECTOR3     D3DXVECTOR3
#define VECTOR2     D3DXVECTOR2
#define LP_VERTEXBUFFER LPDIRECT3DVERTEXBUFFER9
#define LP_DXFONT   LPD3DXFONT
#define LP_VERTEXBUFFER LPDIRECT3DVERTEXBUFFER9




#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


namespace graphicsNS
{
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB ALPHA75 = D3DCOLOR_ARGB(192, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	enum DISPLAY_MODE{ TOGGLE, FULLSCREEN, WINDOW };
}



#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define FVF_CUSTOM (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct CUSTOMVERTEX{
	float x, y, z; //頂点座標
	float rhw;
	COLOR_ARGB dwColor; //頂点から―
	float u, v;
};
struct VertexC              // Vertex with Color
{
	float x, y, z;          // vertex location
	float rhw;              // reciprocal homogeneous W (set to 1)
	unsigned long color;    // vertex color
};

struct SpriteData
{
	int         width;      // width of sprite in pixels
	int         height;     // height of sprite in pixels
	float       x;          // screen location (top left corner of sprite)
	float       y;
	float		z;
	float		ratioZ;
	float		screenX; //カメラスクリーン上のX左上を0,0右下をGAME_WIDTH,GAME_HEIGHTとする
	float		screenY;
	float       scale;      // <1 smaller, >1 bigger
	float       angle;      // rotation angle in radians
	RECT        rect;       // used to select an image from a larger texture
	LP_TEXTURE  texture;    // pointer to texture
	bool        flipHorizontal; // true to flip sprite horizontally (mirror)
	bool        flipVertical;   // true to flip sprite vertically
};

struct MeshData
{
	ID3DXBuffer	 *pMaterials;
	DWORD		 NumMaterial;
	D3DCOLORVALUE MAmbient;
	LPD3DXMESH	 Mesh;
	LP_TEXTURE   *Texture;
	VECTOR3		scale;
	VECTOR3		Angle;
	VECTOR3		position;
	struct Axis_Rotation{
		D3DXVECTOR3 Axis;
		float Angle;
	};
	Axis_Rotation ar;
	bool isWire;

};
struct CameraData
{
	VECTOR3 position;
	VECTOR3 LookAt;
	VECTOR3 Up;
	float fovY;
	float aspect;
	float nearClip;
	float farClip;
	bool orthographics;
	bool initialized = false;
	ELEMID _id;
};

class Graphics
{
private:
	// DirectX pointers and stuff
	LP_3D       direct3d;
	LP_3DDEVICE device3d;
	LP_SPRITE   sprite;

	ID3DXLine* pLine;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	CameraData*  MainCamera;
	HRESULT     result;
	HWND        hwnd;
	bool        fullscreen;
	int         width;
	int         height;
	COLOR_ARGB  backColor;      // background color

	void    initD3Dpp();

public:
	Graphics();
	virtual ~Graphics();

	// Releases direct3d and device3d.
	void    releaseAll();

	// 初期化
	void    initialize(HWND hw, int w, int h, bool full);

	// 頂点バッファ作成
	HRESULT createVertexBuffer(CUSTOMVERTEX verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer);

	// Display a quad (rectangle) with alpha transparency.
	// Pre: createVertexBuffer was used to create vertexBuffer containing four
	//      vertices defining the quad in clockwise order.
	//      g3ddev->BeginScene was called
	bool    drawQuad(LP_VERTEXBUFFER vertexBuffer);

	//円を描きます
	bool    drawCircle(D3DXVECTOR2 *vec);
	//テクスチャの読み込み
	HRESULT loadTexture(const char * filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	//Xファイルからメッシュの読み込み
	HRESULT loadXMesh(const char *file, COLOR_ARGB transcolor, MeshData &mesh, const char *texdir);
	//テクスチャの読み込み
	HRESULT loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);

	// Display the offscreen backbuffer to the screen.
	HRESULT showBackbuffer();

	bool    isAdapterCompatible();


	void    drawSprite(const SpriteData &spriteData,           // sprite to draw
		COLOR_ARGB color = graphicsNS::WHITE);      // default to white color filter (no change)

	void drawMesh(const MeshData &meshData);
	//==============================================================================================
	// GetSpriteScreenPosition(const SpriteData &spriteData)
	//spriteDataとMainCameraを元にspriteDataの画面上の位置を返す
	//==============================================================================================
	D3DXVECTOR2    GetSpriteScreenPosition(const SpriteData &spriteData);
	// Reset the graphics device.
	HRESULT reset();

	// Toggle, fullscreen or window display mode
	// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
	// Post: All user surfaces are recreated.
	void    changeDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// Return length of vector v.
	static float    Vector2Length(const VECTOR2 *v) { return D3DXVec2Length(v); }

	// Return Dot product of vectors v1 and v2.
	static float    Vector2Dot(const VECTOR2 *v1, const VECTOR2 *v2) { return D3DXVec2Dot(v1, v2); }

	// Normalize vector v.
	static void     Vector2Normalize(VECTOR2 *v) { D3DXVec2Normalize(v, v); }

	// Transform vector v with matrix m.
	static VECTOR2* Vector2Transform(VECTOR2 *v, D3DXMATRIX *m) { return D3DXVec2TransformCoord(v, v, m); }

	static float Vector3Dot(const VECTOR3 *v1, const VECTOR3 *v2){ return D3DXVec3Dot(v1, v2); }


	static void     Vector3Normalize(VECTOR3 *v) { D3DXVec3Normalize(v, v); }

	static float    Vector3Length(const VECTOR3 *v) { return D3DXVec3Length(v); }


	// get functions
	// Return direct3d.
	LP_3D   get3D()             { return direct3d; }

	// Return device3d.
	LP_3DDEVICE get3Ddevice()   { return device3d; }

	// Return sprite
	LP_SPRITE   getSprite()     { return sprite; }

	// Return handle to device context (window).
	HDC     getDC()             { return GetDC(hwnd); }

	float getCameraScale(){ return  1; }// MainCamera->getZoomScale(); }
	VECTOR3 getCameraPos(){ return MainCamera->position; }//MainCamera->getcameraPos();}
	const CameraData* getMainCamera(){ return MainCamera; }
	// Test for lost device
	HRESULT getDeviceState();

	// Return fullscreen
	bool    getFullscreen()     { return fullscreen; }

	// Set color used to clear screen
	void setBackColor(COLOR_ARGB c) { backColor = c; }

	void SetMainCamera(CameraData* cam){ MainCamera = cam; }

	//=============================================================================
	// Clear backbuffer and BeginScene()
	//=============================================================================
	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL)
			return result;
		// clear backbuffer to backColor
		device3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(40, 40, 80), 1.0f, 0);
		result = device3d->BeginScene();          // begin scene for drawing

		return result;
	}
	void CameraSet();
	void LightSet();
	//=============================================================================
	// EndScene()
	//=============================================================================
	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		device3d->Present(NULL, NULL, NULL, NULL);
		return result;
	}

	//=============================================================================
	// Sprite Begin
	//=============================================================================
	void spriteBegin()
	{
		//CameraSet();
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	//=============================================================================
	// Sprite End
	//=============================================================================
	void spriteEnd()
	{
		sprite->End();
	}

	void MeshBegin(){
		CameraSet();
		LightSet();

	}

	void MeshEnd(){

	}

	bool transformRotMatToQuaternion(
		D3DXQUATERNION &q,
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33
		);
	void getScreenPosition(D3DXVECTOR3 pos, D3DXVECTOR3 &screenpos);
};

#endif