#include "stdafx.h"
#include "..\public\HpBar.h"

CHpBar::CHpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CHpBar::CHpBar(const CHpBar & rhs)
	: CUI(rhs)
{
}

HRESULT CHpBar::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID, eUIType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHpBar::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(CUI::UI_DESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(CUI::UI_DESC));
	else
	{
		ZeroMemory(&m_UIDesc, sizeof(CUI::UI_DESC));
		m_UIDesc.UI_Size = _float4((_float)300, (_float)30, 0.01f, 1.f);
		m_UIDesc.UI_Position = _float4(-240.f, 265.f, 100.f, 1.f);
		m_UIDesc.TransformDesc.fSpeedPerSec = 0.f;
		m_UIDesc.TransformDesc.fRotationPerSec = 0.f;
	}


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CHpBar::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

void CHpBar::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_float	fUILength = 4.f;
	m_pShaderCom->Set_RawValue("g_fUILength", &fUILength, sizeof(_float));

	m_pShaderCom->Begin(0);

	m_pBufferCom->Render();
	
	return S_OK;
}

HRESULT CHpBar::SetUp_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Shader_UIHpBar"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HpBar"), TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

CHpBar * CHpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID, CUI::UITYPE eUIType)
{
	CHpBar*		pInstance = new CHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID, eUIType)))
	{
		MSG_BOX("Failled to Create : CHpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHpBar::Clone(void * pArg)
{
	CHpBar*		pInstance = new CHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failled to Clone : CHpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHpBar::Free()
{
	__super::Free();
}
