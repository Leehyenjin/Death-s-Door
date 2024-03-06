#include "stdafx.h"
#include "..\public\Door.h"

CDoor::CDoor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CDoor::CDoor(const CDoor & rhs)
	: CInstallation(rhs)
{
}

HRESULT CDoor::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

_uint CDoor::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CDoor::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CDoor::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Door"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CDoor * CDoor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CDoor*	pInstance = new CDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CDoor::Clone(void * pArg)
{
	CDoor*	pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
