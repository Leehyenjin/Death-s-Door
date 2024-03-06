#include "stdafx.h"
#include "..\public\Floor.h"

CFloor::CFloor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CFloor::CFloor(const CFloor & rhs)
	: CInstallation(rhs)
{
}

HRESULT CFloor::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	return S_OK;
}

_uint CFloor::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CFloor::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CFloor::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Floor"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CFloor * CFloor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CFloor*	pInstance = new CFloor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CFloor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFloor::Clone(void * pArg)
{
	CFloor*	pInstance = new CFloor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloor::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
