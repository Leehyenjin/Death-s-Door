#pragma once

#include "Client_Defines.h"
#include "Installation.h"

BEGIN(Client)

class CStageMap : public CInstallation
{
private:
	CStageMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStageMap(const CStageMap& rhs);
	virtual ~CStageMap() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Model();

public:
	static  CStageMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END