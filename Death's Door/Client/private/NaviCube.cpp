#include "stdafx.h"
#include "..\public\NaviCube.h"


CNaviCube::CNaviCube(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CNaviCube::CNaviCube(const CNaviCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNaviCube::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNaviCube::Initialize(void * pArg)
{
	ZeroMemory(&m_GameObjectDesc, sizeof(GAMEOBJECTDESC));

	if (nullptr != pArg)
		memcpy(&m_GameObjectDesc, pArg, sizeof(GAMEOBJECTDESC));
	else
	{
		m_GameObjectDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(0.f);
	}

	if (FAILED(__super::Initialize(&m_GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));

	return S_OK;
}

_uint CNaviCube::Tick(_double TimeDelta)
{
	if (nullptr != m_pCalculatorCom)
		m_pCalculatorCom->Calculate_MousePos(g_hWnd);

	return __super::Tick(TimeDelta);
}

void CNaviCube::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CNaviCube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

_bool CNaviCube::isPeeking(void)
{
	return m_pCalculatorCom->Ray_CubePeeking(m_pVIBufferCom, m_pTransformCom);
}

HRESULT CNaviCube::SetUp_Components()
{
	ENGINE_OPEN;

	// For.Com_Renderer
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VertexSquare"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Square"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Calculator
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Calculator"), TEXT("Com_Calculator"),
		(CComponent**)&m_pCalculatorCom)))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CNaviCube::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	ENGINE_OPEN;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CNaviCube * CNaviCube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CNaviCube*	pInstance = new CNaviCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CNaviCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNaviCube::Clone(void * pArg)
{
	CNaviCube*	pInstance = new CNaviCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CNaviCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNaviCube::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
