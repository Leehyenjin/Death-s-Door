#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;	
class CRenderer;
class CModel;
class CCollider;
class CState;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum WEAPON { WEAPON_SWORD_R, WEAPON_SWORD_L, WEAPON_SWORD_BADY, WEAPON_BOW, WEAPON_END };

public:
	enum COLLSION { COLL_HIT, COLL_RANGE, COLL_END };

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
	friend class CPlayer_State;

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
	CCollider*			m_pDistanceCollider = nullptr;
	CState*				m_pStateCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CWeapon*			m_pWeapon = nullptr;
	
	WEAPON				m_eWeapon = WEAPON_END;
	
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();
	HRESULT	SetUp_State();
	HRESULT Ready_Parts();
	HRESULT Ready_Slash_Parts();
	//HRESULT Ready_Arrow();
	HRESULT Set_OnCell();

	void	isDead(_float fDamge);
	void	Distance(CCollider* TargetDistanceCollider);
	
	_bool	Collision_Damage();

public:
	CCollider* Get_WeaponSet();

private:
	class CPlayer_State*	m_PlayerState = nullptr;	

private:
	vector<CGameObject*>	m_PlayerParts;

public:
	CGameObject*				pSlashPartObject = { nullptr };

public:
	_float					m_fLerpTime = 0.3f;			// 애니메이션 선형보관
	_float					m_fAnimationTime = 0.5f;	// 애니메이션 총 시간

	_bool					m_bAnimFinish = false;
	_bool					m_isWeapon = false; 
	_bool					m_isRender = false;
	
	PLAYERABILITY*			m_pPlayerAbility = nullptr;

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END