#include "stdafx.h"
#include "..\public\Monster.h"
#include "Monster_State.h"
#include "Player.h"
#include "Bone.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 4.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	/*if (FAILED(Ready_Pats()))
		return E_FAIL;*/

	m_pModelCom->Set_Animation(2);
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(96.670f, 4.548f, 29.183f, 1.f));
	//m_pTransformCom->Set_Angle(XMConvertToRadians(136.238f), XMConvertToRadians(-77.522f), XMConvertToRadians(-137.604f));

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(rand() % 100, 4.548f, rand() % 30, 1.f));

	m_pMonsterAbility = new MONSTERABILITY;
	m_pMonsterAbility->fMaxHp = 3.f;
	m_pMonsterAbility->fCurrentHP = 3.f;
	m_pMonsterAbility->fDamage = 1.f;
	m_pMonsterAbility->fCurrentHP = m_pMonsterAbility->fMaxHp;

	return S_OK;
}

_uint CMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fAnimationTime);

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	//Set_OnCell();

	return S_OK;
}

void CMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		if (!m_bDead)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

			for (auto& pCollider : m_pColliderCom)
			{
				m_pRendererCom->Add_DebugRenderGroup(pCollider);
			}
			for (auto & iter : m_pCollHit)
			{
				m_pRendererCom->Add_DebugRenderGroup(iter);
			}

			if (m_pMonsterAbility->fCurrentHP == 0)
			{
				m_bDead = true;
			}
			if (m_bDead && m_pMonsterAbility->fCurrentHP == 0)
			{
				Kill_Obj();
			}
		}
	}
}

HRESULT CMonster::Render()
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

HRESULT CMonster::SetUp_Components()
{
	// For.Com_Renderer
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com.Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com.State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	//// For.Com_AABB
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ColliderDesc.vSize = _float3(0.7f, 1.5f, 0.7f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"),
	//	(CComponent**)&m_pColliderCom, &ColliderDesc)))
	//	return E_FAIL;

	//// For.Com_OBB 
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ColliderDesc.vSize = _float3(1.0f, 1.0f, 1.0f);
	//ColliderDesc.vRotation = _float3(0.f, 0.0f, 0.f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"),
	//	(CComponent**)&m_pColliderCom[COLLTYPE_OBB], &ColliderDesc)))
	//	return E_FAIL;

	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(20.0f, 1.9f, 20.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);
	m_pModelCom->Get_BonePtr("Weapon_hand_R");

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(13.0f, 0.01f, 13.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	// For,Com_SPHERE(HIT)
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 1.f, 3.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Monster_SPHEREHIT"),
		(CComponent**)&m_pCollHit[COLL_HIT], &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	// For.Light
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CMonster::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_MonsterState = CMonster_State::Create(this);
	if (nullptr == m_MonsterState)
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Pats()
{
	CGameObject*				pPartObject = { nullptr };

	CGameInstance*				pGameInstance = GET_INSTANCE(CGameInstance);

	CMonster_Arrow::ARROWDESC	ArrowDesc;

	ZeroMemory(&ArrowDesc, sizeof(CWeapon::WEAPONDESC));

	ArrowDesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	ArrowDesc.pSocket = m_pModelCom->Get_BonePtr("LOCK_Bow_end");
	ArrowDesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(ArrowDesc.pSocket);
	Safe_AddRef(m_pTransformCom);

	pPartObject = pGameInstance->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Arrow"), &ArrowDesc);
	if (nullptr == pPartObject)
		return E_FAIL;

	m_ArrowParts.push_back(pPartObject);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster::Set_OnCell()
{
	ENGINE_OPEN;

	_vector		vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float		fHeight = m_pNavigationCom->Height_OnNavigation(vPos);

	m_pTransformCom->Set_Height(fHeight);

	ENGINE_CLOSE;

	return S_OK;
}

void CMonster::isDead(_float fDamge)
{
	m_pMonsterAbility->fCurrentHP -= fDamge;

	if (m_pMonsterAbility->fCurrentHP <= 0.f)
		m_pMonsterAbility->fCurrentHP = 0;
}

// 공격범위
_bool CMonster::Collision_Attack()
{
	ENGINE_OPEN;

	CCollider*			pTargetColliderTarget = (CCollider*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE_TARGET"));
	if (nullptr == pTargetColliderTarget)
		return true;

	if (m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Collision(pTargetColliderTarget))
	{
		ENGINE_CLOSE;
		return true;
	}
	else
	{
		ENGINE_CLOSE;
		return false;
	}
}

_bool CMonster::Collision_Demage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* TargetPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player")));

	if (TargetPlayer->Get_WeaponSet()->Get_CheckAble())
	{
		if (m_pCollHit[COLL_HIT]->Collision(TargetPlayer->Get_WeaponSet()))
		{
			RELEASE_INSTANCE(CGameInstance);
			return true;
		}
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
	else
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
}

// 쫒아가는 범위
_bool CMonster::Collision_ToPlayer()
{
	ENGINE_OPEN;

	CCollider*			pTargetCollider = (CCollider*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));

	if (m_pColliderCom[COLLTYPE_SPHERE]->Collision(pTargetCollider))
	{
		ENGINE_CLOSE;
		return true;
	}
	else
	{
		ENGINE_CLOSE;
		return false;
	}

}

CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CMonster*		pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg)
{
	CMonster*		pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < COLL_END; ++i)
		Safe_Release(m_pCollHit[i]);

	for (auto& pPart : m_ArrowParts)
		Safe_Release(pPart);

	m_ArrowParts.clear();

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_MonsterState);
	Safe_Release(m_pNavigationCom);

	Safe_Delete<MONSTERABILITY*>(m_pMonsterAbility);
	
}
