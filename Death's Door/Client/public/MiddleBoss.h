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

class CMiddleBoss final : public CGameObject
{
public:
	enum COOLISION { COLL_HIT, COLL_RANGE, COLL_END };

private:
	CMiddleBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiddleBoss(const CMiddleBoss& rhs);
	virtual ~CMiddleBoss() = default;
	friend class CMiddleBoss_State;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pColliderCom[COLLTYPE_END] = { nullptr };
	CCollider*				m_pCollHit[COLL_END] = { nullptr };
	CState*					m_pStateCom = nullptr;

	class CMiddleBoss_State*		m_pMiddleBossState = nullptr;

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_ShaderResources();
	HRESULT				SetUp_State();

public:
	void				Set_ColliderHit();

private:
	_bool				Collision_ToPlayer();
	_bool				Collision_Attack();
	_bool				Collision_Demage();

public:
	_bool				m_bAnimFinish = false;

	_float				m_fLerpTime = 0.2f;
	_float				m_fAnimationTime = 1.f;

public:
	static CMiddleBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END