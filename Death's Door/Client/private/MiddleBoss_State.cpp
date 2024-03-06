#include "stdafx.h"
#include "..\public\MiddleBoss_State.h"
#include "MiddleBoss.h"
#include "GameInstance.h"

CMiddleBoss_State::CMiddleBoss_State()
{
}

HRESULT CMiddleBoss_State::Initialize(CMiddleBoss * MiddleBoss)
{
	m_pMiddleBoss = MiddleBoss;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Chase()))
		return E_FAIL;

	if (FAILED(SetUp_State_Damaged()))
		return E_FAIL;

	/*if (FAILED(SetUp_State_Deth()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CMiddleBoss_State::SetUp_State_Idle()
{
	if (nullptr == m_pMiddleBoss->m_pStateCom)
		return E_FAIL;

	m_pMiddleBoss->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CMiddleBoss_State::Start_Idle)
		.Init_Tick(this, &CMiddleBoss_State::Tick_Idle)
		.Init_End(this, &CMiddleBoss_State::End_Common)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CMiddleBoss_State::isAttack)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CMiddleBoss_State::isChase)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMiddleBoss_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMiddleBoss_State::SetUp_State_Attack()
{
	if (nullptr == m_pMiddleBoss->m_pStateCom)
		return E_FAIL;

	m_pMiddleBoss->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CMiddleBoss_State::Start_Attack)
		.Init_Tick(this, &CMiddleBoss_State::Tick_Attack)
		.Init_End(this, &CMiddleBoss_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMiddleBoss_State::isNonAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMiddleBoss_State::isDamage)


		.Finish_Setting();

	return S_OK;
}

HRESULT CMiddleBoss_State::SetUp_State_Chase()
{
	if (nullptr == m_pMiddleBoss->m_pStateCom)
		return E_FAIL;

	m_pMiddleBoss->m_pStateCom->
		Add_State(TEXT("STATE::CHASE"))
		.Init_Start(this, &CMiddleBoss_State::Start_Chase)
		.Init_Tick(this, &CMiddleBoss_State::Tick_Chase)
		.Init_End(this, &CMiddleBoss_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMiddleBoss_State::isNoneMove)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMiddleBoss_State::isDamage)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CMiddleBoss_State::isAttack)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMiddleBoss_State::SetUp_State_Damaged()
{
	if (nullptr == m_pMiddleBoss->m_pStateCom)
		return E_FAIL;

	m_pMiddleBoss->m_pStateCom->
		Add_State(TEXT("STATE::DAMAGE"))
		.Init_Start(this, &CMiddleBoss_State::Start_Damaged)
		.Init_Tick(this, &CMiddleBoss_State::Tick_Damaged)
		.Init_End(this, &CMiddleBoss_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMiddleBoss_State::isNoneDamage)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CMiddleBoss_State::isNoneMove)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMiddleBoss_State::SetUp_State_Deth()
{
	return E_NOTIMPL;
}

void CMiddleBoss_State::Start_Idle(_double TimeDelta)
{
	m_pMiddleBoss->m_pModelCom->Set_Animation(AIM_IDLE);
}

void CMiddleBoss_State::Start_Attack(_double TimeDelta)
{
	m_pMiddleBoss->m_pModelCom->Set_Animation(AIM_ATK_SLASH_01_L);
}

void CMiddleBoss_State::Start_Chase(_double TimeDelta)
{
	m_pMiddleBoss->m_pModelCom->Set_Animation(AIM_RUN_F);
}

void CMiddleBoss_State::Start_Damaged(_double TimeDelta)
{
	m_pMiddleBoss->m_fAnimationTime = 0.3f;
	m_pMiddleBoss->m_fLerpTime = 0.2f;
	m_pMiddleBoss->m_pModelCom->Set_Animation(AIM_IA_DEATH_WAR);
}

void CMiddleBoss_State::Start_Deth(_double TimeDelta)
{
}

void CMiddleBoss_State::Tick_Idle(_double TimeDelta)
{
	
}

void CMiddleBoss_State::Tick_Attack(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pAttackPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pAttackPos->Get_tState(CTransform::STATE_TRANSLATION);

	m_pMiddleBoss->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CMiddleBoss_State::Tick_Chase(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pTargetPos->Get_tState(CTransform::STATE_TRANSLATION);

	//m_pMiddleBoss->m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f));
	m_pMiddleBoss->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.5f);
	m_pMiddleBoss->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CMiddleBoss_State::Tick_Damaged(_double TimeDelta)
{
	
}

void CMiddleBoss_State::Tick_Deth(_double TimeDelta)
{
}

void CMiddleBoss_State::End_Common(_double TimeDleta)
{
	m_pMiddleBoss->m_fLerpTime = 0.3f;
	m_pMiddleBoss->m_fAnimationTime = 1.f;
}

_bool CMiddleBoss_State::Finishing()
{
	return _bool();
}

_bool CMiddleBoss_State::isAttack()
{
	_bool	bResult = m_pMiddleBoss->Collision_Attack();

	return bResult;
}

_bool CMiddleBoss_State::isDamage()
{
	_bool	bResult = m_pMiddleBoss->Collision_Demage();

	return bResult;
}

_bool CMiddleBoss_State::isNoneMove()
{
	if (m_pMiddleBoss->Collision_ToPlayer())
	{
		m_pMiddleBoss->m_bAnimFinish = false;
	}
	else
	{
		m_pMiddleBoss->m_bAnimFinish = true;
	}

	return m_pMiddleBoss->m_bAnimFinish;
}

_bool CMiddleBoss_State::isChase()
{
	_bool	bResult = m_pMiddleBoss->Collision_ToPlayer();

	return bResult;
}

_bool CMiddleBoss_State::isNonAttack()
{
	if (m_pMiddleBoss->Collision_Attack())
	{
		m_pMiddleBoss->m_bAnimFinish = false;
	}
	else
	{
		m_pMiddleBoss->m_bAnimFinish = true;
	}

	return m_pMiddleBoss->m_bAnimFinish;
}

_bool CMiddleBoss_State::isNoneDamage()
{
	if (m_pMiddleBoss->Collision_Demage())
	{
		m_pMiddleBoss->m_bAnimFinish = false;
	}
	else
	{
		m_pMiddleBoss->m_bAnimFinish = true;
	}

	return m_pMiddleBoss->m_bAnimFinish;
}

CMiddleBoss_State * CMiddleBoss_State::Create(CMiddleBoss * MiddleBoss)
{
	CMiddleBoss_State*		pInstance = new CMiddleBoss_State();

	if (FAILED(pInstance->Initialize(MiddleBoss)))
	{
		MSG_BOX("Failled to Create : CMiddleBoss_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiddleBoss_State::Free()
{
	Safe_Release(m_pMiddleBoss);
}