#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CBlobMonster_State : public CBase
{
public:
	enum ANIMATION { AIM_ATTACK, AIM_MOVE, AIM_IDLE, AIM_SLUMP, AIM_END };

public:
	CBlobMonster_State();
	virtual ~CBlobMonster_State() = default;

public:
	HRESULT Initialize(class CBlobMonster* BlobMonster);

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
	class CBlobMonster*				m_pBlob;
	class CPlayer*					m_pPlayer;
	class CTransform*				m_pTransform;

public:
	static CBlobMonster_State* Create(class CBlobMonster* BlobMonster);
	virtual void Free();
};

END