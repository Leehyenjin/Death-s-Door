#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Bone.h"
#include "Player_State.h"
#include "Sword_Slash.h"
#include "Monster_Arrow.h"
#include "Monster.h"
#include "GrimacerMonster.h"
#include "BlobMonster.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	if (FAILED(Ready_Slash_Parts()))
		return E_FAIL;

	/*if (FAILED(Ready_Arrow()))
		return E_FAIL;*/

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	m_pModelCom->Set_Animation(35);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(73.405f, 0.f, 8.090f, 1.f));	// - 0.505
	m_pTransformCom->Set_Angle(XMConvertToRadians(0.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f));

	m_pPlayerAbility = new PLAYERABILITY;
	m_pPlayerAbility->fHP = 4.f;
	m_pPlayerAbility->fMaxHP = 4.f;
	m_pPlayerAbility->fHP = m_pPlayerAbility->fMaxHP;

	return S_OK;
}

_uint CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fAnimationTime);
	
	for (_uint i = 0; i < m_PlayerParts.size(); ++i)
	{
		m_PlayerParts[i]->Tick(TimeDelta);
	}

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	Set_OnCell();

	return S_OK;
}

void CPlayer::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	for (_uint i = 0; i < m_PlayerParts.size(); ++i)
	{
		m_PlayerParts[i]->Late_Tick(TimeDelta);
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		
		for (auto& pCollider : m_pColliderCom)
		{
			m_pRendererCom->Add_DebugRenderGroup(pCollider);
		}
		for (auto& pColl : m_pCollHit)
		{
			m_pRendererCom->Add_DebugRenderGroup(pColl);
		}

		m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}
}

HRESULT CPlayer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	// For.Com_Renderer
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))	
		return E_FAIL;

	// For.Com_Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Crow"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com_State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;	

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.0f, 1.9f, 3.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(5.0f, 1.f, 5.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_HIT
	ColliderDesc.vSize = _float3(2.f, 1.f, 2.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE_HIT"),
		(CComponent**)&m_pCollHit[COLL_HIT], &ColliderDesc)))
		return E_FAIL;

	// For.Com_Navigation
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"),
		(CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	// For.Lights
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_PlayerState = CPlayer_State::Create(this);
	if (nullptr == m_PlayerState)	
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	CGameObject*			pPartObject = { nullptr };

	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CWeapon::WEAPONDESC		WeaponDesc;

	ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Arm_R");
	WeaponDesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(WeaponDesc.pSocket);
	Safe_AddRef(m_pTransformCom);

	pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword"), &WeaponDesc);
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PlayerParts.push_back(pPartObject);

	static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
				Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.19f, 1.3f, 0.68f, 1.f));
			static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
				Set_Angle(XMConvertToRadians(1.2f), XMConvertToRadians(-1.3f), XMConvertToRadians(108.4f));
			static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
				Set_Scaling(_float3(1.5f, 1.5f, 1.5f));

	RELEASE_INSTANCE(CGameInstance);

	//switch (m_eWeapon)
	//{
	//case WEAPON_SWORD_R:
	//	if (WEAPON_SWORD_R == m_eWeapon)
	//	{
	//		// Weapon_Sword_R
	//		ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	//		WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	//		WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Weapon_R");
	//		WeaponDesc.pTargetTransform = m_pTransformCom;
	//		Safe_AddRef(WeaponDesc.pSocket);
	//		Safe_AddRef(m_pTransformCom);

	//		pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword"), &WeaponDesc);
	//		if (nullptr == pPartObject)
	//			return E_FAIL;

	//		m_PlayerParts.push_back(pPartObject);

	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.9f, 0.603f, -0.4f, 1.f));
	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_Angle(XMConvertToRadians(-90.799f), XMConvertToRadians(-40.569f), XMConvertToRadians(-163.887f));
	//	}
	//	break;

	//case WEAPON_SWORD_L:
	//	if (WEAPON_SWORD_L == m_eWeapon)
	//	{
	//		// Weapon_Sword_L
	//		ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	//		WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	//		WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Weapon_L");
	//		WeaponDesc.pTargetTransform = m_pTransformCom;
	//		Safe_AddRef(WeaponDesc.pSocket);
	//		Safe_AddRef(m_pTransformCom);

	//		pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword"), &WeaponDesc);
	//		if (nullptr == pPartObject)
	//			return E_FAIL;

	//		m_PlayerParts.push_back(pPartObject);

	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.64f, 0.74f, -0.12f, 1.f));
	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_Angle(XMConvertToRadians(70.4f), XMConvertToRadians(-7.8f), XMConvertToRadians(-55.3f));
	//	}
	//	break;

	//case WEAPON_SWORD_BADY:
	//	if (WEAPON_SWORD_BADY == m_eWeapon)
	//	{
	//		// Weapon_Sword_Bady
	//		ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	//		WeaponDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	//		WeaponDesc.pSocket = m_pModelCom->Get_BonePtr("Arm_R");
	//		WeaponDesc.pTargetTransform = m_pTransformCom;
	//		Safe_AddRef(WeaponDesc.pSocket);
	//		Safe_AddRef(m_pTransformCom);

	//		pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword"), &WeaponDesc);
	//		//pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon"), TEXT("Prototype_GameObject_Sword"), TEXT("Sword"), &WeaponDesc);
	//		if (nullptr == pPartObject)
	//			return E_FAIL;

	//		m_PlayerParts.push_back(pPartObject);

	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.19f, 1.3f, 0.68f, 1.f));
	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_Angle(XMConvertToRadians(1.2f), XMConvertToRadians(-1.3f), XMConvertToRadians(108.4f));
	//		static_cast<CTransform*>(pPartObject->Get_Component(TEXT("Com_Transform")))->
	//			Set_Scaling(_float3(1.5f, 1.5f, 1.5f));
	//	}
	//	break;
	//}         

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK; 
}

