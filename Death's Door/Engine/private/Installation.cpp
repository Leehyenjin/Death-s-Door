#include "..\public\Installation.h"

CInstallation::CInstallation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CInstallation::CInstallation(const CInstallation & rhs)
	: CGameObject(rhs)
{
}

HRESULT CInstallation::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstallation::Initialize(void * pArg)
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

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CInstallation::Tick(_double TimeDelta)
{
	/*m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix());*/

	return __super::Tick(TimeDelta);
}

void CInstallation::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	/*if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
	}*/
}

HRESULT CInstallation::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, i);
	}

	return S_OK;
}

HRESULT CInstallation::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	ENGINE_OPEN;

	m_pShaderCom->Set_Matrix("g_WorldMatrix", &(m_pTransformCom->Get_WorldMatrix()));
	m_pShaderCom->Set_Matrix("g_ViewMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShaderCom->Set_Matrix("g_ProjMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));

	const LIGHTDESC*	pLightDesc = ENGINE->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

HRESULT CInstallation::SetUp_Component()
{
	ENGINE_OPEN;

	/*if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;*/

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com.Calculator
	if (FAILED(__super::Add_Component(ENGINE->Get_StaticLevelIndex(), TEXT("Prototype_Component_Calculator"), TEXT("Com_Calculator"),
		(CComponent**)&m_pCalculatorCom)))
		return E_FAIL;

	//// For.Com_Collider(AABB)
	//CCollider::COLLIDERDESC			ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vCenter = _float3(-1.f, ColliderDesc.vSize.y, -1.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"),
	//(CComponent**)&m_pColliderCom, &ColliderDesc)))
	//return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

void CInstallation::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pSquareCom);
	Safe_Release(m_pColliderCom);
}
