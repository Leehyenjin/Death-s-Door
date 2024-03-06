#include "stdafx.h"
#include "..\public\GrimacerMonster.h"
#include "Grimacer_State.h"
#include "Player.h"

CGrimacerMonster::CGrimacerMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CGrimacerMonster::CGrimacerMonster(const CGrimacerMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrimacerMonster::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;
			
	return S_OK;
}

HRESULT CGrimacerMonster::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 2.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(-90.f);

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_Animation(10);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(81.737f, 8.9f, 76.350f, 1.f));
	m_pTransformCom->Set_Scaling(_float3(0.6f, 0.6f, 0.6f));
	//m_pTransformCom->Set_Angle(XMConvertToRadians(0.f), XMConvertToRadians(45.f), XMConvertToRadians(0.f));

	m_pGrimacerAbility = new MONSTERABILITY;
	m_pGrimacerAbility->fMaxHp = 3.f;
	m_pGrimacerAbility->fCurrentHP = 3.f;
	m_pGrimacerAbility->fDamage = 1.f;
	m_pGrimacerAbility->fCurrentHP = m_pGrimacerAbility->fMaxHp;

	return S_OK;
}

_uint CGrimacerMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fAnimationTime);

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_ATTACK]->Update(m_pTransformCom->Get_tWorldMatrix());

	return S_OK;
}

void CGrimacerMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		if (!m_bDead)
		{

			for (auto& pCollider : m_pColliderCom)
			{
				m_pRendererCom->Add_DebugRenderGroup(pCollider);
			}

			for (auto& iter : m_pCollHit)
			{
				m_pRendererCom->Add_DebugRenderGroup(iter);
			}

			if (m_pGrimacerAbility->fCurrentHP == 0)
			{
				m_bDead = true;
			}

			if (m_bDead && m_pGrimacerAbility->fCurrentHP == 0)
			{
				Kill_Obj();
			}
		}
	}
}

HRESULT CGrimacerMonster::Render()
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

HRESULT CGrimacerMonster::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GrimacerMonster"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com.State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE(쫒아가는)
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(40.0f, 1.9f, 4.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET(공격)
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(10.0f, 2.f, 10.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE(HIT)
	ColliderDesc.vSize = _float3(8.f, 2.f, 8.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Grimacer_SPHERE_HIT"),
		(CComponent**)&m_pCollHit[COLL_HIT], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE(ATTACK)
	ColliderDesc.vSize = _float3(3.f, 5.f, 5.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Grimacer_SPHERE_ATTACK"),
		(CComponent**)&m_pCollHit[COLL_ATTACK], &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimacerMonster::SetUp_ShaderResources()
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

HRESULT CGrimacerMonster::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pGrimacerState = CGrimacer_State::Create(this);
	if (nullptr == m_pGrimacerState)
		return E_FAIL;

	return S_OK;
}	
void CGrimacerMonster::isDead(_float fDamge)
{
	m_pGrimacerAbility->fCurrentHP -= fDamge;

	if (m_pGrimacerAbility->fCurrentHP <= 0.f)
		m_pGrimacerAbility->fCurrentHP = 0;
}

// 쫒아가는 범위
_bool CGrimacerMonster::Collision_ToPlayer()
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
// 공격하는 범위
_bool CGrimacerMonster::Collision_Attack()
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

_bool CGrimacerMonster::Collision_Demage()
{
	ENGINE_OPEN;

	CPlayer*	TargetPlayer = static_cast<CPlayer*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player")));

	if (TargetPlayer->Get_WeaponSet()->Get_CheckAble())
	{
		if (m_pCollHit[COLL_HIT]->Collision(TargetPlayer->Get_WeaponSet()))
		{
			ENGINE_CLOSE;
			return true;
		}
		ENGINE_CLOSE;
		return false;
	}
	else
	{
		ENGINE_CLOSE;
		return false;
	}
}

CGrimacerMonster * CGrimacerMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CGrimacerMonster*	pInstance = new CGrimacerMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CGrimacerMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGrimacerMonster::Clone(void * pArg)
{
	CGrimacerMonster*	pInstance = new CGrimacerMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CGrimacerMonster");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CGrimacerMonster::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pGrimacerState);
	Safe_Release(m_pStateCom);

}
