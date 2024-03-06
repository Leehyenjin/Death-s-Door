#include "stdafx.h"
#include "..\public\Sword_Slash.h"
#include "GameInstance.h"
#include "Bone.h"

CSword_Slash::CSword_Slash(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSword_Slash::CSword_Slash(const CSword_Slash & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSword_Slash::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword_Slash::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_SlashDesc, pArg, sizeof(m_SlashDesc));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_uint CSword_Slash::Tick(_double TimeDelta)
{
	_matrix		SocketMatrix = m_SlashDesc.pSocket->Get_OffsetMatrix() * m_SlashDesc.pSocket->Get_CombindMatrix() * XMLoadFloat4x4(&m_SlashDesc.PivotMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	SocketMatrix = SocketMatrix * m_SlashDesc.pTargetTransform->Get_tWorldMatrix();

	XMStoreFloat4x4(&m_SocketMatrix, SocketMatrix);

	m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix() * SocketMatrix);

	return __super::Tick(TimeDelta);
}

void CSword_Slash::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_isRender == true)
	{
		if (nullptr != m_pRendererCom)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

			m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);
		}
	}
}

HRESULT CSword_Slash::Render()
{
	if (m_isRender == true)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

			m_pModelCom->Render(m_pShaderCom, i, nullptr, 1);
		}
	}
	return S_OK;
}

HRESULT CSword_Slash::SetUp_Components()
{
	// For.Com_Renderer
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sword_Slash"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vCenter = _float3(-1.f, ColliderDesc.vSize.y, -1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Collider"),
		(CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword_Slash::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	ENGINE_OPEN;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &ENGINE->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_SocketMatrix", &m_SocketMatrix)))
		return E_FAIL;

	// For.Light
	const LIGHTDESC* pLightDesc = ENGINE->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	ENGINE_CLOSE;

	return S_OK;
}

CSword_Slash * CSword_Slash::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CSword_Slash*		pInstance = new CSword_Slash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CSword_Slash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSword_Slash::Clone(void * pArg)
{
	CSword_Slash*		pInstance = new CSword_Slash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSword_Slash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSword_Slash::Free()
{
	__super::Free();

	if (true == m_isClone)
	{
		Safe_Release(m_SlashDesc.pSocket);
		Safe_Release(m_SlashDesc.pTargetTransform);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
}
