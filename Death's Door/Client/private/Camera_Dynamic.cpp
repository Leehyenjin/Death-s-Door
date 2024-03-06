#include "stdafx.h"
#include "..\public\Camera_Dynamic.h"
#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void * pArg)
{
	//CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&m_CameraDesc, sizeof CAMERADESC);

	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));
	else
	{
		m_CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
		m_CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		m_CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);

		m_CameraDesc.FOV = XMConvertToRadians(60.f);
		m_CameraDesc.Aspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
		m_CameraDesc.NearZ = 0.1f;
		m_CameraDesc.FarZ = 1000.f;

		m_CameraDesc.m_MouseMovement = 0.1f;
		 
		m_CameraDesc.TransformDesc.fSpeedPerSec = 20.f;
		m_CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}

	if (FAILED(CCamera::Initialize(&m_CameraDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_uint CCamera_Dynamic::Tick(_double TimeDelta)
{
	/*if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);*/

	ENGINE_OPEN;

	if (ENGINE->Key_Pressing(DIK_UP))
		m_pTransformCom->Go_Straight(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_DOWN))
		m_pTransformCom->Go_Backward(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_LEFT))
		m_pTransformCom->Go_Left(TimeDelta);

	if (ENGINE->Key_Pressing(DIK_RIGHT))
		m_pTransformCom->Go_Right(TimeDelta);

	if (ENGINE->Mouse_Pressing(DIM_RB))
	{
		_long	MoveX = 0, MoveY = 0;
		
		if (MoveX = ENGINE->Get_DIMouseMove(DIMS_X))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * (_float)MoveX * m_CameraDesc.m_MouseMovement);
		
		if (MoveY = ENGINE->Get_DIMouseMove(DIMS_Y))
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), (_float)TimeDelta * (_float)MoveY * m_CameraDesc.m_MouseMovement);
	}

	ENGINE_CLOSE;

	//Player_Chase(TimeDelta);


	return __super::Tick(TimeDelta);
}

void CCamera_Dynamic::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::SetUp_Components()
{
	return S_OK;
}

void CCamera_Dynamic::Player_Chase(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector		pPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);
	//_vector		pPlayerLook = pTargetPos->Get_State(CTransform::STATE_LOOK);


	// 반대방향으로  카메라 위치를 구하고, 플레이어가 보는 방향을 1로 만들어준다
	//_vector pPlayer = XMVector3Normalize(pPlayerLook);

	// 카메라 위치 * - 10 만큼 정하고
	_vector pCameraPos = pPlayerPos /*+ (pPlayer * -20.f)*/;

	_float4 vPos;
	XMStoreFloat4(&vPos, pCameraPos);

	_float4 vPlay = { vPos.x + 10.f, vPos.y  + 25.f, vPos.z + 20.f , 1.f };

	_vector V = XMLoadFloat4(&vPlay);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, V);


	m_pTransformCom->LookAt(pPlayerPos + XMVectorSet(0.f, 0.1f, 0.f, 0.f));

	ENGINE_CLOSE;
}

CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void * pArg)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();
}
