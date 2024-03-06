#pragma once
namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWindowMode;
		unsigned int	iViewportSizeX;
		unsigned int	iViewportSizeY;
		HWND			hWnd;

	}GRAPHIC_DESC;

	typedef struct tagModelMaterial
	{
		class CTexture*		pTexture[AI_TEXTURE_TYPE_MAX];

	}MODELMATERIAL;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType;
		
		bool			isEnable;
		XMFLOAT4		vDirection;

		// 모든 정점(픽셀)이 받는 빛의 방향벡터가 다 다르게 표현되어야하기때문이다
		// 셰이더내에서 빛의 방향벡터를 구해서 연산한다
		XMFLOAT4		vPosition;
		float			fRange;
		
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

	}LIGHTDESC;

	typedef struct tagKeyframe
	{
		double		Time;
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;

	}KEYFRAME;

	typedef struct tagVertexPosition	// 셀을 그리는 용도이기 떄문에 위치만 가지고있으면된다
	{
		XMFLOAT3		vPosition;

	}VTXPOS;

	typedef struct ENGINE_DLL tagVertexPosition_Declaration
	{
		static const unsigned int			iNumElements = 1;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	}VTXPOS_DECLARATION;

	typedef struct tagFaceIndices16			
	{
		unsigned short			_0, _1, _2;

	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long			_0, _1, _2;

	}FACEINDICES32;

	typedef struct tagVertexPositionTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;

	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexPositionTexture_Declaration
	{
		static const unsigned int			iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	}VTXTEX_DECLARATION;
	
	typedef struct tagVertexSquare
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vTextureUV;

	}VTXSQUARE;

	typedef struct ENGINE_DLL tagVertexSquareDeclaration
	{
		static const unsigned int iNumElement = 2;
		static D3D11_INPUT_ELEMENT_DESC Elements[iNumElement];

	}VTXSQUARE_DECLARATION;


	typedef struct tagVertexPositionNormalTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;

	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexture_Declaration
	{
		static const unsigned int			iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	}VTXNORTEX_DECLARATION;

	typedef struct tagVertexModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTexUV;
		XMFLOAT3		vTangent;

	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int			iNumElements = 4;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	}VTXMODEL_DECLARATION;

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex; 
		XMFLOAT4		vBlendWeight;

	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int			iNumElements = 6;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	}VTXANIMMODEL_DECLARATION;

	typedef struct tagAbility
	{
		_float		fHP;
		_float		fMaxHP;
	
	}PLAYERABILITY;

	typedef struct tagPSwordAbility
	{
		_float		fSwordAttack;

	}PSOWORDABILITY;

	typedef struct tagMonsterAbility
	{
		_float		fCurrentHP;
		_float		fMaxHp;
		_float		fDamage;

	}MONSTERABILITY;

	typedef struct tagBossAbility
	{
		_float		fCurrentHP;
		_float		fMaxHP;
		_float		fDamage;

	}BOSSABILITY;

	typedef struct tagInteractableAbility
	{
		_float		fCurrentHP;
		_float		fMaxHP;

	}INTERACTABLE;
}