HRESULT CPlayer::Ready_Slash_Parts()
{
	ENGINE_OPEN;

	CSword_Slash::SLASHDESC		SlashDesc;
	ZeroMemory(&SlashDesc, sizeof(CSword_Slash::SLASHDESC));

	SlashDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	SlashDesc.pSocket = m_pModelCom->Get_BonePtr("_root");
	SlashDesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(SlashDesc.pSocket);
	Safe_AddRef(m_pTransformCom);
	
	pSlashPartObject = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sword_Slash"), &SlashDesc);
	if (nullptr == pSlashPartObject)
		return E_FAIL;
		
	m_PlayerParts.push_back(pSlashPartObject);

	static_cast<CTransform*>(pSlashPartObject->Get_Component(TEXT("Com_Transform")))->
		Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	static_cast<CTransform*>(pSlashPartObject->Get_Component(TEXT("Com_Transform")))->
		Set_Angle(XMConvertToRadians(10.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	static_cast<CTransform*>(pSlashPartObject->Get_Component(TEXT("Com_Transform")))->
		Set_Scaling(_float3(0.7f, 0.7f, 0.7f));
	

	ENGINE_CLOSE;
	
	return S_OK;
}

//HRESULT CPlayer::Ready_Arrow()
//{
//	ENGINE_OPEN;
//
//	CMonster_Arrow::ARROWDESC		ArrowDesc;
//	ZeroMemory(&ArrowDesc, sizeof(CMonster_Arrow::ARROWDESC));
//
//	ArrowDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
//	ArrowDesc.pSocket = m_pModelCom->Get_BonePtr("_root");
//	ArrowDesc.pTargetTransform = m_pTransformCom;
//	Safe_AddRef(ArrowDesc.pSocket);
//	Safe_AddRef(m_pTransformCom);
//
//	pSlashPartObject[1] = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Arrow"), &ArrowDesc);
//	if (nullptr == pSlashPartObject)
//		return E_FAIL;
//
//	m_PlayerParts.push_back(pSlashPartObject[1]);
//
//	ENGINE_CLOSE;
//
//	return E_NOTIMPL;
//}

HRESULT CPlayer::Set_OnCell()
{
	ENGINE_OPEN;
	
	_vector		vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float		fHeight = m_pNavigationCom->Height_OnNavigation(vPos);

	m_pTransformCom->Set_Height(fHeight); 

	ENGINE_CLOSE;

	return S_OK;
}

CCollider * CPlayer::Get_WeaponSet()
{
	return static_cast<CSword_Slash*>(m_PlayerParts.front())->Get_ColliderWeapon();

	//CWeapon*	pCollWeapon = (CWeapon*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Weapon"), TEXT("Com_Collider_R"));
}

void CPlayer::isDead(_float fDamge)
{
	m_pPlayerAbility->fHP -= fDamge;

	if (m_pPlayerAbility->fHP <= 0.f)
		m_pPlayerAbility->fHP = 0;
}

void CPlayer::Distance(CCollider * TargetDistanceCollider)
{
	/*if (nullptr == m_pDistanceCollider ||
		nullptr == TargetDistanceCollider)
		return;

	_bool bDistance = m_pDistanceCollider->Collision???(TargetDistanceCollider, CCollider::TYPE_SPHERE_TARGET);

	if (bDistance)
	{
		_vector vDistanceVector = m_pDistanceCollider->Get_Center() - TargetDistanceCollider->Get_Center();
		_float	fContectDistance = m_pDistanceCollider->Get_Length() + TargetDistanceCollider->Get_Length();
		_float	fCurrDistace = XMVectorGetX(XMVector3Length(vDistanceVector));
		_float	fResultDistanceSize = fContectDistance - fCurrDistace;

		_float4		fMyPos, fMyDir;
		XMStoreFloat4(&fMyPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
		XMStoreFloat4(&fMyDir, XMVector3Normalize(vDistanceVector));

		if (m_pNavigationCom->isMove_OnNavigation(&fMyPos, &fMyDir))
			m_pTransformCom->Turn(XMVector3Normalize(vDistanceVector), fResultDistanceSize);
		else
		{
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fMyPos));
			m_pTransformCom->Turn(XMVector3Normalize(XMLoadFloat4(&fMyDir)), fResultDistanceSize);
		}
	}*/
}
_bool CPlayer::Collision_Damage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGrimacerMonster* TargetGrimacer = static_cast<CGrimacerMonster*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_GrimacerMonster"), TEXT("GameObject_GrimacerMonster")));

	if (TargetGrimacer->Get_ColliderAttack()->Get_CheckAble())
	{
		if (m_pCollHit[COLL_HIT]->Collision(TargetGrimacer->Get_ColliderAttack()))
		{
			TargetGrimacer->Get_ColliderAttack()->Set_CheckAble(false);
			RELEASE_INSTANCE(CGameInstance);
			return true;
		}
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}

	/*CBlobMonster*	TargetBlob = static_cast<CBlobMonster*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_BlobMonster"), TEXT("GameObject_BlobMonster")));

	if (TargetBlob->Get_ColliderAttack()->Get_CheckAble())
	{
		if (m_pCollHit[COLL_HIT]->Collision(TargetBlob->Get_ColliderAttack()))
		{
			RELEASE_INSTANCE(CGameInstance);
			return true;
		}
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
*/
	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Filed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPalyer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < COLL_END; ++i)
		Safe_Release(m_pCollHit[i]);

	for (auto& pPart : m_PlayerParts)
		Safe_Release(pPart);

	m_PlayerParts.clear();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_PlayerState);
	Safe_Release(m_pNavigationCom);
	Safe_Release(pSlashPartObject);

	Safe_Delete<PLAYERABILITY*>(m_pPlayerAbility);
}
