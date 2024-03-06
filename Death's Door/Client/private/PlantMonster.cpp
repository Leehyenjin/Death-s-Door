#include "stdafx.h"
#include "..\public\PlantMonster.h"
#include "Plant_State.h"
#include "Player.h"

CPlantMonster::CPlantMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlantMonster::CPlantMonster(const CPlantMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlantMonster::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlantMonster::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 4.0f;
	//GameObjectDesc.TransformDesc.fRotationPerSec 

	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	m_pTransformCom->Set_Scaling(_float3(0.5f, 0.5f, 0.5f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(116.f, 0.15f, 69.f, 1.f));

	m_pPlantAbility = new MONSTERABILITY;
	m_pPlantAbility->fMaxHp = 3.f;
	m_pPlantAbility->fCurrentHP = 3.f;
	m_pPlantAbility->fDamage = 1.f;
	m_pPlantAbility->fCurrentHP = m_pPlantAbility->fMaxHp;

	return S_OK;
}

_uint CPlantMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());

	return S_OK;
}

void CPlantMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		for (auto& pCollider : m_pColliderCom)
		{
			m_pRendererCom->Add_DebugRenderGroup(pCollider);
		}
	}
}

HRESULT CPlantMonster::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, "g_BoneMatrices");
	}
	
	return S_OK;
}

HRESULT CPlantMonster::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PlantMonster"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com.State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(5.0f, 0.f, 5.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(10.0f, 0.01f, 10.0f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y /** 1.f*/, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlantMonster::SetUp_ShaderResources()
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

HRESULT CPlantMonster::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_PlantState = CPlant_State::Create(this);
	if (nullptr == m_PlantState)
		return E_FAIL;

	return S_OK;
}

void CPlantMonster::isDead(_float fDamge)
{
	m_pPlantAbility->fCurrentHP -= fDamge;

	if (m_pPlantAbility->fCurrentHP <= 0.f)
		m_pPlantAbility->fCurrentHP = 0;
}

_bool CPlantMonster::Collision_Attack()
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

_bool CPlantMonster::Collision_Damage()
{
	ENGINE_OPEN;

	CPlayer*	TargetPlayer = static_cast<CPlayer*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player")));

	if (TargetPlayer->Get_WeaponSet()->Get_CheckAble())
	{
		if (m_pColliderCom[COLLTYPE_SPHERE]->Collision(TargetPlayer->Get_WeaponSet()))
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

CPlantMonster * CPlantMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CPlantMonster*		pInstance = new CPlantMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Created : CPlantMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlantMonster::Clone(void * pArg)
{
	CPlantMonster*		pInstance = new CPlantMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlantMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlantMonster::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pStateCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_PlantState);
}
