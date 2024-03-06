#include "stdafx.h"
#include "..\public\Pillar.h"

CPillar::CPillar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CPillar::CPillar(const CPillar & rhs)
	: CInstallation(rhs)
{
}

HRESULT CPillar::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPillar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	return S_OK;
}

_uint CPillar::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CPillar::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CPillar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPillar::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Pillar"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CPillar * CPillar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CPillar*	pInstance = new CPillar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CPillar");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPillar::Clone(void * pArg)
{
	CPillar*	pInstance = new CPillar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CPillar");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CPillar::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
