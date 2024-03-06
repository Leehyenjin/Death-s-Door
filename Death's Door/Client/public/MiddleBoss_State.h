#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMiddleBoss_State : public CBase
{
//public:
//	enum ANIMATION { AIM_START_SPIN, AIM_IDLE, AIM_END_SPIN, AIM_IDLE_SPIN, AIM_STARTHYPER, AIM_ENDHYPER, AIM_LIFTSTART, AIM_LIFTIDLE, AIM_SHOOT, AIM_DEATH, AIM_LIFT_DMG_L, AIM_LIFT_DMG_R,
//					 AIM_LIFT_DAMG_IDLE_L, AIM_LIFT_DAMG_IDLE_R, AIM_FALL, AIM_SLAM, AIM_IDLE_LOWER, AIM_HYPER_SPIN, AIM_SLAM_SLOW, AIM_SLAM_SLOW_IDLE, AIM_SLAM_SLOW_END };

public:
	enum ANIMATION { AIM_ATK_SLASH_01_L, AIM_ATK_SLASH_01_L2, AIM_IA_DEATH_WAR, AIM_IA_DEATH_WAR2, AIM_IDLE, AIM_IDLE2, AIM_FINCH, AIM_FINCH2, AIM_RUN_F, AIM_RUN_F2, AIM_WALK_B, AIM_WALK_B1,
					 AIM_WALK_F, AIM_WALK_F2, AIM_WALK_L, AIM_WALK_L2, AIM_WALK_R, AIM
	};

private:
	CMiddleBoss_State();
	virtual ~CMiddleBoss_State() = default;

public:
	HRESULT Initialize(class CMiddleBoss* MiddleBoss);

private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Chase();
	HRESULT SetUp_State_Damaged();
	HRESULT SetUp_State_Deth();

private: // Start //
	void Start_Idle(_double TimeDelta);
	void Start_Attack(_double TimeDelta);
	void Start_Chase(_double TimeDelta);
	void Start_Damaged(_double TimeDelta);
	void Start_Deth(_double TimeDelta);


private: // Tick //
	void Tick_Idle(_double TimeDelta);
	void Tick_Attack(_double TimeDelta);
	void Tick_Chase(_double TimeDelta);
	void Tick_Damaged(_double TimeDelta);
	void Tick_Deth(_double TimeDelta);


private: // END //
	void End_Common(_double TimeDleta);

private:
	_bool	Finishing();
	_bool	isAttack();
	_bool	isDamage();
	_bool	isNoneMove();
	_bool	isChase();
	_bool	isNonAttack();
	_bool	isNoneDamage();

private:
	class CMiddleBoss*			m_pMiddleBoss;
	class CPlayer*				m_pPlayer;
	class CTransform*			m_pTransform;

public:
	static CMiddleBoss_State* Create(class CMiddleBoss* MiddleBoss);
	virtual void Free();
};

END