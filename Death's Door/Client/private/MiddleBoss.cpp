#include "stdafx.h"
#include "..\public\MiddleBoss.h"
#include "MiddleBoss_State.h"
#include "Player.h"
#include "Bone.h"

CMiddleBoss::CMiddleBoss(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMiddleBoss::CMiddleBoss(const CMiddleBoss & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMiddleBoss::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiddleBoss::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 4.f;
	/*GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);*/

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;
	
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_Animation(5);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(26.7f, 8.9f, 18.9f, 1.f));
	m_pTransformCom->Set_Angle(XMConvertToRadians(0.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f));
	m_pTransformCom->Set_Scaling(_float3(0.03f, 0.03f, 0.03f));

	return S_OK;
}

_uint CMiddleBoss::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	return S_OK;
}

void CMiddleBoss::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		for (auto& pCollider : m_pColliderCom)
		{
			m_pRendererCom->Add_DebugRenderGroup(pCollider);
		}

		for (auto & pColliderHit : m_pCollHit)
		{
			m_pRendererCom->Add_DebugRenderGroup(pColliderHit);
		}
	}
	Set_ColliderHit();
}

HRESULT CMiddleBoss::Render()
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

HRESULT CMiddleBoss::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MiddleBoss"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com.State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1000.0f, 200.f, 1000.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(500.0f, 100.f, 500.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE(HIT)
	ColliderDesc.vSize = _float3(300.f, 100.f, 300.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_MiddleBoss_SPHEREHIT"),
	(CComponent**)&m_pCollHit[COLL_HIT], &ColliderDesc)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CMiddleBoss::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	ENGINE_OPEN;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	// For.Light
	const LIGHTDESC* pLightDesc = ENGINE->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CMiddleBoss::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pMiddleBossState = CMiddleBoss_State::Create(this);
	if (nullptr == m_pMiddleBossState)
		return E_FAIL;

	return S_OK;
}

void CMiddleBoss::Set_ColliderHit() //Bone_Tidehunter_Hand_L			Bone_Tidehunter_Hand_R
{
	_matrix		m_pCollRight =
		m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_R")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_R")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();

	_matrix		m_pCollLeft =
		m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_L")->Get_OffsetMatrix()
		* m_pModelCom->Get_BonePtr("Bone_Tidehunter_Hand_L")->Get_CombindMatrix()
		* m_pModelCom->Get_PivotMatrix();

	m_pCollRight.r[0] = XMVector3Normalize(m_pCollRight.r[0]);
	m_pCollRight.r[1] = XMVector3Normalize(m_pCollRight.r[1]);
	m_pCollRight.r[2] = XMVector3Normalize(m_pCollRight.r[2]);

	m_pCollLeft.r[0] = XMVector3Normalize(m_pCollLeft.r[0]);
	m_pCollLeft.r[1] = XMVector3Normalize(m_pCollLeft.r[1]);
	m_pCollLeft.r[2] = XMVector3Normalize(m_pCollLeft.r[2]);

	m_pCollRight = m_pCollRight * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
	//m_pCollLeft = m_pCollLeft * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());

	m_pCollHit[COLL_HIT]->Update(m_pCollRight);
	//m_pCollHit[COLL_RANGE]->Update(m_pCollLeft);

}

_bool CMiddleBoss::Collision_ToPlayer()
{
	ENGINE_OPEN;

	CCollider*		pTargetCollider = (CCollider*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));

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

_bool CMiddleBoss::Collision_Attack()
{
	ENGINE_OPEN;

	CCollider*		pTargetCollider = (CCollider*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE_TARGET"));
	if (nullptr == pTargetCollider)
		return true;

	if (m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Collision(pTargetCollider))
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

_bool CMiddleBoss::Collision_Demage()
{
	ENGINE_OPEN;

	CPlayer* TargetPlayer = static_cast<CPlayer*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player")));

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

CMiddleBoss * CMiddleBoss::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CMiddleBoss*		pInstance = new CMiddleBoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CMiddleBoss");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiddleBoss::Clone(void * pArg)
{
	CMiddleBoss*		pInstance = new CMiddleBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMiddleBoss");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiddleBoss::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < COLL_END; ++i)
		Safe_Release(m_pCollHit[i]);

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMiddleBossState);
}
