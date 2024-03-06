#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CWeaponBigBox : public CUI
{
private:
	CWeaponBigBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponBigBox(const CWeaponBigBox& rhs);
	virtual ~CWeaponBigBox() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	SetUp_Component();

private:
	CUI::UI_DESC			m_UIDesc;

public:
	static CWeaponBigBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, _uint eUIType);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END