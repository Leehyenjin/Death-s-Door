#pragma once

#include "Client_Defines.h"
#include "Installation.h"

BEGIN(Client)

class CCubeBox : public CInstallation
{
private:
	CCubeBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCubeBox(const CCubeBox& rhs);
	virtual ~CCubeBox() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Model();
	HRESULT Player_Damaged(_float fDamage);

	_bool	Collision_Attack();

public:
	INTERACTABLE*			m_pInteractable = nullptr;

public:
	static CCubeBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END