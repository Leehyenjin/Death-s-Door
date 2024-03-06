#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CGrimacer_State : public CBase
{
public:
	enum ANIMATION { AIM_BACKSTEP, AIM_DASH, AIM_DEAD, AIM_DEFEND_BREAK, AIM_DEFEND_IMPACKT, AIM_DEFEND_START, AIM_DEFEND_TUEN_CCW, AIM_DEFEND_TUEN_CW, AIM_DEFEND, AIM_GETUP_FROMSPIN, AIM_IDLE,
					 AIM_RUN, AIM_SPIN_CW, AIM_VOMIT, AIM_END };

public:
	CGrimacer_State();
	virtual ~CGrimacer_State() = default;

public:
	HRESULT Initialize(class CGrimacerMonster* GrimacerMonster);

private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Walk();
	HRESULT SetUp_State_Chase();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Damaged();
	HRESULT SetUp_State_Deth();

private: // Start //
	void Start_Idle(_double TimeDelta);
	void Start_Walk(_double TimeDelta);
	void Start_Chase(_double TimeDelta);
	void Start_Attack(_double TimeDelta);
	void Start_Damaged(_double TimeDelta);
	void Start_Deth(_double TimeDelta);

private: // Tick //
	void Tick_Idle(_double TimeDelta);
	void Tick_Walk(_double TimeDelta);
	void Tick_Chase(_double TimeDelta);
	void Tick_Attack(_double TimeDelta);
	void Tick_Damaged(_double TimeDelta);
	void Tick_Deth(_double TimeDelta);

private: // END //
	void End_Common(_double TimeDleta);

private:
	_bool	Finishing();
	_bool	isAttack();
	_bool	isChase();
	_bool	isDamage();
	_bool	isNoneMove();
	_bool	isNonAttack();
	_bool	isNoneDamage();
	_bool	isDeath();

	_float	m_fDamage = 1.f;

private:
	class CGrimacerMonster*			m_pGrimacer;
	class CPlayer*					m_pPlayer;
	class CTransform*				m_pTransform;

public:
	static CGrimacer_State* Create(class CGrimacerMonster* GrimacerMonster);
	virtual void Free();
};

END