#include "stdafx.h"
#include "..\public\BlobMonster_State.h"
#include "BlobMonster.h"
#include "GameInstance.h"

CBlobMonster_State::CBlobMonster_State()
{
}

HRESULT CBlobMonster_State::Initialize(CBlobMonster * BlobMonster)
{
	m_pBlob = BlobMonster;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

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

HRESULT CBlobMonster_State::SetUp_State_Idle()
{
	if (nullptr == m_pBlob->m_pStateCom)
		return E_FAIL;

	m_pBlob->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CBlobMonster_State::Start_Idle)
		.Init_Tick(this, &CBlobMonster_State::Tick_Idle)
		.Init_End(this, &CBlobMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CBlobMonster_State::isChase)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CBlobMonster_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CBlobMonster_State::isDamage)


		.Finish_Setting();

	return S_OK;
}

HRESULT CBlobMonster_State::SetUp_State_Walk()
{
	return E_NOTIMPL;
}

HRESULT CBlobMonster_State::SetUp_State_Chase()
{
	if (nullptr == m_pBlob->m_pStateCom)
		return E_FAIL;

	m_pBlob->m_pStateCom->
		Add_State(TEXT("STATE::CHASE"))
		.Init_Start(this, &CBlobMonster_State::Start_Chase)
		.Init_Tick(this, &CBlobMonster_State::Tick_Chase)
		.Init_End(this, &CBlobMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CBlobMonster_State::isNoneMove)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CBlobMonster_State::isAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CBlobMonster_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CBlobMonster_State::SetUp_State_Attack()
{
	if (nullptr == m_pBlob->m_pStateCom)
		return E_FAIL;

	m_pBlob->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CBlobMonster_State::Start_Attack)
		.Init_Tick(this, &CBlobMonster_State::Tick_Attack)
		.Init_End(this, &CBlobMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CBlobMonster_State::isNonAttack)
		.Init_Changer(TEXT("STATE::DAMAGE"), this, &CBlobMonster_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CBlobMonster_State::SetUp_State_Damaged()
{
	if (nullptr == m_pBlob->m_pStateCom)
		return E_FAIL;

	m_pBlob->m_pStateCom->
		Add_State(TEXT("STATE::DAMAGE"))
		.Init_Start(this, &CBlobMonster_State::Start_Damaged)
		.Init_Tick(this, &CBlobMonster_State::Tick_Damaged)
		.Init_End(this, &CBlobMonster_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CBlobMonster_State::isNoneDamage)
		.Init_Changer(TEXT("STATE::CHASE"), this, &CBlobMonster_State::isNoneMove)
		.Init_Changer(TEXT("STATE::DEATH"), this, &CBlobMonster_State::isDeath)

		.Finish_Setting();

	return S_OK;
}

HRESULT CBlobMonster_State::SetUp_State_Deth()
{
	if (nullptr == m_pBlob->m_pStateCom)
		return E_FAIL;

	m_pBlob->m_pStateCom->
		Add_State(TEXT("STATE::DEATH"))
		.Init_Start(this, &CBlobMonster_State::Start_Deth)
		.Init_Tick(this, &CBlobMonster_State::Tick_Deth)
		.Init_End(this, &CBlobMonster_State::End_Common)

		.Finish_Setting();

	return S_OK;
}

void CBlobMonster_State::Start_Idle(_double TimeDelta)
{
	m_pBlob->m_pModelCom->Set_Animation(AIM_IDLE);
}

void CBlobMonster_State::Start_Walk(_double TimeDelta)
{
	//m_pBlob->m_pModelCom->Set_Animation(AIM_IDLE);
}

void CBlobMonster_State::Start_Chase(_double TimeDelta)
{
	m_pBlob->m_pModelCom->Set_Animation(AIM_MOVE);
}

void CBlobMonster_State::Start_Attack(_double TimeDelta)
{
	m_pBlob->m_pModelCom->Set_Animation(AIM_ATTACK);
}

void CBlobMonster_State::Start_Damaged(_double TimeDelta)
{
	m_pBlob->m_pModelCom->Set_Animation(AIM_SLUMP);
}

void CBlobMonster_State::Start_Deth(_double TimeDelta)
{
	m_pBlob->m_pModelCom->Set_Animation(AIM_SLUMP);
}

void CBlobMonster_State::Tick_Idle(_double TimeDelta)
{
}

void CBlobMonster_State::Tick_Walk(_double TimeDelta)
{
}

void CBlobMonster_State::Tick_Chase(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pBlob->m_pTransformCom->Chase(vPlayerPos, TimeDelta * 0.5f);
	m_pBlob->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CBlobMonster_State::Tick_Attack(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pAttackPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pAttackPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pBlob->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CBlobMonster_State::Tick_Damaged(_double TimeDelta)
{
}

void CBlobMonster_State::Tick_Deth(_double TimeDelta)
{
}

void CBlobMonster_State::End_Common(_double TimeDleta)
{
}

_bool CBlobMonster_State::Finishing()
{
	return _bool();
}

_bool CBlobMonster_State::isAttack()
{
	_bool bResult = m_pBlob->Collision_Attack();

	return bResult;
}

_bool CBlobMonster_State::isChase()
{
	_bool bResult = m_pBlob->Collision_ToPlayer();

	return bResult;
}

_bool CBlobMonster_State::isDamage()
{
	_bool bResult = m_pBlob->Collision_Demage();

	return bResult;
}

_bool CBlobMonster_State::isNoneMove()
{
	if (m_pBlob->Collision_ToPlayer())
	{
		m_pBlob->m_bAnimFinish = false;
	}
	else
	{
		m_pBlob->m_bAnimFinish = true;
	}

	return m_pBlob->m_bAnimFinish;
}

_bool CBlobMonster_State::isNonAttack()
{
	if (m_pBlob->Collision_Attack())
	{
		m_pBlob->m_bAnimFinish = false;
	}
	else
	{
		m_pBlob->m_bAnimFinish = true;
	}

	return m_pBlob->m_bAnimFinish;
}

_bool CBlobMonster_State::isNoneDamage()
{
	if (m_pBlob->Collision_Demage())
	{
		m_pBlob->m_bAnimFinish = false;
	}
	else
	{
		m_pBlob->m_bAnimFinish = true;
	}

	return m_pBlob->m_bAnimFinish;
}

_bool CBlobMonster_State::isDeath()
{
	m_pBlob->isDead(m_fDamge);

	return S_OK;
}

CBlobMonster_State * CBlobMonster_State::Create(CBlobMonster * BlobMonster)
{
	CBlobMonster_State*		pInstance = new CBlobMonster_State();

	if (FAILED(pInstance->Initialize(BlobMonster)))
	{
		MSG_BOX("Failled to Create : BlobMonster_State");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlobMonster_State::Free()
{
	Safe_Release(m_pBlob);
}
