#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Player.h"
#include "Door.h"
#include "Stairs.h"
#include "Stairs02.h"
#include "Fance.h"
#include "FanceBar.h"
#include "Wall04.h"
#include "Wall03.h"
#include "Pillar.h"
#include "CubeBox.h"
#include "Floor.h"
#include "Weapon.h"
#include "Sword_Slash.h"
#include "Monster.h"
#include "NaviCube.h"
#include "StageMap.h"
#include "GrimacerMonster.h"
#include "PlantMonster.h"
#include "MiddleBoss.h"
#include "BlobMonster.h"
#include "Monster_Arrow.h"

////// UI /////////
#include "HpBar.h"
#include "SkillBox.h"
#include "WeaponNoBox.h"
#include "SkillBoxUp.h"
#include "WeaponBoxUp.h"
#include "Icon_Arrow.h"
#include "Icon_Fireball.h"
#include "WeaponBigBox.h"
#include "Hp_Empty.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}	

_uint APIENTRY LoadingThread(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogo();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 로딩을 하기위한 추가적인 흐름을 만든다 (Thread).*/
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
	
	return S_OK;
}

/* 로고를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_ForLogo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/First_Logo.jpg")))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("객체원형을 생성중입니다. "));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩끝. "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

/* 게임플레이를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_ForGamePlay()
{
	ENGINE_OPEN;

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.dds"), 1))))
		return E_FAIL;
	
	/////////////////////////////////     UI	///////////////////////////////////////////////////////

	// For.Prototype_Component_Texture_HpBar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/HpBar/HpBar.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_Hp_Empty
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp_Empty"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/HpBar/NoHp.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_SkillBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkillBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SkillBar/NoSkill.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_SkillBoxUp
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkillBoxUp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SkillBar/SkillBar.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_WeaponBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WeaponNoBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/WeaponBar/WeaponBox.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_WeaponBoxUp
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WeaponBoxUp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/WeaponBar/NoWeaponBox.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_Icon_Arrow
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Icon_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/WeaponBar/Icon_Arrow.png")))))
		return E_FAIL;

	//// For.Prototype_Component_Texture_Icon_Fireball
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Icon_Fireball"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/WeaponBar/Icon_Fireball.png")))))
	//	return E_FAIL;

	// For.Prototype_Component_Texture_WeaponBoxBig
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WeaponBoxBig"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/WeaponBar/WeaponOnBox.png")))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// For.Prototype_Component_Texture_Explosion
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/"), ? ))))
		return E_FAIL;*/

	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	PivotMatrix = XMMatrixIdentity();

	// For.Prototype_Component_Model_Crow
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(360.0f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Crow"),			// Crow
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Player/Crow.fbx", PivotMatrix))))
		return E_FAIL;
	
	// For.Prototype_Component_Model_Arrow
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Arrow"),		// Arrow
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Weapon/Arrow.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Model_MiddleBoss
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-180.f));
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MiddleBoss"),		// MiddleBoss
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/MiddleBoss/MiddleBoss.fbx", PivotMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	// For.Prototype_Component_Model_GrimacerMonster
	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GrimacerMonster"),		// GrimacerMonster
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster2/Monster2.fbx", PivotMatrix))))
		return E_FAIL;	

	// For.Prototype_Component_Model_blobMonster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BlobMonster"),				// blobMonster
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster4/blob.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Model_AroMage
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_AroMageMonster"),		// AroMageMonster
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster5/AroMage.fbx", PivotMatrix))))
		return E_FAIL;*/

	// For.Prototype_Component_Model_PlantMonster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PlantMonster"),		// pIantMonster
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/plantMonster/plantMonster.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Sword_Slash
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sword_Slash"),			// Sword_Slash
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Weapon/Slash.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Bow
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bow"),					// Bow
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Weapon/Bow.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Model_Monster(궁수)
	PivotMatrix = XMMatrixScaling(0.8f, 0.8f, 0.8f) * XMMatrixRotationY(XMConvertToRadians(90.0f)) * XMMatrixTranslation(0.15f, 0.f, -0.2f);
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Monster.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_StageMap(1스테이지)
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_StageMap"),	// StageMap1
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/TestMap/TestMap.fbx", PivotMatrix))))
	//	return E_FAIL;

	// For.Prototype_Component_Door
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Door"), // Door
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Door/Door.fbx", PivotMatrix)))) 
		return E_FAIL;

	// For.Prototype_Component_Stairs
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Stairs"), // Stairs
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stairs/Stairs1/Stairs1.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Stairs02
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Stairs02"), // Stairs02
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stairs/Stairs2/Stairs2.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Fance
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Fance"),		// Fance
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fance/Fance/fenceSpike2.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_FanceBar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_FanceBar"),	// FanceBar
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fance/FanceBar/fenceBar.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Wall03
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Wall03"),		// Wall03
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Wall/lowWall3/lowWall3.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Wall04
	//if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Wall04"),		// Wall04	로드가 오래걸림
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Wall/lowWall4/lowWall4.fbx", PivotMatrix))))
	//	return E_FAIL;

	// For.Prototype_Component_Pillar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Pillar"),		// Pillar
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Pillar/Pillar.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_CubeBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_CubeBox"),		// CubeBox	
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CubeBox/CubeBox.fbx", PivotMatrix))))
		return E_FAIL;
	
	// For.Prototype_Component_Floor
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Floor"),			// Floor
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/floor/floor.fbx", PivotMatrix))))
		return E_FAIL;

	// For.Prototype_Component_Sword
	//PivotMatrix = XMMatrixScaling(2.f, 2.f, 2.f); //* XMMatrixRotationY(XMConvertToRadians(360.0f)) * XMMatrixTranslation(-0.1f, -0.4f, -0.1f);
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sword"),
		(CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Weapon/Sword.fbx", PivotMatrix)))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("콜라이더를 로딩중입니다. "));
	// For.Prototype_Component_Collider_AABB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	// For.Prototype_Component_Collider_OBB
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	// For.Prototype_Component_Collider_SPHERE
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	// For.Prototype_Component_Collider_SPHERE_TARGET
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE_TARGET))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));
	// For.Prototype_Component_Shader_Terrain
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	// For.Prototype_Component_Shader_VtxModel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	// For.Portotype_Component_State
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		CState::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// For.Prototype_Component_Shader_VtxAnimModel
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	// For.Prototype_Component_Shader_VtxSquare
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VertexSquare"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VertexSquare.hlsl"), VTXSQUARE_DECLARATION::Elements, VTXSQUARE_DECLARATION::iNumElement))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체원형을 생성중입니다. "));
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext, ID_ENVIRONMENT))))
		return E_FAIL;

	// For.Prototype_GameObject_Player
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext, ID_PLAYER))))
		return E_FAIL;

	// For.Prototype_GameObject_MiddleBoss
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MiddleBoss"),
		CMiddleBoss::Create(m_pDevice, m_pContext, ID_MIDDLEBOSS))))
		return E_FAIL;*/

	// For.Prototype_GameObject_Door
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door"), 
		CDoor::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObejct_Stairs
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stairs"),
		CStairs::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_Stairs02
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stairs02"),
		CStairs02::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_Fance
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Fance"),
		CFance::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_FanceBar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FanceBar"),
		CFanceBar::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_Wall03
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wall03"),
		CWall03::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_Wall04
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wall04"),
		CWall04::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;*/

	// For.Prototype_GameObject_Pillar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Pillar"),
		CPillar::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_CubeBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeBox"),
		CCubeBox::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_Floor
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Floor"),
		CFloor::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;

	// For.Prototype_GameObject_StageMap
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StageMap"),
		CStageMap::Create(m_pDevice, m_pContext, ID_INSTALLATLON))))
		return E_FAIL;*/

	// For.Prototype_GameObject_Monster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	// For.Prototype_GameObject_GrimacerMonster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_GrimacerMonster"),
		CGrimacerMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	// For.Prototype_GameObject_PlantMonster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_PlantMonster"),
		CPlantMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	// For.Prototype_GameObject_blobMonster
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_BlobMonster"),
		CBlobMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;

	// For.Prototype_GameObject_AroMageMonster
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_AroMageMonster"),
		CAroMageMonster::Create(m_pDevice, m_pContext, ID_MONSTER))))
		return E_FAIL;*/

	// For.Prototpye_GameObject_Weapon(Sword)
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword"),	
		CWeapon::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	// For.Prototype_GameObject_Weapon(Bow)
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Bow"),
		CWeapon::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	// For.Prototpye_GamObject_Weapon(Arrow)	
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Arrow"),
		CMonster_Arrow::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;
	
	// For.Prototype_GameObject_Sword_Slash
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword_Slash"),
		CSword_Slash::Create(m_pDevice, m_pContext, ID_WEAPON))))
		return E_FAIL;

	// For.Prototype_GameObject_NaviCube
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_NaviCube"),
		CNaviCube::Create(m_pDevice, m_pContext, ID_CUBE))))
		return E_FAIL;

	// For.Prototype_Component_Navigation
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation.dat")))))
		return E_FAIL;

	// For.Prototype_Component_HpBar
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HpBar"),
		(CHpBar::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_Hp_Empty
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Hp_Empty"),
		CHp_Empty::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW))))
		return E_FAIL;
		
	// For.Prototype_Component_SkillBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SkillBox"),
		(CSkillBox::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_SkillBoxUp
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SkillBoxUp"),
		(CSkillBoxUp::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_WeaponBox
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WeaponBox"),
		(CWeaponNoBox::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_WeaponBoxUp
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WeaponBoxUp"),
		(CWeaponBoxUp::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_WeapoBoxBig
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_WeaponBoxBig"),
		(CWeaponBigBox::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_Icon_Arrow
	if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Icon_Arrow"),
		(CIcon_Arrow::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;

	// For.Prototype_Component_Icon_Firebal
	/*if (FAILED(ENGINE->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Icon_Fireball"),
		(CIcon_Fireball::Create(m_pDevice, m_pContext, ID_UI, CUI::UI_WINDOW)))))
		return E_FAIL;*/

	lstrcpy(m_szLoadingText, TEXT("로딩끝. "));

	m_isFinished = true;

	ENGINE_CLOSE;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteObject(m_hThread);
	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
