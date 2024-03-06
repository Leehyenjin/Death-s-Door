#include "stdafx.h"
#include "..\public\Player_State.h"
#include "Player.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Sword_Slash.h"

CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CPlayer * Player)
{
	m_Player = Player;

	if (FAILED(SetUp_State_Idle()))
		return E_FAIL;

	if (FAILED(SetUp_State_Walk()))
		return E_FAIL;

	if (FAILED(SetUp_State_Roll()))
		return E_FAIL;

	if (FAILED(SetUp_State_Attack()))
		return E_FAIL;

	if (FAILED(SetUp_State_Damag()))
		return E_FAIL;

	if (FAILED(SetUp_State_Skill()))
		return E_FAIL;

	if (FAILED(SetUp_State_Death()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Idle()
{
	if (nullptr == m_Player->m_pStateCom)
		return E_FAIL;

	m_Player->m_pStateCom
		->Set_Root(TEXT("STATE::IDLE"))

		.Add_State(TEXT("STATE::IDLE"))
		.Init_Start(this, &CPlayer_State::Start_Idle)
		.Init_Tick(this, &CPlayer_State::Tick_Idle)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)
		
		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Walk()
{
	if (nullptr == m_Player->m_pStateCom)
		return E_FAIL;

	m_Player->m_pStateCom->

		Add_State(TEXT("STATE::WALK_W"))
		.Init_Start(this, &CPlayer_State::Start_Walk_W)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_W)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_S"))
		.Init_Start(this, &CPlayer_State::Start_Walk_S)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_S)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_A"))
		.Init_Start(this, &CPlayer_State::Start_Walk_A)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_A)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_D"))
		.Init_Start(this, &CPlayer_State::Start_Walk_D)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_D)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_WA"))
		.Init_Start(this, &CPlayer_State::Start_Walk_WA)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_WA)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_WD"))
		.Init_Start(this, &CPlayer_State::Start_Walk_WD)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_WD)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::WALK_SA"))
		.Init_Start(this, &CPlayer_State::Start_Walk_SA)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_SA)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)
		
		.Add_State(TEXT("STATE::WALK_SD"))
		.Init_Start(this, &CPlayer_State::Start_Walk_SD)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_SD)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::KeyInput_None)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Attack()
{
	if (nullptr == m_Player->m_pModelCom)
		return E_FAIL;

	m_Player->m_pStateCom->

		Add_State(TEXT("STATE::ATTACK"))
		.Init_Start(this, &CPlayer_State::Start_Attack)
		.Init_Tick(this, &CPlayer_State::Tick_Attack)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::ATTACK2"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::ATTACK2"))
		.Init_Start(this, &CPlayer_State::Start_Attack2)
		.Init_Tick(this, &CPlayer_State::Tick_Attack2)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space )
		.Init_Changer(TEXT("STATE::ATTACK3"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Add_State(TEXT("STATE::ATTACK3"))
		.Init_Start(this, &CPlayer_State::Start_Attack3)
		.Init_Tick(this, &CPlayer_State::Tick_Attack3)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		//.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Skill()
{
	if (nullptr == m_Player->m_pModelCom)
		return E_FAIL;

	m_Player->m_pStateCom->

		Add_State(TEXT("STATE::SKILL"))
		.Init_Start(this, &CPlayer_State::Start_Skill)
		.Init_Tick(this, &CPlayer_State::Tick_Skill)
		.Init_End(this, &CPlayer_State::End_Common)
		//.Init_Changer(TEXT("STATE::WALK_W"), this, &CPlayer_State::KeyInput_W)
		//.Init_Changer(TEXT("STATE::WALK_S"), this, &CPlayer_State::KeyInput_S)
		//.Init_Changer(TEXT("STATE::WALK_A"), this, &CPlayer_State::KeyInput_A)
		//.Init_Changer(TEXT("STATE::WALK_D"), this, &CPlayer_State::KeyInput_D)
		//.Init_Changer(TEXT("STATE::WALK_WA"), this, &CPlayer_State::KeyInput_WA)
		//.Init_Changer(TEXT("STATE::WALK_WD"), this, &CPlayer_State::KeyInput_WD)
		//.Init_Changer(TEXT("STATE::WALK_SA"), this, &CPlayer_State::KeyInput_SA)
		//.Init_Changer(TEXT("STATE::WALK_SD"), this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(TEXT("STATE::ROLL"), this, &CPlayer_State::KeyInput_Space)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Roll()
{
	if (nullptr == m_Player->m_pStateCom)
		return E_FAIL;

	m_Player->m_pStateCom->
		Add_State(TEXT("STATE::ROLL"))
		.Init_Start(this, &CPlayer_State::Start_Roll)
		.Init_Tick(this, &CPlayer_State::Tick_Roll)
		.Init_End(this, &CPlayer_State::End_Common)
		//.Init_Changer(TEXT("STATE::ATTACK"), this, &CPlayer_State::MouseDown_LB)
		//.Init_Changer(TEXT("STATE::SKILL"), this, &CPlayer_State::MouseDown_RB)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)
		.Init_Changer(TEXT("STATE::DAMAG"), this, &CPlayer_State::isDamage)


		.Finish_Setting();
	

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Damag()
{
	if (nullptr == m_Player->m_pStateCom)
		return E_FAIL;

	m_Player->m_pStateCom->
		Add_State(TEXT("STATE::DAMAG"))
		.Init_Start(this, &CPlayer_State::Start_Damag)
		.Init_Tick(this, &CPlayer_State::Tick_Damag)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::DAMAGING"), this, &CPlayer_State::Set_isFinish)

		.Add_State(TEXT("STATE::DAMAGING"))
		.Init_Start(this, &CPlayer_State::Start_Damaging)
		.Init_Tick(this, &CPlayer_State::Tick_Damaging)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::DAMAG_END"), this, &CPlayer_State::Set_isFinish)

		.Add_State(TEXT("STATE::DAMAG_END"))
		.Init_Start(this, &CPlayer_State::Start_Damag_End)
		.Init_Tick(this, &CPlayer_State::Tick_Dama_End)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(TEXT("STATE::IDLE"), this, &CPlayer_State::Set_isFinish)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Death()
{
	if (nullptr == m_Player->m_pStateCom)
		return E_FAIL;

	m_Player->m_pStateCom->
		Add_State(TEXT("STATE::DEATH"))
		.Init_Start(this, &CPlayer_State::Start_Death)
		.Init_Tick(this, &CPlayer_State::Tick_Death)
		.Init_End(this, &CPlayer_State::End_Common)

		.Finish_Setting();

	return S_OK;
}

void CPlayer_State::Start_Idle(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_IDLE_1);
}

void CPlayer_State::Start_Walk_W(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_S(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_A(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_D(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_WA(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_WD(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_SA(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Walk_SD(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_RUN);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Attack(_double TimeDelta)
{
	/*m_Player->m_fAnimationTime = 0.01f;
	m_Player->m_fLerpTime = 0.7f;*/
	m_Player->m_pModelCom->Set_Animation(AIM_SLASH_HEAVY_R);	// 왼쪽 공격 모션(1타)
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_L);	
}

void CPlayer_State::Start_Attack2(_double TimeDelta)
{
	m_Player->m_fAnimationTime = 1.f;
	m_Player->m_pModelCom->Set_Animation(AIM_SLASH_LIGHT_R_NEW);	// 오른쪽 공격 모션(2타)
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_R);
}

void CPlayer_State::Start_Attack3(_double TimeDelta)
{
	m_Player->m_fAnimationTime = 1.f;
	m_Player->m_pModelCom->Set_Animation(AIM_SLASH_HEAVY_R);	// 다시 오른쪽 공격 모션(3타)
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_L);
}

void CPlayer_State::Start_Skill(_double TimeDelta)
{
	m_Player->m_fAnimationTime = 0.5f;
	m_Player->m_pModelCom->Set_Animation(AIM_MAGIC);				// 불 스킬 모션
}

void CPlayer_State::Start_Skill2(_double TimeDelta)
{
	m_Player->m_fAnimationTime = 0.5f;
	m_Player->m_pModelCom->Set_Animation(AIM_SLASH_LIGHT_L_NEW);		// 화살 스킬 모션
}

void CPlayer_State::Start_Death(_double TimeDelta)
{
	m_Player->m_fAnimationTime = 0.5f;
	m_Player->m_pModelCom->Set_Animation(AIM_DEAD);
}

void CPlayer_State::Start_Roll(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_ROLL);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Damag(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_HIT_BACK);
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Start_Damaging(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_HIT_IDLE);
}

void CPlayer_State::Start_Damag_End(_double TimeDelta)
{
	m_Player->m_pModelCom->Set_Animation(AIM_HIT_RECOVER);
}

void CPlayer_State::Tick_Idle(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	static_cast<CSword_Slash*>(m_Player->pSlashPartObject)->m_isRender = false;
	//m_Player->Ready_Parts(CPlayer::WEAPON_SWORD_BADY);
}

void CPlayer_State::Tick_Walk_W(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_Player->m_pTransformCom->Go_Straight(TimeDelta, m_Navi);
}

void CPlayer_State::Tick_Walk_S(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	m_Player->m_pTransformCom->Go_Straight(TimeDelta, m_Navi);
}

void CPlayer_State::Tick_Walk_A(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	//m_Player->m_pTransformCom->Turn(XMVectorSet(0.0f, 0.1f, 0.f, 0.f), TimeDelta * -0.5f);
	m_Player->m_pTransformCom->Go_Straight(TimeDelta, m_Navi);
}

void CPlayer_State::Tick_Walk_D(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	m_Player->m_pTransformCom->Go_Straight(TimeDelta, m_Navi);
}

void CPlayer_State::Tick_Walk_WA(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_Navi);
	m_Player->m_pTransformCom->Turn(XMVectorSet(0.0f, 0.1f, 0.f, 0.f), TimeDelta * -1.f);
	//m_Player->m_pTransformCom->Go_Left(TimeDelta * 1.f);
	//m_Player->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
}

void CPlayer_State::Tick_Walk_WD(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_Navi);
	m_Player->m_pTransformCom->Turn(XMVectorSet(0.0f, 0.1f, 0.f, 0.f), TimeDelta * 1.f);
}

void CPlayer_State::Tick_Walk_SA(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_Navi);
	m_Player->m_pTransformCom->Turn(XMVectorSet(0.0f, 0.1f, 0.f, 0.f), TimeDelta * 1.f);
}

void CPlayer_State::Tick_Walk_SD(_double TimeDelta)
{	
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 1.f, m_Navi);
	m_Player->m_pTransformCom->Turn(XMVectorSet(0.0f, 0.1f, 0.f, 0.f), TimeDelta * -1.f);
}

void CPlayer_State::Tick_Attack(_double TimeDelta)
{
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 0.1f, m_Navi);
	AnimIntervalChecker(AIM_SLASH_HEAVY_R, 0.0, 1.0);
	static_cast<CSword_Slash*>(m_Player->pSlashPartObject)->m_isRender = true;
}

void CPlayer_State::Tick_Attack2(_double TimeDelta)
{
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 0.1f, m_Navi);
	AnimIntervalChecker(AIM_SLASH_LIGHT_R_NEW, 0.0, 1.0);
}

void CPlayer_State::Tick_Attack3(_double TimeDelta)
{
	m_Navi = m_Player->m_pNavigationCom;
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 0.1f, m_Navi);
	AnimIntervalChecker(AIM_SLASH_HEAVY_R, 0.0, 1.0);
}

