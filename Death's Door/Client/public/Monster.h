#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Monster_Arrow.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CState;
class Navigation;
END

BEGIN(Client)

class CMonster final : public CGameObject
{
public:
	enum COOLISION { COLL_HIT, COLL_RANGE, COLL_END };

private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;
	friend class CMonster_State;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*			m_pCollHit[COLL_END] = { nullptr };
	CState*				m_pStateCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;	
	CMonster_Arrow*		m_pArrow = nullptr;

	class CMonster_State*		m_MonsterState	 = nullptr;	

	MONSTERABILITY*		m_pMonsterAbility = nullptr;

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();
	HRESULT				Ready_Pats();
	HRESULT				Set_OnCell();

	void				isDead(_float fDamge);

private:
	_bool				Collision_ToPlayer();
	_bool				Collision_Attack();
	_bool				Collision_Demage();

private:
	vector<CGameObject*>	m_ArrowParts;

public:
	_bool					m_bAnimFinish = false;

	_float					m_fLerpTime = 0.2f;
	_float					m_fAnimationTime = 1.f;

public:
	CCollider* Get_Coll() { return m_pCollHit[COLL_HIT]; }

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END