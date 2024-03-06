#pragma once

#include "Client_Defines.h"
#include "Installation.h"

BEGIN(Client)

class CFance : public CInstallation
{
private:
	CFance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFance(const CFance& rhs);
	virtual ~CFance() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Model();

private:
	class CRenderer*		m_pRendererCom = nullptr;
	class CCollider*		m_pColliderCom = nullptr;


public:
	static CFance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END