void CPlayer_State::Tick_Skill(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
}

void CPlayer_State::Tick_Skill2(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
}

void CPlayer_State::Tick_Death(_double TimeDelta)
{
}

void CPlayer_State::Tick_Roll(_double TimeDelta)
{
	m_Player->m_pTransformCom->Go_Straight(TimeDelta * 1.5f, m_Navi);
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
}

void CPlayer_State::Tick_Damag(_double TimeDelta)
{
}

void CPlayer_State::Tick_Damaging(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
}

void CPlayer_State::Tick_Dama_End(_double TimeDelta)
{
	static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
}

void CPlayer_State::End_Common(_double TimeDelta)
{
	m_Player->m_fLerpTime = 0.5f;
	m_Player->m_fAnimationTime = 1.f;

	//m_Slash->m_isRender = false;
}

_bool CPlayer_State::AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate)
{
	ENGINE_OPEN;
	if (m_Player->m_pModelCom->Get_AnimIndex(eAnim)->Get_PlayRate() >= StartRate
		&& m_Player->m_pModelCom->Get_AnimIndex(eAnim)->Get_PlayRate() <= FinishRate)
	{
		static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(true);
	}
	else
	{
		static_cast<CCollider*>(m_Player->Get_WeaponSet())->Set_CheckAble(false);
	}
	ENGINE_CLOSE;

	return true;
}

