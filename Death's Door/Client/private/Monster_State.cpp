#include "stdafx.h"
#include "..\public\Monster_State.h"
#include "Monster.h"
#include "GameInstance.h"


CMonster_State::CMonster_State()
{
}

HRESULT CMonster_State::Initialize(CMonster * Monster)
{
	m_pMonster = Monster;

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

HRESULT CMonster_State::SetUp_State_Idle()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;

	m_pMonster->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CMonster_State::Start_Idle)
		.Init_Tick(this, &CMonster_State::Tick_Idle)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CMonster_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CMonster_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMonster_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Walk()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;
	
	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::WALK"))
		.Init_Start(this, &CMonster_State::Start_Walk)
		.Init_Tick(this, &CMonster_State::Tick_Walk)
		.Init_End(this, &CMonster_State::End_Common)
		//.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isNonAim)
		//.Init_Changer(TEXT("STATE::ATTACK"), this, &CMonster_State::isAttack)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Chase()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;

	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::CHASE"))
		.Init_Start(this, &CMonster_State::Start_Chase)
		.Init_Tick(this, &CMonster_State::Tick_Chase)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isNoneMove)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CMonster_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMonster_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Attack()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;

	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CMonster_State::Start_Attack)
		.Init_Tick(this, &CMonster_State::Tick_Attack)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isNonAttack)
		//.Init_Changer(TEXT("STATE::WALK"), this, &CMonster_State::isWalking)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CMonster_State::isDamage)

		.Finish_Setting();


	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Damaged()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;

	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::DAMAGE"))
		.Init_Start(this, &CMonster_State::Start_Damaged)
		.Init_Tick(this, &CMonster_State::Tick_Damaged)
		.Init_End(this, &CMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CMonster_State::isNoneDamage)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CMonster_State::isNoneMove)
		.Init_Changer(TEXT("STATE::DEATH"), this, &CMonster_State::isDeath)
		
		.Finish_Setting();

	return S_OK;
}

HRESULT CMonster_State::SetUp_State_Deth()
{
	if (nullptr == m_pMonster->m_pStateCom)
		return E_FAIL;

	m_pMonster->m_pStateCom->
		Add_State(TEXT("STATE::DEATH"))
		.Init_Start(this, &CMonster_State::Start_Deth)
		.Init_Tick(this, &CMonster_State::Tick_Deth)
		.Init_End(this, &CMonster_State::End_Common)

		.Finish_Setting();
	return S_OK;
}

void CMonster_State::Start_Idle(_double TimeDelta)
{
	m_pMonster->m_pModelCom->Set_Animation(AIM_IDLE_BOW);
}

void CMonster_State::Start_Walk(_double TimeDelta)
{
	m_pMonster->m_pModelCom->Set_Animation(AIM_WALK);
}

void CMonster_State::Start_Chase(_double TimeDelta)
{
	m_pMonster->m_fAnimationTime = 0.1f;
	m_pMonster->m_pModelCom->Set_Animation(AIM_RUN_BOW);
}

void CMonster_State::Start_Attack(_double TimeDelta)
{
	m_pMonster->m_fAnimationTime = 0.1f;
	m_pMonster->m_pModelCom->Set_Animation(AIM_SHOOT_BOW);
}

void CMonster_State::Start_Damaged(_double TimeDelta)
{
	m_pMonster->m_fAnimationTime = 0.1f;
	m_pMonster->m_fLerpTime = 0.3f;
	m_pMonster->m_pModelCom->Set_Animation(AIM_HIT_BOW);
}

void CMonster_State::Start_Deth(_double TimeDelta)
{
	/*m_pMonster->m_pModelCom->Set_Animation(AIM_HIT_BOW);*/
}

void CMonster_State::Tick_Idle(_double TimeDelta)
{
}

void CMonster_State::Tick_Walk(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform* pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector		vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pMonster->m_pTransformCom->Chase(vPlayerPos, TimeDelta);
	m_pMonster->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CMonster_State::Tick_Chase(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pMonster->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.2f);
	m_pMonster->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CMonster_State::Tick_Attack(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pAttackPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pAttackPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pMonster->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CMonster_State::Tick_Damaged(_double TimeDelta)
{
}

void CMonster_State::Tick_Deth(_double TimeDelta)
{

}

void CMonster_State::End_Common(_double TimeDleta)
{
	m_pMonster->m_fLerpTime = 0.3f;
	m_pMonster->m_fAnimationTime = 1.f;
}

_bool CMonster_State::Finishing(void)
{
	//_bool bResult = m_pMonster->m_pModelCom->Get_isFinish();
	//m_pMonster->m_pModelCom->Reset_isFinish();
	////m_pMonster->m_pModelCom->isFinish();
	//m_pMonster->m_bAnimFinish = bResult;

	//return bResult;
	return S_OK;
}
// 쫒아가는 콜라이더
_bool CMonster_State::isChase(void)
{
	_bool	bResult = m_pMonster->Collision_ToPlayer();

	return bResult;
}
_bool CMonster_State::isDamage()
{
	_bool	bResult = m_pMonster->Collision_Demage();

	return bResult;
}
// 공격 콜라이더
_bool CMonster_State::isAttack(void)	
{
	_bool	bResult = m_pMonster->Collision_Attack();	

	return bResult;
}

_bool CMonster_State::isNoneMove()
{
	if (m_pMonster->Collision_ToPlayer())
	{
		m_pMonster->m_bAnimFinish = false;
	}
	else
	{
		m_pMonster->m_bAnimFinish = true;
	}

	return m_pMonster->m_bAnimFinish;
}

_bool CMonster_State::isNonAttack()
{
	if (m_pMonster->Collision_Attack())
	{
		m_pMonster->m_bAnimFinish = false;
	}
	else
	{
		m_pMonster->m_bAnimFinish = true;
	}

	return m_pMonster->m_bAnimFinish;
}

_bool CMonster_State::isNoneDamage()
{
	if (m_pMonster->Collision_Demage())
	{
		m_pMonster->m_bAnimFinish = false;
	}
	else
	{
		m_pMonster->m_bAnimFinish = true;
	}

	return m_pMonster->m_bAnimFinish;
}

_bool CMonster_State::isDeath()
{
	m_pMonster->isDead(m_fDamge);

	return S_OK;
}

CMonster_State * CMonster_State::Create(CMonster * Monster)
{
	CMonster_State*		pInstance = new CMonster_State();

	if (FAILED(pInstance->Initialize(Monster)))
	{
		MSG_BOX("Failed to Create : CMonster_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_State::Free()
{
	Safe_Release(m_pMonster);
	//Safe_Release(m_pPlayer);
	//Safe_Release(m_pTransform);
}
