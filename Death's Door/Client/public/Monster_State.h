#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMonster_State : public CBase
{
public:
	enum ANIMATION { AIM_DROWN, AIM_HIT_BOW, AIM_IDLE_BOW, AIM_IDLE_SWORD, AIM_IDLE, AIM_RUN_BOW, AIM_RUN_EMPTY, AIM_RUN_SWORD, AIM_SHOOT_BOW_RAPID, AIM_SHOOT_BOW, AIM_WALK, AIM_END };

private:
	CMonster_State();
	virtual ~CMonster_State() = default;

public:
	HRESULT Initialize(class CMonster* Monster);

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

	_float	m_fDamge = 1.f;

private:
	class CMonster*		m_pMonster;	
	class CPlayer*		m_pPlayer;
	class CTransform*	m_pTransform;

public:
	static CMonster_State* Create(class CMonster* Monster);
	virtual void Free();
};

END