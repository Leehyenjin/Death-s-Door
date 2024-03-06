#include "stdafx.h"
#include "..\public\Grimacer_State.h"
#include "GrimacerMonster.h"
#include "GameInstance.h"

CGrimacer_State::CGrimacer_State()
{
}

HRESULT CGrimacer_State::Initialize(CGrimacerMonster * GrimacerMonster)
{
	m_pGrimacer = GrimacerMonster;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	/*if (FAILED(SetUp_State_Walk()))
		return E_FAIL;*/

	if (FAILED(SetUp_State_Chase()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Damaged()))
		return E_FAIL;

	if (FAILED(SetUp_State_Deth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Idle()
{
	if (nullptr == m_pGrimacer->m_pStateCom)
		return E_FAIL;

	m_pGrimacer->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CGrimacer_State::Start_Idle)
		.Init_Tick(this, &CGrimacer_State::Tick_Idle)
		.Init_End(this, &CGrimacer_State::End_Common)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CGrimacer_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CGrimacer_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CGrimacer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Walk()
{
	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Chase()
{
	if (nullptr == m_pGrimacer->m_pStateCom)
		return E_FAIL;

	m_pGrimacer->m_pStateCom->
		Add_State(TEXT("STATE::CHASE"))
		.Init_Start(this, &CGrimacer_State::Start_Chase)
		.Init_Tick(this, &CGrimacer_State::Tick_Chase)
		.Init_End(this, &CGrimacer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrimacer_State::isNoneMove)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CGrimacer_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CGrimacer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Attack()
{
	if (nullptr == m_pGrimacer->m_pStateCom)
		return E_FAIL;

	m_pGrimacer->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CGrimacer_State::Start_Attack)
		.Init_Tick(this, &CGrimacer_State::Tick_Attack)
		.Init_End(this, &CGrimacer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrimacer_State::isNonAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CGrimacer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Damaged()
{
	if (nullptr == m_pGrimacer->m_pStateCom)
		return E_FAIL;

	m_pGrimacer->m_pStateCom->
		Add_State(TEXT("STATE::DAMAGE"))
		.Init_Start(this, &CGrimacer_State::Start_Damaged)
		.Init_Tick(this, &CGrimacer_State::Tick_Damaged)
		.Init_End(this, &CGrimacer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CGrimacer_State::isNoneDamage)
		//.Init_Changer(TEXT("STATE::CHASE"), this, &CGrimacer_State::isNoneMove)
		.Init_Changer(TEXT("STATE::DEATH"), this, &CGrimacer_State::isDeath)

		.Finish_Setting();

	return S_OK;
}

HRESULT CGrimacer_State::SetUp_State_Deth()
{
	if (nullptr == m_pGrimacer->m_pStateCom)
		return E_FAIL;

	m_pGrimacer->m_pStateCom->
		Add_State(TEXT("STATE::DETH"))
		.Init_Start(this, &CGrimacer_State::Start_Deth)
		.Init_Tick(this, &CGrimacer_State::Tick_Deth)
		.Init_End(this, &CGrimacer_State::End_Common)

		.Finish_Setting();

	return S_OK;
}

void CGrimacer_State::Start_Idle(_double TimeDelta)
{
	m_pGrimacer->m_pModelCom->Set_Animation(AIM_IDLE);
}

void CGrimacer_State::Start_Walk(_double TimeDelta)
{
	/*m_pGrimacer->m_pModelCom->Set_Animation()*/
}

void CGrimacer_State::Start_Chase(_double TimeDelta)
{
	m_pGrimacer->m_pModelCom->Set_Animation(AIM_RUN);
}

void CGrimacer_State::Start_Attack(_double TimeDelta)
{
	m_pGrimacer->m_pModelCom->Set_Animation(AIM_DASH);
}

void CGrimacer_State::Start_Damaged(_double TimeDelta)
{
	//m_pGrimacer->m_fAnimationTime = 0.1f;
	m_pGrimacer->m_pModelCom->Set_Animation(AIM_DEFEND_IMPACKT);
}

void CGrimacer_State::Start_Deth(_double TimeDelta)
{
	m_pGrimacer->m_pModelCom->Set_Animation(AIM_DEAD);
}

void CGrimacer_State::Tick_Idle(_double TimeDelta)
{
}

void CGrimacer_State::Tick_Walk(_double TimeDelta)
{
}

void CGrimacer_State::Tick_Chase(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pGrimacer->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 1.f);
	m_pGrimacer->m_pTransformCom->LookAt(vPlayerPos/*, TimeDelta * 0.5f*/);

	ENGINE_CLOSE;
}

void CGrimacer_State::Tick_Attack(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pAttackPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pAttackPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pGrimacer->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CGrimacer_State::Tick_Damaged(_double TimeDelta)
{
	/*ENGINE_OPEN;

	CTransform*		pDamage = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pDamage->Get_State(CTransform::STATE_END);

	ENGINE_CLOSE;*/
}

void CGrimacer_State::Tick_Deth(_double TimeDelta)
{
}

void CGrimacer_State::End_Common(_double TimeDleta)
{
	m_pGrimacer->m_fLerpTime = 0.3f;
	m_pGrimacer->m_fAnimationTime = 1.f;

	m_pGrimacer->Get_ColliderAttack()->Set_CheckAble(false);
}

_bool CGrimacer_State::Finishing()
{
	return _bool();
}

_bool CGrimacer_State::isAttack()
{
	_bool	bResult = m_pGrimacer->Collision_Attack();

	return bResult;
}

_bool CGrimacer_State::isChase()
{
	_bool	bResult = m_pGrimacer->Collision_ToPlayer();

	//m_pGrimacer->Get_ColliderAttack()->Set_CheckAble(true);

	return bResult;
}

_bool CGrimacer_State::isDamage()
{
	_bool	bResult = m_pGrimacer->Collision_Demage();

	return bResult;
}

_bool CGrimacer_State::isNoneMove()
{
	if (m_pGrimacer->Collision_ToPlayer())
	{
		m_pGrimacer->m_bAnimFinish = false;
	}
	else
	{
		m_pGrimacer->m_bAnimFinish = true;
	}

	return m_pGrimacer->m_bAnimFinish;
}

_bool CGrimacer_State::isNonAttack()
{
	if (m_pGrimacer->Collision_Attack())
	{
		m_pGrimacer->m_bAnimFinish = false;
	}
	else
	{
		m_pGrimacer->m_bAnimFinish = true;
	}

	return m_pGrimacer->m_bAnimFinish;
}

_bool CGrimacer_State::isNoneDamage()
{
	if (m_pGrimacer->Collision_Demage())
	{
		m_pGrimacer->m_bAnimFinish = false;
	}
	else
	{
		m_pGrimacer->m_bAnimFinish = true;
	}

	return m_pGrimacer->m_bAnimFinish;
}

_bool CGrimacer_State::isDeath()
{
	m_pGrimacer->isDead(m_fDamage);

	return S_OK;
}

CGrimacer_State * CGrimacer_State::Create(CGrimacerMonster * GrimacerMonster)
{
	CGrimacer_State*		pInstance = new CGrimacer_State();

	if (FAILED(pInstance->Initialize(GrimacerMonster)))
	{
		MSG_BOX("Failed to Create : CGrimacer_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrimacer_State::Free()
{
	Safe_Release(m_pGrimacer);
	//Safe_Release(m_pPlayer);
	//Safe_Release(m_pTransform);
}
