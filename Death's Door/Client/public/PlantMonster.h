#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CState;
class CModel;
class CCollider;
class CRenderer;
END

BEGIN(Client)

class CPlantMonster final : public CGameObject
{
public:
	enum COLLSION { COLL_HIT, COLL_ATTACK, COLL_END };

private:
	CPlantMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlantMonster(const CPlantMonster& rhs);
	virtual ~CPlantMonster() = default;
	friend class CPlant_State;

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
	CState*					m_pStateCom = nullptr;

	class CPlant_State*		m_PlantState = nullptr;

	MONSTERABILITY*			m_pPlantAbility = nullptr;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();
	HRESULT					SetUp_State();

	void					isDead(_float fDamge);

private:
	_bool					Collision_Attack();
	_bool					Collision_Damage();

public:
	_bool					m_bAnimFinish = false;

public:
	static CPlantMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END