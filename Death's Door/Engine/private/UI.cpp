#include "..\public\UI.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI::Initialize_Prototype(OBJECT_ID eID, _uint eUIType)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	m_eType = (UITYPE)eUIType;

	return S_OK;
}

HRESULT CUI::Initialize(void * pArg)
{
	ZeroMemory(&m_UIDesc, sizeof(UI_DESC));

	if (nullptr != pArg)
		memcpy(&m_UIDesc, pArg, sizeof(UI_DESC));

	if (FAILED(__super::Initialize(&m_UIDesc)))
		return E_FAIL;

	if (FAILED(Setup_ShaderResources()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(m_UIDesc.UI_Size.x, m_UIDesc.UI_Size.y, m_UIDesc.UI_Size.z));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		XMVectorSet(m_UIDesc.UI_Position.x, m_UIDesc.UI_Position.y, m_UIDesc.UI_Position.z, m_UIDesc.UI_Position.w));

	return S_OK;
}

_uint CUI::Tick(_double TimeDelta)
{
	if (FAILED(Setup_ShaderResources()))
		return 0;

	return __super::Tick(TimeDelta);
}

void CUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Setup_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)WINCX, (_float)WINCY, 0.f, 200.f));

	m_pShaderCom->Set_Matrix("g_matWorld", &(m_pTransformCom->Get_WorldMatrix()));
	m_pShaderCom->Set_Matrix("g_matView", &m_matView);
	m_pShaderCom->Set_Matrix("g_matProj", &m_matProj);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture");

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
}
