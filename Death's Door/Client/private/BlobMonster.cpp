#include "stdafx.h"
#include "..\public\BlobMonster.h"
#include "BlobMonster_State.h"
#include "Player.h"

CBlobMonster::CBlobMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBlobMonster::CBlobMonster(const CBlobMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlobMonster::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlobMonster::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObejctDesc;
	ZeroMemory(&GameObejctDesc, sizeof GameObejctDesc);

	GameObejctDesc.TransformDesc.fSpeedPerSec = 2.f;
	//GameObejctDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&GameObejctDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_State()))
		return E_FAIL;

	m_pModelCom->Set_Animation(1);

	m_pTransformCom->Set_Scaling(_float3(0.7f, 0.7f, 0.7f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(77.7f, 8.9f, 36.6f, 1.f));

	m_pBlobMonsterAbility = new MONSTERABILITY;
	m_pBlobMonsterAbility->fMaxHp = 3.f;
	m_pBlobMonsterAbility->fCurrentHP = 3.f;
	m_pBlobMonsterAbility->fDamage = 1.f;
	m_pBlobMonsterAbility->fCurrentHP = m_pBlobMonsterAbility->fMaxHp;

	return S_OK;
}

_uint CBlobMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta, m_fLerpTime, m_fAnimationTime);

	m_pColliderCom[COLLTYPE_SPHERE]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Update(m_pTransformCom->Get_tWorldMatrix());
	m_pCollHit[COLL_HIT]->Update(m_pTransformCom->Get_tWorldMatrix());

	return S_OK;
}

void CBlobMonster::Late_Tick(_double TimeDelta)
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
			for (auto& iter : m_pCollHit)
			{
				m_pRendererCom->Add_DebugRenderGroup(iter);
			}

			if (m_pBlobMonsterAbility->fCurrentHP == 0)
				m_bDead = true;

			if (m_bDead && m_pBlobMonsterAbility->fCurrentHP == 0)
				Kill_Obj();
		}
	}
}

HRESULT CBlobMonster::Render()
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

HRESULT CBlobMonster::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BlobMonster"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com.State
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"), TEXT("Com_State"),
		(CComponent**)&m_pStateCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;

	// For.Com_SPHERE
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(20.f, 1.f, 20.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE_TARGET
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(5.f, 1.f, 5.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vCenter.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE_TARGET"), TEXT("Com_SPHERE_TARGET"),
		(CComponent**)&m_pColliderCom[COLLTYPE_SPHERE_TARGET], &ColliderDesc)))
		return E_FAIL;

	// For.Com_SPHERE(HIT)
	ColliderDesc.vSize = _float3(3.f, 1.f, 3.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y * 1.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Monster_SPHEREHIT"),
		(CComponent**)&m_pCollHit[COLL_HIT], &ColliderDesc)))
		return E_FAIL;
		
	return S_OK;
}

HRESULT CBlobMonster::SetUp_ShaderResources()
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

HRESULT CBlobMonster::SetUp_State()
{
	if (nullptr == m_pStateCom)
		return E_FAIL;

	m_pBlobState = CBlobMonster_State::Create(this);
	if (nullptr == m_pBlobState)
		return E_FAIL;

	return S_OK;
}

void CBlobMonster::isDead(_float fDamge)
{
	m_pBlobMonsterAbility->fCurrentHP -= fDamge;

	if (m_pBlobMonsterAbility->fCurrentHP <= 0.f)
		m_pBlobMonsterAbility->fCurrentHP = 0;
}

_bool CBlobMonster::Collision_ToPlayer()
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

_bool CBlobMonster::Collision_Attack()
{
	ENGINE_OPEN;

	CCollider*			pAttackCollider = (CCollider*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE_TARGET"));
	if (nullptr == pAttackCollider)
		return E_FAIL;

	if (m_pColliderCom[COLLTYPE_SPHERE_TARGET]->Collision(pAttackCollider))
	{
		ENGINE_CLOSE;
		return true;
	}
	else
	{
		ENGINE_CLOSE;
		return false;
	}

	ENGINE_CLOSE;
}

_bool CBlobMonster::Collision_Demage()
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

CBlobMonster * CBlobMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CBlobMonster*	pInstance = new CBlobMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CBlobMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlobMonster::Clone(void * pArg)
{
	CBlobMonster*	pInstance = new CBlobMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CBlobMonster");
		Safe_Release(pInstance);
	} 

	return pInstance;
}

void CBlobMonster::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (_uint i = 0; i < COLL_END; ++i)
		Safe_Release(m_pCollHit[i]);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateCom);
	//Safe_Release(m_pBlobMonsterAbility);
}
