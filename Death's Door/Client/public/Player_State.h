#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CPlayer_State : public CBase
{
public:
	enum ANIMATION {  AIM_BOOB, AIM_BOOB_IDLE, AIM_MAGIC, AIM_ARRROW_STOP, AIM_ARROW_ZOOM, AIM_BRACE_LOOP, AIM_BRACE, AIM_CHARGE_DAGGER, AIM_CHARGE_HEAVY_FULL, AIM_CHARGE_HEAVY_L, 
					 AIM_CHARGE_HEAVY_R, CHARGE_MAX_L, CHARGE_MAX_R, AIM_CHARGE_SLAM_OVERHEAD, AIM_CHARGE_SLASH_L, AIM_CHARGE_SLASH_R, AIM_CLIMBING_LADDER_DOWN, AIM_CLIMBRING_LADDER, AIM_CLIMBING_OFF_LADDER_TOP, AIM_CUTSCENE_TURN_END, 
					 AIM_CUTSCENE_TURN_HALF, AIM_CUTSCENE_TURN_STOPPED, AIM_DEAD, AIM_DROWN, AIM_FALLING, AIM_FLY_HEAVY, AIM_FLY, AIM_GETITM, AIM_HIT_BACK, AIM_HIT_IDLE, 
					 AIM_HIT_RECOVER, AIM_HOOKSHOT_FLY, AIM_HOOKSHOT, AIM_IDLE_0_HEAVY, AIM_IDLE_0, AIM_IDLE_1, AIM_INJURED_FALL, AIM_INJURED_GETUP, AIM_INJURED_LAND, AIM_LAND, 
					 AIM_PLUNGE_HEAVY, AIM_PLUNGE_LAND_HEAVY, AIM_PLUNGE_LAND, AIM_PLUNGE_PREP_HEAVY, AIM_PLUNGE_PREP, AIM_PLUNGE, AIM_PREPFLY_HEAVY, AIM_PREPFLY, AIM_PUSH_LEVER, AIM_ROLL_HEAVY, 
					 AIM_ROLL_SLASH_END, AIM_ROLL_SLASH, AIM_ROLL, AIM_RUN_HEAVY, AIM_RUN, AIM_SLASH_HEAVY_L, AIM_SLASH_HEAVY_R, AIM_SLASH_LIGHT_L_NEW, AIM_SLASH_LIGHT_R_NEW, AIM_WALK, AIM_BOOB_END, AIM_END };

private:
	CPlayer_State();
	virtual ~CPlayer_State() = default;

public:
	HRESULT Initialize(class CPlayer* Player);

private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Walk();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Skill();
	HRESULT SetUp_State_Roll();
	HRESULT SetUp_State_Damag();
	HRESULT SetUp_State_Death();

private: // Start
	void Start_Idle(_double TimeDelta);

	void Start_Walk_W(_double TimeDelta);
	void Start_Walk_S(_double TimeDelta);
	void Start_Walk_A(_double TimeDelta);
	void Start_Walk_D(_double TimeDelta);
	void Start_Walk_WA(_double TimeDelta);
	void Start_Walk_WD(_double TimeDelta);
	void Start_Walk_SA(_double TimeDelta);
	void Start_Walk_SD(_double TimeDelta);

	void Start_Roll(_double TimeDelta);

	void Start_Damag(_double TimeDelta);
	void Start_Damaging(_double TimeDelta);
	void Start_Damag_End(_double TimeDelta);


	void Start_Attack(_double TimeDelta);
	void Start_Attack2(_double TimeDelta);
	void Start_Attack3(_double TimeDelta);
	
	void Start_Skill(_double TimeDelta);
	void Start_Skill2(_double TimeDelta);

	void Start_Death(_double TimeDelta);

private: // Tick
	void Tick_Idle(_double TimeDelta);

	void Tick_Walk_W(_double TimeDelta);
	void Tick_Walk_S(_double TimeDelta);
	void Tick_Walk_A(_double TimeDelta);
	void Tick_Walk_D(_double TimeDelta);
	void Tick_Walk_WA(_double TimeDelta);
	void Tick_Walk_WD(_double TimeDelta);
	void Tick_Walk_SA(_double TimeDelta);
	void Tick_Walk_SD(_double TimeDelta);

	void Tick_Roll(_double TimeDelta);

	void Tick_Damag(_double TimeDelta);
	void Tick_Damaging(_double TimeDelta);
	void Tick_Dama_End(_double TimeDelta);

	void Tick_Attack(_double TimeDelta);
	void Tick_Attack2(_double TimeDelta);
	void Tick_Attack3(_double TimeDelta);
	
	void Tick_Skill(_double TimeDelta);
	void Tick_Skill2(_double TimeDelta);

	void Tick_Death(_double TimeDelta);

private: // END
	void End_Common(_double TimeDelta);

private: // Changer
	_bool	KeyInput_None();
	_bool	KeyInput_W();
	_bool	KeyInput_S();
	_bool	KeyInput_A();
	_bool	KeyInput_D();
	_bool	KeyInput_WA();
	_bool	KeyInput_WD();
	_bool	KeyInput_SA();
	_bool	KeyInput_SD();
	_bool	KeyInput_Space();
	_bool	MouseDown_LB();
	_bool	MouseDown_RB();
	_bool	Set_isFinish();
	_bool	isDamage();
	_bool	Finish();
	_bool	AnimIntervalChecker(ANIMATION eAnim, _double StartRate, _double FinishRate);

private:
	class CPlayer*		m_Player;
	class CNavigation*	m_Navi;
	class CSword_Slash*	m_Slash = nullptr;
		
	_bool		m_isCollCheck = false;
	_bool		m_isRenderCheck = false;

public:
	static CPlayer_State* Create(class CPlayer* Player);
	virtual void Free();
};

END