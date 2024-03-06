#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CPlant_State : public CBase
{
public:
	enum AIMATION {	AIM_IDLE, AIM_BITE, AIM_DIE, AIM_HIT, AIM_END	};

private:
	CPlant_State();
	virtual ~CPlant_State() = default;

public:
	HRESULT Initialize(class CPlantMonster* PlnatMonster);

private:
	HRESULT SetUp_State_Idle();
	HRESULT SetUp_State_Attack();
	HRESULT SetUp_State_Damaged();
	HRESULT SetUp_State_Deth();
	
private: // Start //
	void Start_Idle(_double TimeDelta);
	void Start_Attack(_double TimeDelta);
	void Start_Damaged(_double TimeDelta);
	void Start_Deth(_double TimeDelta);

private: // Tick //
	void Tick_Idle(_double TimeDelta);
	void Tick_Attack(_double TimeDelta);
	void Tick_Damaged(_double TimeDelta);
	void Tick_Deth(_double TimeDelta);

private: // END //
	void End_Common(_double TimeDleta);

private:
	_bool	Finishing();
	_bool	isAttack();
	_bool	isDamage();
	_bool	isNonAttack();
	_bool	isNoneDamage();
	_bool	isDeath();

	_float	m_fDamge = 1.f;

private:
	class CPlantMonster*		m_pPlantMonster;
	class CPlayer*				m_pPlayer;
	class CTransform*			m_pTransform;

public:
	static CPlant_State* Create(class CPlantMonster* PlantMonster);
	virtual void Free();

};

END