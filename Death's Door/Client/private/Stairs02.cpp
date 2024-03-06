#include "stdafx.h"
#include "..\public\Stairs02.h"

CStairs02::CStairs02(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInstallation(pDevice, pContext)
{
}

CStairs02::CStairs02(const CStairs02 & rhs)
	: CInstallation(rhs)
{
}

HRESULT CStairs02::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStairs02::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Model()))
		return E_FAIL;

	return S_OK;
}

_uint CStairs02::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CStairs02::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}
}

HRESULT CStairs02::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStairs02::SetUp_Model()
{
	ENGINE_OPEN;

	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Model_Stairs02"), TEXT("Buffer_Component"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CStairs02 * CStairs02::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CStairs02*	pInstance = new CStairs02(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failled to Create : CStairs02");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStairs02::Clone(void * pArg)
{
	CStairs02*	pInstance = new CStairs02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CStairs02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStairs02::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