_bool CPlayer_State::KeyInput_None()
{
	ENGINE_OPEN;
	_bool	bResult =
		!ENGINE->Key_Pressing(DIK_W) &&
		!ENGINE->Key_Pressing(DIK_A) &&
		!ENGINE->Key_Pressing(DIK_S) &&
		!ENGINE->Key_Pressing(DIK_D);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_W()
{
	ENGINE_OPEN;
	_bool bResult =
		 ENGINE->Key_Pressing(DIK_W) &&
		!ENGINE->Key_Pressing(DIK_S) &&
		!ENGINE->Key_Pressing(DIK_A) &&
		!ENGINE->Key_Pressing(DIK_D) &&
		!KeyInput_WA() && !KeyInput_WD();
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_S()
{
	ENGINE_OPEN;
	_bool bResult =
		!ENGINE->Key_Pressing(DIK_W) &&
		 ENGINE->Key_Pressing(DIK_S) &&
		!ENGINE->Key_Pressing(DIK_A) &&
		!ENGINE->Key_Pressing(DIK_D) &&
		!KeyInput_SA() && !KeyInput_SD();
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_A()
{
	ENGINE_OPEN;
	_bool bResult =
		!ENGINE->Key_Pressing(DIK_W) &&
		!ENGINE->Key_Pressing(DIK_S) &&
		 ENGINE->Key_Pressing(DIK_A) &&
		!ENGINE->Key_Pressing(DIK_D) &&
		!KeyInput_SA() && !KeyInput_WA();
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_D()
{
	ENGINE_OPEN;
	_bool bResult =
		!ENGINE->Key_Pressing(DIK_W) &&
		!ENGINE->Key_Pressing(DIK_S) &&
		!ENGINE->Key_Pressing(DIK_A) &&
		 ENGINE->Key_Pressing(DIK_D) &&
		!KeyInput_WD() && !KeyInput_SD();
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_WA()
{
	ENGINE_OPEN;
	_bool bResult =
		ENGINE->Key_Pressing(DIK_W) && ENGINE->Key_Pressing(DIK_A)
		&& !ENGINE->Key_Pressing(DIK_S) && !ENGINE->Key_Pressing(DIK_D);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_WD()
{
	ENGINE_OPEN;
	_bool bResult =
		ENGINE->Key_Pressing(DIK_W) && ENGINE->Key_Pressing(DIK_D)
		&& !ENGINE->Key_Pressing(DIK_S) && !ENGINE->Key_Pressing(DIK_A);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_SA()
{
	ENGINE_OPEN;
	_bool bResult =
		ENGINE->Key_Pressing(DIK_S) && ENGINE->Key_Pressing(DIK_A)
		&& !ENGINE->Key_Pressing(DIK_W) && !ENGINE->Key_Pressing(DIK_D);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_SD()
{
	ENGINE_OPEN;
	_bool bResult =
		ENGINE->Key_Pressing(DIK_S) && ENGINE->Key_Pressing(DIK_D)
		&& !ENGINE->Key_Pressing(DIK_W) && !ENGINE->Key_Pressing(DIK_A);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::KeyInput_Space()
{
	ENGINE_OPEN;
	_bool bResult = ENGINE->Key_Pressing(DIK_SPACE);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::MouseDown_LB()
{
	ENGINE_OPEN;
	_bool bResult = ENGINE->Mouse_Down(DIM_LB);
	/*_bool bResult = ENGINE->Key_Down(DIK_K);*/
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::MouseDown_RB()
{
	ENGINE_OPEN;
	/*_bool bResult = ENGINE->Mouse_Down(DIM_RB);*/
	_bool bResult = ENGINE->Key_Pressing(DIK_E);
	ENGINE_CLOSE;

	return bResult;
}

_bool CPlayer_State::Set_isFinish()
{
	_bool bResult = m_Player->m_pModelCom->Get_isFinish();
	m_Player->m_pModelCom->Reset_isFinish();

	m_Player->m_bAnimFinish = bResult;

	return bResult;
}

_bool CPlayer_State::isDamage()
{
	_bool	bResult = m_Player->Collision_Damage();

	return bResult;
}

_bool CPlayer_State::Finish()
{
	return m_Player->m_pModelCom->isFinish();
}

CPlayer_State * CPlayer_State::Create(CPlayer * Player)
{
	CPlayer_State*		pInstance = new CPlayer_State();

	if (FAILED(pInstance->Initialize(Player)))
	{
		MSG_BOX("Failed to Create : CPlayer_State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_State::Free()
{
	//Safe_Release(m_Player);
}
