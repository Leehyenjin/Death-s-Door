#include "stdafx.h"
#include "..\public\StageMap.h"

CStageMap::CStageMap(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CStageMap::CStageMap(const CStageMap & rhs)
	: CInstallation(rhs)
{
}

HRESULT CStageMap::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageMap::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

_uint CStageMap::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CStageMap::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CStageMap::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageMap::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_StageMap"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CStageMap * CStageMap::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CStageMap*	pInstance = new CStageMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CStageMap::Clone(void * pArg)
{
	CStageMap*	pInstance = new CStageMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CStageMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageMap::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
