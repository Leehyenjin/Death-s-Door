#include "stdafx.h"
#include "..\public\Plant_State.h"
#include "PlantMonster.h"
#include "GameInstance.h"


CPlant_State::CPlant_State()
{
}

HRESULT CPlant_State::Initialize(CPlantMonster * PlnatMonster)
{
	m_pPlantMonster = PlnatMonster;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Damaged()))
		return E_FAIL;

	if (FAILED(SetUp_State_Deth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlant_State::SetUp_State_Idle()
{
	if (nullptr == m_pPlantMonster->m_pStateCom)
		return E_FAIL;

	m_pPlantMonster->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CPlant_State::Start_Idle)
		.Init_Tick(this, &CPlant_State::Tick_Idle)
		.Init_End(this, &CPlant_State::End_Common)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlant_State::isAttack)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlant_State::SetUp_State_Attack()
{
	if (nullptr == m_pPlantMonster->m_pStateCom)
		return E_FAIL;

	m_pPlantMonster->m_pStateCom->
		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CPlant_State::Start_Attack)
		.Init_Tick(this, &CPlant_State::Tick_Attack)
		.Init_End(this, &CPlant_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlant_State::isNonAttack)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlant_State::SetUp_State_Damaged()
{
	if (nullptr == m_pPlantMonster->m_pStateCom)
		return E_FAIL;

	m_pPlantMonster->m_pStateCom->
		Add_State(TEXT("STATE::DAMAG"))
		.Init_Start(this, &CPlant_State::Start_Damaged)
		.Init_Tick(this, &CPlant_State::Tick_Damaged)
		.Init_End(this, &CPlant_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlant_State::isNonAttack)
		.Init_Changer(TEXT("STATE::DEATH"), this, &CPlant_State::isDeath)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlant_State::SetUp_State_Deth()
{
	if (nullptr == m_pPlantMonster->m_pStateCom)
		return E_FAIL;

	m_pPlantMonster->m_pStateCom->
		Add_State(TEXT("STATE::DEATH"))
		.Init_Start(this, &CPlant_State::Start_Deth)
		.Init_Tick(this, &CPlant_State::Tick_Deth)
		.Init_End(this, &CPlant_State::End_Common)

		.Finish_Setting();

	return S_OK;
}

void CPlant_State::Start_Idle(_double TimeDelta)
{
	m_pPlantMonster->m_pModelCom->Set_Animation(AIM_IDLE);
}

void CPlant_State::Start_Attack(_double TimeDelta)
{
	m_pPlantMonster->m_pModelCom->Set_Animation(AIM_BITE);
}

void CPlant_State::Start_Damaged(_double TimeDelta)
{
	m_pPlantMonster->m_pModelCom->Set_Animation(AIM_HIT);
}

void CPlant_State::Start_Deth(_double TimeDelta)
{
	m_pPlantMonster->m_pModelCom->Set_Animation(AIM_DIE);
}

void CPlant_State::Tick_Idle(_double TimeDelta)
{
}

void CPlant_State::Tick_Attack(_double TimeDelta)
{
	ENGINE_OPEN;

	CTransform*		pTargetPos = (CTransform*)ENGINE->Get_ComponentPtr(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	_vector			vPlayerPos = pTargetPos->Get_State(CTransform::STATE_TRANSLATION);

	m_pPlantMonster->m_pTransformCom->LookAt(vPlayerPos);

	ENGINE_CLOSE;
}

void CPlant_State::Tick_Damaged(_double TimeDelta)
{
}

void CPlant_State::Tick_Deth(_double TimeDelta)
{
}

void CPlant_State::End_Common(_double TimeDleta)
{
}

_bool CPlant_State::Finishing()
{
	return _bool();
}

_bool CPlant_State::isAttack()
{
	_bool	bResult = m_pPlantMonster->Collision_Attack();

	return bResult;
}

_bool CPlant_State::isDamage()
{
	_bool	bResult = m_pPlantMonster->Collision_Damage();

	return bResult;
}

_bool CPlant_State::isNonAttack()
{
	if (m_pPlantMonster->Collision_Attack())
	{
		m_pPlantMonster->m_bAnimFinish = false;
	}
	else
	{
		m_pPlantMonster->m_bAnimFinish = true;
	}

	return m_pPlantMonster->m_bAnimFinish;
}

_bool CPlant_State::isNoneDamage()
{
	if (m_pPlantMonster->Collision_Damage())
	{
		m_pPlantMonster->m_bAnimFinish = false;
	}
	else
	{
		m_pPlantMonster->m_bAnimFinish = true;
	}

	return m_pPlantMonster->m_bAnimFinish;
}

_bool CPlant_State::isDeath()
{
	m_pPlantMonster->isDead(m_fDamge);

	return S_OK;
}

CPlant_State * CPlant_State::Create(CPlantMonster * PlantMonster)
{
	CPlant_State*	pInstance = new CPlant_State();

	if (FAILED(pInstance->Initialize(PlantMonster)))
	{
		MSG_BOX("Failled to Create : CPlant_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_State::Free()
{
	Safe_Release(m_pPlantMonster);
}
