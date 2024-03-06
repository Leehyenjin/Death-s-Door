#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CState;
END

BEGIN(Client)

class CBlobMonster final : public CGameObject
{
public:
	enum COLLSION { COLL_HIT, COLL_ATTACK, COLL_END };

private:
	CBlobMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlobMonster(const CBlobMonster& rhs);
	virtual ~CBlobMonster() = default;
	friend class CBlobMonster_State;

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

	class CBlobMonster_State*		m_pBlobState = nullptr;

	MONSTERABILITY*		m_pBlobMonsterAbility = nullptr;

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();

	void				isDead(_float fDamge);

private:
	_bool				Collision_ToPlayer();
	_bool				Collision_Attack();
	_bool				Collision_Demage();

public:
	_bool				m_bAnimFinish = false;

	_float				m_fLerpTime = 0.2f;
	_float				m_fAnimationTime = 1.f;

public:
	CCollider*			Get_ColliderAttack() { return m_pCollHit[COLL_ATTACK]; }

public:
	static CBlobMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END