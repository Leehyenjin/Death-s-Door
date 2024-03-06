#include "stdafx.h"
#include "..\public\CubeBox.h"
#include "Player.h"
#include "Fance.h"

CCubeBox::CCubeBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CInstallation(pDevice, pContext)
{
}

CCubeBox::CCubeBox(const CCubeBox & rhs)
	: CInstallation(rhs)
{
}

HRESULT CCubeBox::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeBox::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;
	
	m_pInteractable = new INTERACTABLE;
	m_pInteractable->fCurrentHP = 1.f;
	m_pInteractable->fMaxHP = 1.f;
	m_pInteractable->fCurrentHP = m_pInteractable->fMaxHP;

	return S_OK;
}

_uint CCubeBox::Tick(_double TimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix());

	return __super::Tick(TimeDelta);
}

void CCubeBox::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CCubeBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeBox::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
	(CComponent**)&m_pRendererCom)))
	return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_CubeBox"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com_Collider(AABB)
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vSize = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"),
		(CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CCubeBox::Player_Damaged(_float fDamage)
{
	m_pInteractable->fCurrentHP -= fDamage;
	if (m_pInteractable->fCurrentHP <= 0.f)
		m_pInteractable->fCurrentHP = 0.f;

	return S_OK;
}

_bool CCubeBox::Collision_Attack()
{
	ENGINE_OPEN;

	CPlayer* TargetPlayer = static_cast<CPlayer*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));
	if (TargetPlayer->Get_WeaponSet()->Get_CheckAble())
	{
		if (m_pColliderCom->Collision(TargetPlayer->Get_WeaponSet()))
		{
			ENGINE_CLOSE;
			Player_Damaged(1.f);
			return true;
		}

		ENGINE_CLOSE;
		return false;
	}

	/*CFance*		FancePos = static_cast<CFance*>(ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Installation"), TEXT("Prototype_GamObject_Installation")));
	if (m_bDead)
	{
		FancePos->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, _float3())
	}*/
}

CCubeBox * CCubeBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CCubeBox*	pInstance = new CCubeBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CCubeBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CCubeBox::Clone(void * pArg)
{
	CCubeBox*	pInstance = new CCubeBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CCubeBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeBox::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Delete<INTERACTABLE*>(m_pInteractable);
}
