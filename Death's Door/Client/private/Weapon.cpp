#include "stdafx.h"
#include "..\public\Weapon.h"
#include "GameInstance.h"
#include "Bone.h"

CWeapon::CWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon & rhs)	
	: CGameObject(rhs)
{
}

HRESULT CWeapon::Initialize_Prototype(OBJECT_ID eID)
{
	if (FAILED(__super::Initialize_Prototype(eID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_WeaponDesc, pArg, sizeof(m_WeaponDesc));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	/*m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.9f, 0.603f, -0.4f, 1.f));
	m_pTransformCom->Set_Angle(XMConvertToRadians(-90.799f), XMConvertToRadians(-40.569f), XMConvertToRadians(-163.887f));*/
	m_pTransformCom->Set_Scaling(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

_uint CWeapon::Tick(_double TimeDelta)
{
	_matrix			SocketMatrix = m_WeaponDesc.pSocket->Get_OffsetMatrix() * m_WeaponDesc.pSocket->Get_CombindMatrix() * XMLoadFloat4x4(&m_WeaponDesc.PivotMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	SocketMatrix = SocketMatrix * m_WeaponDesc.pTargetTransform->Get_tWorldMatrix();

	XMStoreFloat4x4(&m_SocketMatrix, SocketMatrix);

	//m_pColliderCom/*[COLL_L]*/->Update(m_pTransformCom->Get_tWorldMatrix() * SocketMatrix);
	//m_pColliderCom->Update(m_pTransformCom->Get_tWorldMatrix() * SocketMatrix);
	/*m_pColliderCom[COLL_R]->Update(m_pTransformCom->Get_tWorldMatrix() * SocketMatrix);
	m_pColliderCom[COLL_BADY]->Update(m_pTransformCom->Get_tWorldMatrix() * SocketMatrix);*/

	return __super::Tick(TimeDelta);
}

void CWeapon::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		/*for (auto& pCollWeapon : m_pColliderCom)
			m_pRendererCom->Add_DebugRenderGroup(pCollWeapon);*/

		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom);

	}
}

HRESULT CWeapon::Render()
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

	return S_OK;
}

HRESULT CWeapon::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sword"), TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For.Com_Model
	/*if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bow"), TEXT("Com_Model_Bow"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;*/

	// For.Com_Collider_R
	//CCollider::COLLIDERDESC			ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vCenter = _float3(-1.f, ColliderDesc.vSize.y, -1.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Collider_R"),
	//	(CComponent**)&m_pColliderCom/*[COLL_R]*/, &ColliderDesc)))
	//	return E_FAIL;

	//// For.Com_Collider_L
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vSize.y, 0.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Collider_L"),
	//	(CComponent**)&m_pColliderCom[COLL_L], &ColliderDesc)))
	//	return E_FAIL;

	//// For.Com_Collider_Bady
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vCenter = _float3(1.f, ColliderDesc.vSize.y, 1.f);

	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Collider_Bady"),
	//	(CComponent**)&m_pColliderCom[COLL_BADY], &ColliderDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_SocketMatrix", &m_SocketMatrix)))
		return E_FAIL;

	// For.Light
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CWeapon * CWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, OBJECT_ID eID)
{
	CWeapon*		pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eID)))
	{
		MSG_BOX("Failed to Create : CWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon*		pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
	__super::Free();

	if (true == m_isClone)
	{
		Safe_Release(m_WeaponDesc.pSocket);
		Safe_Release(m_WeaponDesc.pTargetTransform);
	}

	/*for (_uint i = 0; i < COLL_END; ++i)
		Safe_Release(m_pColliderCom[i]);*/

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
}
