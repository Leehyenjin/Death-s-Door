#include "stdafx.h"
#include "..\public\SkillBoxUp.h"

CSkillBoxUp::CSkillBoxUp(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CSkillBoxUp::CSkillBoxUp(const CSkillBoxUp & rhs)
	: CUI(rhs)
{
}

HRESULT CSkillBoxUp::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillBoxUp::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UI_DESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UI_DESC));
	else
	{
		m_UIDesc.UI_Size = _float4((_float)30, (_float)30, 0.01f, 1.f);
		m_UIDesc.UI_Position = _float4(-345.f, 235.f, 99.f, 1.f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CSkillBoxUp::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CSkillBoxUp::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CSkillBoxUp::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CSkillBoxUp::SetUp_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_SkillBoxUp"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkillBoxUp"), TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CSkillBoxUp * CSkillBoxUp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, _uint eUIType)
{
	CSkillBoxUp*		pInstance = new CSkillBoxUp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eUIType)))
	{
		MSG_BOX("Failled to Create : CSkillBoxUp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkillBoxUp::Clone(void * pArg)
{
	CSkillBoxUp*		pInstance = new CSkillBoxUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CSkillBoxUp");
		Safe_Release(pInstance);
	}
	return pInstance;			
}

void CSkillBoxUp::Free()
{
	__super::Free();
}
