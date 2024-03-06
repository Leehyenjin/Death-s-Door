#include "stdafx.h"
#include "..\public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SkillBox.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GrimacerMonster(TEXT("Layer_GrimacerMonster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlantMonster(TEXT("Layer_PlantMonster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon(TEXT("Layer_Weapon"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_InstallationObject(TEXT("Game_Play_Layer_Object"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MiddleBoss(TEXT("Layer_MiddleBoss"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BlobMonster(TEXT("Layer_BlobMonster"))))
		return E_FAIL;

	/*ENGINE_OPEN;
	
	ENGINE->Play_Sound(TEXT("79306965.mp3"), 0.5, true, 0);

	ENGINE_CLOSE;*/

	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_GamePlay::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level : GAMEPLAY"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC				LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.isEnable = true;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);		// 1.f, 1.f, 1.f, 1.f
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);		// 1.f, 1.f, 1.f, 1.f

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.isEnable = true;
	LightDesc.vPosition = _float4(10.f, 3.f, 5.f, 1.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.4f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"), TEXT("GameObject_Terrain"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);

	CameraDesc.FOV = XMConvertToRadians(60.f);
	CameraDesc.Aspect = g_iWinSizeX / _float(g_iWinSizeY);
	CameraDesc.NearZ = 0.2f;
	CameraDesc.FarZ = 300.f;

	CameraDesc.m_MouseMovement = 0.1f;

	CameraDesc.TransformDesc.fSpeedPerSec = 20.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), TEXT("GameObject_Dynamic_Camera"), &CameraDesc)))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Static"), TEXT("GameObject_Static_Camera"))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	ENGINE_OPEN;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_HpBar"), TEXT("GameObject_HpBar"))))
		return E_FAIL;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Hp_Empty"), TEXT("GameObject_Hp_Empty"))))
		return E_FAIL;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SkillBox"), TEXT("GameObject_SkillBox"))))
		return E_FAIL;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_WeaponBox"), TEXT("GameObject_WeaponBox"))))
		return E_FAIL;
	
	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_SkillBoxUp"), TEXT("GameObject_SkillBoxUp"))))
		return E_FAIL;
	
	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_WeaponBoxUp"), TEXT("GameObject_WeaponBoxUp"))))
		return E_FAIL;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Icon_Arrow"), TEXT("GameObject_Icon_Arrow"))))
		return E_FAIL;

	if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_WeaponBoxBig"), TEXT("GameObject_WeaponBoxBig"))))
		return E_FAIL;
		
	/*if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Icon_Fireball"), TEXT("GameObject_Icon_Fireball"))))
		return E_FAIL;*/

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), TEXT("GameObject_Player"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	

	for (_uint i = 0; i < 4; ++i)
	{
		if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"), TEXT("GameObject_Monster"))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_GrimacerMonster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_GrimacerMonster"), TEXT("GameObject_GrimacerMonster"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _tchar * pLayerTag)
{
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	///*if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Bow"), TEXT("GameObject_Bow"))))
	//	return E_FAIL;*/

	//if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sword_Slash"), TEXT("GameObject_Sword_Slash"))))
	//	return E_FAIL;

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_InstallationObject(const _tchar * pLayerTag)
{
	/*CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_IntallationObject"), TEXT("GameObject_IntallationObejctr"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_PlantMonster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PlantMonster"), TEXT("GameObject_PlantMonster"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MiddleBoss(const _tchar * pLayerTag)
{
	ENGINE_OPEN;

	/*if (FAILED(ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MiddleBoss"), TEXT("GameObject_MiddleBoss"))))
		return E_FAIL;*/

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BlobMonster(const _tchar * pLayerTag)
{
	ENGINE_OPEN;

	if (ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_BlobMonster"), TEXT("GameObject_BlobMonster")))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
